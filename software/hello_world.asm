@import libs/import_all_libs.asm

label hello
@string-nl-0 Hello, word!

label start
    setlab hello
    cpy R1
    callf print
    quit

