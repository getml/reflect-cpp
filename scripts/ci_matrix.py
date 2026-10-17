#!/usr/bin/env python3
"""Run selected Linux CI jobs locally, retaining separate build trees and logs."""

import argparse
import itertools
import json
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import time

import yaml


ROOT = Path(__file__).resolve().parents[1]


def matrix_jobs():
    workflow = yaml.safe_load((ROOT / '.github/workflows/linux.yaml').read_text())
    matrix = workflow['jobs']['linux']['strategy']['matrix']
    axes = {key: value for key, value in matrix.items() if key not in ('exclude', 'include')}
    jobs = []
    for values in itertools.product(*axes.values()):
        job = dict(zip(axes, values))
        if not any(all(job.get(k) == v for k, v in rule.items()) for rule in matrix.get('exclude', [])):
            jobs.append(job)
    jobs.extend(matrix.get('include', []))
    return jobs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--list', action='store_true', help='print selected jobs without running them')
    parser.add_argument('--compiler', action='append', help='filter, e.g. gcc-14 or llvm-19; repeatable')
    parser.add_argument('--format', action='append', help='filter formats; repeatable')
    parser.add_argument('--cxx', type=int, choices=(20, 23), help='filter language standard')
    parser.add_argument('--modules', choices=('ON', 'OFF'), default='OFF')
    parser.add_argument('--cxx-executable', help='override compiler for a single selected compiler family/version')
    parser.add_argument('--system-deps', action='store_true', help='use system packages instead of CI vcpkg')
    parser.add_argument('--jobs', type=int, default=4, help='parallel build and test jobs')
    parser.add_argument('--container', help='run inside this prebuilt Ubuntu image')
    parser.add_argument('--build-root', default='build/local-ci/native', help='directory for builds, logs and results')
    parser.add_argument('--runtime', default='podman', choices=('podman', 'docker'))
    parser.add_argument('--allow-missing', action='store_true', help='record missing compilers as skipped')
    args = parser.parse_args()
    if args.jobs < 1:
        parser.error('--jobs must be positive')
    if args.cxx_executable and (not args.compiler or len(args.compiler) != 1):
        parser.error('--cxx-executable requires exactly one --compiler filter')
    if args.container:
        forwarded = sys.argv[1:]
        for flag in ('--container', '--runtime'):
            if flag in forwarded:
                index = forwarded.index(flag)
                del forwarded[index:index + 2]
            forwarded = [arg for arg in forwarded if not arg.startswith(flag + '=')]
        return subprocess.call([
            args.runtime, 'run', '--rm', *(['--userns=keep-id'] if args.runtime == 'podman' else []),
            '-v', f'{ROOT}:/work', '-w', '/work',
            '-e', 'VCPKG_BINARY_SOURCES=clear;files,/work/build/local-ci/vcpkg-cache,readwrite',
            args.container, 'python3', 'scripts/ci_matrix.py', *forwarded,
            '--build-root', 'build/local-ci/container',
        ])

    selected = []
    for job in matrix_jobs():
        if job.get('modules', 'OFF') != args.modules:
            continue
        compiler = f"{job['compiler']}-{job['compiler-version']}"
        if args.compiler and compiler not in args.compiler:
            continue
        if args.format and job['format'] not in args.format:
            continue
        if args.cxx and job['cxx'] != args.cxx:
            continue
        if args.modules == 'ON' and job['compiler-version'] < (14 if job['compiler'] == 'gcc' else 19):
            continue
        selected.append((job, compiler))
    if not selected:
        parser.error('no matching jobs (module jobs require GCC 14+ or Clang 19+)')
    if args.list:
        for job, compiler in selected:
            print(f"{job['format']}-cxx{job['cxx']}-{compiler}-modules-{args.modules}")
        print(f'{len(selected)} jobs')
        return 0

    output = ROOT / args.build_root
    output.mkdir(parents=True, exist_ok=True)
    results = []
    for job, compiler in selected:
        label = f"{job['format']}-cxx{job['cxx']}-{compiler}-modules-{args.modules}"
        cxx = args.cxx_executable or f"{'g++' if job['compiler'] == 'gcc' else 'clang++'}-{job['compiler-version']}"
        result = dict(job=label, compiler=cxx, dependency_mode='system' if args.system_deps else 'vcpkg')
        results.append(result)
        if not shutil.which(cxx):
            result['status'] = 'skipped' if args.allow_missing else 'failed'
            result['reason'] = f'compiler not found: {cxx}'
            print(f"{label}: {result['reason']}", flush=True)
            continue
        build = output / label
        flags = [f'-DCMAKE_CXX_COMPILER={shutil.which(cxx)}', f"-DCMAKE_CXX_STANDARD={job['cxx']}",
                 '-DCMAKE_BUILD_TYPE=Release', f'-DREFLECTCPP_BUILD_MODULES={args.modules}',
                 f"-DREFLECTCPP_USE_VCPKG={'OFF' if args.system_deps else 'ON'}"]
        cc = cxx.replace('clang++', 'clang').replace('g++', 'gcc')
        if cc != cxx and shutil.which(cc):
            flags.append(f'-DCMAKE_C_COMPILER={shutil.which(cc)}')
        fmt = job['format']
        if fmt == 'benchmarks':
            flags += ['-DREFLECTCPP_BUILD_BENCHMARKS=ON', '-DREFLECTCPP_ALL_FORMATS=ON']
        elif fmt == 'headers':
            flags += ['-DREFLECTCPP_CHECK_HEADERS=ON']
        else:
            flags += ['-DREFLECTCPP_BUILD_TESTS=ON']
            if fmt != 'JSON':
                flags += ['-DREFLECTCPP_JSON=OFF', f'-DREFLECTCPP_{fmt}=ON']
        commands = [[cxx, '--version'], ['cmake', '-S', str(ROOT), '-B', str(build), '-G', 'Ninja', *flags],
                    ['cmake', '--build', str(build), f'-j{args.jobs}']]
        if fmt not in ('benchmarks', 'headers'):
            commands.append(['ctest', '--test-dir', str(build), '--output-on-failure', '--no-tests=error', f'-j{args.jobs}'])
        if fmt == 'benchmarks':
            for kind in ('all', 'json'):
                filters = ('canada_read', 'canada_write', 'licenses_read', 'licenses_write', 'person_read', 'person_write') if kind == 'all' else ('person_read', 'canada', 'licenses')
                for benchmark_filter in filters:
                    commands.append([str(build / f'benchmarks/{kind}/reflect-cpp-{kind}-format-benchmarks') if kind == 'all' else str(build / 'benchmarks/json/reflect-cpp-json-benchmarks'), f'--benchmark_filter={benchmark_filter}'])
        started = time.monotonic()
        print(f'Running {label} with {cxx}', flush=True)
        with (output / f'{label}.log').open('w') as log:
            for command in commands:
                log.write('$ ' + shlex.join(command) + '\n')
                log.flush()
                try:
                    code = subprocess.call(command, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
                except OSError as error:
                    log.write(str(error) + '\n')
                    code = 127
                if code:
                    result.update(status='failed', command=command, exit_code=code)
                    break
            else:
                result['status'] = 'passed'
        result['seconds'] = round(time.monotonic() - started, 1)
        result['log'] = str(output / f'{label}.log')
        (output / 'results.json').write_text(json.dumps(results, indent=2) + '\n')
        print(f"{label}: {result['status']} ({result['seconds']}s); {result['log']}", flush=True)
    (output / 'results.json').write_text(json.dumps(results, indent=2) + '\n')
    return int(any(result['status'] == 'failed' for result in results))


if __name__ == '__main__':
    sys.exit(main())
