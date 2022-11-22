# A Translator for Logisim MPU
###### Converts your assembly code into hex machine code

## Conventions
* Max line length is 120 characters
* Max instruction count is 256 due to ROM size
* RAM size is 16 bytes
* Only one instruction per line
* The rest is pretty much the same as in the AVR assembler
## How to use
1. Compile it
2. Open terminal
3. Call it
```
translator <asm file name goes here>
```
4. Creates a .hex file with the same name as the input
## Supported instructions

| ALU Instructions  | Description                                                        |
| ----------------- | ------------------------------------------------------------------ |
| ```ADD Rd, Rs```  | Add two registers and store result in **Rd**                       |
| ```ADC Rd, Rs```  | Add two registers with carry and store result in **Rd**            |
| ```INC Rd```      | Increment **Rd**                                                   |
| ```DEC Rd, Rs```  | Decrement **Rd**                                                   |
| ```NEG Rd, Rs```  | Negate **Rd**                                                      |
| ```SUB Rd, Rs```  | Subtract **Rs** from **Rd** and store result in **Rd**             |
| ```SBC Rd, Rs```  | Subtract **Rs** from **Rd** with carry and store result in **Rd**  |
| ```MOV Rd, Rs```  | Copy **Rs** to **Rd**                                              |
| ```AND Rd, Rs```  | **Rd** & **Rs** and store result in **Rd**                         |
| ```OR Rd, Rs```   | **Rd** | **Rs** and store result in **Rd**                         |
| ```XOR Rd, Rs```  | **Rd** ^ **Rs** and store result in **Rd**                         |
| ```NOT Rd```      | Invert all bits in **Rd**                                          |
| ```LSL Rd```      | Logical shift left of **Rd**                                       |
| ```LSR Rd```      | Logical shift right of **Rd**                                      |
| ```ROL Rd```      | Rotate **Rd** through carry to the left                            |
| ```ROR Rd, Rs```  | Rotate **Rd** through carry to the right                           |

| Compare Instruction |                                                                  |
| ------------------- | ---------------------------------------------------------------- |
| ```CMP Rd, Rs ```   | Perform **Rd** - **Rs** and get SREG flags changed as a result   |

| Interaction with RAM |                                                                |
| -------------------- | -------------------------------------------------------------- |
| ```LDI Rd, A ```     | Load value **A** into **Rd**                                   |
| ```LDR Rd, A```      | Load value from RAM at address **A** and store into **Rd**     |
| ```STR Rd, A```      | Store value into RAM at address **A** from **Rd**              |

| Jump/Branch Instructions|                                                            |
| ----------------------- | ---------------------------------------------------------- |
| ```JMP A```             | Jump to instruction **A**. Instructions start from 0       |
| ```BRCC A```            | Branch to address **A** if Carry flag is cleared           |
| ```BRCS A```            | Branch to address **A** if Carry flag is set               |
| ```BRZC A```            | Branch to address **A** if Zero flag is cleared            |
| ```BRZS A```            | Branch to address **A** if Zero flag is set                |
| ```BRNC A```            | Branch to address **A** if Negative flag is cleared        |
| ```BRNS A```            | Branch to address **A** if Negative flag is set            |
| ```BRVC A```            | Branch to address **A** if Signed overflow flag is cleared |
| ```BRVS A```            | Branch to address **A** if Signed overflow flag is set     |
