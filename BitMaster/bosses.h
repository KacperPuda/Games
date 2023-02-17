#pragma once
#ifndef bosses_h
#define bosses_h
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include"bosses.h"
#include"structs.h"
#include"funkcjeDoS.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

void bossAttac1(thing player, boss& beza, world& info);
void bossAttac1_2(thing player, boss& beza, world& info);
void bossAttac2(thing &player, boss& beza, world& info);
void move(thing* player, boss beza, world &info);
void bossAttac3(thing player, boss& beza, world& info);
void bossAttac4(thing player, boss& beza, world& info);
void bossAttac5(thing player, boss& beza, world& info);
void bossAttac6(thing player, boss& beza, world& info);
void bossAttac7(thing player, boss& beza, world& info);
void bossAttac7_2(thing player, boss& beza, world& info);
void bossAttac8(thing player, boss& beza, world& info);
#endif