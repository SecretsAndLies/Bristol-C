#include "parse.h"
// <PROG> ::= "START" <INSLST>
// <INSLST> ::= "END" | <INS> <INSLST>
// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
// <FWD> ::= "FORWARD" <VARNUM>
// <RGT> ::= "RIGHT" <VARNUM>
// <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
// <LOOP> ::= "LOOP" <LTR> "OVER" <LST> <INSLST>
// <SET> ::= "SET" <LTR> "(" <PFIX>
// <VARNUM> ::= <VAR> | <NUM>
// % Variables e.g. $A, $B, $Z etc.
// <VAR> ::= $<LTR>
// % One Uppercase letter
// <LTR> ::= A, B ... Z
// % Any valid double (as defined by scanf("%lf"...)
// <NUM> ::= 10 or -17.99 etc.
// % A single word (as defined by scanf("%s"...) with double-quotes around it
// % Valid colours include "BLACK", "RED", "GREEN", "BLUE",
// % "YELLOW", "CYAN", "MAGENTA", "WHITE"
// <WORD> ::= "RED", "BLUE", "HELLO!" or "178"
// <LST> ::= "{" <ITEMS>
// <ITEMS> ::= "}" | <ITEM> <ITEMS>
// <ITEM> ::= <VARNUM> | <WORD>
// <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
// % A single mathematical operation character
// <OP> ::= + - / *

/// TODO DELETE
// TODO delete debug

// https://docs.google.com/spreadsheets/d/1jdvzG2hEDDXJ83oQDXqG4Nl4RXmUWUHoT9VOMphs4Qk/edit#gid=0
int main( int argc, char *argv[] )  
{
   test();
   if(argc!=2){
      on_error("Wrong number of arguments.");
   }
   Program * prog = get_program(argv[1]);
   if(program(prog)){
      puts("PARSED!"); // TODO REMOVE.
   }
   else{
      free(prog);
      on_error("Cannot parse.");
   }
   free(prog);
   exit(EXIT_SUCCESS);
}

void test(void)
{
   test_num();
   test_ins();
   test_check_ltr();
   test_word();
   test_var();
   test_col();
   test_is_operator();
   test_program();
   test_is_number();
   test_loop();
}

bool is_operator(char c)
{
   return c == '+' || c == '-' || c == '/' || c == '*';
}

void test_is_operator(void)
{
   assert(is_operator('+') == true);
   assert(is_operator('-') == true);
   assert(is_operator('*') == true);
   assert(is_operator('/') == true);
   assert(is_operator('a') == false);
   assert(is_operator('1') == false);
   assert(is_operator('&') == false);
   assert(is_operator(' ') == false);
}


// <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
bool pfix(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, ")")){
      p->curr_word++;
      return true;
   }
   if(is_operator(FIRST_LETTER))
   {
      if(!op(p)){
         return false;
      }
   }
   else {
      if(!varnum(p)){
         return false;
      }
   }

   // in both cases you need to prefix.
   if(!pfix(p)){
      return false;
   }

   return true;
}

// <OP> ::= + - / *
bool op(Program *p)
{
   DEBUG
   if(strlen(CURRENT_WORD)!=1){
      return false;
   }
   if(!is_operator(FIRST_LETTER)){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_op(void)
{
   Program* prog = calloc(1, sizeof(Program));
   // invalid too long
   strcpy(prog->words[0],"+1");
   assert(!op(prog));
   prog->curr_word = 0;

   // too short
   strcpy(prog->words[0],"");
   assert(!op(prog));
   prog->curr_word = 0;
   
   // valid 
   strcpy(prog->words[0],"+");
   strcpy(prog->words[1],"-");
   strcpy(prog->words[2],"/");
   strcpy(prog->words[3],"*");
   assert(op(prog));
   assert(prog->curr_word==1);
   assert(op(prog));
   assert(prog->curr_word==2);
   assert(op(prog));
   assert(prog->curr_word==3);
   assert(op(prog));
   assert(prog->curr_word==4);

   prog->curr_word = 0;

   free(prog);

}

// <SET> ::= "SET" <LTR> "(" <PFIX>
bool set(Program *p)
{
   DEBUG
   // go past set.
   p->curr_word++;
   if(!check_ltr(CURRENT_WORD)){
      return false;
   }
   // go past the letter.
   p->curr_word++;
   if(!strsame(CURRENT_WORD, "(")){
      DEBUG
      return false;
   }
   // go past the open bracket.
   p->curr_word++;
   if(!pfix(p)){
      return false;
   }

   return true;
}

// <VAR> ::= $<LTR>
bool var(Program *p)
{
   DEBUG
   if(strlen(CURRENT_WORD)<2){
      return false;
   }
   if(FIRST_LETTER!='$'){
      return false;
   }
   if(!check_ltr(&CURRENT_WORD[1])){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_var(void)
{
   Program* prog = calloc(1, sizeof(Program));
   // invalid too long
   strcpy(prog->words[0],"START");
   assert(!var(prog));
   prog->curr_word = 0;

   // too short
   strcpy(prog->words[0],"$");
   assert(!var(prog));
   prog->curr_word = 0;
   
   // no dollar sign
   strcpy(prog->words[0],"10");
   assert(!var(prog));
   prog->curr_word = 0;

   // not a letter.
   strcpy(prog->words[0],"$7");
   assert(!var(prog));
   prog->curr_word = 0;

   // lower case 
   strcpy(prog->words[0],"$a");
   assert(!var(prog));
   prog->curr_word = 0;

   // valid 
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"$Z");
   assert(var(prog));
   assert(prog->curr_word==1);
   assert(var(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;

   free(prog);
}

// <LST> ::= "{" <ITEMS>
bool lst(Program *p)
{
   DEBUG
   if(!strsame(CURRENT_WORD, "{")){
      return false;
   }
   // pass {
   p->curr_word++;
   if(!items(p)){
      return false;
   }
   return true;
}

// <ITEMS> ::= "}" | <ITEM> <ITEMS>
bool items(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, "}")){
      p->curr_word++;
      return true;
   }
   if(!item(p)){
      return false;
   }
   if(!items(p)){
      return false;
   }
   return true;
}

bool is_number(char * str)
{
   double a;
   if(sscanf(str,"%lf", &a)!=1){
      return false;
   }
   a=0; // switching off warnings :-(
   return true;
}

void test_is_number(void)
{
   assert(is_number("7"));
   assert(is_number("-7.0"));
   assert(!is_number("t"));
}

// <VARNUM> ::= <VAR> | <NUM>
bool varnum(Program * p)
{
   DEBUG
   if(FIRST_LETTER=='$'){
      if(!var(p)){
         return false;
      }
   }
   else {
      if(!num(p)){
         return false;
      }
   }
   return true;
}

// <ITEM> ::= <VARNUM> | <WORD>
bool item(Program *p)
{
   DEBUG
   if(FIRST_LETTER=='$' || is_number(CURRENT_WORD)){
      if(!varnum(p)){
         return false;
      }
   }
   // otherwise it's a word.
   else {
      if(!word(p)){
         return false;
      }
   }
   return true;
}

// <WORD> ::= "RED", "BLUE", "HELLO!" or "178"
bool word(Program *p)
{
   DEBUG
   int len = strlen(CURRENT_WORD);
   if(len<3){
      return false;
   }
   if(FIRST_LETTER != '"'){
      return false;
   }
   if(CURRENT_WORD[len-1] != '"'){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_word(void)
{
  Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"\"RED\"");
   assert(word(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"BLUE\"");
   assert(word(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"HELLO!\"");
   assert(word(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"178!\"");
   assert(word(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   // missing all quotes
   strcpy(prog->words[0],"178!");
   assert(!word(prog));
   assert(prog->curr_word==0);

   // missing opening quotes
   strcpy(prog->words[0],"178!\"");
   assert(!word(prog));
   assert(prog->curr_word==0);

   // missing closing quotes
   strcpy(prog->words[0],"\"178!");
   assert(!word(prog));
   assert(prog->curr_word==0);

   free(prog);
}

// <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
bool col(Program *p)
{
   DEBUG
   // skip past COLOUR
   p->curr_word++;
   if(FIRST_LETTER=='$'){
      if(!var(p)){
         return false;
      }
   }
   // otherwise, it's a word.
   else{
      if(!word(p)){
         return false;
      }
   }

   return true;
}

void test_col(void)
{
   Program* prog = calloc(1, sizeof(Program));
   // "COLOUR" <VAR> 
   strcpy(prog->words[0],"COLOUR");
   strcpy(prog->words[1],"$A");
   assert(col(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   // "COLOUR" <WORD>
   strcpy(prog->words[0],"COLOUR");
   strcpy(prog->words[1],"\"HELLO!\"");
   assert(col(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 2;

   // TODO test invalid var and words? or are we happy that they're tested in their functions?

   free(prog);
}

bool check_ltr(char * s)
{
   int len = strlen(s);
   if(len>1){
      return false;
   }
   char letter = s[0];
   if(!isupper(letter)){
      return false;
   }
   return true;
}

void test_check_ltr(void)
{
   // too long
   assert(!check_ltr("START"));
   assert(!check_ltr("$START"));

   // not a letter
   assert(!check_ltr("1"));
   assert(!check_ltr("$1"));

   // lowercase
   assert(!check_ltr("a"));
   assert(!check_ltr("$a"));

   // dollar signs.
   assert(!check_ltr("$Z"));
   assert(!check_ltr("$A"));

   // valid 
   assert(check_ltr("A"));
   assert(check_ltr("Z"));

}

// <LOOP> ::= "LOOP" <LTR> "OVER" <LST> <INSLST>
bool loop(Program *p)
{
   DEBUG
   // skip "LOOP"
   p->curr_word++;
   if(!check_ltr(CURRENT_WORD)){
      return false;
   }
   // go past the letter.
   p->curr_word++;
   if(!strsame(CURRENT_WORD, "OVER")){
      return false;
   }
   // go past OVER
   p->curr_word++;
   if(!lst(p)){
      return false;
   }
   if(!inslst(p)){
      return false;
   }
   return true;
}

void test_loop(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"\"RED\"");
   strcpy(prog->words[5],"\"GREEN\"");
   strcpy(prog->words[6],"\"YELLOW\"");
   strcpy(prog->words[7],"\"BLUE\"");
   strcpy(prog->words[8],"}");
   strcpy(prog->words[9],"COLOUR");
   strcpy(prog->words[10],"$C");
   strcpy(prog->words[11],"FORWARD");
   strcpy(prog->words[12],"$D");
   strcpy(prog->words[13],"RIGHT");
   strcpy(prog->words[14],"90");
   strcpy(prog->words[15],"END");
   assert(loop(prog));
   assert(prog->curr_word==15);
   prog->curr_word=0;
   free(prog);
}

// <NUM>    ::= 10 or -17.99 etc.
bool num(Program *p)
{
   DEBUG
   if(!is_number(CURRENT_WORD)){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_num(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"START");
   assert(!num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"1");
   assert(num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"10");
   assert(num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"-17.99");
   assert(num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"0");
   assert(num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"11231230");
   assert(num(prog));
   prog->curr_word = 0;
   free(prog);
}

// <FWD> ::= "FORWARD" <VARNUM>
bool fwd(Program *p)
{
   DEBUG
   // advance past FORWARD
   p->curr_word++;
   return varnum(p);
}


// <RGT> ::= "RIGHT" <VARNUM>
bool rgt(Program *p)
{
   DEBUG
   // advance past RIGHT
   p->curr_word++;
   return varnum(p);
}

// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
bool ins(Program *p)
{
   DEBUG
   // todo switch?
   // todo think about changing this so that the words are passed in the function.
   if(strsame(CURRENT_WORD, "FORWARD")){
      return fwd(p);
   }
   else if(strsame(CURRENT_WORD, "RIGHT")){
      return rgt(p);
   }
   else if(strsame(CURRENT_WORD, "COLOUR")){
      return col(p);
   }
   else if(strsame(CURRENT_WORD, "LOOP")){
      return loop(p);
   }
   else if(strsame(CURRENT_WORD, "SET")){
      return set(p);
   }
   // if ins contains none of those words, 
   // it's an invalid instruction.
   return false;
}

void test_ins(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"123");
   assert(ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"-82.43");
   assert(ins(prog));
   prog->curr_word = 0;

   // testing with random strings and non numbers.
   strcpy(prog->words[0],"MOW");
   strcpy(prog->words[1],"-82.43");
   assert(!ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"far");
   assert(!ins(prog));
   prog->curr_word = 0;

   free(prog);
}

// <INSLST> ::= "END" | <INS> <INSLST>
bool inslst(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, "END")){
      return true;
   }
   if(!ins(p)){
      return false;
   }
   if(!inslst(p)){
      return false;
   }
   return true;
}

void test_inslst(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"END");
   assert(inslst(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"s");
   strcpy(prog->words[1],"START");
   strcpy(prog->words[2],"START");
   strcpy(prog->words[3],"START");
   strcpy(prog->words[4],"END");
   assert(inslst(prog));
   prog->curr_word = 0;

   free(prog);}

// <PROG>   ::= "START" <INSLST>
bool program(Program *p)
{
   DEBUG
   if(!strsame(CURRENT_WORD, "START")){
      return false;
   }
   p->curr_word++;
   return inslst(p);
}

void test_program(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"END");
   assert(program(prog));
   prog->curr_word = 0;

   // starting with END should fail
   strcpy(prog->words[0],"END");
   assert(!program(prog));
   prog->curr_word = 0;

   // ending with START should fail
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"START");
   assert(!program(prog));
   prog->curr_word = 0;

   // valid files should increment curr_word
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"FORWARD");
   strcpy(prog->words[2],"5");
   strcpy(prog->words[3],"RIGHT");
   strcpy(prog->words[4],"5.2");
   strcpy(prog->words[5],"FORWARD");
   strcpy(prog->words[6],"-2.2");
   strcpy(prog->words[7],"END");
   assert(program(prog));
   assert(prog->curr_word==7);
   prog->curr_word = 0;

   free(prog);
}

Program * get_program(char * prog_name)
{
   FILE * f = nfopen(prog_name,"r");
   Program* prog = calloc(1, sizeof(Program));
   int i=0;
   while(fscanf(f, "%s", prog->words[i])==1 && i<MAXNUMTOKENS){
      if(strlen(prog->words[i])>=MAXTOKENSIZE-1){
         on_error("File contains words that are too long.");
      }
      i++;
   };
   if(i>=MAXNUMTOKENS){
      on_error("File is too long (too many words).");
   }
   // TODO test that this fails with strings of 99, 100, 101
   // and files longer 99, 100, 101 words.
   fclose(f);
   return prog;
}

void* nfopen(char* fname, char* mode)
{
   FILE* fp;
   fp = fopen(fname, mode);
   if(!fp){
      on_error("Cannot open file");
   }
   return fp;
}

void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}