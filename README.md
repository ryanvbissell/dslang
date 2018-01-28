
DSLANG
========
An embeddable, extensible Scheme interpreter for C++

It is very much in an early stage of development, and will likely
change very drastically from one release to the next.  This may limit
its usefulness to you.

But in the event that it is, I offer it under a BSD-2-Clause license.
(See the enclosed 'LICENSE' file for exact license terms.)

It is intended to be grafted into other projects, such as via a git
submodule.  Its dependencies are:

* My 'make-forge' project
* My 'cx' project
* C++17 compiler (tested w/ G++ on Linux)
* GNU Make 4.1 or better

My 'make-forge' and 'cx' submodules must also be fetched whenever
you clone this repo:

    git clone git@github.com:ryanvbissell/dslang.git
    cd dslang
    git submodule update --init

I don't recomment `git clone --recursive`, as that will pull down a
redundant copy of make-forge for CX (which *would* be needed if you
were hacking on CX, but in this case DSLANG's own make-forge
instance suffices for both, when building DSLANG.)

Each submodule (generally, under ./sub) has its own respective copyright
and license terms.  Please refer to the appropriate files for more info.
