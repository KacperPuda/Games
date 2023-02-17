#pragma once
#ifndef funkcjeDoS_h
#define funkcjeDoS_h
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include"bosses.h"
#include"structs.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}



void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
void events(world& info, thing& player, boss& beza, background& ground);
void movePlayer(thing* player, world& info);
void settingPlayer(thing& player);
void bossShoots(thing& player, boss& beza, world& info);
void fighting(thing& player, world& info,  boss& beza, background& ground);
void clear(world info, int ret);
void settingBeza(boss& beza);
void settingBackground(background& ground);
void setting_all(thing& player, boss& beza, background& ground, world& info);
void set_colors(world& info);
void rendering(world& info, thing& player, boss& beza, background& ground);
void fpslimit(world& info);
void firsRender(world& info);
void drawbackground(world& info, background& ground, thing& player);
#endif
