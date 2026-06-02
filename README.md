# Blockchain-Voting-system
Decentralized Voting System implemented in C++ using blockchain concepts. Features include voter verification, duplicate vote prevention, tamper detection, and vote counting.

# Blockchain Voting System

## Overview

A simple blockchain-based voting system built in C++. Each vote is stored as a block, and any modification to the data can be detected through hash verification.

## Features

* Genesis block creation
* Vote validation
* Duplicate vote prevention
* Vote counting
* Winner declaration
* Tie detection
* Blockchain integrity verification
* Tampering detection

## Registered Voters

```text
VOTER101
VOTER102
VOTER103
```

## Candidates

```text
Alice
Bob
Charlie
```

## Validation Checks

The system rejects:

* Duplicate votes
* Unregistered voters
* Invalid candidates
* Votes after the deadline

## Compile and Run

```bash
g++ blockchain_voting_system.cpp -o blockchain_voting_system
./blockchain_voting_system
```





  
