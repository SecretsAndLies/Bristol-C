==PARSE==
- wrote tests for each function testing the basic functionality.

- created a #define #define DEBUG printf("%s %s %s\n",CURRENT_WORD, __func__, __LINE__);
to help with debugging the parser. adding debug at the top of every function
helped identify where things were going wrong (and then I could write tests to catch errors)

- wrote a bash script (test.sh) that determines that each of the provided example 
scripts exit with the correct error code.

- fequal to handle floating point imprecision

- added a add_to_angle and subtract from angle to avoid overflow issues.


== INTERP ==
- Tried downarrow and saw that I wasn't executing the rest of the program 
if the loop was defined at the start of the prog.

- wrote a bash script to verify that the outputs of the program matched yours, 
but they were sublty different. I eventually decided that I would check each one
and decide if it looked right. Then in future, I tested against this output.
This allowed me to tell if the output had changed in a significant way, while I 
did my refactoring.