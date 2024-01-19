#include "extension.h"

int main(void) 
{
   test();
   SDL_Simplewin sw;
   Neill_SDL_Init(&sw);
   printf("START\n");
   Turtle * ttl = init_ttl_vars();
   do {
      SDL_Delay(MILLISECONDDELAY);
      draw(&sw, ttl);
   } while (!ttl->finished);
   free(ttl);
   return 0;
}

void* acalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v==NULL){
      on_error("Cannot calloc() space");
   }
   return v;
}

void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}

Turtle * init_ttl_vars(void)
{
   Turtle * ttl = acalloc(1, sizeof(Turtle));
   ttl->x = START_X;
   ttl->y = START_Y;
   ttl->colour=white;
   ttl->angle=START_ANGLE;
   ttl->ins_amount=0;
   ttl->instruction[0]='\0';
   return ttl;
}

void set_draw_colour(SDL_Simplewin *sw, neillcol colour)
{
   if(colour==green){
      Neill_SDL_SetDrawColour(sw, GREEN);
   }
   if(colour==white){
      Neill_SDL_SetDrawColour(sw, WHITE);
   }
   if(colour==red){
      Neill_SDL_SetDrawColour(sw, RED);
   }
   if(colour==blue){
      Neill_SDL_SetDrawColour(sw, BLUE);
   }
   if(colour==black){
      Neill_SDL_SetDrawColour(sw, WHITE);
   }
   if(colour==cyan){
      Neill_SDL_SetDrawColour(sw, CYAN);
   }
   if(colour==magenta){
      Neill_SDL_SetDrawColour(sw, MAGENTA);
   }
   if(colour==black){
      Neill_SDL_SetDrawColour(sw, BLACK);
   }
   if(colour==yellow){
      Neill_SDL_SetDrawColour(sw, YELLOW);
   }
   if(colour==dark_green){
      Neill_SDL_SetDrawColour(sw, DARK_GREEN);
   }
}

void draw(SDL_Simplewin *sw, Turtle * ttl)
{
   draw_ttl(sw,ttl);
   do_input(ttl);
   int orig_x = ttl->x;
   int orig_y = ttl->y;
   move_ttl(ttl);
   draw_trail(sw,orig_x,orig_y,ttl);
}

void draw_ttl(SDL_Simplewin *sw, Turtle * ttl)
{
   SDL_SetRenderTarget(sw->renderer, sw->backBuffer);
   Neill_SDL_SetDrawColour(sw, BLACK);
   SDL_RenderClear(sw->renderer);
   SDL_RenderCopy(sw->renderer, sw->midBuffer, NULL, NULL);
   set_draw_colour(sw, dark_green);
   Neill_SDL_RenderFillCircle(sw->renderer, ttl->x, ttl->y, CIRCLE_RAD);
   double radians = 
   DEGREES_TO_RADIANS(ttl->angle % DEGREES_IN_CIRC);
   SDL_SetRenderTarget(sw->renderer, sw->backBuffer);
   set_draw_colour(sw, ttl->colour);
   SDL_RenderDrawLine(sw->renderer, ttl->x, ttl->y, 
                     ttl->x-CIRCLE_RAD * cos(radians), 
                     ttl->y-CIRCLE_RAD * sin(radians));
   SDL_SetRenderTarget(sw->renderer, NULL);
   SDL_RenderCopy(sw->renderer, sw->backBuffer, NULL, NULL);
   SDL_RenderPresent(sw->renderer);
}

void draw_trail(SDL_Simplewin *sw, int orig_x, int orig_y, Turtle * ttl)
{
   SDL_SetRenderTarget(sw->renderer, sw->midBuffer);
   set_draw_colour(sw, ttl->colour);
   SDL_RenderDrawLine(sw->renderer, orig_x, orig_y, ttl->x, ttl->y);
}

void move_ttl(Turtle * ttl)
{
   double radians = 
   DEGREES_TO_RADIANS(ttl->angle % DEGREES_IN_CIRC);
   if (ttl->ctl.up == 1) {
      go_fwd(ttl, radians);
   }
   if (ttl->ctl.right == 1) {
      go_rgt(ttl);
   }
}

void go_rgt(Turtle * ttl)
{
   ttl->angle += STEP_SIZE;
   if(strcmp(ttl->instruction, "RIGHT")==0) {
      ttl->ins_amount=
      (ttl->ins_amount+STEP_SIZE)%DEGREES_IN_CIRC;
   }
   else { 
      print_inst_if_exists(ttl);
      sprintf(ttl->instruction,"RIGHT");
      ttl->ins_amount=STEP_SIZE;
   }
}

void go_fwd(Turtle * ttl, double radians)
{
   ttl->x -= STEP_SIZE * cos(radians);
   ttl->y -= STEP_SIZE * sin(radians);
   if(strcmp(ttl->instruction, "FORWARD")==0) {
      ttl->ins_amount++;
   }
   else { 
      print_inst_if_exists(ttl);
      sprintf(ttl->instruction,"FORWARD");
      ttl->ins_amount=1;
   }
}

void do_input(Turtle * ttl)
{
   SDL_Event event;

   while (SDL_PollEvent(&event)) {
      switch (event.type) {
         case SDL_QUIT:
            print_inst_if_exists(ttl);
            ttl->finished = 1;
            SDL_Quit();
            atexit(SDL_Quit);
            printf("END\n");
            break;

         case SDL_KEYDOWN:
            do_key_down(&event.key, ttl);
            break;

         case SDL_KEYUP:
            do_key_up(&event.key, ttl);
            break;

         default:
            break;
      }
   }
}

void do_key_down(SDL_KeyboardEvent *event, Turtle * ttl)
{
   if (event->repeat == 0) {
      if (event->keysym.scancode == SDL_SCANCODE_UP) {
         ttl->ctl.up = 1;
      }
      if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
         ttl->ctl.right = 1;
      }
      if (event->keysym.scancode == SDL_SCANCODE_W) {
         printf("COLOUR \"WHITE\"\n");
         ttl->colour=white;
      }
      if (event->keysym.scancode == SDL_SCANCODE_G) {
         printf("COLOUR \"GREEN\"\n");
         ttl->colour=green;
      }
      if (event->keysym.scancode == SDL_SCANCODE_R) {
         printf("COLOUR \"RED\"\n");
         ttl->colour=red;
      }
      if (event->keysym.scancode == SDL_SCANCODE_Y) {
         printf("COLOUR \"YELLOW\"\n");
         ttl->colour=yellow;
      }
      if (event->keysym.scancode == SDL_SCANCODE_B) {
         printf("COLOUR \"BLUE\"\n");
         ttl->colour=blue;
      }
      if (event->keysym.scancode == SDL_SCANCODE_M) {
         printf("COLOUR \"MAGENTA\"\n");
         ttl->colour=magenta;
      }
      if (event->keysym.scancode == SDL_SCANCODE_C) {
         printf("COLOUR \"CYAN\"\n");
         ttl->colour=cyan;
      }
      if (event->keysym.scancode == SDL_SCANCODE_K) {
         ttl->colour=black;
         printf("COLOUR \"BLACK\"\n");
      }
   }
}

void print_inst_if_exists(Turtle * ttl)
{
   if(ttl->ins_amount) {
      printf("%s %i\n",ttl->instruction, ttl->ins_amount);
   }
}

void do_key_up(SDL_KeyboardEvent *event, Turtle * ttl)
{
   if (event->repeat == 0) {
      if (event->keysym.scancode == SDL_SCANCODE_UP) {
         ttl->ctl.up = 0;
      }
      if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
         ttl->ctl.right = 0;
      }
   }
}

void test(void)
{
   Turtle * ttl = init_ttl_vars();
   assert(ttl->angle==START_ANGLE);
   assert(ttl->x==START_X);
   assert(ttl->y==START_Y);
   assert(ttl->ins_amount==0);
   assert(ttl->instruction[0]=='\0');
   free(ttl);
}
