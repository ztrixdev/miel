# Ivo

A cozy, beginner friendly language with systems capabilities

## Sections

- [Installation](#installation)
  - [By GCC](#by-gcc)
  - [By Zig](#by-zig)
- [License](#license)

## Installation

### By GCC

```bash
gcc src/*.c -o target/ivo.exe -O2 -Wall -Wextra -std=c23 -g
```

### By Zig

You will need Zig 0.16.0

```bash
zig build -Doptimize=ReleaseFast

# the result should be in zig-out/bin
```

## License

Ivo is licensed under [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0/). See [LICENSE](./LICENSE) for more details

