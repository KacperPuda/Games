#pragma once

#ifndef structs_h
#define structs_h

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include"bosses.h"
#define MENU	1

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define BulletHitboxU	-2
#define BulletHitboxD	40
#define BulletHitboxR	23
#define BulletHitboxL	-4

struct thing
{
	double x;
	double y;
	double speed;
	SDL_Surface* texture, * textureR, * textureL, * textureD, * textureU,*me;
	double hitboxL;
	double hitboxR;
	double hitboxU;
	double hitboxD;
	int health,score=1000,bonus=1;
	int direct,inv,invtimer;
	bool exist;
	double plusX;
	double plusY;
	double ax, ay;
};

struct boss
{
	double health, mHealth;
	int xCenter,yCenter;
	double passedTime,comboTime;
	int attack,shootinfo;
	int repeats;
	double hitboxL,hitboxR,hitboxU,hitboxD;
	double BhitboxL, BhitboxR, BhitboxU, BhitboxD;
	double x, y;
	int comborepeats,combo;
	int milliseconds;
	int animation;
	int textre;
	SDL_Surface* maksHealth,*half1,*half2,*end,*left,*up,*down,* right,*normal;
};

struct background
{
	SDL_Surface* menu;
	double x, y;
	SDL_Surface* fight1;
	int animation;
	SDL_Surface* gameover,*nav, * winscreen;
};

typedef struct projectile
{
	double x;
	double y;
	double xa, ya;
	double hitboxL;
	double hitboxR;
	double hitboxU;
	double hitboxD;
	struct projectile* next;
}projectile_type;

typedef struct bullet
{
	double x;
	double y;
	int direct;
	int bulletType;
	double hitboxL;
	double hitboxR;
	double hitboxU;
	double hitboxD;
	double speed;
	double plusX;
	double plusY;
	double timer;
	double r;
	double angle;
	struct bullet* next;
}bullet_type;

struct world
{
	int t1, t2, quit = 0, frames = 0, rc, plan, tryb = MENU;
	SDL_Event event;
	SDL_Surface* screen, * charset, *bigbullet,*bonusH,* bullet,*brim, * tlo, * bossT, * me,*mybullet,*goodLife,*badLife,*screenTexture,*trapdor,*bulletbomb,*bulletarea,*bullethotarea,*bulletdestroyF, * bulletdestroyS, * bulletdestroyT,*bulletdestroy,*grade,*ga,*gb,*gs,*gc,*gf,*gd;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	double delta, worldTime = 0.0, fpsTimer = 0, fps = 0, distance, speed2 = 0.00,bonusTimer=0, bonusX, bonusY;
	projectile_type*pociskiHead;
	bullet_type* bulletHead;
	int czarny, czerwony, zielony, niebieski;
	int lvl;
	SDL_Surface* ex1, * ex2, * ex3, * ex4, * ex5,* ex;
	bool bonus = false;
};

#endif