#include "extension.h"

int main(void) 
{
   SDL_Simplewin sw;
   Neill_SDL_Init(&sw);
   sw.ttl.x = (WWIDTH - RECTSIZE) / 2;
   sw.ttl.y = (WHEIGHT - RECTSIZE) / 2; // TODO NEIL MIGHT CONSIDER MAGIC.
   set_draw_colour(&sw, GREEN);
   sw.ttl.angle=START_ANGLE;

   do {
      SDL_Delay(MILLISECONDDELAY);
      draw(&sw);
   } while (!sw.finished);

   SDL_Quit();
   atexit(SDL_Quit);
   return 0;
}

void set_draw_colour(SDL_Simplewin *sw, int red, int green, int blue)
{
   Neill_SDL_SetDrawColour(sw, red, green, blue);
}

void draw(SDL_Simplewin *sw)
{
   Neill_SDL_RenderFillCircle(sw->renderer, sw->ttl.x, sw->ttl.y, 10);
   Neill_SDL_UpdateScreen(sw);
   do_input(sw);
   int stepSize = 5; // TODO MAGIC.
   double radians = DEGREES_TO_RADIANS(sw->ttl.angle%360);
   if(sw->ctl.up==1){
      sw->ttl.x -= stepSize * cos(radians);
      sw->ttl.y -= stepSize * sin(radians);
      printf("FORWARD 1\n");
   }
   if(sw->ctl.right==1){
      sw->ttl.angle += 5; // TODO MAGIC
      printf("RIGHT 5\n");
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
