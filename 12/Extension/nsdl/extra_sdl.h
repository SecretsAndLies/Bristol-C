#define DEGREES_TO_RADIANS(degrees) ((degrees) * (M_PI / 180.0))
#define START_ANGLE 90
#define MAX_INSTRUCTIONS 200000
#define MAX_INSTRUCTION_LEN 200

typedef struct {
   int up;
   int right;
} Controls;

enum neillcol {black=30, red, green, yellow, blue, magenta, cyan, white, dark_green};
typedef enum neillcol neillcol;

typedef struct {
   int x;
   int y;
   int angle;
   neillcol colour;
   char instruction[200];
   int ins_amount;
} Turtle;

struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
   SDL_Texture *display;
   SDL_Texture *backBuffer;
   SDL_Texture *midBuffer;
   Controls ctl;
   Turtle ttl;
};
typedef struct SDL_Simplewin SDL_Simplewin;
