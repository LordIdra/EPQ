# NOP
`NOP` | `00 0 0 0 0`
- No operation

# LDA
`LDA $A $B $C` <-> `01 A B C 0`
- Memory address (MSB to LSB) ABC
- Loads most significant half of specified address to MDR1
- Loads least significant half of specified address to MDR2

# STA
`STA $A $B $C` <-> `02 A B C 0`
- Memory address (MSB to LSB) ABC
- Stores MDR1 in most significant half of specified address
- Stores MDR2 in least significant half of specified address

# ADD
`ADD $A $B $C` <-> `03 A B C 0`
- C = A + B

# ADC
`ADDC $A $B $C` <-> `03 A B C 1`
- C = A + B
- If previous operation overflowed, add 1 to C

# SUB
`SUB $A $B $C` <-> `03 A B C 2
- C = A - B

# SBC
`SUBC $A $B $C` <-> `03 A B C 3`
- C = A - B
- If previous operation underflowed, subtract 1 from C

# AND
`AND $A $B $C` <-> `04 A B C 0
- C = A and B

# ORR
`OR $A $B $C` <-> `05 A B C 0
- C = A or B

# XOR
`XOR $A $B $C` <-> `06 A B C 0
- C = A xor B

# NOT
`NOT $A $B` <-> `07 0 B C 0
- B = not A

# PSH
`PUSH` <-> `08 0 0 0 0
- Stores MDR1 at most significant half of SP
- Stores MDR2 at least significant half of SP
- Increments SP

# POP
`POP` <-> `09 0 0 0 0
- Stores data at most significant half of SP in MDR1
- Stores data at least significant half of SP in MDR2
- Decrements SP

# PPC
`PUSHPC #A` <-> `10 0 0 A 0
- Sends PC to MEM adder
- Increments PC by A
- Stores most significant nibble of new PC at least significant half of SP
- Stores middle nibble of new PC at most significant half of SP
- Increments SP
- Stores least significant nibble of new PC at least significant half of SP
- Stores 0 at most significant half of SP

# RET
`RET` <-> `11 0 0 0 0`
- Sends least significant nibble of SP to least significant nibble of PC
- Decrements SP
- Sends most significant nibble of SP to middle nibble of PC
- Sends least significant nibble of SP to most significant  nibble of PC
- Decrements SP

# BRA
`BR $A $B $C` <-> `12 A B C 0`
- Branches to memory address (MSB to LSB) ABC

# BRP
`BRP $A $B $C $D` <-> `13 A B C D`
- Branches to memory address (MSB to LSB) ABC if MSB of D is 1

# MOV
`MV $A $B` <-> `14 A 0 B 0`
- Sets B to A

# SET
`SET #A $B` <-> `15 A 0 B 0`
- Sets B to A