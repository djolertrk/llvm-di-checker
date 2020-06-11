# [PROPOSAL] LLVM DI Checker

The ``LLVM DI Checker`` checks Debug Info Preservation in Optimizations.

NOTE: This is based on LLVM project and the patches could applied on the 872c5fb14324 commit.

The idea is to create a tool (utility; LLVM Pass) that checks the preservation of the debug info metadata after optimization passes. An alternative to this is the ``debugify`` utility, but the diference is that the ``LLVM DI Checker`` deals with real debug info, rather than with the syntetic ones (artifitial ones; basically, the ``debugify`` generates syntetic debug info before a pass and checks if that was preserved after the pass).

## How it works?

The ``LLVM DI Checker`` contains a pair of passes where one of them collects the debug info before each pass, and the other one checks if the pass dropped (or didn't generate) the debug info. Currently, it supports checking of ``DISuprogram`` and ``dbg-location`` related bugs only. We can extend the support for the other metadata as well.

Further more, it is implemented as an IR pass, but it can handle (with some extra work) the Machine IR level debug info checking as well.

The report about the bugs have been found is printed out on the ``std::err`` or into the ``json`` file. The ``json`` file could be further on processed, and as a final result we can have an HTML page representing the information about the bugs. Please find more details below.

## Building the LLVM DI Checker

Steps we recommend for GNU/Linux (or any Unix) OS:

1) Set up the CC and CXX env variables
2) Set up build dirs

       $ mkdir build && cd build
       $ cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="llvm;clang;libcxx;libcxxabi;" -DLLVM_ENABLE_LIBCXX=ON -DLLVM_TARGETS_TO_BUILD="X86" $PATH_TO_DI_CHECKER_PROJECT/llvm-di-checker/llvm-project/llvm
       $ ninja && ninja check-all

Please consider the page: https://llvm.org/docs/GettingStarted.html for additional details.

## Using the LLVM DI Checker

There are several ways of using it, either from ``opt`` or from ``clang``.

1) From the ``opt`` tool and 

        $ opt -O2 -di-checker test.ll -S -o test-processed.ll
        Force set function attributes: PASS
        Infer set function attributes: PASS
        Interprocedural Sparse Conditional Constant Propagation: PASS
        Called Value Propagation: PASS
        Global Variable Optimizer: PASS
        Promote Memory to Register: PASS
        Dead Argument Elimination: PASS
        Combine redundant instructions: PASS
        Simplify the CFG: PASS
        Globals Alias Analysis: PASS
        Remove unused exception handling info: PASS
        Function Integration/Inlining: PASS
        OpenMP specific optimizations: PASS
        Deduce function attributes: PASS
        SROA: PASS
        Early CSE w/ MemorySSA: PASS
        Speculatively execute instructions if target has divergent branches: PASS
        Jump Threading: PASS
        Value Propagation: PASS
        Simplify the CFG: PASS
          ***ERROR: Combine redundant instructions dropped DILocation for the   %add = or i32 %0, %reass.add (BB name: entry) from file: simple.c
        Combine redundant instructions: FAIL
        Conditionally eliminate dead library calls: PASS
        PGOMemOPSize: PASS
        Tail Call Elimination: PASS
        Simplify the CFG: PASS
        Reassociate expressions: PASS
        Rotate Loops: PASS
        Loop Invariant Code Motion: PASS
        Unswitch loops: PASS
        Simplify the CFG: PASS
        Combine redundant instructions: PASS
        Induction Variable Simplification: PASS
        Recognize loop idioms: PASS
        Delete dead loops: PASS
        Unroll loops: PASS
        MergedLoadStoreMotion: PASS
        Global Value Numbering: PASS
        MemCpy Optimization: PASS
        Sparse Conditional Constant Propagation: PASS
        Bit-Tracking Dead Code Elimination: PASS
        Combine redundant instructions: PASS
        Jump Threading: PASS
        Value Propagation: PASS
        Dead Store Elimination: PASS
        Loop Invariant Code Motion: PASS
        ...

2) From the ``opt`` tool by outputing the failures into the ``json`` file (The ``json`` file conatins per line JSON objects that is parsed by the ``di-checker.py`` for the purpose of generating an HTML page with the data).

        $ opt -O2 -di-checker -di-checker-export=test.json test.ll -S -o test-processed.ll
        ...
        Combine redundant instructions: FAIL
        Conditionally eliminate dead library calls: PASS
        ...
        $ cat test.json
        {"file":"simple.c", "pass":"Combine redundant instructions", "bugs": [[{"action":"drop","bb-name":"entry","fn-name":"fn2","instr":"shl","metadata":"DILocation"},{"action":"drop","bb-name":"entry","fn-name":"fn2","instr":"or","metadata":"DILocation"}]]}
        $ llvm-di-checker/di-checker.py test.json di-checker-example.html
        The di-checker-example.html generated.
 
3) The same checking can be enabled from ``clang`` by using driver (cc1) options as following

        $ clang -g -O2 -Xclang -fenable-di-checker test.c
Or,

        $ clang -g -O2 -Xclang -fenable-di-checker -Xclang -fexport-di-checker-info=~/di-checker-report-bugs.json test.c


### Using the LLVM DI Checker on GDB project (version 7.11)

This section will show the usage of the tool on the real example such as GDB 7.11.

1) Download the source code of the GDB 7.11 from https://www.gnu.org/software/gdb/
2) Set up build dir and run the ``make``
  
       $ mkdir build && cd build
       $ ../gdb-source/configure CC=$PATH_TO_DI_CHECKER_BUILD/bin/clang CXX=/$PATH_TO_DI_CHECKER_BUILD/bin/clang++ CFLAGS="-g -O2 -Wno-error -Xclang -fenable-di-checker -Xclang -fexport-di-checker-info=~/gdb-report-bugs.json" CXXFLAGS="-g -O2 -Wno-error -Xclang -fenable-di-checker -Xclang -fexport-di-checker-info=~/gdb-report-bugs.json" --enable-werror=no
       $ make -j3
4) Generate the HTML page with the data about bugs

       $ $PATH_TO_DI_CHECKER_BUILD/di-checker.py ~/gdb-report-bugs.json gdb-report-bugs.html
5) Please take a look at the example (actually piece of the page) of the ``gdb-report-bugs.html`` at: https://djolertrk.github.io/di-checker-html-report-example/

6) A particular case could be examined by using the ``-fenable-di-checker`` option only on the file where the bug occured (so we can see what is the instruction(s) that caused the problem)

       $ clang -g -O2 -Wno-error -Xclang -fenable-di-checker   -I. -I../../gdb-source/gdb ... ../../gdb-source/gdb/p-valprint.c
       ...
       Promote Memory to Register: PASS
       Dead Argument Elimination: PASS
         ***ERROR: Combine redundant instructions dropped DILocation for the   %7 = zext i1 %6 to i32 (BB name: no-name) from file: ../../gdb-source/gdb/p-valprint.c
       Combine redundant instructions: FAIL
         ***ERROR: Simplify the CFG dropped DILocation for the   br label %133 (BB name: no-name) from file: ../../gdb-source/gdb/p-valprint.c
       Simplify the CFG: FAIL
       Globals Alias Analysis: PASS
       ...

The tool has found **17924** cases that may indicate bugs within compiler.

