# Moonborne Source Structure

The source for Moonborne is found here, in the `moonborne/src` directory. Here, the Makefile, when run, compiles `main.cpp`, which includes all the hand-written artisan header files found in `src/`, `ncurses.h`, and many headers from the C and C++ standard libraries.

The source tree is as follows:

```
moonborne/
|- README.md
|- src/
|
|--- Makefile
|--- STRUCTURE.md
|
|--- main.cpp
|--- person.h
|--- render.h
|--- vcp.h
|
|--- app (executable, created upon compile)
|
|--- config/
|----- TBD, will contain configuration files for the engine and game
```

### `person.h`

The header `person.h` contains the definition of the struct `Person` and support structure(s), the main type for describing people in Moonborne. It contains functions relating to creating (with the notable exception of `vcp.h`) and managing people.

### `render.h`

The header `render.h` contains all (with the exception of `vcp.h`) functions that deal with text rendering via ncurses. It has both window-generic and window-specific functions, and as of yet no special data types.

### `vcp.h`

The header `vcp.h` contains the function `visual_create_person()`, which uses the interact window to do an interactive ("visual") traditional character-creation screen. Because this is going to end up being a *long* function, and it could really fit into either `render.h` (because it uses the TUI) or `person.h` (because it returns a `Person`), I put it into a separate file to make it more manageable.

---

### Dependencies

Moonborne depends on `ncurses`, a standard (but not part of *the* standard) library for doing TUI stuff, and [TOML11](https://github.com/toruniina/toml11), a TOML parsing library, as well as the following C/C++ standard library headers:

```
STANDARD LIBRARIES (cross-platform)
stdio.h, stdlib.h, string.h, time.h (C)
vector, string, sstream (C++)

LINUX-SPECIFIC HEADERS
sys/ioctl.h, unistd.h (C)

LIBRARIES
ncurses.h (C)
toml11/toml.hpp (C++)
```

Note again that as of now this project is Linux-only, I plan to port this to Windows and potentially Mac (I don't have a Mac to test on). You're also free to do it yourself, and I'll gladly work with you to get it ported wherever.
