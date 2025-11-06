#!/bin/bash

# Directory containing the students' output files
STUDENT_DIR="tests/solutions"

# Directory containing the example files
EXAMPLES_DIR="tests/examples"

# If $1 exists, append it to EXAMPLES_DIR
if [ -n "$1" ]; then
    EXAMPLES_DIR="$EXAMPLES_DIR/$1"
fi

# Iterate through each file in the directory
for file in "$EXAMPLES_DIR"/*.soln; do
  student_file="$STUDENT_DIR/$(basename "$file")"

  if [[ ! -f "$student_file" ]]; then
    echo "Missing an output for $(basename "$file")"
    continue
  fi

  # Compare only the last few lines of each file
  lines=5
  diff -w <(tail -n $lines "$file") <(tail -n $lines "$student_file") > /dev/null
  status=$?

  if [[ $status -eq 0 ]]; then
    echo "✅ PASSED: $(basename "$file") matches (last $lines lines)!"
  elif [[ $status -eq 1 ]]; then
    echo "❌ FAILED: $(basename "$file") differs (last $lines lines)!"
  else
    echo "⚠️ Error comparing $(basename "$file")"
  fi
done
