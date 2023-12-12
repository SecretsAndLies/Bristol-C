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

int main( int argc, char *argv[] )  
{
   test();
   if(argc!=2){
      return 1;
   }
   Program * prog = get_program(argv[1]);
   parse_program(prog);
   free(prog);
   return 0;
}

void test(void)
{
   test_parse_program();
   test_parse_num();
   test_parse_ins();
}

// <NUM>    ::= 10 or -17.99 etc.
bool parse_num(Program *p)
{
   double d;
   if(sscanf(p->words[p->curr_word],"%lf", &d)!=1){
      return false;
   }
   d=0; // todo ASK, this is to switch off warnings.
   return true;
}

void test_parse_num(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"START");
   assert(!parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"1");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"10");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"-17.99");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"0");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"11231230");
   assert(parse_num(prog));
   prog->curr_word = 0;
   free(prog);
}

// <FWD> ::= "FORWARD" <VARNUM>
bool parse_fwd(Program *p)
{
   // if we're here,
   // we can just advance past, we know the word is FORWARD.
   p->curr_word++;
   return parse_num(p);
}


// <RGT> ::= "RIGHT" <VARNUM>
bool parse_rgt(Program *p)
{
   // if we're here,
   // we can just advance past, we know the word is RIGHT.
   p->curr_word++;
   return parse_num(p);
}

// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
bool parse_ins(Program *p)
{
   if(strsame(p->words[p->curr_word], "FORWARD")){
      return parse_fwd(p);
   }
   else if(strsame(p->words[p->curr_word], "RIGHT")){
      return parse_rgt(p);
   }
   // <COL> | <LOOP> | <SET> TODO
   return false;
}

void test_parse_ins(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"123");
   assert(parse_ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"-82.43");
   assert(parse_ins(prog));
   prog->curr_word = 0;

   // testing with random strings and non numbers.
   strcpy(prog->words[0],"MOW");
   strcpy(prog->words[1],"-82.43");
   assert(!parse_ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"far");
   assert(!parse_ins(prog));
   prog->curr_word = 0;

   free(prog);
}

// <INSLST> ::= "END" | <INS> <INSLST>
bool parse_inslst(Program *p)
{
   if(strsame(p->words[p->curr_word], "END")){
      return true;
   }
   p->curr_word++;
   if(!parse_ins(p)){
      return false;
   }
   if(!parse_inslst(p)){
      return false;
   }
   return true;
}

void test_parse_inslst(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"END");
   assert(parse_inslst(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"s");
   strcpy(prog->words[1],"START");
   strcpy(prog->words[2],"START");
   strcpy(prog->words[3],"START");
   strcpy(prog->words[4],"END");
   assert(parse_inslst(prog));
   prog->curr_word = 0;

   free(prog);}

// <PROG>   ::= "START" <INSLST>
bool parse_program(Program *p)
{
   if(!strsame(p->words[p->curr_word], "START")){
      return false;
   }
   p->curr_word++;
   return parse_inslst(p);
}

void test_parse_program(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"END");
   assert(parse_program(prog));
   prog->curr_word = 0;

   // starting with END should fail
   strcpy(prog->words[0],"END");
   assert(!parse_program(prog));
   prog->curr_word = 0;

   // ending with START should fail
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"START");
   assert(!parse_program(prog));
   prog->curr_word = 0;

   free(prog);
}

Program * get_program(char * prog_name)
{
   FILE * f = nfopen(prog_name,"r");
   Program* prog = calloc(1, sizeof(Program));
   int i=0;
   while(fscanf(f, "%s", prog->words[i++])==1 && i<MAXNUMTOKENS);
   if(i>=MAXNUMTOKENS){
      ERROR("File contains too many tokens to parse.");
   }
   fclose(f);
   return prog;
}

void* nfopen(char* fname, char* mode)
{
   FILE* fp;
   fp = fopen(fname, mode);
   if(!fp){
      ERROR("Cannot open file");
   }
   return fp;
}