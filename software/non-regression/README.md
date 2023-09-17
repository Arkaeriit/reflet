# Non regression test

To ensure that the simulator and the assembler don't have regressions, some programs can be run and their output compared to expected values. Each of the tests can be run by the interpreter <TODO> and is written in the following way:

* It can start with an optional shebang to the interpreter.
* Then there is the command line that invoke `reflet-sim` with the desired arguments.
* Then for each line of output of the program there is one off the following:

|Command|Argument      |Effect|
|-------|--------------|------|
|`raw`  |Arbitrary text|The tester compares the raw text with the output if the simulator.|
|`mis`  |Nothing       |The tester wants a misaligned access message.|
|`dbg`  |A number      |The tester expects a debug instruction and checks the value of the working register against the argument.|
|`dbg*` |Nothing       |The tester expects a debug instruction but don't care about the value in the working register.|

