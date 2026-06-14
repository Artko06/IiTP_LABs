# OSISP Course Work - File Manager

This is an interactive console application for navigating and manipulating the file system.

## Features

*   Display a directory tree.
*   Navigate the file system.
*   File operations: view, copy, move, delete, create.
*   Error handling for file operations.

## Build

To build the application, use the following command:

```bash
make
```

## Run

To run the application, use the following command:

```bash
./file_manager
```

## Testing

### Performance Test

To run the performance test, use the following command:

```bash
make performance-test
```

This test will measure the time it takes to perform a series of file operations. The expected time is less than 1 second.

### Correctness Test

To run the correctness test, use the following command:

```bash
make correctness-test
```

This test will verify that the file operations are performed correctly. It will create a test directory, perform a series of operations, and then clean up the test directory. The expected result is that all operations complete without errors.
