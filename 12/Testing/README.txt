==PARSE==
- wrote tests for each function testing the basic functionality.

- During development I created a #define:
 DEBUG printf("%s %s %s\n",CURRENT_WORD, __func__, __LINE__);
to help with debugging the parser. 
I added this line at the top of every function to
 identify in which function things were going wrong.
TODO ASK NEIL IF HE CONSIDERS THIS A TEST.

- I wrote my own .TTL files to test sprecific functionality that was 
 cumbersome to test using inline functions. For example:
* file contains too many words
* file contains word that's too long.
* incorrect arguments.

- I wrote a bash script (test.sh) that determines that .TTL files
 exit with the correct error code. This was useful for testing
 specific failure conditions (eg: too many arguments) or 
 automatically checking sucess of valid files.

== INTERP ==
- Tried downarrow and saw that I wasn't executing the rest of the program 
if the loop was defined at the start of the prog.

- wrote a bash script to verify that the outputs of the program matched yours. 
after changing how I rounded, I was able to get all but four to perfectly match.
Most likely because of the same bug, labyrinth was going out of bounds.

- fequal to handle floating point imprecision

- added a add_to_angle and subtract from angle to avoid overflow issues.

- As above, created functions to test the main functionality. 
Occasionally it was simpler to have a function test bevhiour of multiple functions,
for example, test_get_and_set_variables tests the beheviour of three functions.

- out of bounds causes error and exit.

- all globals, no loop scoping.