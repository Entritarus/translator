# A Translator for Logisim MPU
## Converts your assembly code into hex machine code

## Conventions
* Max line length is 120 characters
* Max instruction count is 256 due to ROM size
* RAM size is 16 bytes
## How to use
1. Compile it
2. Open terminal
3. Call it
```
translator <asm file name goes here>
```

## Supported instructions

1. ALU instructions
  * ADD Rd, Rs
    *
  * ADC Rd, Rs
  * INC Rd, Rs
  * DEC Rd, Rs
  * NEG Rd, Rs
  * SUB Rd, Rs
  * SBC Rd, Rs
  * MOV Rd, Rs
  * AND Rd, Rs
  * OR Rd, Rs
  * XOR Rd, Rs
  * NOT Rd, Rs
  * LSL Rd, Rs
  * LSR Rd, Rs
  * ROL Rd, Rs
  * ROR Rd, Rs
