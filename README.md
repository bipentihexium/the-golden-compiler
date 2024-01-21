# The Golden compiler

This is a compiler for a programming language The Golden ([official interpreter and documentation](https://github.com/Pandicon/The-Golden)) made with LLVM.

- [The Golden compiler](#the-golden-compiler)
	- [Usage](#usage)
		- [Options](#options)
		- [Supported The Golden versions](#supported-the-golden-versions)
	- [Building from source](#building-from-source)
	- [The Golden E doc](#the-golden-e-doc)

## Usage

```sh
tgc [options] <sources files...>
```

The compiler produces object file generated from code in source files.

In order to use the generated code, you'll need to have a C++ compiler installed in order to link the object file with `libtgcore` (library which is part of the compiler and on which the generated code depends), `libc` (C's standard library) and `libm` (C's standard math library).

The usual compilation of single-file program then looks like this

```sh
# compile TG source
tgc [options] program.au -o program.o
# link the program
g++ -L/path/to/libtgcore program.o -ltgcore -o ./program
# run the program
./program
```

### Options

option | effect
--- | ---
`--help` | show help
`--version` | show version
`-v [ --verbose ]` | show verbose output
`--ver version` | set TG version
`-o [ --output ] output_file` | set output file
`--input-file arg` | add input file
`--show-ir` | show LLVM IR of compiled modules
`--show-instrs` | show TG instructions (for debugging the compiler)
`--optimize` | run LLVM O2 optimization passes on the IR


### Supported The Golden versions

For now, the only supported TG version is `0.4.0E` (which is different from the [official interpreter's](https://github.com/Pandicon/The-Golden) `0.4.0`, but it's not far from it).

## Building from source

You'll need to have a C++ compiler, CMake, Boost (library) and LLVM (library) installed (findable by CMake's `find_package` and with working `llvm-config`). Then you can build the compiler from this directory:

```sh
cmake -S. -Bbuild --config Release
cmake --build build
```

The compiler and libtgcore will then be in `./out/`.

## The Golden E doc

[The Golden 0.4.0E doc is here](doc/the_golden_e/tg_0_4_0e.md)
