#!/usr/bin/env python3
"""Compare PCH on/off for the same module importer across Clang versions."""

import argparse
import json
from pathlib import Path
import shlex
import shutil
import subprocess
import time


ROOT = Path(__file__).resolve().parents[1]
SOURCE = 'tests/json/test_add_namespaced_tags_to_rfl_variants.cpp'
TARGET = 'tests/json/CMakeFiles/reflect-cpp-json-tests.dir/test_add_namespaced_tags_to_rfl_variants.cpp.o'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', action='append', help='repeatable Clang executable')
    parser.add_argument('--build-root', default='build/pch-matrix')
    parser.add_argument('--jobs', type=int, default=3)
    parser.add_argument('--pch', choices=('ON', 'OFF', 'BOTH'), default='BOTH')
    parser.add_argument('--full', action='store_true', help='also build and run the full JSON/CLI tests')
    args = parser.parse_args()
    compilers = args.compiler or [f'clang++-{version}' for version in range(19, 23)]
    output = ROOT / args.build_root
    output.mkdir(parents=True, exist_ok=True)
    results = []
    for compiler in compilers:
        executable = shutil.which(compiler)
        if executable is None:
            results.append(dict(compiler=compiler, status='missing'))
            continue
        version = subprocess.check_output([executable, '--version'], text=True).splitlines()[0]
        for pch in (('ON', 'OFF') if args.pch == 'BOTH' else (args.pch,)):
            label = f'{Path(compiler).name}-pch-{pch}'
            build = output / label
            commands = [
                ['cmake', '-S', str(ROOT), '-B', str(build), '-G', 'Ninja',
                 f'-DCMAKE_CXX_COMPILER={executable}',
                 f'-DCMAKE_C_COMPILER={executable.replace("clang++", "clang")}',
                 '-DCMAKE_CXX_STANDARD=20', '-DCMAKE_BUILD_TYPE=Release',
                 '-DREFLECTCPP_USE_VCPKG=OFF', '-DREFLECTCPP_BUILD_TESTS=ON',
                 '-DREFLECTCPP_BUILD_MODULES=ON',
                 f'-DCMAKE_DISABLE_PRECOMPILE_HEADERS={"OFF" if pch == "ON" else "ON"}'],
                ['cmake', '--build', str(build), '--target', TARGET, f'-j{args.jobs}'],
            ]
            if args.full:
                commands += [
                    ['cmake', '--build', str(build), f'-j{args.jobs}'],
                    ['ctest', '--test-dir', str(build), '--output-on-failure',
                     '--no-tests=error', f'-j{args.jobs}'],
                ]
            result = dict(compiler=compiler, version=version, pch=pch,
                          source=SOURCE, scope='full' if args.full else 'importer',
                          status='passed', log=str(output / f'{label}{"-full" if args.full else ""}.log'))
            started = time.monotonic()
            print(f'Running {label}: {version}', flush=True)
            with Path(result['log']).open('w') as log:
                log.write(version + '\n')
                for command in commands:
                    log.write('$ ' + shlex.join(command) + '\n')
                    log.flush()
                    code = subprocess.call(command, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
                    if code:
                        result.update(status='failed', exit_code=code, command=command)
                        break
            database = build / 'compile_commands.json'
            if database.exists():
                entry = next((entry for entry in json.loads(database.read_text())
                              if entry['file'] == str(ROOT / SOURCE)), None)
                if entry:
                    result['compile_command'] = entry['command']
                    result['pch_in_command'] = '-include-pch' in entry['command']
            result['seconds'] = round(time.monotonic() - started, 1)
            results.append(result)
            if build.exists():
                individual = build / ('full-result.json' if args.full else 'pch-result.json')
                individual.write_text(json.dumps(result, indent=2) + '\n')
            report = output / ('full-results.json' if args.full else 'results.json')
            report.write_text(json.dumps(results, indent=2) + '\n')
            print(f'{label}: {result["status"]} ({result["seconds"]}s)', flush=True)
    report = output / ('full-results.json' if args.full else 'results.json')
    report.write_text(json.dumps(results, indent=2) + '\n')
    return int(any(result['status'] != 'passed' for result in results))


if __name__ == '__main__':
    raise SystemExit(main())
