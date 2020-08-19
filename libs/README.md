# Description

Builds all required dependencies from source for compiling Forsaken (http://fly.thruhere.net).


## Why not use system packages ?

You can of course choose to as long as it properly manages pkgconfig for each of the packages since that's how the makefile was designed.

Although over time we've seen each distribution build certain things in odd ways and/or have bugs:

* bad support for targeting multiple cpu architectures
* missing support for certain compile time features
* sloppy / broken / incomplete pkconfigs
* missing static libraries for many packages
* pulse audio bugs in ubuntu that took ages for them to fix


## OSX

Getting all these libraries to work on osx can be a ton of fun with ports/brew madness and not all of them are easy or available.

This folder makes it much easier and it also supports patching some things up where needed to get around osx bugs.

## Win32

Windows support is in a similar situation as OSX and since most Forsaken developers primarily use unix systems we generally cross compile using mingw from linux targeting win32.  This makes it easy for anyone on windows/linux/osx using mingw to easily build in the same unified way..

Another repo similar to this has pre-compiled libraries for win32 but will soon be merged into this project allowing for a reproducable build for win32 libraries via gcc/mingw.


## Cross platform

Hence these helper scripts give us a baseline that allows us to easily compile all required dependencies from scratch for any target system.

This also makes it much easier when time comes to port to other devices such as Android, Pandora, Emscripten, PS3, Xbox, ARM, etc...


## Easy to get started

It takes all the headace out of maintaing a developer setup guide.  Devs just clone this along with Forsaken and it magically works, it's clean straight forward build scripts, they can get up to speed easily.

# Usage

The basic convention is to clone this to your [forsaken](https://github.com/ForsakenX/forsaken) source folder
as `libs`.

Then the simple next step is to run `./libs/src/build.sh` but please finish reading the sections below as you
may desire to build using other methods.

## Multiple copies

Currently the default build script will build for your native system type hence if your using 64bit os you'll be buiding 64bit binaries.

You obviously don't want to constantly rebuild this folder every time you want to push a 32bit, 64bit or mingw32 version.

You can simply create multiple clones of this project and update prefix= in the pkgconfig files *.pc with the new pathname that you have chosen.

```shell
sed -i "s/prefix=.*/prefix=$new_path/g" lib/pkgconfig/* src/pkgconfig/*
```

## Scripts

* ./src/build.sh              = build everything.
* ./src/32bit-build.sh        = build everything with -m32.
* ./src/em-build.sh           = build everything using emscripten.
* ./src/SDL-2.0.build.latest  = clone latest SDL from HG repo and build.
* ./src/clean.sh              = run `make clean` everywhere.
* ./reset.sh                  = reset this folder back to clean state.
* ./pkgconfig.sh              = run pkgconfig pointing to this folder for testing.
* ./make.sh                   = run make on Forsaken with PKG_CONFIG_PATH set properly.
* ./run.sh                    = run Forsaken with {LD,DYLD}_LIBRARY_PATH set properly.


## Running Forsaken

The convention is to place this folder in forsaken/src/libs and run:

```shell
./libs/make.sh && ./libs/run.sh
```

Generally you end up writing a custom wrapper script around this.

See the Forsaken Makefile for information on various build options you can pass.

### GDB

Note that run.sh uses gdb to launch Forsaken allowing you to catch runtime issues, generate stack traces, set breakpoints, watch variables, inspect the stack, etc.. 

### CGDB

A nice ncurses frontend to gdb that also tracks the current stack pointer through the source files and provides other more interactive features: http://cgdb.github.com/


## Building 32bit binaries on Ubuntu 64bit

* ./ubuntu64-ia32-pkgs.sh = install required packages to compile 32bit on 64bit ubuntu.
* ./ubuntu64-ia32-make.sh = compile 32bit forsaken on 64bit ubuntu

Due to a pending issue you must remove libs/lib/libSDL\* and -lSDLmain from libs/lib/pkgconfig/sdl.pc.

Ubuntu doesn't really support building 32bit and 64bit same time since attempting to install 32bit packages will replace the 64bit packages and vice versa which really increases your cycle time.

The scripts above gets around this by doing a few non standard symlinks.


## Mac OSX Linking

### Home Brew

Note that many build scripts expect gnu like utilities.

Particularly you'll need to alias find=gfind and sed=gsed.

### What happened to LD_LIBRARY_PATH ?

Normally on linux one would just set LD_LIBRARY_PATH.

Although on osx attempting to use DYLD_LIBRARY_PATH will cause the following error:

```
Symbol not found: __cg_png_create_info_struct
Referenced from: /System/Library/Frameworks/ApplicationServices.framework/
Versions/A/Frameworks/ImageIO.framework/Versions/A/ImageIO
Expected in: $libs/lib/libpng14.14.dylib
```

### Relative Paths

OSX generally requires absolute paths to be set in the binary.

You can get around this with tricks like @executable_path which will be resolved at runtime (see make.sh which does this for you).


### Listing Paths

Hence make sure dylib paths are all correct:

```shell
dyldinfo -dylibs ./projectx
```


#### Rewriting the Paths

[make.sh](https://github.com/ForsakenX/forsaken-libs/blob/master/make.sh) now does this for you by default.

It will use @executable_path so to run your binaries you will need to have the libs/lib folder in the current path.


#### Static Builds

We generally distribute static builds on linux and windows which makes things much easier for users.

But OSX does not support static linking...


#### Bundles

OSX bundles are really folders that end in a .app extension and at the desktop level appear as a single clickable app to the user.

This allows us to distribute the compiled libs along with forsaken and data files inside the bundle.

Inspect the current [Forsaken bundles](https://github.com/ForsakenX/forsaken/wiki/Quickstart) for an example.
