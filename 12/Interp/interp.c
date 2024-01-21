#include "interp.h"

int main( int argc, char *argv[] )  
{
   test();
   Program * prog = init_program(argv, argc);
   if(!run_program(prog)){
      free_prog(prog);
      on_error("Error - cannot interpret program.");
   }
   if(prog->output_location==TXT){
      print_arr_to_txt_file(prog);
   }
   if(prog->output_location==PS){
      handle_ps_output(prog);
   }
   free_prog(prog);
   exit(EXIT_SUCCESS);
}

void test(void)
{
   test_skip_empty_loops();
   test_run_fwd();
   test_eval_args();
   test_add_to_angle();
   test_run_pfix();
   test_fequal();
   test_is_filetype();
   test_get_and_set_variables();
   test_run_num();
   test_run_ins();
   test_check_ltr();
   test_run_word();
   test_run_var();
   test_run_col();
   test_is_operator();
   test_get_number();
   test_run_loop();
   test_run_op();
   test_create_command();
   test_free_prog();
   test_run_lst();
   test_run_items();
   test_is_number();
   test_run_varnum();
   test_run_item();
   test_run_col();
   test_execute_loop();
   test_run_one_loop();
   test_get_ps_colour();
   test_is_out_of_bounds();
   test_run_rgt();
   test_run_inslst();
   test_set_prog_output_to_spaces();
   test_init_prog_variables();
   test_write_ps_move();
   test_set_var_to_stck();
   test_run_set();
}

void handle_ps_output(Program * prog)
{
   write_start_text_to_ps(prog->output_file_name);
   write_prog_text_to_ps(prog);
   append_to_file(prog->output_file_name, "showpage");
   char command[MEDIUMSTR];
   create_command(prog, command);
   system(command);
}

void create_command(Program * prog, char command[MEDIUMSTR])
{
   int len = strlen(prog->output_file_name);
   char file_name_no_ext[SMALLSTR];
   strcpy(file_name_no_ext, prog->output_file_name);
   file_name_no_ext[len - PS_EXT_LEN] = '\0';
   sprintf(command, "ps2pdf %s %s.pdf", prog->output_file_name, file_name_no_ext);
}

void test_create_command(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   strcpy(prog->output_file_name,"test.ps");
   char command[MEDIUMSTR];
   create_command(prog, command);
   free(prog);
   assert(STRSAME(command,"ps2pdf test.ps test.pdf"));
}

void write_prog_text_to_ps(Program * prog)
{
   int i=0;
   while(i<prog->ps_num_words){
      if(prog->ps_output[i][0]!='\0'){
         append_to_file(prog->output_file_name, prog->ps_output[i]);
      }
      i++;
   }
}

void write_start_text_to_ps(char * filename)
{
   char * content = "0.2 setlinewidth \n"
                     "10 10 scale";
   FILE * fp = nfopen(filename, "w");
   fprintf(fp, "%s\n", content);
   fclose(fp);
}

void append_to_file(char * filename, char * content)
{
   FILE * fp = nfopen(filename, "a");
   fprintf(fp, "%s\n", content);
   fclose(fp);
}

void print_arr_to_txt_file(Program * p)
{
   FILE *fp = nfopen(p->output_file_name, "w");
   for(int r=0; r<SCREEN_HEIGHT; r++){
      for(int c=0; c<SCREEN_WIDTH; c++){
         char letter = p->output[r][c];
         fprintf(fp, "%c", letter);
      }  
      fprintf(fp, "\n");
   }  
   fprintf(fp, "\n"); 
   fclose(fp);
}

bool free_prog(Program * p)
{
   if(p->output_location==SCREEN){
      neillreset();
      printf("\n");
   }
   stack_free(p->stck);
   free(p);
   return true;
}

void test_free_prog(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   prog->stck = stack_init();
   prog->output_location=TXT;
   assert(prog);
   assert(prog->stck);
   assert(free_prog(prog));
}

void test_get_and_set_variables(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // intially no variables are set.
   assert(!get_var_from_variables('A', prog));
   assert(prog->curr_var.var_type==NOT_SET);

   // now we set a to 2.0
   set_variable_to_num('A', prog, 2.0);
   assert(get_var_from_variables('A', prog));
   assert(fequal(prog->curr_var.num_var,2.0));
   assert(prog->curr_var.var_type==NUM);

   // test Z with a string
   set_variable_to_string('Z',prog,"TEST2");
   assert(get_var_from_variables('Z', prog));
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"TEST2"));
   
   // reset A to a different type (overwting)
   set_variable_to_string('A',prog,"TEST");
   assert(get_var_from_variables('A', prog));
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"TEST"));

   free(prog);
}

void set_variable_to_num(char var, Program * p, double num)
{
   Variable * v = &p->variables[var-'A'];
   v->var_type=NUM;
   v->num_var=num;
}

void set_variable_to_string(char var, Program * p, char * str)
{
   Variable * v = &p->variables[var-'A'];   
   v->var_type=STRING;
   strcpy(v->str_var, str);
}

// Looks up var and sets the current variable to it
// returns false if that variable is unset.
// you'll need to check elsewhere that it's a type you're expecting.
bool get_var_from_variables(char var, Program * prog)
{
   Variable v = prog->variables[var-'A'];
   if(v.var_type==NOT_SET){
      return false;
   }
   prog->curr_var = v;
   return true;
}

void eval_args(int argc, char *argv[], File_Type * ft)
{
   if(argc>MAX_ARGS){
      on_error("Wrong number of arguments.");
   }
   if(!is_filetype(argv[INPUT_FILE_INDEX], ".ttl")){
      on_error("Invalid filename for arg 1 "
      "expected a .ttl file");
   }
   set_file_type(argc, argv, ft);
}

// sets the file type to ps, screen or txt, 
// depending on the command line arguments.
void set_file_type(int argc, char *argv[], File_Type * ft)
{
   if(argc==MAX_ARGS){
      bool is_txt = is_filetype(argv[OUTPUT_FILE_INDEX], ".txt");
      bool is_ps = is_filetype(argv[OUTPUT_FILE_INDEX], ".ps");
      if(!is_ps && !is_txt){
         on_error("Invalid filename for arg 2, "
         "expected a .ps or .txt file");
      }
      if(is_ps){
         *ft = PS;
         return;
      }
      if(is_txt){
         *ft = TXT;
         return;
      }
   }
   *ft = SCREEN;
}

void test_eval_args(void)
{   
   char * argv3[] = {"./interp_s", "TTLs/octagon1.ttl"};
   File_Type ft;
   eval_args(2, argv3, &ft);
   assert(ft==SCREEN);

   char * argv[] = {"./interp_s", "TTLs/octagon1.ttl", "out_octagon1.txt"};
   eval_args(3, argv, &ft);
   assert(ft==TXT);

   char * argv2[] = {"./interp_s", "TTLs/octagon1.ttl", "out_octagon1.ps"};
   eval_args(3, argv2, &ft);
   assert(ft==PS);
}

// determines if the filename matches the given extension.
bool is_filetype(char * filename, char * ext)
{
   int len = strlen(filename);
   int ext_len = strlen(ext);
   // we add 1 because a filename must be letter.extension
   if(len<ext_len+1){
      return false;
   }
   if(!STRSAME(&filename[len-ext_len],ext)){
      return false;
   }
   return true;
}

void test_is_filetype(void)
{
   assert(is_filetype("a.ttl",".ttl"));
   assert(is_filetype("asdasdas.ttl",".ttl"));
   // too short.
   assert(!is_filetype(".ttl",".ttl"));
   assert(is_filetype("v.ps",".ps"));
   assert(is_filetype("v.txt",".txt"));
   //wrong ext
   assert(!is_filetype("v.txt",".ttl"));
}

bool is_operator(char c)
{
   return c == '+' || c == '-' || c == '/' || c == '*';
}

void test_is_operator(void)
{
   assert(is_operator('+'));
   assert(is_operator('-'));
   assert(is_operator('/'));
   assert(is_operator('*'));
   assert(!is_operator('a'));
}


// <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
bool run_pfix(Program *p)
{
   if(STRSAME(CURRENT_WORD, ")")){
      p->curr_word++;
      return true;
   }
   if(strlen(CURRENT_WORD)==1 && is_operator(FIRST_LETTER))
   {
      if(!run_op(p)){
         return false;
      }      
   }
   else {
      if(!run_varnum(p)){
         return false;
      }
      stack_push(p->stck, p->curr_var.num_var);
   }
   if(!run_pfix(p)){
      return false;
   }
   return true;
}


void test_run_pfix(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   prog->stck = stack_init();

   // PARSE TESTS:
   // invalid operator +a
   strcpy(prog->words[0],"5");
   strcpy(prog->words[1],"+a");
   strcpy(prog->words[2],")");
   assert(!run_pfix(prog));
   prog->curr_word = 0;
   // invalid varnum $AA + )
   strcpy(prog->words[0],"$AA");
   strcpy(prog->words[1],"+");
   strcpy(prog->words[2],")");
   assert(!run_pfix(prog));
   prog->curr_word = 0;

   // undeclared var
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"+");
   strcpy(prog->words[2],")");
   assert(!run_pfix(prog));
   prog->curr_word = 0;


   // INTERP tests:
   double res;

   set_variable_to_num('Z',prog, 45.7);
   set_variable_to_num('A',prog, 1);
   strcpy(prog->words[0],"$Z");
   strcpy(prog->words[1],"$A");
   strcpy(prog->words[2],"+");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,46.7));
   prog->curr_word=0;

   strcpy(prog->words[0],"45");
   strcpy(prog->words[1],")");
   assert(run_pfix(prog));
   stack_pop(prog->stck, &res);
   assert(fequal(res,45));
   prog->curr_word=0;

   strcpy(prog->words[0],"45");
   strcpy(prog->words[1],"1");
   strcpy(prog->words[2],"+");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,46));
   prog->curr_word=0;

   strcpy(prog->words[0],"22");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"/");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,11));
   prog->curr_word=0;

   strcpy(prog->words[0],"22");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"*");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,44));
   prog->curr_word=0;

   strcpy(prog->words[0],"22");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"-");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,20));
   prog->curr_word=0;

   strcpy(prog->words[0],"2");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"-"); //0
   strcpy(prog->words[3],"1");
   strcpy(prog->words[4],"+"); // 1
   strcpy(prog->words[5],"4");
   strcpy(prog->words[6],"*"); // 4
   strcpy(prog->words[7],"2");
   strcpy(prog->words[8],"/"); // 2
   strcpy(prog->words[9],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,2));
   prog->curr_word=0;

   stack_free(prog->stck);
   free(prog);
}

// <OP> ::= + - / *
// takes two numbers on the stack and performs the op.
bool run_op(Program *p)
{
   if(strlen(CURRENT_WORD)!=1){
      return false;
   }
   if(!is_operator(FIRST_LETTER)){
      return false;
   }
   if(!eval_operator(FIRST_LETTER, p)){
      return false;
   }
   p->curr_word++;
   return true;
}

bool eval_operator(char op, Program * p)
{
   double second_num;
   double first_num;
   if(!stack_pop(p->stck, &second_num)){
      return false;
   }
   if(!stack_pop(p->stck, &first_num)){
      return false;
   }
   if(op=='+'){
      stack_push(p->stck, first_num+second_num);
   }
   if(op=='-'){
      stack_push(p->stck, first_num-second_num);
   }
   if(op=='*'){
      stack_push(p->stck, first_num*second_num);
   }
   if(op=='/'){
      if(fequal(second_num,0)){
         return false;
      }
      stack_push(p->stck, first_num/second_num);
   }
   return true;
}

void test_run_op(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   prog->stck = stack_init();

   stack_push(prog->stck, 2);
   stack_push(prog->stck, 2);
   strcpy(prog->words[0],"+");
   double res;
   assert(run_op(prog));
   stack_pop(prog->stck, &res);
   assert(fequal(res,4));
   prog->curr_word=0;

   // 0 / 2 should be 0
   stack_push(prog->stck, 0);
   stack_push(prog->stck, 2);
   strcpy(prog->words[0],"/");
   assert(run_op(prog));
   stack_pop(prog->stck, &res);
   assert(fequal(res,0));
   prog->curr_word=0;

   // 2/0 should be an error.
   stack_push(prog->stck, 2);
   stack_push(prog->stck, 0);
   strcpy(prog->words[0],"/");
   assert(!run_op(prog));
   prog->curr_word=0;


   stack_free(prog->stck);
   free(prog);
}
// <SET> ::= "SET" <LTR> "(" <PFIX>
bool run_set(Program *p)
{
   if(!STRSAME(CURRENT_WORD,"SET")){
      return false;
   }
   p->curr_word++;
   if(!check_ltr(CURRENT_WORD)){
      return false;
   }
   char letter = FIRST_LETTER;
   p->curr_word++;
   if(!STRSAME(CURRENT_WORD, "(")){
      return false;
   }
   p->curr_word++;
   if(!run_pfix(p)){
      return false;
   }
   if(!set_var_to_stck(p,letter)){
      return false;
   }
   return true;
}

// sets variable to what's in the stack.
// returns false if the stack contains >1 item.
bool set_var_to_stck(Program * p, char letter)
{
   double num;
   if(!stack_pop(p->stck, &num)){
      return false;
   }
   set_variable_to_num(letter,p,num);
   double fake;
   if(stack_peek(p->stck, &fake)){
      return false;
   }
   return true;
}

void test_set_var_to_stck(void)
{
   Program* p = ncalloc(1, sizeof(Program));
   init_prog_variables(p);
   stack_push(p->stck, 2);
   assert(set_var_to_stck(p,'A'));
   assert(fequal(p->variables[0].num_var,2));
   stack_push(p->stck, 2);
   stack_push(p->stck, 2);
   assert(!set_var_to_stck(p,'A'));

   stack_free(p->stck);
   free(p);

}

void test_run_set(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // SET B ( -5.3 $A + )
   init_prog_variables(prog);
   set_variable_to_num('A', prog, 1);
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"B");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],")");
   assert(run_set(prog));
   assert(prog->curr_word==7);
   get_var_from_variables('B',prog);
   assert(prog->curr_var.var_type==NUM);
   assert(fequal(prog->curr_var.num_var,-4.3));
   prog->curr_word = 0;

   // SET B ( -5.3 )
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"B");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],")");
   assert(run_set(prog));
   assert(prog->curr_word==5);
   get_var_from_variables('B',prog);
   assert(prog->curr_var.var_type==NUM);
   assert(fequal(prog->curr_var.num_var,-5.3));
   prog->curr_word = 0;

   // empty should fail (overpopping stack)
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],")");
   assert(!run_set(prog));
   prog->curr_word = 0;

   // other overpop test.
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"3");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],"+");
   strcpy(prog->words[7],")");
   assert(!run_set(prog));
   prog->curr_word = 0;

   // leaving too much on the stack
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"3");
   strcpy(prog->words[5],")");
   assert(!run_set(prog));
   prog->curr_word = 0;

   // set misspelled.
   strcpy(prog->words[0],"SEET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],")");
   assert(!run_set(prog));
   prog->curr_word = 0;
   // not a letter.
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"a");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],")");
   assert(!run_set(prog));
   prog->curr_word = 0;
   // no opening bracket
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"/");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],")");
   assert(!run_set(prog));
   prog->curr_word = 0;
   // no closing bracket
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"5");
   strcpy(prog->words[6],"+");
   strcpy(prog->words[7],"+");
   assert(!run_set(prog));
   prog->curr_word = 0;

   stack_free(prog->stck);
   free(prog);
}

// <VAR> ::= $<LTR>
bool run_var(Program *p)
{
   if(strlen(CURRENT_WORD)<VAR_LEN){
      return false;
   }
   if(FIRST_LETTER!='$'){
      return false;
   }
   if(!check_ltr(&CURRENT_WORD[1])){
      return false;
   }
   if(!get_var_from_variables(CURRENT_WORD[1], p)){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_run_var(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // invalid too long
   strcpy(prog->words[0],"START");
   assert(!run_var(prog));
   prog->curr_word = 0;

   // too short
   strcpy(prog->words[0],"$");
   assert(!run_var(prog));
   prog->curr_word = 0;
   
   // no dollar sign
   strcpy(prog->words[0],"10");
   assert(!run_var(prog));
   prog->curr_word = 0;

   // not a letter.
   strcpy(prog->words[0],"$7");
   assert(!run_var(prog));
   prog->curr_word = 0;

   // lower case 
   strcpy(prog->words[0],"$a");
   assert(!run_var(prog));
   prog->curr_word = 0;

   set_variable_to_num('A', prog, 1);
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"$Z");
   assert(run_var(prog));
   assert(prog->curr_word==1);
   assert(prog->curr_var.var_type==NUM);
   assert(fequal(prog->curr_var.num_var,1));
   // unset variable
   assert(!run_var(prog));
   prog->curr_word = 0;

   free(prog);
}

// <LST> ::= "{" <ITEMS>
bool run_lst(Program *p)
{
   if(!STRSAME(CURRENT_WORD, "{")){
      return false;
   }
   p->curr_word++;
   if(!run_items(p)){
      return false;
   }
   return true;
}

void test_run_lst(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // test unitialized variable.
   strcpy(prog->words[0],"{");
   strcpy(prog->words[1],"\"RED\"");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"}");
   assert(!run_lst(prog));
   prog->curr_word = 0;

   // Note: this is set for the rest of this function.
   set_variable_to_num('A',prog,1);

   strcpy(prog->words[0],"{");
   strcpy(prog->words[1],"\"RED\"");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"}");
   assert(run_lst(prog));
   assert(prog->curr_word==5);
   prog->curr_word = 0;

   // no opening {
   strcpy(prog->words[0],"10");
   strcpy(prog->words[1],"\"RED\"");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"}");
   assert(!run_lst(prog));
   prog->curr_word = 0;

   // no closing {
   strcpy(prog->words[0],"{");
   strcpy(prog->words[1],"\"RED\"");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"-1");
   assert(!run_lst(prog));
   prog->curr_word = 0;

   // weird list varialbe string vs "string"
   strcpy(prog->words[0],"{");
   strcpy(prog->words[1],"RED");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"}");
   assert(!run_lst(prog));
   prog->curr_word = 0;

   free(prog);
}

// <ITEMS> ::= "}" | <ITEM> <ITEMS>
bool run_items(Program *p)
{
   if(STRSAME(CURRENT_WORD, "}")){
      p->curr_word++;
      return true;
   }
   if(!run_item(p)){
      return false;
   }
   if(!run_items(p)){
      return false;
   }
   return true;
}

void test_run_items(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   
   // unitialized var
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"}");
   assert(!run_items(prog));
   prog->curr_word = 0;

   set_variable_to_string('A',prog,"RED");
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"}");
   assert(run_items(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"2");
   strcpy(prog->words[1],"}");
   assert(run_items(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"RED\"");
   strcpy(prog->words[1],"}");
   assert(run_items(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"RED\"");
   strcpy(prog->words[1],"1");
   strcpy(prog->words[2],"2");
   strcpy(prog->words[3],"3");
   strcpy(prog->words[4],"15");
   strcpy(prog->words[5],"}");
   assert(run_items(prog));
   assert(prog->curr_word==6);
   prog->curr_word = 0;

   // random bad value in the middle of a list.
   strcpy(prog->words[0],"\"RED\"");
   strcpy(prog->words[1],"1");
   strcpy(prog->words[2],"2");
   strcpy(prog->words[3],"3asd");
   strcpy(prog->words[4],"15");
   strcpy(prog->words[5],"}");
   assert(!run_items(prog));
   prog->curr_word = 0;

   // no closing paren.
   strcpy(prog->words[0],"\"RED\"");
   strcpy(prog->words[1],"1");
   strcpy(prog->words[2],"2");
   strcpy(prog->words[3],"3");
   strcpy(prog->words[4],"15");
   strcpy(prog->words[5],"12");
   assert(!run_items(prog));
   prog->curr_word = 0;

   free(prog);
}

bool get_number(char * str, double * num)
{
   if(!is_number(str)){
      return false;
   }
   if(sscanf(str,"%lf", num)!=1){
      return false;
   }
   return true;
}

void test_get_number(void)
{
   double num;
   assert(!get_number("23a", &num));
   assert(get_number("3", &num));
   assert(fequal(num, 3));
   assert(get_number("-22.31", &num));
   assert(fequal(num, -22.31));
}

bool is_number(char * str)
{
   int str_index = 0;
   if(str[str_index] == '-'){
      str_index++;
   }
   if(!isdigit(str[str_index]) && str[str_index] != '.'){
      return false;
   }
   while(isdigit((str[str_index]))){
      str_index++;
   }
   if(str[str_index] == '.'){
      str_index++;
      if (!isdigit((str[str_index])) && str[str_index] != '\0'){
            return false;
      }
      while(isdigit((str[str_index]))){
            str_index++;
      }
   }
   return str[str_index] == '\0';
}

void test_is_number(void)
{
   assert(is_number("7"));
   assert(is_number("-17123212"));
   assert(is_number("-7.0"));
   assert(is_number("-7."));
   assert(is_number("-.1"));
   assert(is_number(".1"));
   assert(is_number("000.01"));
   assert(!is_number("t"));
   assert(!is_number("81f"));
   assert(!is_number("as8"));
   assert(!is_number("8.a0"));
}

// <VARNUM> ::= <VAR> | <NUM>
bool run_varnum(Program * p)
{
   if(FIRST_LETTER=='$'){
      if(!run_var(p)){
         return false;
      }
   }
   else {
      if(!run_num(p)){
         return false;
      }
   }
   return true;
}

void test_run_varnum(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   
   // unitialized var
   strcpy(prog->words[0],"$A");
   assert(!run_varnum(prog));
   prog->curr_word = 0;

   set_variable_to_string('A',prog,"RED");
   strcpy(prog->words[0],"$A");
   assert(run_varnum(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"1");
   assert(run_varnum(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"a");
   assert(!run_varnum(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"1!");
   assert(!run_varnum(prog));
   prog->curr_word = 0;

   free(prog);
}

// <ITEM> ::= <VARNUM> | <WORD>
bool run_item(Program *p)
{
   double num;
   if(FIRST_LETTER=='$' || get_number(CURRENT_WORD, &num)){
      if(!run_varnum(p)){
         return false;
      }
   }
   // otherwise it's a word.
   else {
      if(!run_word(p)){
         return false;
      }
   }
   return true;
}

void test_run_item(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   
   // unitialized.
   strcpy(prog->words[0],"$A");
   assert(!run_item(prog));
   prog->curr_word = 0;

   set_variable_to_string('A',prog, "\"RED\"");
   strcpy(prog->words[0],"$A");
   assert(run_item(prog));
   assert(prog->curr_word==1);
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"\"RED\""));
   prog->curr_word = 0;

   strcpy(prog->words[0],"1");
   assert(run_item(prog));
   assert(prog->curr_word==1);
   assert(prog->curr_var.var_type==NUM);
   assert(fequal(prog->curr_var.num_var, 1));
   prog->curr_word = 0;

   strcpy(prog->words[0],"-1.123");
   assert(run_item(prog));
   assert(prog->curr_word==1);
   assert(prog->curr_var.var_type==NUM);
   assert(fequal(prog->curr_var.num_var, -1.123));
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"RED\"");
   assert(run_item(prog));
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"\"RED\""));
   prog->curr_word = 0;

   strcpy(prog->words[0],"a1");
   assert(!run_item(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"$2");
   assert(!run_item(prog));
   prog->curr_word = 0;

   free(prog);
}

// <WORD> ::= "RED", "BLUE", "HELLO!" or "178"
bool run_word(Program *p)
{
   int len = strlen(CURRENT_WORD);
   int end_index = len-1;
   if(len<MIN_WORD_LEN){
      return false;
   }
   if(FIRST_LETTER != '"'){
      return false;
   }
   if(CURRENT_WORD[end_index] != '"'){
      return false;
   }
   p->curr_var.var_type=STRING;
   strcpy(p->curr_var.str_var, CURRENT_WORD);
   p->curr_word++;
   return true;
}

void test_run_word(void)
{
   Program* prog = ncalloc(1, sizeof(Program));

   strcpy(prog->words[0],"\"RED\"");
   assert(run_word(prog));
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"\"RED\""));
   prog->curr_word=0;

   strcpy(prog->words[0],"\"HELLO!\"");
   assert(run_word(prog));
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"\"HELLO!\""));
   prog->curr_word=0;

   strcpy(prog->words[0],"\"178\"");
   assert(run_word(prog));
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"\"178\""));
   prog->curr_word=0;

   //missing open
   strcpy(prog->words[0],"RED\"");
   assert(!run_word(prog));
   prog->curr_word=0;

   //missing close
   strcpy(prog->words[0],"\"RED");
   assert(!run_word(prog));
   prog->curr_word=0;

   // too short
   strcpy(prog->words[0],"\"\"");
   assert(!run_word(prog));
   prog->curr_word=0;

   free(prog);
}

// <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
bool run_col(Program *p)
{
   if(!STRSAME(CURRENT_WORD,"COLOUR")){
      return false;
   }
   p->curr_word++;
   if(FIRST_LETTER=='$'){
      if(!run_var(p)){
         return false;
      }
   }
   else{
      if(!run_word(p)){
         return false;
      }
   }
   if(!set_col(p)){
      return false;
   }
   return true;
}

void test_run_col(void)
{
   Program* prog = ncalloc(1, sizeof(Program));

   strcpy(prog->words[0],"COLOUR");
   strcpy(prog->words[1],"\"RED\"");
   assert(run_col(prog));
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"\"RED\""));
   assert(prog->ttl.col==red);
   prog->curr_word=0;

   set_variable_to_string('H',prog,"\"YELLOW\"");
   strcpy(prog->words[0],"COLOUR");
   strcpy(prog->words[1],"$H");
   assert(run_col(prog));
   assert(prog->curr_var.var_type==STRING);
   assert(STRSAME(prog->curr_var.str_var,"\"YELLOW\""));
   assert(prog->ttl.col==yellow);
   prog->curr_word=0;

   //missing colour
   strcpy(prog->words[0],"RED\"");
   assert(!run_col(prog));
   prog->curr_word=0;

   // wrong colour
   strcpy(prog->words[0],"COLOUR");
   strcpy(prog->words[1],"\"ORANGE\"");
   assert(!run_col(prog));
   prog->curr_word=0;

   free(prog);
}

bool set_col(Program * p)
{
   if(STRSAME(p->curr_var.str_var,"\"BLACK\"")){
      p->ttl.col=black;
   }
   else if(STRSAME(p->curr_var.str_var,"\"RED\"")){
      p->ttl.col=red;
   }
   else if(STRSAME(p->curr_var.str_var,"\"GREEN\"")){
      p->ttl.col=green;
   }
   else if(STRSAME(p->curr_var.str_var,"\"BLUE\"")){
      p->ttl.col=blue;
   }
   else if(STRSAME(p->curr_var.str_var,"\"YELLOW\"")){
      p->ttl.col=yellow;
   }
   else if(STRSAME(p->curr_var.str_var,"\"CYAN\"")){
      p->ttl.col=cyan;
   }
   else if(STRSAME(p->curr_var.str_var,"\"MAGENTA\"")){
      p->ttl.col=magenta;
   }
   else if(STRSAME(p->curr_var.str_var,"\"WHITE\"")){
      p->ttl.col=white;
   }
   else{
      return false;
   }
   return true;
}

void test_set_col(void)
{   
   Program* prog = ncalloc(1, sizeof(Program));

   prog->curr_var.var_type=STRING;

   strcpy(prog->curr_var.str_var,"\"RED\"");
   assert(set_col(prog));
   assert(prog->ttl.col==red);

   strcpy(prog->curr_var.str_var,"\"BLACK\"");
   assert(set_col(prog));
   assert(prog->ttl.col==black);

   strcpy(prog->curr_var.str_var,"\"GREEN\"");
   assert(set_col(prog));
   assert(prog->ttl.col==green);

   strcpy(prog->curr_var.str_var,"\"BLUE\"");
   assert(set_col(prog));
   assert(prog->ttl.col==blue);

   strcpy(prog->curr_var.str_var,"\"YELLOW\"");
   assert(set_col(prog));
   assert(prog->ttl.col==yellow);

   strcpy(prog->curr_var.str_var,"\"CYAN\"");
   assert(set_col(prog));
   assert(prog->ttl.col==cyan);

   strcpy(prog->curr_var.str_var,"\"MAGENTA\"");
   assert(set_col(prog));
   assert(prog->ttl.col==magenta);

   strcpy(prog->curr_var.str_var,"\"WHITE\"");
   assert(set_col(prog));
   assert(prog->ttl.col==white);

   // invalid colour
   strcpy(prog->curr_var.str_var,"\"ORANGE\"");
   assert(!set_col(prog));

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
   assert(check_ltr("A"));
   assert(check_ltr("Z"));
   assert(check_ltr("H"));
   assert(!check_ltr("HA"));
   assert(!check_ltr(""));
   assert(!check_ltr("a"));
}

// <LOOP> ::= "LOOP" <LTR> "OVER" <LST> <INSLST>
bool run_loop(Program *p)
{
   if(!STRSAME(CURRENT_WORD,"LOOP")){
      return false;
   }
   p->curr_word++;
   if(!check_ltr(CURRENT_WORD)){
      return false;
   }
   char letter = FIRST_LETTER;
   p->curr_word++;
   if(!STRSAME(CURRENT_WORD, "OVER")){
      return false;
   }
   p->curr_word++;
   if(skip_empty_loops(p)){
      p->curr_word--;
   }
   else if (!execute_loop(p,letter)){
      return false;
   }
   p->curr_word++;
   if(!run_inslst(p)){
      return false;
   }
   return true;
}

bool skip_empty_loops(Program * p)
{
   if(STRSAME(p->words[p->curr_word+1],"}")){
      int loop_c = 1;
      while(loop_c!=0){
         if(p->curr_word==MAXNUMTOKENS){
            return false;
         }
         if(STRSAME(CURRENT_WORD,"LOOP")){
            loop_c++;
         }
         if(STRSAME(CURRENT_WORD,"END")){
            loop_c--;
         }         
         p->curr_word++;
      }
      return true;
   }
   return false;
}

void test_skip_empty_loops(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;

   // unterminated end loop doesn't cause segfault.
   prog->curr_word=3;
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"H");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"}");
   strcpy(prog->words[5],"FORWARD");
   strcpy(prog->words[6],"AS");
   assert(!skip_empty_loops(prog));
   prog->curr_word=0;

   prog->curr_word=3;
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"H");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"}");
   strcpy(prog->words[5],"FORWARD");
   strcpy(prog->words[6],"AS");
   strcpy(prog->words[7],"END");
   strcpy(prog->words[8],"END");
   assert(skip_empty_loops(prog));
   assert(prog->curr_word==8);
   prog->curr_word=0;
   stack_free(prog->stck);
   free(prog);

}

void test_run_loop(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;

   // doesn't include the word LOOP
   strcpy(prog->words[0],"loop");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   assert(!run_loop(prog));
   prog->curr_word=0;

   // doesn't include OVER
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"over");
   strcpy(prog->words[3],"{");
   assert(!run_loop(prog));
   prog->curr_word=0;

   // mangled instruction
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"\"RED\"");
   strcpy(prog->words[5],"\"GREEN\"");
   strcpy(prog->words[6],"YELLOW\""); // MANGLED
   strcpy(prog->words[7],"\"BLUE\"");
   strcpy(prog->words[8],"}");
   strcpy(prog->words[9],"COLOUR");
   strcpy(prog->words[10],"$C");
   strcpy(prog->words[11],"FORWARD");
   strcpy(prog->words[12],"$D");
   strcpy(prog->words[13],"RIGHT");
   strcpy(prog->words[14],"90");
   strcpy(prog->words[15],"END");
   strcpy(prog->words[16],"END");
   assert(!run_loop(prog));
   prog->curr_word=0;

   stack_free(prog->stck);
   init_prog_variables(prog);

   // test that a blank loop does nothing
   // would throw an error, but the loop doesn't execute.
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"H");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"}");
   strcpy(prog->words[5],"FORWARD");
   strcpy(prog->words[6],"AS");
   strcpy(prog->words[7],"END");
   strcpy(prog->words[8],"END");
   assert(run_loop(prog));
   assert(prog->curr_word==8);
   prog->curr_word=0;
   stack_free(prog->stck);
   free(prog);

   // 3x NESTED LOOP.
   Program* p = ncalloc(1, sizeof(Program));
   init_prog_variables(p);
   p->output_location=TXT;
   p->curr_word=0;
   set_variable_to_num('D',p,0);
   // LOOP A {1,10}
   strcpy(p->words[0],"LOOP");
   strcpy(p->words[1],"A");
   strcpy(p->words[2],"OVER");
   strcpy(p->words[3],"{");
   strcpy(p->words[4],"1");
   strcpy(p->words[5],"10");
   strcpy(p->words[6],"}");
   // LOOP B {20,200}
   strcpy(p->words[7],"LOOP");
   strcpy(p->words[8],"B");
   strcpy(p->words[9],"OVER");
   strcpy(p->words[10],"{");
   strcpy(p->words[11],"20");
   strcpy(p->words[12],"200");
   strcpy(p->words[13],"}");
   // LOOP C {500,5000}
   strcpy(p->words[14],"LOOP");
   strcpy(p->words[15],"C");
   strcpy(p->words[16],"OVER");
   strcpy(p->words[17],"{");
   strcpy(p->words[18],"500");
   strcpy(p->words[19],"5000");
   strcpy(p->words[20],"}");
   // SET D ( $A $B $C $D + + +)
   strcpy(p->words[21],"SET");
   strcpy(p->words[22],"D");
   strcpy(p->words[23],"(");
   strcpy(p->words[24],"$A");
   strcpy(p->words[25],"$B");
   strcpy(p->words[26],"$C");
   strcpy(p->words[27],"$D");
   strcpy(p->words[28],"+");
   strcpy(p->words[29],"+");
   strcpy(p->words[30],"+");
   strcpy(p->words[31],")");

   // END END END END
   strcpy(p->words[32],"END");
   strcpy(p->words[33],"END");
   strcpy(p->words[34],"END");
   strcpy(p->words[35],"END");

   assert(run_loop(p));

   assert(p->curr_word==35);
   get_var_from_variables('D',p);
   assert(p->curr_var.var_type==NUM);
   assert(fequal(p->curr_var.num_var,22924));
   
   get_var_from_variables('A',p);
   assert(p->curr_var.var_type==NUM);
   assert(fequal(p->curr_var.num_var,10));

   get_var_from_variables('B',p);
   assert(p->curr_var.var_type==NUM);
   assert(fequal(p->curr_var.num_var,200));

   get_var_from_variables('C',p);
   assert(p->curr_var.var_type==NUM);
   assert(fequal(p->curr_var.num_var,5000));

   p->curr_word=0;
   stack_free(p->stck);
   free(p);

   // loop involving a set.
   Program* pr = ncalloc(1, sizeof(Program));
   init_prog_variables(pr);
   pr->output_location=TXT;
   strcpy(pr->words[0],"LOOP");
   strcpy(pr->words[1],"A");
   strcpy(pr->words[2],"OVER");
   strcpy(pr->words[3],"{");
   strcpy(pr->words[4],"1");
   strcpy(pr->words[5],"}");
   strcpy(pr->words[6],"SET");
   strcpy(pr->words[7],"C");
   strcpy(pr->words[8],"(");
   strcpy(pr->words[9],"$A");
   strcpy(pr->words[10],"1.25");
   strcpy(pr->words[11],"*");
   strcpy(pr->words[12],")");
   strcpy(pr->words[13],"END");
   strcpy(pr->words[14],"END");
   assert(run_loop(pr));
   get_var_from_variables('A',pr);
   assert(pr->curr_word==14);
   assert(fequal(pr->curr_var.num_var,1));
   get_var_from_variables('C',pr);
   assert(fequal(pr->curr_var.num_var,1.25));
   pr->curr_word=0;
   stack_free(pr->stck);
   free(pr);

}

bool execute_loop(Program * p, char letter)
{
   int first_item_index = p->curr_word+1;
   if(!run_lst(p)){
      return false;
   }
   int last_item_index = p->curr_word-LOOP_BACK_OFFSET;
   int num_items_in_loop = (last_item_index-first_item_index)+1;
   int start_of_ins_list = p->curr_word;
   for (int i=0; i<num_items_in_loop; i++)
   {
      if(!run_one_loop(p, i, letter, first_item_index, 
                  num_items_in_loop, start_of_ins_list)){
                     return false;
                  }
   }
   return true;
}

void test_execute_loop(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;
   prog->curr_word=3;
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"H");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"\"RED\"");
   strcpy(prog->words[5],"\"GREEN\"");
   strcpy(prog->words[6],"\"BLUE\"");
   strcpy(prog->words[7],"}");
   strcpy(prog->words[8],"COLOUR");
   strcpy(prog->words[9],"$H");
   // loop end
   strcpy(prog->words[10],"END");
   // prog end.
   strcpy(prog->words[11],"END");
   execute_loop(prog,'H');
   assert(prog->curr_word==10);
   assert(prog->ttl.col==blue);

   stack_free(prog->stck);
   free(prog);   
}

bool run_one_loop(Program * p, int i, char letter, 
               int first_item_index, int num_items_in_loop, 
               int start_of_ins_list)
{
   int current_item_index = first_item_index+i;
   double num;
   if(get_number(p->words[current_item_index], &num)){
      set_variable_to_num(letter,p,num);
   }
   else{
      set_variable_to_string(letter,p,p->words[current_item_index]);
   }
   if(!run_inslst(p)){
      return false;
   }
   if(i!=num_items_in_loop-1){
      p->curr_word = start_of_ins_list;
   }
   return true;
}

void test_run_one_loop(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;
   prog->curr_word=8;
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"H");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"\"RED\"");
   strcpy(prog->words[5],"\"GREEN\"");
   strcpy(prog->words[6],"\"BLUE\"");
   strcpy(prog->words[7],"}");
   strcpy(prog->words[8],"COLOUR");
   strcpy(prog->words[9],"$H");
   // loop end
   strcpy(prog->words[10],"END");
   // prog end.
   strcpy(prog->words[11],"END");
   run_one_loop(prog,0,'H',4,3,8);
   assert(prog->curr_word==8);
   assert(prog->ttl.col==red);
   run_one_loop(prog,1,'H',4,3,8);
   assert(prog->ttl.col==green);
   run_one_loop(prog,2,'H',4,3,8);
   assert(prog->curr_word==10);
   assert(prog->ttl.col==blue);

   stack_free(prog->stck);
   free(prog);   
}

// <NUM>    ::= 10 or -17.99 etc.
// populates num_var and returns true if suceeded.
bool run_num(Program *p)
{
   double num;
   if(!get_number(CURRENT_WORD, &num)){
      return false;
   }
   p->curr_var.var_type=NUM;
   p->curr_var.num_var=num;
   p->curr_word++;
   return true;
}

void test_run_num(void)
{
   Program* prog = ncalloc(1, sizeof(Program));

   strcpy(prog->words[0],"10");
   assert(run_num(prog));
   assert(prog->curr_var.var_type==NUM);
   assert(fequal(prog->curr_var.num_var,10));
   assert(prog->curr_word==1);
   prog->curr_word=0;

   strcpy(prog->words[0],"-17.99");
   assert(run_num(prog));
   assert(prog->curr_var.var_type==NUM);
   assert(fequal(prog->curr_var.num_var, -17.99));
   assert(prog->curr_word==1);
   prog->curr_word=0;

   strcpy(prog->words[0],"17.-99");
   assert(!run_num(prog));
   prog->curr_word=0;

   free(prog);
}

// <FWD> ::= "FORWARD" <VARNUM>
bool run_fwd(Program *p)
{
   if(!STRSAME(CURRENT_WORD,"FORWARD")){
      return false;
   }
   p->curr_word++;
   if(!run_varnum(p)){
      return false;
   }
   // check we've got a number we can use.
   if(p->curr_var.var_type!=NUM){
      return false;
   }
   p->ttl.distance=p->curr_var.num_var;
   if(p->ttl.distance<0){
      p->ttl.back=true;
   }
   if(!go_fwd(p)){
      return false;
   }
   return true;
}

void test_run_fwd(void)
{
   // does this fail when 'forward is not a word'
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;
   strcpy(prog->words[0],"fads");
   strcpy(prog->words[1],"3.0");
   assert(!run_fwd(prog));
   prog->curr_word = 0;

   // out of bounds error.
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"15");
   assert(run_fwd(prog));
   assert(prog->output[START_Y][START_X]=='W');
   assert(prog->output[START_Y-15][START_X]=='W');
   assert(prog->output[START_Y][START_X+1]==' ');
   // test going out of bounds.
   strcpy(prog->words[3],"FORWARD");
   strcpy(prog->words[4],"2");
   assert(!run_fwd(prog));
   prog->curr_word = 0;
   stack_free(prog->stck);

   init_prog_variables(prog);
   // test that other colours work.
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"15");
   prog->ttl.col=black;
   assert(run_fwd(prog));
   assert(prog->output[START_Y][START_X]=='K');
   assert(prog->output[START_Y-15][START_X]=='K');
   assert(prog->output[START_Y][START_X+1]==' ');
   prog->curr_word = 0;

   set_variable_to_num('A', prog, -0.1);
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"$A");
   assert(run_fwd(prog));
   prog->curr_word = 0;

   stack_free(prog->stck);
   free(prog);   

   Program* prog2 = ncalloc(1, sizeof(Program));
   init_prog_variables(prog2);
   prog2->output_location=TXT;
   strcpy(prog2->words[0],"FORWARD");
   strcpy(prog2->words[1],"-5");
   assert(run_fwd(prog2));
   assert(prog2->output[START_Y+3][START_X]=='W');
   assert(prog2->output[START_Y+5][START_X]=='W');
   assert(prog2->output[START_Y][START_X-1]==' ');
   prog2->curr_word = 0;
   stack_free(prog2->stck);
   free(prog2);   

}

// populates the colour string.
bool get_ps_colour(char * colour, Program * p)
{
   if(p->ttl.col==white){
      sprintf(colour, WHITE_PS);
   }
   else if(p->ttl.col==black){
      sprintf(colour, BLACK_PS);
   }
   else if(p->ttl.col==red){
      sprintf(colour, RED_PS);
   }
   else if(p->ttl.col==green){
      sprintf(colour, GREEN_PS);
   }
   else if(p->ttl.col==yellow){
      sprintf(colour, YELLOW_PS);
   }
   else if(p->ttl.col==blue){
      sprintf(colour, BLUE_PS);
   }
   else if(p->ttl.col==magenta){
      sprintf(colour, MAGENTA_PS);
   }
   else if(p->ttl.col==cyan){
      sprintf(colour, CYAN_PS);
   }
   else{
      return false;
   }
   return true;
}

void test_get_ps_colour(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   prog->output_location=TXT;
   char colour[MEDIUMSTR];
   prog->ttl.col=cyan;
   assert(get_ps_colour(colour, prog));
   assert(STRSAME(colour, CYAN_PS));

   prog->ttl.col=blue;
   assert(get_ps_colour(colour, prog));
   assert(STRSAME(colour, BLUE_PS));

   free_prog(prog);

}

bool go_fwd(Program * p)
{
   // first we write to the location we're already in.
   if(!write_turtle_to_arr(p)){
      return false;
   }

   double orig_x = p->ttl.x;
   double orig_y = p->ttl.y;
   
   char colour[SMALLSTR];
   get_ps_colour(colour, p);

   if(!execute_move(p)){
      return false;
   }
   char move_str[MEDIUMSTR];
   write_ps_move(move_str, orig_x,orig_y, p, colour);

   return true;
}

void write_ps_move(char move_str[MEDIUMSTR],
                           double orig_x, double orig_y, 
                           Program * p,   
                           char colour[SMALLSTR])
{
   sprintf(move_str, "newpath\n"
            "%lf %lf moveto\n" 
            "%lf %lf lineto\n" 
            "%s setrgbcolor\n"
            "stroke", 
            orig_x,orig_y,
            p->ttl.x,p->ttl.y,
            colour);
   strcpy(p->ps_output[p->ps_num_words], move_str);
   p->ps_num_words++;
}

void test_write_ps_move(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   prog->ttl.x=1;
   prog->ttl.y=2;
   char move_str[MEDIUMSTR];
   write_ps_move(move_str, 0,0, prog, WHITE_PS);
   assert(prog->ps_num_words==1);
   assert(STRSAME(move_str,
   "newpath\n"
   "0.000000 0.000000 moveto\n"
   "1.000000 2.000000 lineto\n"
   "0.8 0.8 0.8 setrgbcolor\n"
   "stroke"));
   free(prog);
}

// calculates the move coordinates and executes the move, 
// printing to the screen on each step.
bool execute_move(Program * p)
{
   if(p->ttl.back){
      p->ttl.angle = add_to_angle(p->ttl.angle,HALF_CIRCLE);
   }
   double angle_in_radians = DEGREES_TO_RADIANS(p->ttl.angle);
   double x_move = cos(angle_in_radians);
   double y_move = sin(angle_in_radians);
   for (int i=0; i<abs(p->ttl.distance); i++){
      if(p->output_location!=PS){
         p->ttl.x -= x_move;
         p->ttl.y -= y_move;
         if(!write_turtle_to_arr(p)){
            return false;
         }
      }
      else{
         p->ttl.x -= x_move;
         p->ttl.y += y_move;
      }
   }
   finish_mv_vars(p);
   return true;
}

void finish_mv_vars(Program * p)
{
   if(p->output_location==SCREEN){
      print_arr_to_screen(p);
   }
   if(p->output_location!=PS){
      p->ttl.x=round(p->ttl.x);
      p->ttl.y=round(p->ttl.y);
   }
   if(p->ttl.back){
      p->ttl.angle = add_to_angle(p->ttl.angle,HALF_CIRCLE);
      p->ttl.back=false;
   }
}

bool is_out_of_bounds(Program * p)
{
   int height;
   int width;
   if(p->output_location==PS){
      height=A4_HEIGHT;
      width=A4_WIDTH;
   }
   else{
      height=SCREEN_HEIGHT;
      width=SCREEN_WIDTH;
   }
   if (round(p->ttl.y) < 0 || round(p->ttl.y) >= height || 
      round(p->ttl.x) < 0 || round(p->ttl.x) >= width){
      return true;
   }
   return false;
}

void test_is_out_of_bounds(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // by default, shouldn't ever be out of bounds.
   prog->output_location=SCREEN;
   init_prog_variables(prog);
   assert(!is_out_of_bounds(prog));
   prog->ttl.x=-1;
   assert(is_out_of_bounds(prog));
   prog->ttl.x=17;
   prog->ttl.y=-1;
   assert(is_out_of_bounds(prog));
   prog->ttl.x=SCREEN_WIDTH;
   prog->ttl.y=0;
   assert(is_out_of_bounds(prog));
   prog->ttl.x=17;
   prog->ttl.y=SCREEN_HEIGHT;
   assert(is_out_of_bounds(prog));
   stack_free(prog->stck);
   free(prog);

   Program* prog2 = ncalloc(1, sizeof(Program));
   prog2->output_location=TXT;
   init_prog_variables(prog2);
   assert(!is_out_of_bounds(prog2));
   prog2->ttl.x=-1;
   assert(is_out_of_bounds(prog2));
   prog2->ttl.x=17;
   prog2->ttl.y=-1;
   assert(is_out_of_bounds(prog2));
   prog2->ttl.x=SCREEN_WIDTH;
   prog2->ttl.y=0;
   assert(is_out_of_bounds(prog2));
   prog2->ttl.x=17;
   prog2->ttl.y=SCREEN_HEIGHT;
   assert(is_out_of_bounds(prog2));
   stack_free(prog2->stck);
   free(prog2);

   Program* prog3 = ncalloc(1, sizeof(Program));
   prog3->output_location=PS;
   init_prog_variables(prog3);
   assert(!is_out_of_bounds(prog3));
   prog3->ttl.x=-1;
   assert(is_out_of_bounds(prog3));
   prog3->ttl.x=17;
   prog3->ttl.y=-1;
   assert(is_out_of_bounds(prog3));
   prog3->ttl.x=A4_WIDTH;
   prog3->ttl.y=0;
   assert(is_out_of_bounds(prog3));
   prog3->ttl.x=17;
   prog3->ttl.y=A4_HEIGHT;
   assert(is_out_of_bounds(prog3));
   stack_free(prog3->stck);
   free(prog3);
}

bool write_turtle_to_arr(Program * p)
{
   int c = round(p->ttl.x);
   int r = round(p->ttl.y); 
   // we don't write to the array in this case.
   if(p->output_location==PS){
      return true;
   }
   if(is_out_of_bounds(p)){
      return false;
   }
   else if(p->ttl.col==white){
      p->output[r][c]='W';
   }
   else if(p->ttl.col==black){
      p->output[r][c]='K';
   }
   else if(p->ttl.col==red){
      p->output[r][c]='R';
   }
   else if(p->ttl.col==green){
      p->output[r][c]='G';
   }
   else if(p->ttl.col==yellow){
      p->output[r][c]='Y';
   }
   else if(p->ttl.col==blue){
      p->output[r][c]='B';
   }
   else if(p->ttl.col==magenta){
      p->output[r][c]='M';
   }
   else if(p->ttl.col==cyan){
      p->output[r][c]='C';
   }
   else{
      return false;
   }
   return true;
}

void test_write_turtle_to_arr(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   prog->output_location=PS;
   // doesn't matter, is always valid for ps.
   prog->ttl.x=-1;
   prog->ttl.y=-1;
   assert(write_turtle_to_arr(prog));
   prog->output_location=SCREEN;
   // out of bounds;
   prog->ttl.x=-1;
   prog->ttl.y=-1;
   assert(!write_turtle_to_arr(prog));
   prog->ttl.col=cyan;
   prog->ttl.x=0;
   prog->ttl.y=0;
   assert(write_turtle_to_arr(prog));
   assert(prog->output[0][0]='C');
   prog->ttl.col=white;
   prog->ttl.x=1;
   prog->ttl.y=0;
   assert(write_turtle_to_arr(prog));
   assert(prog->output[0][1]='K');

   stack_free(prog->stck);
   free(prog);
}

void print_letter_w_colour_code(char letter)
{
   if(letter == 'W'){
      neillfgcol(white);
   }
   if(letter == 'K'){
      neillfgcol(black);
   }
   if(letter == 'R'){
      neillfgcol(red);
   }
   if(letter == 'G'){
      neillfgcol(green);
   }
   if(letter == 'Y'){
      neillfgcol(yellow);
   }
   if(letter == 'B'){
      neillfgcol(blue);
   }
   if(letter == 'M'){
      neillfgcol(magenta);
   }
   if(letter == 'C'){
      neillfgcol(cyan);
   }
   printf("%c",letter);
}

void print_arr_to_screen(Program * p)
{
   neillclrscrn();
   for(int r=0; r<SCREEN_HEIGHT; r++){
      for(int c=0; c<SCREEN_WIDTH; c++){
         char letter = p->output[r][c];
         print_letter_w_colour_code(letter);
      }  
      printf("\n");
   }  
   printf("\n");
   neillbusywait(1);
}

// <RGT> ::= "RIGHT" <VARNUM>
bool run_rgt(Program *p)
{
   if(!STRSAME(CURRENT_WORD,"RIGHT")){
      return false;
   }
   p->curr_word++;
   if(!run_varnum(p)){
      return false;
   }
   p->ttl.angle = add_to_angle(p->ttl.angle, 
                              p->curr_var.num_var);
   return true;
}

void test_add_to_angle(void) 
{
   assert(fequal(add_to_angle(30.0, 40.0), 70.0));
   assert(fequal(add_to_angle(350.0, 20.0), 10.0));
   assert(fequal(add_to_angle(350.0, 10.0), 0.0));
   assert(fequal(add_to_angle(10.0, -20.0), 350.0));
   assert(fequal(add_to_angle(0.0, 720.0), 0.0));
   assert(fequal(add_to_angle(-30.0, 60.0), 30.0));
}

double add_to_angle(double current_angle, double add_value)
{
   double new_angle = fmod(current_angle + add_value, MAX_DEGREES);

  // Handling negative angles
   if (new_angle < 0) {
      new_angle += MAX_DEGREES;
   }
   return new_angle;
}

void test_run_rgt(void)
{
   Program* prog = ncalloc(1, sizeof(Program));

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"45");
   run_rgt(prog);
   assert(fequal(prog->ttl.angle, 45));
   prog->curr_word=0;

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"$A");
   assert(!run_rgt(prog));
   prog->curr_word = 0;

   set_variable_to_num('A', prog, 45);
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"$A");
   run_rgt(prog);
   assert(fequal(prog->ttl.angle, 90));
   prog->curr_word = 0;

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"-10");
   assert(run_rgt(prog));
   assert(prog->curr_word == 2);
   assert(fequal(prog->ttl.angle, 80));
   prog->curr_word = 0;

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"3.3");
   assert(run_rgt(prog));
   assert(fequal(prog->ttl.angle, 83.3));
   prog->curr_word = 0;

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"276.7");
   assert(run_rgt(prog));
   assert(fequal(prog->ttl.angle, 0));
   prog->curr_word = 0;

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"1");
   assert(run_rgt(prog));
   assert(fequal(prog->ttl.angle, 1));
   prog->curr_word = 0;

   // right missing.
   strcpy(prog->words[0],"fads");
   strcpy(prog->words[1],"3.0");
   assert(!run_rgt(prog));
   prog->curr_word = 0;

   free(prog);
}

// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
bool run_ins(Program *p)
{
   if(STRSAME(CURRENT_WORD, "FORWARD")){
      return run_fwd(p);
   }
   else if(STRSAME(CURRENT_WORD, "RIGHT")){
      return run_rgt(p);
   }
   else if(STRSAME(CURRENT_WORD, "COLOUR")){
      return run_col(p);
   }
   else if(STRSAME(CURRENT_WORD, "LOOP")){
      return run_loop(p);
   }
   else if(STRSAME(CURRENT_WORD, "SET")){
      return run_set(p);
   }
   return false;
}

void test_run_ins(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"12");
   assert(run_ins(prog));
   assert(fequal(prog->ttl.y, START_Y-12));
   prog->curr_word = 0;

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"-82.43");
   assert(run_ins(prog));
   assert(fequal(prog->ttl.angle, START_ANGLE-82.43));
   prog->curr_word = 0;
   set_variable_to_num('Z', prog, 8);
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"$Z");
   assert(run_ins(prog));
   assert(fequal(prog->ttl.angle, START_ANGLE-82.43+8));
   prog->curr_word = 0;

   // testing with random strings and non numbers.
   strcpy(prog->words[0],"MOW");
   strcpy(prog->words[1],"-82.43");
   assert(!run_ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"far");
   assert(!run_ins(prog));
   prog->curr_word = 0;

   stack_free(prog->stck);
   free(prog);
}

// <INSLST> ::= "END" | <INS> <INSLST>
bool run_inslst(Program *p)
{
   if(STRSAME(CURRENT_WORD, "END")){
      return true;
   }
   if(!run_ins(p)){
      return false;
   }
   if(!run_inslst(p)){
      return false;
   }
   return true;
}

void test_run_inslst(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;
   set_variable_to_num('Z', prog, 8);
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"12");
   strcpy(prog->words[2],"RIGHT");
   strcpy(prog->words[3],"-82.43");
   strcpy(prog->words[4],"RIGHT");
   strcpy(prog->words[5],"$Z");
   strcpy(prog->words[6],"COLOUR");
   strcpy(prog->words[7],"\"RED\"");
   strcpy(prog->words[8],"END");
   assert(run_inslst(prog));
   assert(prog->curr_word==8);
   assert(fequal(prog->ttl.y, START_Y-12));
   assert(fequal(prog->ttl.angle, START_ANGLE-82.43+8));
   assert(prog->ttl.col==red);
   prog->curr_word = 0;

   // testing with random strings and non numbers.
   strcpy(prog->words[0],"MOW");
   strcpy(prog->words[1],"-82.43");
   assert(!run_inslst(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"far");
   assert(!run_inslst(prog));
   prog->curr_word = 0;

   // test no end.
   set_variable_to_num('Z', prog, 8);
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"12");
   strcpy(prog->words[2],"RIGHT");
   strcpy(prog->words[3],"-82.43");
   strcpy(prog->words[4],"RIGHT");
   strcpy(prog->words[5],"$Z");
   strcpy(prog->words[6],"COLOUR");
   strcpy(prog->words[7],"\"RED\"");
   strcpy(prog->words[8],"FORWARD");
   assert(!run_inslst(prog));
   prog->curr_word = 0;

   stack_free(prog->stck);
   free(prog);
}

// <PROG>   ::= "START" <INSLST>
bool run_program(Program *p)
{
   if(!STRSAME(CURRENT_WORD, "START")){
      return false;
   }
   p->curr_word++;
   return run_inslst(p);
}

void test_run_program(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"END");
   assert(run_program(prog));
   prog->curr_word = 0;

   // starting with END should fail
   strcpy(prog->words[0],"END");
   assert(!run_program(prog));
   prog->curr_word = 0;

   // ending with START should fail
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"START");
   assert(!run_program(prog));
   prog->curr_word = 0;

   // valid file simple
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"FORWARD");
   strcpy(prog->words[2],"5");
   strcpy(prog->words[3],"RIGHT");
   strcpy(prog->words[4],"90");
   strcpy(prog->words[5],"FORWARD");
   strcpy(prog->words[6],"1");
   strcpy(prog->words[7],"COLOUR");
   strcpy(prog->words[8],"\"BLUE\"");
   strcpy(prog->words[9],"END");
   assert(run_program(prog));
   assert(prog->curr_word==9);
   assert(fequal(prog->ttl.y, START_Y-5));
   assert(fequal(prog->ttl.x, START_X+1));
   assert(fequal(prog->ttl.angle, START_ANGLE+90));
   assert(prog->ttl.col==blue);

   prog->curr_word = 0;


   stack_free(prog->stck);
   init_prog_variables(prog);

   // testing that a  recursive function suceeds.
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"FORWARD");
   strcpy(prog->words[2],"5");
   strcpy(prog->words[3],"LOOP");
   strcpy(prog->words[4],"C");
   strcpy(prog->words[5],"OVER");
   strcpy(prog->words[6],"{");
   strcpy(prog->words[7],"\"RED\"");
   strcpy(prog->words[8],"\"GREEN\"");
   strcpy(prog->words[9],"\"YELLOW\"");
   strcpy(prog->words[10],"\"BLUE\"");
   strcpy(prog->words[11],"}");
   strcpy(prog->words[12],"COLOUR");
   strcpy(prog->words[13],"$C");
   strcpy(prog->words[14],"FORWARD");
   strcpy(prog->words[15],"0");
   strcpy(prog->words[16],"RIGHT");
   strcpy(prog->words[17],"90");
   strcpy(prog->words[18],"END");
   strcpy(prog->words[19],"END");
   assert(run_program(prog));
   assert(prog->curr_word==19);
   prog->curr_word = 0;

   // testing failure when one string is modified.
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"FORWARD");
   strcpy(prog->words[2],"5");
   strcpy(prog->words[3],"LOOP");
   strcpy(prog->words[4],"C");
   strcpy(prog->words[5],"OVER");
   strcpy(prog->words[6],"{");
   strcpy(prog->words[7],"\"RED\"");
   strcpy(prog->words[8],"\"GREEN\"");
   strcpy(prog->words[9],"\"YELLOW\"");
   strcpy(prog->words[10],"\"BLUE"); // modified line.
   strcpy(prog->words[11],"}");
   strcpy(prog->words[12],"COLOUR");
   strcpy(prog->words[13],"$C");
   strcpy(prog->words[14],"FORWARD");
   strcpy(prog->words[15],"$D");
   strcpy(prog->words[16],"RIGHT");
   strcpy(prog->words[17],"90");
   strcpy(prog->words[18],"END");
   assert(!run_program(prog));
   prog->curr_word = 0;

   stack_free(prog->stck);
   free(prog);
}

Program * init_program(char * argv[], int argc)
{
   File_Type ft;
   eval_args(argc, argv, &ft);
   Program* prog = ncalloc(1, sizeof(Program));
   get_prog_from_file(argv[INPUT_FILE_INDEX], prog);
   prog->output_location = ft; 
   init_prog_variables(prog);
   if(prog->output_location!=SCREEN){
      if(strlen(argv[OUTPUT_FILE_INDEX])>=SMALLSTR){
         on_error("output file too long.");
      }
      strcpy(prog->output_file_name, argv[OUTPUT_FILE_INDEX]);
   }
   return prog;
}

void init_prog_variables(Program * prog)
{
   set_prog_output_to_spaces(prog);
   prog->ttl.angle=START_ANGLE;
   if(prog->output_location==PS){
      prog->ttl.x = START_X_PS;
      prog->ttl.y = START_Y_PS;
   }
   else{
      prog->ttl.x = START_X;
      prog->ttl.y = START_Y;
   }
   prog->ttl.col=white;
   prog->stck = stack_init();
}

void test_init_prog_variables(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   init_prog_variables(prog);
   prog->output_location=TXT;
   assert(prog->output[0][0]=' ');
   assert(prog->output[SCREEN_HEIGHT-1][SCREEN_WIDTH-1]=' ');
   assert(fequal(prog->ttl.angle,START_ANGLE));
   assert(fequal(prog->ttl.x,START_X));
   assert(fequal(prog->ttl.y,START_Y));
   assert(prog->ttl.col==white);

   prog->output_location=PS;
   stack_free(prog->stck);
   init_prog_variables(prog);
   assert(fequal(prog->ttl.angle,START_ANGLE));
   assert(fequal(prog->ttl.x,START_X_PS));
   assert(fequal(prog->ttl.y,START_Y_PS));
   assert(prog->ttl.col==white);
   stack_free(prog->stck);
   free(prog);
}

void set_prog_output_to_spaces(Program * p)
{
   for(int r=0; r<SCREEN_HEIGHT; r++){
         for(int c=0; c<SCREEN_WIDTH; c++){
            p->output[r][c]=' '; 
         }  
      }  
}

void test_set_prog_output_to_spaces(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   set_prog_output_to_spaces(prog);
   assert(prog->output[0][0]=' ');
   assert(prog->output[SCREEN_HEIGHT-1][SCREEN_WIDTH-1]=' ');
   free(prog);
}

void get_prog_from_file(char * filename, Program * prog)
{
   FILE * f = nfopen(filename,"r");
   int i=0;
   while(fscanf(f, "%s", prog->words[i++])==1 && i<MAXNUMTOKENS);
   fclose(f);
   if(i>=MAXNUMTOKENS){
      free(prog);
      on_error("File contains too many tokens to parse.");
   }
   prog->output_location=SCREEN;
}

bool fequal(double a, double b)
{
    return fabs(a-b) < FLOAT_PRECISION;
}

void test_fequal(void)
{
   assert(fequal(0.01,0.01));
   assert(fequal(-0.01,-0.01));
   assert(!fequal(0.02,0.01));
   assert(!fequal(-0.02,-0.01));
}
