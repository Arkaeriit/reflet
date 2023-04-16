@import libs/import_all_libs.asm

label hello
@string Hello, word!
@rawbytes 0A 0

label start
    setlab hello
    cpy R1
    callf print
    quit

