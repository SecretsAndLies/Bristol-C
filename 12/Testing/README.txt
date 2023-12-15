==PARSE==
- wrote tests for each function testing the basic functionality.

- created a #define #define DEBUG printf("%s %s %s\n",CURRENT_WORD, __func__, __LINE__);
to help with debugging the parser. adding debug at the top of every function
helped identify where things were going wrong (and then I could write tests to catch errors)

- wrote a bash script (test.sh) that determines that each of the provided example 
scripts exit with the correct error code.