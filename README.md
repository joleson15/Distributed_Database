# Distributed Database Implementation

## Replicated Concurrency Control and Recovery (RepCRec)

## To Run:
- Run **make RepCRec** to compile, output will be file called **RepCRec**. <br>
The program can then be run in multiple ways:
 - usage: **./RepCRec <testCase.txt>**. Replacing "testCase" with the name of the test (i.e. test1).
 - usage: **./RepCRec** accepts standard input line by line. See input instructions below and test cases for examples.
 - usage: **./testCheck.sh** runs a shell script that runs all tests and places the output in **./outputs** in the form of a **.txt** file
   
Code compiles using **gcc version 11.5.0**, written with **C++17**


### This project involves implementing a distributed database system with the following key features:

- Serializable Snapshot Isolation (SSI)
- Replication
- Failure Recovery

## Data Overview

### Variables
- The database contains **20 distinct variables** (`x1, ..., x20`).
- Variables are distributed across **10 sites** (`1 to 10`).

### Replication
- **Odd-indexed variables** are stored on one site only, determined as `1 + (index number mod 10)`.
- **Even-indexed variables** are replicated across all sites.
- Example: `x3` and `x13` are stored on **site 4**.

### Initialization
- Each variable `xi` is initialized to `10 * i`.

### Time Continuation
- This project uses a discrete event simulation structure. Each new line in the input means time advances by one, and there will be only one instruction per line.

### Site Failures
- Each site maintains its own Serializable Snapshot Isolation information.
- If a site fails, all of its SSI information is erased.


## Algorithms

### Available Copies Approach to Serializable Snapshot Isolation
- **Writes**:
  - Writes can go to available sites.
  - If a site fails after a write but before the transaction commits, the transaction must abort.
- **Reads**:
  - A transaction `T` can read a variable `xi` if it meets the following conditions:
    1. For non-replicated variables: The site holding `xi` is up.
    2. For replicated variables: The site containing `xi` was up from the last commit of `xi` until `T` began.

### Concurrency-Induced Aborts
- Detect and handle:
  - **First-committer-wins rule**.
  - **Consecutive Read-Write (RW) edges** forming a cycle in the serialization graph.
- Transactions do not restart automatically after aborting.

### Failure Recovery
- Upon recovery:
  - Non-replicated variables are available for reads and writes immediately.
  - Replicated variables are available for writing but not reading until a committed write occurs after recovery.

### Input Format
- Instructions are read from a file or standard input, one per line.
- Examples:
  - `begin(T1)` - Begin transaction `T1`.
  - `R(T1, x4)` - Transaction `T1` reads variable `x4`.
  - `W(T1, x6, v)` - Transaction `T1` writes value `v` to `x6` on all available sites.
  - `dump()` - Print committed values of all variables on all sites.
  - `end(T1)` - End transaction `T1` and print commit or abort status.
  - `fail(6)` - Site 6 fails.
  - `recover(7)` - Site 7 recovers.
    
- Input is to be well defined

### Output Format
- **Reads**:
  - Example: `x4: 5`
- **Writes**:
  - Indicate affected sites.
- **Commit/Abort**:
  - Example: `T1 commits`, `T1 aborts`.
- **Dump**:
  - Committed values of all variables for each site, sorted by variable name.
  - Example:
    ```
    site 1 - x2: 6, x3: 2, ... x20: 3
    site 10 - x2: 14, ... x8: 12, x9: 7, ... x20: 3
    ```
- **Wait**:
  - Example: 'T1 Waits'
    
- **Fail/Recover**:
  - Example: 'Site 7 Fails', 'Site 7 Recovers'.

### Example

#### Partial Script
```plaintext
begin(T1)
begin(T2)
begin(T3)
W(T1, x1, 5)
W(T3, x2, 32)
W(T2, x1, 17)
end(T1)
end(T3)
end(T2)
```

#### Expected Behavior
- **`T1` commits**.
- **`T3` commits**.
- **`T2` aborts** due to conflict with `T1`.

## Design Overview

### Components
1. **Transaction Manager (TM)**
   - Manages read and write requests.
   - Routes requests to appropriate sites.
   - Tracks sites' status (up/down).
   - Ensures SSI and resolves conflicts.
2. **Data Manager (DM)**
   - Each site has an independent DM.
   - Maintains commit times and failure/recovery history.

### Failures and Recovery
- Transactions waiting on a failed site will continue execution until commit time.
- Recovered sites require a committed write to enable reading of replicated variables.


## Testing and Validation
- Testing is done with various failure and recovery scenarios.
- Verification of correctness of read and write operations under SSI is required.
- Assurance of proper handling of abort and commit conditions.

---

### Notes
- Transactions must wait if no site containing the required data is available.
- A read rom a transaction that begins after the recovery of a site **s** or a replicated variable **x** will not be allowed until a committed write to **x** takes place on **s**
- Failure scenarios simulate real-world distributed database behavior.

