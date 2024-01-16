#define DEGREES_TO_RADIANS(degrees) ((degrees) * (M_PI / 180.0))
#define START_ANGLE 90


typedef struct {
   int up;
   int right;
} Controls;

typedef struct {
   int x;
   int y;
   int angle;
   int red;
   int green;
   int blue;
} Turtle;

/* All info required for windows / renderer & event loop */
struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
   SDL_Texture *display;
   Controls ctl;
   Turtle ttl;
};
typedef struct SDL_Simplewin SDL_Simplewin;
void draw(SDL_Simplewin *sw);