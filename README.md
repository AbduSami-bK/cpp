# Debuggable C++ Template

A modern C++ template with logging, CLI args, configs, signals, optional HTTP API, tests, and CI.

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/main \
	--config configs/config.json \
	--enable-api \
	--log-level debug
```
