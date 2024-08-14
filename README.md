[TOC]

# Boiler-plate code for MIPS32 emulator

Here you should find enough information to get you started with the code provided by your instructors.

**Important note** In the following, a command that you have to type in the GNU/Linux *terminal* is preceded by a *dollar* sign (which you should not type!), and the output of the command possibly follows on the next line. For example:
```bash
$ command
output
of
the
command
execution
```

A command you have to type in the MIPS *emulator* is preceded by ">>". For example:

```bash
>> disp mem map
```

---
# Directory structure

- `root`
  - `Doxyfile`: Basic configuration file for the documentation (**do not modify**)
  - `Makefile`: Building instructions for the emulator (from the `C` code, **do not modify**)
  - `simpleUnitTest.sh` : `bash` script to run emulator tests
  - `README.md`: Basic documentation file (what you are currently reading, **do not modify**)
  - `doc/html/index.html` : Main entry point of browsable documentation
  - `elf` : MIPS ELF files.
  - `include/` : Emulator `C` header files
    - `elf/` : `ELF` loader `C` header files (**do not modify**)
  - `script/` : Emulator scripts, not for testing
  - `src/` : Emulator main `C` source code
    - `commands/` : Emulator commands `C` source code
    - `elf/` : `ELF` loader `C` source code (**do not modify**)
    - `insns/` : `MIPS32` instructions emulation `C` source code
  - `testing/` : Emulator testing files (commands and MIPS assembly)


---
# Compiling the emulator code

The code may be built in two modes: `debug` and `release` modes.

## Compiling in debug mode

The `debug` version should be used when developing your code, as it:
  - enables debugging with `gdb` (*resp.* `lldb`), if `gcc` (*resp.* `clang`) is your `C` compiler;
  - produces more verbose output: the `DEBUG` macro becomes active, as well as the [`assert(3)`](https://www.man7.org/linux/man-pages/man3/assert.3.html) mechanism.

To compile the `debug` version of the code, type:

```bash
$ make debug
```

## Compiling in release mode

The `release` version suppresses the above features and should be used to generate production code.

To compile the `release` version of the code, type:

```bash
$ make release
```

## Cleaning before compiling

To clean the emulator project directory, type:

```bash
$ make clean
```

Therefore, you may want to always clean before compiling your code:

```bash
$ make clean && make debug
```

# Running the emulator

Once your emulator is compiled, an executable called `emulmips` is produced.

## Getting help

Type:

```bash
$ ./emulmips -h
```

## Executing the emulator as an interpreter

Type:

```bash
$ ./emulmips [-v|-vv|-vvv]
```

The `-v` (*resp.* `-vv`, `-vvv`) option sets the verbosity level. See [below](@ref secdoc) for more more information on the verbosity level.

## Running an emulator script

To run a scrit calld `my-script` in the `script` directory, type:

```bash
$ ./emulmips script/my-script |-v|-vv|-vvv]
```

The `-v` (*resp.* `-vv`, `-vvv`) option sets the verbosity level. See [below](@ref secdoc) for more more information on the verbosity level.

# Documentation

## Required software

The documentation is built using [`doxygen`](https://www.doxygen.nl). In case you want to (re)build the documentation, make sure `doxygen` is installed on your system. For example, type:
  - on Debian/Ubuntu systems:
```bash
$ sudo apt install doxygen
```
  - on RedHat/CentOS systems:
```bash
$ sudo yum install doxygen
```

For other systems, check their manual to learn how to install additional software.

## Building the documentation

Once `doxygen` is available, type:

```bash
$ make documentation
```

## Documenting your code

It is (very) good practice to document your code. Nearly every single entity in the code we provide has been documented, so you should find it easier to do the same with yours! In case of trouble, please refer to the [Doxygen manual](https://www.doxygen.nl/manual/docblocks.html).

## Accessing the documentation

Once the documentation is (re)built, open `doc/html/index.html` in your browser.

---

# Interpreter commands

## Commands already available

The code we provide already implements two interpreter commands:
  - `bar` : a command to demonstrate how to write commands,
  - `load` : the *skeleton* of the `load` command,
  - `exit` : Allow to get out of the interpretor,
  - `disp` : Displays on the screen either a memory map, a certain wide of memory given by the operator, or the registers given by the operator,
  - `set` : Allows to write a value on a memory location if it's a byte or a word and it can also write a word on a register location,
  - `assert` : Check if the value entered by the operator is at a certain memory location, or a register location,
  - `debug` : Stop a script when it encounter "debug",
  - `resume` : When a script is stopped, typing "resume" will resume the script where it stopped,


## Adding a command to the interpreter

Suppose you want to add command `foo` to the interpreter. The following steps shall be required:
1. Create a new file called `src/commands/cmd_foo.c`;
2. In `src/commands/cmd_foo.c`, code your logic in the function called :
```C
cmd_rval_t cmd_foo( interpreter interp );
```
3. Add the above prototype to `include/commands.h`;
4. Add the following snippet to the function [execute_cmd](@ref execute_cmd) in `src/emulmips.c`:
```C
  if ( 0 == strcmp( token, "foo" ) ) {
    return cmd_foo( interp );
  }
```

Feel free to get inspired by the commands already available!

*Notes*
1. As far as `git` is concerned, do not forget to:
```bash
$ git add src/commands/cmd_foo.c
$ git commit -am "Added src/commands/cmd_foo.c"
```
2. In `src/commands/cmd_foo.c`, add:
```C
#include "commands.h"
```

# MIPS32 instructions

This shall be discussed later in class in more details. Yet, this section summarizes the guidelines on how to add a new MIPS32 instruction to the emulator.

The code we provide does not include any `MIPS32` instructions. However, suppose you want to add instruction `bar` to the emulator. The steps resemble the above:
1. Create a new file called `src/insns/insn_bar.c`;
2. In `src/insns/insn_bar.c`, code the instruction behavior in the function called:
```C
int insn_exec_bar( interpreter interp, uint32_t insn );
```
and code the instruction disassembly in the function:
```C
int insn_disasm_bar( interpreter interp, uint32_t insn );
```
3. Add the above two prototypes to `include/insns.h`;
4. Insert the new `bar` instruction into your emulator logic.

*Notes*
1. As far as `git` is concerned, do not forget to:
```bash
$ git add src/commands/cmd_foo.c
$ git commit -am "Added src/commands/cmd_foo.c"
```
2. In `src/insns/insn_bar.c`, add:
```C
#include "insns.h"
```


# Handling verbosity... {#secdoc}

By default, software is supposed to execute without printing anything, except warnings and errors. However, you may want to give the user the ability to monitor more precisely what is going on during execution, and to do so in an incremental fashion. This is called *setting the verbosity level*.

The basic mechanism of the verbosity level is already implemented. You only have to learn how to use it and how to activate it, which is fairly straightforward.

## ... from the C code

The verbose level is a global `C` variable (an `int`) in the code that is called `verbose_level`. It can be set from `0` (the `INFO` macro does not print anything) to `3` (maximum verbosity).

If you only want to use one level of verbosity, which is perfectly fine, just use the `INFO` macro in your code:
```C
INFO( "I'm doing this!\n" );
```

In the pretty rare cases you want to use higher levels of verbosity, control them like this:
```C
if ( verbose_level > 1 ) {
  INFO( "In more details!\n" );
}
if ( verbose_level > 2 ) {
  INFO( "In many more details!\n" );
}
```

## ... from the command line

When the user wants to enable verbose output, he/she should use the `-v` (*resp.* `-vv`, `-vvv`) option of the `emulmips` executable to set the `verbose_level` global variable to `1` (*resp.* `2`, `3`).

---
# Tests

*No one could stress enough the importance of testing code thoroughly!* -- Bill Gates upon releasing Windows 2000 (kidding).

## Why software testing is both mandatory and hard

Most users will discover your program and its capabilities by trial and error. That's the sad rule of most people not reading the documentation in the first place. Therefore, *you must assume your code should properly handle any possible input and react accordingly*. People only use software when it is documented and *robust* -- and only after they can decide if it is good enough for their purpose. Two corollaries follow directly:
1. Use the [*Principle of Least Surprise*](https://en.wikipedia.org/wiki/Principle_of_least_astonishment);
2. Useful tests are written only by dedicated people.

The *Principle of Least Surprise* basically states that *your code should behave as a naive user would think it should behave without reading the doc*. We've got you covered on this part, since the interpreter syntax is designed this way (and so is the command line interface of `emulmips`).

The smallest feature you add to your code (a new command or a new instruction) *must be tested in every conceivable way*. That's a golden rule. This is a difficult task because it involves finding and testing all corner cases in addition to the normal use cases. This is what dedicated people excel at.

Software testing is a domain of its own. Yet, two kinds of tests are regarded as mainstream:
1. *Unit tests*: they are designed to test a single feature of the code (a command or an instruction) -- think of unit tests as low-level tests;
2. *Regression tests*: they are designed to ensure adding a new feature to the code does not break it -- think of regression tests as high-level tests.

Your tests should encompass both situations where a correct input is provided (and your code should pass these tests), and situations where an incorrect input is provided by the user (and your code should fail these tests).

## A simple script to perform tests

We provide a script called `simpleUnitTest.sh` which you will use to write your own unit and regression tests using the emulator in script mode. Store your tests in the `testing/` directory.

Tests can be launched by providing `simpleUnitTest.sh` with the name of the emulator executable (after option `-e`) and the list of the test scripts:
- To launch tests one by one:
```bash
$ ./simpleUnitTest.sh -e ./emulmips ./testing/*.emu
```
- To launch tests in one batch:
```bash
$ ./simpleUnitTest.sh -e ./emulmips -b ./testing/*.emu
```

The script also comes with a builtin help that documents advanced options:
```bash
$ ./simpleUnitTest.sh -h
```

## How to write a test {#secwritetests}

Given the above, writing a test is a fairly simple operation. Everything happens in the test scripts that will be executed by the emulator.

### Mandatory step

Each test script *must* begin with at least *one comment* (*i.e.*, starting with a #) that will be read by *simpleUnitTest.h*.

This comment must be in the form:
```bash
# TEST_RETURN_CODE={val}
```
where `{val}` takes one of the following values:
- `PASS`, indicating that the test is expected to pass;
- `FAIL`, indicating that the test is expected to fail;
- `DO_NOT_CHECK`, indicating that the test result should not be taken into account.

### Optional step

Next, you may want to include another comment indicating the feature the test refers to. Introduce a test comment in a script comment using:
```bash
# TEST_COMMENT="Your wonderful string explaining what you're testing"
```

## Examples

### Using the provided test scripts

We provide only a few test scripts for the `bar`, `exit` and `load` commands.

They will all need you to write the `exit` command before you can use them.

After that, you can test the `load` command with:
```bash
$ ./simpleUnitTest.sh -e ./emulmips -b testing/cmd/load/*.emu
```

And the `bar` command with:
```bash
$ ./simpleUnitTest.sh -e ./emulmips -b testing/cmd/load/*.emu
```


### Building your own

Imagine you have to test the interpreter command called `foo`, which is supposed to be successful if provided solely with the argument `1` and unsuccessful otherwise. In your `C` code, this means that the function `cmd_foo` will return `CMD_RVAL_OK` if and only if there is a single argument which is `1`, otherwise it will return `CMD_RVAL_ERROR`. Now, in this simple toy example, we will have to generate four test scripts.

Note that for easier management, you should store all test files pertaining to the `foo` command in the `testing/cmd/foo/` directory and you may want to number them as follows.

The four test scripts we must write are:
- `00_no_arg.emu`: to test whether the `foo` command was given no argument;
- `01_too_much_args.emu`: to test whether the `foo` command was given more than one argument;
- `02_not_one.emu`: to test whether the `foo` command was given one argument and it is not `1`;
- `03_success.emu`: to test whether the `foo` command was given one argument and it is `1`.

Let's now list the contents of `00_no_arg.emu` with the standard `cat` command of the terminal:
```bash
$ cat testing/cmd/foo/00_no_arg.emu

# TEST_RETURN_CODE=FAIL
# TEST_COMMENT="Testing command foo with no arguments."

foo
```

The other test files are straightforward. Testing for too much arguments:
```bash
$ cat testing/cmd/foo/01_too_much_args.emu

# TEST_RETURN_CODE=FAIL
# TEST_COMMENT="Testing command foo with too much arguments."

foo 1 2 3 4
```

Testing for the right number of arguments, but providing a wrong one:

```bash
$ cat testing/cmd/foo/02_not_one.emu

# TEST_RETURN_CODE=FAIL
# TEST_COMMENT="Testing command foo with wrong argument."

foo 42
```

Finally, testing for the only valid input:

```bash
$ cat testing/cmd/foo/03_success.emu

# TEST_RETURN_CODE=PASS
# TEST_COMMENT="Testing command foo correctly at last!"

foo 1
```

One can now launch the tests all at once like:
```bash
$ ./simpleUnitTest.sh -e ./emulmips -b ./testing/cmd/foo/*.emu
```

Watch the output in the terminal to find out which test(s) failed miserably! You now have a way to easily spot which part of the `cmd_foo` function is possibly flawed, or even if it behaves correctly (which may eventually happen!)

With some experience, you will quickly find idiomatic expressions for writing your interpreter commands in `C` (how to make sure the required number of args is present, *etc.*) Once these `C` code patterns are validated, the number of tests you'll have to write will dramatically decrease and allow you to only concentrate on more involved issues.

## Relation to the assert interpreter command

The `assert` command will test whether a register or a memory location holds the expected value. You should use it to write tests like:
```bash
$ cat testing/mytest.emu

# TEST_RETURN_CODE=PASS
# TEST_COMMAND="Ensuring the register $v0 (resp. $v1) contains 0x42 (resp. 0x10) after running elf/myelffile.o"

load elf/myelffile.o
run
assert byte 0x42 $v0
assert byte 0x10 $v1
```

This way, you are able to test for multiple expected values in registers or memory locations and the first `assert` command that fails will exit the script with an error.

This should have made obvious why the `assert` interpreter command is *very* important and should be one of the first to be up and running correctly: this is the way to go when testing MIPS32 instructions!

---
# Reporting bugs and requests for improvements

## Bugs

No bug bounty program has been set up. Yet, in case you find a tricky bug, we might however consider giving you extra point(s) ;-)

If you find a bug in this code, please report it with a detailed description along with a minimal example allowing to reproduce it to [François Cayre](mailto:francois.cayre@grenoble-inp.fr?subject=[emulmips] Looks like a bug!).

## Requests for improvements

Also, feel free to report requests for improvement (either in the code or in the documents) to [François Cayre](mailto:francois.cayre@grenoble-inp.fr?subject=[emulmips] Request for improvement).
