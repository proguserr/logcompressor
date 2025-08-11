
# logcompressor (C++)

A fast, block-based log compressor with structure-aware pre-pass and pluggable entropy (LZ4).

## Build

```bash
cmake -S . -B build
cmake --build build -j
./build/logc --help
```

## Quick start

```bash
# compress
./build/logc -i input.log -o output.lc --codec lz4 --block 512k --threads 8

# decompress
./build/logc -d -i output.lc -o restored.log

# verify
./build/logc --verify -i output.lc
```

## Notes
- Uses FetchContent to pull **xxhash** and **lz4** at configure time.
- Format: header + blocks + footer index for random access.
- Integrity: xxhash32 per raw block.
