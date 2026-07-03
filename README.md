# Digital Logic Design Simulator (Binary Toolkit)

## Overview
A C++ simulator built to demonstrate core concepts of digital logic design. This toolkit provides utilities for fractional base conversions via batch CSV processing and an interactive command-line interface for 32-bit binary arithmetic across multiple signed and unsigned representations.

## Key Features
* **Batch Base Conversion (Bases 2-20):** Processes an `input.csv` file to convert integer and fractional numbers between any bases from binary to vigesimal (base-20), outputting results and validation matches to `output.csv`.
* **32-Bit Binary Arithmetic:** Performs addition, subtraction, multiplication, and division on 32-bit binary strings. 
* **Multiple Data Representations:** Supports standard digital logic formats:
  * Unsigned Binary
  * Signed Magnitude
  * 1's Complement
  * 2's Complement
* **Overflow/Underflow Detection:** Accurately detects and reports boundary limit errors for all 32-bit operations.
* **Operation Logging:** Automatically records all interactive arithmetic operations and results to a local `history.txt` log file.

## Tech Stack
* **Language:** C++ (Standard Library: `<iostream>`, `<fstream>`, `<bitset>`, etc.)

## Setup and Compilation

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/coder14ss/Binary_Toolkit.git](https://github.com/coder14ss/Binary_Toolkit.git)
   cd Binary_Toolkit
