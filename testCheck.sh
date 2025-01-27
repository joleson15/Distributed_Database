#!/bin/bash

# Directory containing test case files
TEST_CASE_DIR="./tests"

# Directory to store outputs
OUTPUT_DIR="./outputs"

# Compiled C++ program
CPP_PROGRAM="./RepCRec"

# Create the output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Check if the C++ program exists
if [ ! -f "$CPP_PROGRAM" ]; then
  echo "Error: Compiled C++ program not found at $CPP_PROGRAM"
  exit 1
fi

# Iterate over all text files in the test case directory
for TEST_CASE in "$TEST_CASE_DIR"/*.txt; do
  # Get the base name of the test case file (without path)
  TEST_CASE_BASENAME=$(basename "$TEST_CASE")
  
  # Define the output file path
  OUTPUT_FILE="$OUTPUT_DIR/${TEST_CASE_BASENAME%.txt}_output.txt"
  
  # Run the C++ program with the test case as input and redirect output
  ./$CPP_PROGRAM $TEST_CASE > $OUTPUT_FILE
done
