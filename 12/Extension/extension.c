#include "extension.h"

int main(void) {
  SDL_Simplewin sw;
  Neill_SDL_Init(&sw);
  do {
    SDL_Delay(MILLISECONDDELAY);
    Neill_SDL_SetDrawColour(&sw, GREEN);
    int cx = (WWIDTH - RECTSIZE) / 2;
    int cy = (WHEIGHT - RECTSIZE) / 2;
    Neill_SDL_RenderFillCircle(sw.renderer, cx, cy, 10);
    Neill_SDL_UpdateScreen(&sw);
    doInput(&sw);

  } while (!sw.finished);

  /* Clear up graphics subsystems */
  SDL_Quit();
  atexit(SDL_Quit);

  return 0;
}


void doInput(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				doKeyDown(&event.key);
				break;

			case SDL_KEYUP:
				doKeyUp(&event.key);
				break;

			default:
				break;
		}
	}
}

void doKeyDown(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			app.up = 1;
        }

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			app.right = 1;
		}
	}
}

void doKeyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			app.up = 0;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			app.right = 0;
		}
	}
}
