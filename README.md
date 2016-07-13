

## Test Suite for Fault Injection Experiments

To analyze a program's vulnerability to hardware faults, faults are typically simulated by _injecting_ erroneous data during program execution.
Since faults can be injected at any given point in time during program execution, the space of all possible faults grows with the size of the program that is being investigated.
The test programs in this suite have deliberately been kept small so that exploration of the fault space is possible with moderate resources.
While being small, the test programs represent typical algorithmic tasks such as sorting, tree/graph traversal, signal processing etc.
 
The source code for each test program is structured into at least two source files.
Let's say that the program's name is `test`.
Then there will be a `test.main.c` and a `test.enc.c` source file.
The file `test.main.c` contains the `main` function, which mostly performs book-keeping tasks such as marshalling inputs, timing, logging outputs.
The file `test.enc.c` contains a (usually small) number of functions that implement the algorithm to be subjected to fault injection.
If this test suite is used to study the effectiveness of a fault-tolerance scheme, the scheme should be applied to the source `test.enc.c`.
The infix `enc` in the file name derives from the fact that any fault-tolerance scheme will typically transform either the source code or the resulting binary, a process which we refer to as _encoding_.
 
At least one of the functions in `test.enc.c` is prefixed with `___enc_`.
These functions are intended to act as interfaces between encoded parts of the program (i.e. parts that have been treated with a fault-tolerance scheme) and un-encoded parts.
Hence only those functions in `test.enc.c` that are prefixed with `___enc_` are called from `main`.
   

### Getting started

The suite of test programs can be built with [CMake](https://cmake.org/).
A working installation of [LLVM](http://llvm.org) is required. 
Additionally, the build systems must be given the path of an executable that applies the fault-tolerance scheme under consideration.
In keeping with our terminology, this executable is referred to as the _encoder_.
The encoder is expected to operate on LLVM intermediate representation/bitcode.
(If fault-tolerance studies are not your major motivation for using this test suite, you may not have access to an encoder.
In this case you can simply use LLVM's `opt` binary as your encoder, which will perform no source or binary transformations.) 

The path of the encoder must be specified in the CMake option `ENCODER`.
The path to the LLVM installation should be specified in the environment variable `LLVM_DIR` during the first execution of `cmake`, for example:

  `LLVM_DIR=/usr/lib/cmake/llvm/ cmake <PATH-TO-FAULTS-TEST-SUITE> -DENCODER=/usr/bin/opt`

where it is assummed that LLVM has been installed under `/usr`.

The following CMake options also exist:

  1. `REPORT_CHECKSUM`: When set to `TRUE`, a checksum of the program results will be computed and output to the terminal.

  2. `REPORT_CYCLES`: When set to `TRUE`, the cycles spent executing each test program's core algorithm will be counted and output to the terminal.
     Note that cycle counting only works on x86 ISAs which support the instructions `rdtsc` and `rdtscp`.
     Our cycle counting follows the recommendations given in an [Intel white paper](http://www.intel.com/content/www/us/en/embedded/training/ia-32-ia-64-benchmark-code-execution-paper.html).

  3. `REPETITIONS` determines how many times each test program's core algorithm will be executed.
     More repetitions should reduce the variance of cycle counts.

  4. `LENGTH` specifies the size of input arrays for a number of test programs.

  5. `DES_INPUT_SIZE` specifies the size of the input for the `des` test program.

  6. `DIJKSTRA_NUM_NODES` specifies the size of the graph for the `dijkstra` test program.

  7. `FIBONACCI_LENGTH` specifies the input for the `fibonacci` test program.

  8. `MATMUL_LENGTH` specifies the length of the input vector for the matrix-vector multiplication test program `matmul`.

After a successful run of `make`, the directory `<BUILD-DIR>/tests` contains one folder for each test program.
This folder contains two executables: `test.plain` and `test.encoded`.
The executable `test.plain` is a standard build from the test program's sources using the LLVM toolchain.
During building of `test.encoded` the encoder has been applied to the source file `test.enc.c`, producing a fault-tolerant executable.
A number of build artifacts will also be created.
Most notably, `test.plain.s` and `test.encoded.s` are assembly files which may be useful for understanding and debugging the encoder.

There is also a `test.out` binary file, which contains results computed by a correct (i.e. non-faulty) execution of `test.plain`.
This is useful for analyzing individual fault-injection experiments since any fault that occurs during program execution may corrupt the results of the test program.
 

## Advanced: config files

The build system can be configured to generate config files, with the file name extension `cfg`.
Config files contain information about how fault-injection campaigns should be conducted.
To generate config files, the CMake option `BUILD_CONFIGS` must be set to `TRUE`.

In order to configure fault-injection campaigns, additional CMake options must be supplied.
Specifically, the build system assumes that a fault injector based on the [Intel Pin Tool](https://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool) is available.
The additional CMake options are:

  1. `PIN_COMMAND` is the command which runs the Pin Tool.
     Note that this may be different from the path to the Pin Tool's executable if additional command line options are given to the command which runs the Pin Tool.

  2. `BFI_SO_PATH` is the path to a Pin Tool plug-in which implements a fault injector.
     Pin Tool plug-ins are shared library objects.
     Example plug-ins for fault injection are the [BFI bit-flip injector](https://bitbucket.org/db7/bfi) and [FastFI](https://github.com/normanrink/fastfi).

  3. `SUITE_FOLDER` is the path to a folder where executables and artifacts of the fault-injection campaign should be stored.

When `BUILD_CONFIGS` is set to `TRUE`, running `make` produces a directory `<BUILD-DIR>/configs` containing configuration files for the entire suite of test programs.
Individual config files for single test programs are located below each test's folder.
That is, for a test program named `test` there is now a folder `<BUILD-DIR>/tests/test/configs`.
The config files can be parsed using Python's configuration file parser [`ConfigParser`](https://docs.python.org/2/library/configparser.html).


## Bug reports and suggestion

Bugs and suggestions for further development and improvement should be submitted to norman.rink@tu-dresden.de.
