#include "extension.h"

int main(void) 
{
   SDL_Simplewin sw;
   Neill_SDL_Init(&sw);
   printf("START\n");
   init_ttl_vars(&sw);
   do {
      SDL_Delay(MILLISECONDDELAY);
      draw(&sw);
   } while (!sw.finished);

   SDL_Quit();
   atexit(SDL_Quit);
   return 0;
}

// todo this is testable.
void init_ttl_vars(SDL_Simplewin *sw)
{
   sw->ttl.x = START_X;
   sw->ttl.y = START_Y;
   sw->ttl.colour=white;
   sw->ttl.angle=START_ANGLE;
   sw->ttl.ins_amount=0;
   sw->ttl.instruction[0]='\0';
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

void draw(SDL_Simplewin *sw)
{
   // Set render target to back buffer
   SDL_SetRenderTarget(sw->renderer, sw->backBuffer);

   /* Clear back renderer, & set draw colour to black */
   Neill_SDL_SetDrawColour(sw, BLACK);
   SDL_RenderClear(sw->renderer);

   // Render midBuffer onto backBuffer
   SDL_RenderCopy(sw->renderer, sw->midBuffer, NULL, NULL);

    // Draw turtle the back buffer
   set_draw_colour(sw, dark_green); // todo this is where I was changing the colour to what's in the struct.
   Neill_SDL_RenderFillCircle(sw->renderer, sw->ttl.x, sw->ttl.y, 10); // todo magic.

   // draw a line on the turtle to give viewers direction sense.
   double radians_2 = DEGREES_TO_RADIANS(sw->ttl.angle % DEGREES_IN_CIRC);
   SDL_SetRenderTarget(sw->renderer, sw->backBuffer);
   set_draw_colour(sw, sw->ttl.colour);
   SDL_RenderDrawLine(sw->renderer, sw->ttl.x, sw->ttl.y, 
                     sw->ttl.x-10 * cos(radians_2), 
                     sw->ttl.y-10 * sin(radians_2));

   // Reset render target to default (to render to the screen)
   SDL_SetRenderTarget(sw->renderer, NULL);

   // Swap the back buffer with the front buffer
   SDL_RenderCopy(sw->renderer, sw->backBuffer, NULL, NULL);
   SDL_RenderPresent(sw->renderer);

   do_input(sw);
   int orig_x = sw->ttl.x;
   int orig_y = sw->ttl.y;
   move_ttl(sw);
   draw_trail(sw,orig_x,orig_y);
}

void draw_trail(SDL_Simplewin *sw, int orig_x, int orig_y)
{
   SDL_SetRenderTarget(sw->renderer, sw->midBuffer);
   set_draw_colour(sw, sw->ttl.colour);
   SDL_RenderDrawLine(sw->renderer, orig_x, orig_y, sw->ttl.x, sw->ttl.y);
}

void move_ttl(SDL_Simplewin *sw)
{
   double radians = DEGREES_TO_RADIANS(sw->ttl.angle % DEGREES_IN_CIRC);
   if (sw->ctl.up == 1) {
      sw->ttl.x -= STEP_SIZE * cos(radians);
      sw->ttl.y -= STEP_SIZE * sin(radians);
      if(strcmp(sw->ttl.instruction, "FORWARD")==0){
         sw->ttl.ins_amount++;
      }
      else{ 
         if(sw->ttl.ins_amount){
            printf("%s %i\n",sw->ttl.instruction, sw->ttl.ins_amount);
         }
         sprintf(sw->ttl.instruction,"FORWARD");
         sw->ttl.ins_amount=1;
      }
  }
   if (sw->ctl.right == 1) {
      sw->ttl.angle += STEP_SIZE;
      // duplicate code.TODO
      if(strcmp(sw->ttl.instruction, "RIGHT")==0){
         sw->ttl.ins_amount=(sw->ttl.ins_amount+STEP_SIZE)%DEGREES_IN_CIRC;
      }
      else{ 
         if(sw->ttl.ins_amount){
            printf("%s %i\n",sw->ttl.instruction, sw->ttl.ins_amount);
         }
         sprintf(sw->ttl.instruction,"RIGHT");
         sw->ttl.ins_amount=STEP_SIZE;
      }
   }
}

void do_input(SDL_Simplewin *sw)
{
   SDL_Event event;

   while (SDL_PollEvent(&event)) {
      switch (event.type) {
         case SDL_QUIT:
            exit(0);
            break;

         case SDL_KEYDOWN:
            do_key_down(&event.key, sw);
            break;

         case SDL_KEYUP:
            do_key_up(&event.key, sw);
            break;

         default:
            break;
      }
   }
}

void do_key_down(SDL_KeyboardEvent *event, SDL_Simplewin *sw)
{
   if (event->repeat == 0) {
      if (event->keysym.scancode == SDL_SCANCODE_UP) {
         sw->ctl.up = 1;
      }
      if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
         sw->ctl.right = 1;
      }
      if (event->keysym.scancode == SDL_SCANCODE_W) {
         printf("COLOUR \"WHITE\"\n");
         sw->ttl.colour=white;
      }
      if (event->keysym.scancode == SDL_SCANCODE_G) {
         printf("COLOUR \"GREEN\"\n");
         sw->ttl.colour=green;
      }
      if (event->keysym.scancode == SDL_SCANCODE_R) {
         printf("COLOUR \"RED\"\n");
         sw->ttl.colour=red;
      }
      if (event->keysym.scancode == SDL_SCANCODE_Y) {
         printf("COLOUR \"YELLOW\"\n");
         sw->ttl.colour=yellow;
      }
      if (event->keysym.scancode == SDL_SCANCODE_B) {
         printf("COLOUR \"BLUE\"\n");
         sw->ttl.colour=blue;
      }
      if (event->keysym.scancode == SDL_SCANCODE_M) {
         printf("COLOUR \"MAGENTA\"\n");
         sw->ttl.colour=magenta;
      }
      if (event->keysym.scancode == SDL_SCANCODE_C) {
         printf("COLOUR \"CYAN\"\n");
         sw->ttl.colour=cyan;
      }
      if (event->keysym.scancode == SDL_SCANCODE_K) {
         printf("COLOUR \"BLACK\"\n");
         sw->ttl.colour=black;
      }
      if (event->keysym.scancode == SDL_SCANCODE_Q) {
         if(sw->ttl.ins_amount){
            printf("%s %i\n",sw->ttl.instruction, sw->ttl.ins_amount);
         }
         sw->finished = 1;
         printf("END\n");
      }
   }
}

void do_key_up(SDL_KeyboardEvent *event, SDL_Simplewin *sw)
{
   if (event->repeat == 0) {
      if (event->keysym.scancode == SDL_SCANCODE_UP) {
         sw->ctl.up = 0;
      }
      if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
         sw->ctl.right = 0;
      }
   }
}
