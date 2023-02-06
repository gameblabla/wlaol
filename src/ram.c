#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "define.h"

SDL_Surface *g_screen;
int g_scene;
int g_scene_req;
int32_t gameflag[1024];
int32_t gameflag2[1024];

int32_t g_DebugDisp;
int32_t g_DebugStageProduction;
int32_t g_DebugMuteki;
int32_t g_DebugKey;

int32_t g_DebugKeyStat;
