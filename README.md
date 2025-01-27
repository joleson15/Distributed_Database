# Distributed Database Implementation

This project involves implementing a distributed database system with the following key features:

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

### Site Failures
- Each site maintains its own Serializable Snapshot Isolation information.
- If a site fails, all of its data is erased.

## Algorithms

### Available Copies Approach
- **Writes**:
  - Writes can go to available sites.
  - If a site fails after a write but before the transaction commits, the transaction must abort.
- **Reads**:
  - A transaction `T` can read a variable `xi` if it meets the following conditions:
    1. For non-replicated variables: The site holding `xi` is up.
    2. For replicated variables: The site containing `xi` was up from the last commit of `xi` until `T` began.

### Concurrency-Induced Abortions
- Detect and handle:
  - **First-committer-wins rule**.
  - **Consecutive Read-Write (RW) edges** forming a cycle in the serialization graph.
- Transactions should not restart automatically after aborting.

### Failure Recovery
- Upon recovery:
  - Non-replicated variables are available for reads and writes immediately.
  - Replicated variables are available for writing but not reading until a committed write occurs after recovery.

## Instructions

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
- **Events**:
  - Indicate transaction aborts due to conflicts or available copies rules.

### Examples

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
   - Tracks site statuses (up/down).
   - Ensures SSI and resolves conflicts.
2. **Data Manager (DM)**
   - Each site has an independent DM.
   - Maintains commit times and failure/recovery history.

### Failures and Recovery
- Transactions waiting on a failed site will continue execution until commit time.
- Recovered sites require a committed write to enable reading of replicated variables.

## Debugging Tools
- `querystate()` - Outputs the state of the TM, DM, data distribution, and values.

## Testing and Validation
- Test with various failure and recovery scenarios.
- Verify correctness of read and write operations under SSI.
- Ensure proper handling of abort and commit conditions.

---

### Notes
- Transactions must wait if no site containing the required data is available.
- The program output must align with the specified input and output formats.
- Failure scenarios must simulate real-world distributed database behavior.

---

### Key Considerations

1. **Concurrency Control**: Ensure that SSI is strictly enforced.
2. **Fault Tolerance**: Plan for scenarios where failures occur mid-transaction.
3. **Scalability**: Design the system to handle increasing numbers of variables and transactions efficiently.
4. **Test Coverage**: Create tests for edge cases, including simultaneous site failures and complex transaction conflicts.

By following the implementation guidelines, the project will serve as a robust introduction to distributed database systems while exploring the challenges of maintaining consistency and availability in a multi-node environment.
