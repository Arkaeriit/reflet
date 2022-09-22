; This programs test the unaligned memory access on a 32-bit CPU

; RAM starts at 0x1000

; Writing the RAM address and some patterns
setr R1 0x1000
setr R2 0xABCDEF00
setr R3 0x01020304

@define inc_R1 1
    set $1
    add R1
    cpy R1
@end

; Testing 32 bits writes
read R2
str R1
inc_R1 4

read R3
str R1
inc_R1 4

; Testing 16 bit writes
set 4
cpy SR

read R2
str R1
inc_R1 2

read R3
str R1
inc_R1 2

; Testing 8 bit writes
set 6
cpy SR

set 10
str R1
inc_R1 1

set 11
str R1
inc_R1 1

set 12
str R1
inc_R1 1

set 13
str R1
inc_R1 1

; Reading the 4 word written
set 0
cpy SR
setr R1 0x1000

load R1
debug
inc_R1 4
load R1
debug
inc_R1 4
load R1
debug
inc_R1 4
load R1
debug


quit
