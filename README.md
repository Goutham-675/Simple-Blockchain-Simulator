# Simple Blockchain Simulator

An educational tool that simulates a basic blockchain for learning purposes.  
This project demonstrates core blockchain concepts like blocks, hashes, and proof-of-work.

## Features
- Create a blockchain with multiple blocks
- Each block contains: index, timestamp, data, previous hash, current hash, and a nonce
- Simple proof-of-work simulation (adjustable difficulty)
- Verify blockchain integrity
- Export blockchain to CSV and JSON for reporting

## Tech / Languages
- Language: C
- Uses standard libraries (`stdio.h`, `stdlib.h`, `string.h`, `time.h`)
- Fully local simulation; safe for educational purposes

## Usage
1. Compile the program:
```bash
gcc -std=c99 -Wall -O2 -o blockchain main.c# Simple-Blockchain-Simulator
