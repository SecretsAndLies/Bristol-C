#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "../General/general.h"
#include "nsdl/neillsdl2.h"

#define WHITE 255, 255, 255
#define YELLOW 255, 255, 0
#define BLACK 0, 0, 0
#define GREEN 0, 100, 0

typedef struct {
	int up;
	int right;
} app;

void doInput(void);

void doKeyDown(SDL_KeyboardEvent *event);
void doKeyUp(SDL_KeyboardEvent *event);
