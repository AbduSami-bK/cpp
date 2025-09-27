# C++ Template

A C++ project template

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
[user0@COMPUTER cpp]$ ./build/main -h
C++ Template
Usage: ./build/main [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -c,--config TEXT [config/config.json]
                              Path to config file
  --dry-run [0]               Run without performing actions
  --http-api [0]              Enable HTTP API
  -l,--log TEXT [info]        Log level
  -v,--version                Show version and exit
```
