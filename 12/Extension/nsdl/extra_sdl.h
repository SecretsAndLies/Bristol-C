#define DEGREES_TO_RADIANS(degrees) ((degrees) * (M_PI / 180.0))
#define START_ANGLE 90
#define MAX_INSTRUCTIONS 200000
#define MAX_INSTRUCTION_LEN 200

struct SDL_Simplewin {
   SDL_Window *win;
   SDL_Renderer *renderer;
   SDL_Texture *display;
   SDL_Texture *backBuffer;
   SDL_Texture *midBuffer;
};
typedef struct SDL_Simplewin SDL_Simplewin;
