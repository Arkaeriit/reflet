@define swapBytes 2
    @rawbytes $2 $1
@end

@define doubldeByte 1
    @rawbytes $1 $1
@end

@label start
@import code.asm
@label endCode
@align 16
@labref coucou
@align 16
@label coucou
@rawbytes 12 34 56 78 9A BC DE F0
@labref coucou
@labref start
@labref endCode

swapBytes 11 88
doubldeByte 22

