# Tests for flexbuffers

This folder contains the tests for flexbuffers. A Docker-based build pipeline is provided for Linux users.

Build the Docker container containing the build environment:

```
./build_docker.sh
```

Compile the tests:

```
./compile.sh
```

Run the tests:

```
./build/reflect-cpp-tests
```

To run the scripts, you may have to run `chmod +x build_docker.sh` and `chmod +x compile.sh` first.
