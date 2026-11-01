# 16-Bit-Address-Accumulator-Register

A 16-Bit address accumulator register designed with discrete 74LS and 74HC series TTL logic. This register allows my custom 4-Bit CPU to use a register's data to increment or decrement the Address Register. That way, we can address up to 64KB of RAM, using only 4-bit steps inside a 16-bit address space.

## Overview

If we use a register's data or the ALU's output from a 4-bit CPU, we can only access 16 bytes of RAM, which is not enough for decent algorithms or small video games. To solve this, this Memory Address Register uses an internal adder that takes 4 bits coming from a register in the register file (or from the ALU) and adds or subtracts it (depending on the selected operation) from the 16-bit address stored inside the register. This way, we can use a 16-bit address bus, which gives us up to approximately 64 KB of RAM. This allows us to store more data for our programs and build more complex applications.

## Architecture

This small module uses two's complement to perform subtraction. The DEC pin controls both the carry-in of the first 74HC283 full adder and the XOR gates that negate the incoming data. When DEC is 1, the first adder receives a carry-in of 1, and the XOR gates invert the input data. This performs a two's complement operation, allowing us to move backward and forward within the 16-bit address space.

The 16-bit register is built using four 74HC173 ICs. Its outputs serve as both the memory address and the A operand of the full adders. The full adders receive the register's output as operand A, and the incoming 4-bit data (expanded to 16 bits) as operand B.

## Schematic

<img width="1253" height="885" alt="image" src="https://github.com/user-attachments/assets/938376b2-8250-45d4-b1be-df80492b42fb" />


## Components Used
| IC | Function | Quantity | Purpose |
|---|---|---|---|
| 74HC283 | 4-bit Full Adder | 4 | Performs the 16-bit addition/subtraction |
| 74HC173 | 4-bit Register | 4 | Stores the 16-bit address |
| 74HC86 | Quad 2-input XOR | 4 | Inverts the input data for two's complement subtraction |

## Control Pins
This module has four control pins:

- **NS (Not Store)**: Controls the store function of the 74HC173 registers. When 0, the internal registers store the new address coming from the adders. When 1, the registers do not store the new address, allowing us to freeze the address while other instructions are executed.

- **MR (Master Reset)**: Resets the internal registers. When 1, the registers reset to 0x0, giving us the address value 0x0000.
- **DEC (Decrement)**: Selects whether to add or subtract the incoming data from our address register.
- **CP (Clock Pulse)**: The clock signal for the internal registers.

## Verification

The module was verified on a breadboard using an Arduino Mega 2560 as a test controller. A dedicated test [`firmware`](./src/tester/src) was written to exercise the accumulator's read and write logic, validating both increment and decrement operations across the entire address space.

The test sequence performs the following steps:
1. Resets the internal register to `0x0000`.
2. Increments the MAR by `15` (0x0F) repeatedly until it reaches `0xFFFF`.
3. Decrements the MAR by `15` (0x0F) repeatedly until it returns to `0x0000`.

Each step is compared against the expected result, and the test suite prints a formatted table with `OK` or `FAIL` for every operation.

**Test Output Sample:**
```text
| MAR: 0x0000 | Data: 0x000F | Op: Inc | Expected: 0x000F | Got: 0x000F | OK |
| MAR: 0x000F | Data: 0x000F | Op: Inc | Expected: 0x001E | Got: 0x001E | OK |
...
| MAR: 0x0010 | Data: 0x000F | Op: Dec | Expected: 0x0001 | Got: 0x0001 | OK |
| MAR: 0x0001 | Data: 0x000F | Op: Dec | Expected: 0x0000 | Got: 0x0000 | OK |
```

## License

Licensed under the Solderpad Hardware License v2.1.
See the LICENSE file for full terms.

## Contact

If you have any questions or suggestions, feel free to reach out:

- GitHub: [Neowizen](https://github.com/Neowizen)
