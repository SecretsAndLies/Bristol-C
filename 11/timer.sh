#!/bin/bash

# Check if an argument is given
if [ $# -eq 0 ]; then
    echo "No arguments supplied. Please provide a program name."
    exit 1
fi

# Compile the program
make $1

# Check if make was successful
if [ $? -ne 0 ]; then
    echo "Make failed. Exiting."
    exit 1
fi

# Initialize total user time
total_user_time=0

# Set the TIMEFORMAT to only get the user time
TIMEFORMAT=%6U

# Run the program 20 times and sum the user times
for i in {1..20}; do
    # Capture the user time using 'time' command
    sys_time=$( { time ./"$1" > /dev/null 2>&1; } 2>&1 )

    # Check if sys_time is a number
    if ! [[ $sys_time =~ ^[0-9]+([.][0-9]+)?$ ]]; then
        echo "Error capturing user time. Exiting."
        exit 1
    fi
    
    # Sum up the user times
    total_user_time=$(awk "BEGIN {print $total_user_time+$sys_time}")
done

# Reset TIMEFORMAT to default
unset TIMEFORMAT

# Calculate the average user time
average_user_time=$(awk "BEGIN {print $total_user_time / 20}")

# Output the average user time
echo "Average user Time: $average_user_time seconds"
echo "Total user Time: $total_user_time seconds"

exit 0