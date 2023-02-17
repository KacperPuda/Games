#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"bosses.h"
#include"structs.h"
#include"definitions.h"
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


void DrawString(SDL_Surface* screen, int x, int y, const char* text,SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 5;
	dest.y = y - sprite->h / 20;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void firebullet(world& info, thing& player, double x, double y)
{
	if (info.pociskiHead == NULL)
	{
		info.pociskiHead= (projectile_type*)malloc(sizeof(projectile_type));
		info.pociskiHead->x= player.x;
		info.pociskiHead->y = player.y;
		info.pociskiHead->xa = x;
		info.pociskiHead->ya = y;
		info.pociskiHead->hitboxU = BulletHitboxU;
		info.pociskiHead->hitboxD = BulletHitboxD;
		info.pociskiHead->hitboxR = BulletHitboxR;
		info.pociskiHead->hitboxL = BulletHitboxL;
		info.pociskiHead->next = NULL;
	}
	else
	{
		projectile_type*pocisk;
		pocisk = (projectile_type*)malloc(sizeof(projectile_type));
		pocisk->next = info.pociskiHead;
		pocisk->x = player.x;
		pocisk->y = player.y;
		pocisk->xa = x;
		pocisk->ya = y;
		pocisk->hitboxD= BulletHitboxD;
		pocisk->hitboxU = BulletHitboxU;
		pocisk->hitboxR = BulletHitboxR;
		pocisk->hitboxL = BulletHitboxL;
		info.pociskiHead = pocisk;
	}
	player.direct = Playershotspeed;
}

bool projectileColision(projectile_type* bullet, boss beza)
{
	SDL_Rect box1, box2;
	if (bullet == NULL)return false;
	box1.x = bullet->x + bullet->hitboxL;
	box1.y = bullet->y + bullet->hitboxU;
	box1.h = bullet->hitboxD - bullet->hitboxU;
	box1.w = bullet->hitboxR - bullet->hitboxL;
	box2.x = beza.x + beza.hitboxL;
	box2.y = beza.y + beza.hitboxU;
	box2.h = beza.hitboxD - beza.hitboxU;
	box2.w = beza.hitboxR - beza.hitboxL;
	if (SDL_HasIntersection(&box1, &box2)) return true;
	return false;
}

bool playerColision(thing player, boss beza)
{
	SDL_Rect box1, box2;
	box1.x = player.x + player.hitboxL;
	box1.y = player.y + player.hitboxU;
	box1.h = player.hitboxD - player.hitboxU;
	box1.w = player.hitboxR - player.hitboxL;
	box2.x = beza.x + beza.hitboxL;
	box2.y = beza.y + beza.hitboxU;
	box2.h = beza.hitboxD - beza.hitboxU;
	box2.w = beza.hitboxR - beza.hitboxL;
	if (SDL_HasIntersection(&box1, &box2)) return true;
	return false;
}

bool bonusColision(thing player, world info)
{
	SDL_Rect box1, box2;
	box1.x = player.x + player.hitboxL;
	box1.y = player.y + player.hitboxU;
	box1.h = player.hitboxD - player.hitboxU;
	box1.w = player.hitboxR - player.hitboxL;
	box2.x = info.bonusX-1;
	box2.y = info.bonusY - 1;
	box2.h = 28;
	box2.w = 28;
	if (SDL_HasIntersection(&box1, &box2)) return true;
	return false;
}

void removeProjectile(projectile_type*&pocisk,world &info,projectile_type *&prev)
{
	if (pocisk == info.pociskiHead)
	{
		projectile_type* temp = NULL;
		temp = info.pociskiHead->next;
		free(info.pociskiHead);
		info.pociskiHead = temp;
		pocisk = info.pociskiHead;
	}
	else
	{
		prev->next = pocisk->next;
		free(pocisk);
		pocisk = prev;
	}
}

static void doBullets(world& info,boss &beza,thing &player)
{
	projectile_type* pocisk = info.pociskiHead;
	projectile_type* tmp,*prev;
	prev = pocisk;
	while (pocisk!= NULL)
	{
		pocisk->x+=pocisk->xa*info.delta*DeltaToSecond-player.ax;
		pocisk->y += pocisk->ya * info.delta * DeltaToSecond -player.ay;
		tmp = pocisk;
		if (pocisk->x > SCREEN_WIDTH || pocisk->x < 0 || pocisk->y > SCREEN_HEIGHT || pocisk->y < 0)
		{
			removeProjectile(pocisk, info, prev);
			if (player.bonus > 0)player.bonus--;
		}
		if (projectileColision(pocisk, beza))
		{
			removeProjectile(pocisk, info, prev);
			beza.health--;
			player.score += 10*player.bonus;
			if (player.bonus < 10)player.bonus++;
		}
		prev = pocisk;
		if (pocisk == NULL) break;
			else pocisk = pocisk->next;
	}
}

void movePlayer(thing* player, world &info)
{
	const auto klawisze = SDL_GetKeyboardState(NULL);
	double x, y;
	x = player->x;
	y = player->y;
	if (player->direct > 0) player->direct--;

	if ((klawisze[SDL_SCANCODE_A]) && (player->x + player->hitboxL > 10)&& (!klawisze[SDL_SCANCODE_D]))
	{
		player->x = player->x - info.speed2* info.delta * DeltaToSecond + info.delta * DeltaToSecond;
		if (info.speed2 < player->speed) info.speed2 += 1;
		player->ax = -4 * info.delta * DeltaToSecond;
		if ((!klawisze[SDL_SCANCODE_W]) && (!klawisze[SDL_SCANCODE_S]))player->ay = 0;
		player->me = player->textureL;
	}
	else if ((!klawisze[SDL_SCANCODE_A]) && (player->ax < 0)) player->ax = 0;

	if ((klawisze[SDL_SCANCODE_D]) && (player->x + player->hitboxR < SCREEN_WIDTH-205) && (!klawisze[SDL_SCANCODE_A]))
	{
		player->x = player->x + info.speed2 * info.delta * DeltaToSecond -info.delta * DeltaToSecond;
		if (info.speed2 < player->speed) info.speed2 += 1;
		player->ax = 4 * info.delta * DeltaToSecond;
		if ((!klawisze[SDL_SCANCODE_W]) && (!klawisze[SDL_SCANCODE_S]))player->ay = 0;
		player->me = player->textureR;
	}
	else if ((!klawisze[SDL_SCANCODE_D]) && (player->ax > 0)) player->ax = 0;

	if ((klawisze[SDL_SCANCODE_W]) && (player->y + player->hitboxU > 5) && (!klawisze[SDL_SCANCODE_S]))
	{
		player->y = player->y - info.speed2 * info.delta * DeltaToSecond +info.delta * DeltaToSecond;
		if (info.speed2 < player->speed) info.speed2 += 1;
		player->ay = -2 * info.delta * DeltaToSecond;
		if ((!klawisze[SDL_SCANCODE_A]) && (!klawisze[SDL_SCANCODE_D]))player->ax = 0;
		player->me = player->textureU;
	}
	else if ((!klawisze[SDL_SCANCODE_W]) && (player->ay < 0)) player->ay = 0;

	if ((klawisze[SDL_SCANCODE_S]) && (player->y + player->hitboxD < SCREEN_HEIGHT-5) && (!klawisze[SDL_SCANCODE_W]))
	{
		player->y = player->y + info.speed2 * info.delta * DeltaToSecond - info.delta * DeltaToSecond;
		if (info.speed2 < player->speed) info.speed2 += 1;
		player->ay = 2 * info.delta * DeltaToSecond;
		if ((!klawisze[SDL_SCANCODE_A]) && (!klawisze[SDL_SCANCODE_D]))player->ax = 0;
		player->me = player->textureD;
	}
	else if ((!klawisze[SDL_SCANCODE_S]) && (player->ay > 0)) player->ay = 0;

	if ((!klawisze[SDL_SCANCODE_S]) && (!klawisze[SDL_SCANCODE_W]) && (!klawisze[SDL_SCANCODE_D]) && (!klawisze[SDL_SCANCODE_A]))
	{
		info.speed2 = 0.0;
		player->ax = 0;
		player->ay = 0;
		player->me = player->texture;
	}
	if((klawisze[SDL_SCANCODE_A]) && (klawisze[SDL_SCANCODE_D]))player->ax = 0;
	if ((klawisze[SDL_SCANCODE_W]) && (klawisze[SDL_SCANCODE_S]))player->ay = 0;

	if (klawisze[SDL_SCANCODE_UP])
	{
		if (player->direct == 0)firebullet(info, *player, 0, -PlayerBulletSpeed);
	}
	if (klawisze[SDL_SCANCODE_DOWN])
	{
		if (player->direct == 0)firebullet(info, *player, 0, PlayerBulletSpeed);
	}
	if (klawisze[SDL_SCANCODE_RIGHT])
	{
		if (player->direct == 0)firebullet(info, *player, PlayerBulletSpeed, 0);
	}
	if (klawisze[SDL_SCANCODE_LEFT])
	{
		if (player->direct == 0)firebullet(info, *player, -PlayerBulletSpeed, 0);
	}
}

void set_colors(world& info)
{
	info.czarny = SDL_MapRGB(info.screen->format, 0x00, 0x00, 0x00);
	info.zielony = SDL_MapRGB(info.screen->format, 0x00, 0xFF, 0x00);
	info.czerwony = SDL_MapRGB(info.screen->format, 0xFF, 0x00, 0x00);
	info.niebieski = SDL_MapRGB(info.screen->format, 0x11, 0x11, 0xCC);
}

void settingBeza(boss& beza)
{
	beza.attack = 0;
	beza.passedTime = 0;
	beza.repeats = 0;
	beza.x = BossStartCenterX;
	beza.y = BossStartCenterY;
	beza.milliseconds = AnimationMS;
	beza.animation = AnimationChange;
	beza.BhitboxD = CFBulletHitboxD;
	beza.BhitboxL = CFBulletHitboxL;
	beza.BhitboxR = CFBulletHitboxR;
	beza.BhitboxU = CFBulletHitboxU;
}

void settingPlayer(thing& player)
{
	player.x = StartPositionX;
	player.y = StartPositionY;
	player.speed = PlayerSpeed;
	player.hitboxD = PlayerHitboxD;
	player.hitboxU = PlayerHitboxU;
	player.hitboxL = PlayerHitboxL;
	player.hitboxR = PlayerHitboxR;
	if(player.health<=0) player.health = PlayerHealth;
	player.ax = 0;
	player.ay = 0;
	player.direct = Reload;
	player.inv = InvOff;
	player.invtimer = 0;
	player.texture = SDL_LoadBMP("./ph/me.bmp");
	player.textureL = SDL_LoadBMP("./ph/me2.bmp");
	player.textureR = SDL_LoadBMP("./ph/me3.bmp");
	player.textureD= SDL_LoadBMP("./ph/me4.bmp");
	player.textureU = SDL_LoadBMP("./ph/me5.bmp");
	player.me = player.texture;
}

void settingBackground(background& ground)
{
	ground.menu= SDL_LoadBMP("./ph/menu.bmp");
	ground.fight1= SDL_LoadBMP("./ph/tloF.bmp");
	ground.gameover= SDL_LoadBMP("./ph/tlo3.bmp");
	ground.winscreen = SDL_LoadBMP("./ph/winscreen.bmp");
	ground.x = BackgroundFightCenterX;
	ground.y = BackgroundFightCenterY;
	ground.nav= SDL_LoadBMP("./ph/nav.bmp");
}

void settingInfo(world& info) 
{
	info.screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	info.scrtex = SDL_CreateTexture(info.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	info.charset = SDL_LoadBMP("./ph/cs8x8.bmp");
	info.mybullet= SDL_LoadBMP("./ph/mybullet.bmp");
	info.bulletbomb= SDL_LoadBMP("./ph/bulletbomb.bmp");
	info.badLife = SDL_LoadBMP("./ph/lifeB.bmp");
	info.goodLife = SDL_LoadBMP("./ph/lifeG.bmp");
	info.screenTexture= SDL_LoadBMP("./ph/tlotexture.bmp");
	info.pociskiHead = (projectile_type*)malloc(sizeof(projectile_type));
	info.pociskiHead = NULL;
	info.bulletHead = (bullet_type*)malloc(sizeof(bullet_type));
	info.bulletHead = NULL;
	info.t1 = SDL_GetTicks();
	info.lvl = 1;
	info.ex1 = SDL_LoadBMP("./ph/wybuch1.bmp");
	info.ex2 = SDL_LoadBMP("./ph/wybuch2.bmp");
	info.ex3 = SDL_LoadBMP("./ph/wybuch3.bmp");
	info.ex4 = SDL_LoadBMP("./ph/wybuch4.bmp");
	info.ex5 = SDL_LoadBMP("./ph/wybuch5.bmp");
	info.ex = info.ex1;
	info.gf = SDL_LoadBMP("./ph/gf.bmp");
	info.gd = SDL_LoadBMP("./ph/gd.bmp");
	info.gc = SDL_LoadBMP("./ph/gc.bmp");
	info.gb = SDL_LoadBMP("./ph/gb.bmp");  
	info.ga = SDL_LoadBMP("./ph/ga.bmp");
	info.gs = SDL_LoadBMP("./ph/gs.bmp");
	info.bonusH= SDL_LoadBMP("./ph/bonus.bmp");
}

void Fbossshoots(thing & player, boss & beza, world & info)
{
	beza.shootinfo = CommonBullet;
	if ((beza.repeats <= 0) && (beza.passedTime > 0.2))
	{
		srand(time(NULL));
		beza.attack = rand() % 10;
		if (beza.attack <= 6)beza.repeats = 15;
		else beza.repeats = 8;	
	}
	if ((beza.passedTime > 0.3) && (beza.repeats > 0) && (beza.attack <= 4))
	{
		bossAttac2(player, beza, info);
		beza.passedTime = 0;
		if (beza.repeats < 5)
		{
			if (beza.repeats % 2 == 0)bossAttac1(player, beza, info);
			else bossAttac1_2(player, beza, info);
		}
		beza.repeats--;
	}
	if ((beza.passedTime > 0.3) && (beza.repeats > 0) && (beza.attack > 4) && (beza.attack <= 7))
	{
		if (beza.repeats % 2 == 0)bossAttac1(player, beza, info);
		else bossAttac1_2(player, beza, info);
		beza.passedTime = 0;
		beza.repeats--;
	}
	if ((beza.passedTime >1) && (beza.repeats > 0) && (beza.attack > 7))
	{
		bossAttac3(player, beza, info); 
		beza.passedTime = 0;
		beza.repeats--;
	}
}

void Sbossshoots(thing& player, boss& beza, world& info)
{
	if ((beza.repeats <= 0) && (beza.passedTime > 1))
	{
		beza.attack = rand() % 10;
		if (beza.attack <= 5)beza.repeats = 30;
		else beza.repeats = 4;
	}
	if ((beza.passedTime > 0.1) && (beza.repeats >0) && (beza.attack <= 2))
	{
		beza.shootinfo = SpiralBullet;
		bossAttac4(player, beza, info);
		beza.passedTime = 0;
		beza.shootinfo = BoomBullet;
		if(beza.repeats==10)bossAttac6(player, beza, info);
		beza.repeats--;
	}
	if ((beza.passedTime > 0.1) && (beza.attack > 2)&&(beza.attack <= 5) && (beza.repeats > 0))
	{
		beza.shootinfo = WaveBulletFirst;
		bossAttac5(player, beza, info);
		beza.shootinfo = CommonBullet;
		if (beza.repeats <10)bossAttac2(player, beza, info);
		beza.passedTime = 0;
		beza.repeats--;
	}
	if ((beza.passedTime > 0.6) && (beza.attack > 5) && (beza.repeats > 0))
	{
		beza.shootinfo = BoomBullet;
		bossAttac6(player, beza, info);
		beza.passedTime = 0;
		beza.repeats--;
	}
}

void shoot3blue(boss &beza,thing &player,world &info)
{
	if ((beza.repeats == 20))
	{
		beza.BhitboxD = CFBulletHitboxD;
		beza.BhitboxL = CFBulletHitboxL;
		beza.BhitboxR = CFBulletHitboxR;
		beza.BhitboxU = CFBulletHitboxU;
		beza.shootinfo = CommonBullet;
		bossAttac3(player, beza, info);
		beza.repeats--;
		beza.passedTime = -0.5;
	}
	else if ((beza.passedTime > 0.1))
	{
		beza.BhitboxD = BrimHitboxD;
		beza.BhitboxL = BrimHitboxL;
		beza.BhitboxR = BrimHitboxR;
		beza.BhitboxU = BrimHitboxU;
		beza.shootinfo = Laser;
		bossAttac8(player, beza, info);
		beza.repeats--;
		beza.passedTime = 0;
	}
}

void combo1(thing& player, boss& beza, world& info)
{
	if ((beza.comboTime > 0.1)&&(beza.comborepeats>0))
	{
		beza.BhitboxD = CFBulletHitboxD;
		beza.BhitboxL = CFBulletHitboxL;
		beza.BhitboxR = CFBulletHitboxR;
		beza.BhitboxU = CFBulletHitboxU;
		beza.shootinfo = SpiralBullet;
		bossAttac4(player, beza, info);
		beza.shootinfo = CommonBullet;
		if (beza.comborepeats % 2 == 0)bossAttac1(player, beza, info);
		else bossAttac1_2(player, beza, info);
		beza.comboTime = 0;
		beza.comborepeats--;
	}
	
}
void combo2(thing& player, boss& beza, world& info)
{
	if ((beza.comboTime > 0.1) && (beza.comborepeats > 0))
	{
		beza.BhitboxD = CFBulletHitboxD;
		beza.BhitboxL = CFBulletHitboxL;
		beza.BhitboxR = CFBulletHitboxR;
		beza.BhitboxU = CFBulletHitboxU;
		if (beza.comborepeats == 10)
		{
			beza.shootinfo = BoomBullet;
			bossAttac2(player, beza, info);
		}
		beza.shootinfo = WaveBulletFirst;
		bossAttac5(player, beza, info);

		beza.comboTime = 0;
		beza.comborepeats--;
	}

}

void Tbossshoots(thing& player, boss& beza, world& info)
{
	beza.maksHealth = beza.normal;
	if (beza.repeats <= 0)
	{
		if (beza.x + 40 > player.x)
		{
			beza.x -= info.delta * BossSpeedDelta;
			beza.maksHealth = beza.left;
		}
		else if (beza.x + 45 < player.x)
		{
			beza.x += info.delta * BossSpeedDelta;
			beza.maksHealth = beza.right;
		}
		if (beza.y + 150 > player.y)
		{
			beza.y -= info.delta * BossSpeedDelta;
			beza.maksHealth = beza.up;
		}
		else if (beza.y + 155 < player.y)
		{
			beza.y += info.delta * BossSpeedDelta;
			beza.maksHealth = beza.down;
		}
	}
	info.bossT = beza.maksHealth;
	beza.comboTime += info.delta;
	if (beza.comboTime > 5)
	{
		beza.combo = rand() % 4;
		beza.comborepeats = 10;
		beza.comboTime = 0;
	}
	if ((beza.repeats <= 0) && (beza.passedTime > 1.2))
	{
		beza.attack = rand() % 10;
		if (beza.attack <= 2)beza.repeats = 1;
		else
		{
			if(beza.attack <= 6)beza.repeats = 1;
			else beza.repeats = 20;
			
		}
	}
	if ((beza.passedTime > 0.1) && (beza.repeats > 0) && (beza.attack > 2) && (beza.attack <= 6))
	{
		beza.BhitboxD = ErFD;
		beza.BhitboxL = ErFL;
		beza.BhitboxR = ErFR;
		beza.BhitboxU = ErFU;
		beza.shootinfo = HomingBullet;
		bossAttac2(player, beza, info);
		beza.passedTime = 0;
		beza.repeats--;
	}
	if ((beza.passedTime >2) && (beza.repeats > 0) && (beza.attack <= 2))
	{
		beza.BhitboxD = AreaBulletHitboxD;
		beza.BhitboxL = AreaBulletHitboxL;
		beza.BhitboxR = AreaBulletHitboxR;
		beza.BhitboxU = AreaBulletHitboxU;
		beza.shootinfo = WarningArea;
		int x = rand() % 2;
		if(x==0)	bossAttac7(player, beza, info);
		else bossAttac7_2(player, beza, info);
		beza.passedTime = 0;
		beza.repeats--;
	}
	if ((beza.repeats > 0) &&(beza.attack > 6))shoot3blue(beza,player,info);	
	if(beza.combo==1)combo1(player, beza, info);
	if (beza.combo == 2)combo2(player, beza, info);
}

void bossShoots(thing& player,boss &beza,world &info)
{
	beza.shootinfo = CommonBullet;
	if (info.lvl == 1) Fbossshoots(player, beza, info);
	if (info.lvl == 2) Sbossshoots(player, beza, info);
	if (info.lvl == 3) Tbossshoots(player, beza, info);
	move(&player,beza,info);
}

void doBoss(thing& player, world& info, boss& beza)
{
	bossShoots(player, beza, info);
	doBullets(info, beza, player);
}


void drawBullets(world &info)
{
	if (info.pociskiHead != NULL)
	{
		projectile_type* pocisk = info.pociskiHead;
		do
		{
			DrawSurface(info.screen, info.mybullet, pocisk->x, pocisk->y);
			pocisk = pocisk->next;
		} while (pocisk != NULL);

	}

	if (info.bulletHead != NULL)
	{
		bullet_type* pocisk = info.bulletHead;
		do
		{
			if(pocisk->bulletType== CommonBullet|| pocisk->bulletType == SpiralBullet ||pocisk->bulletType == WaveBulletFirst|| pocisk->bulletType == WaveBulletSecond)DrawSurface(info.screen, info.bullet, pocisk->x, pocisk->y);
			if (pocisk->bulletType == BoomBullet)DrawSurface(info.screen, info.bulletbomb, pocisk->x, pocisk->y);
			if (pocisk->bulletType == WarningArea)DrawSurface(info.screen, info.bulletarea, pocisk->x, pocisk->y);
			if (pocisk->bulletType == HotArea)DrawSurface(info.screen, info.bullethotarea, pocisk->x, pocisk->y);
			if (pocisk->bulletType == Laser)DrawSurface(info.screen, info.brim, pocisk->x, pocisk->y);
			if (pocisk->bulletType == HomingBullet)DrawSurface(info.screen, info.bigbullet, pocisk->x, pocisk->y);
			if((pocisk->bulletType >= CBulletAnimatonOne)&& (pocisk->bulletType < 30))DrawSurface(info.screen, info.bulletdestroy, pocisk->x, pocisk->y);
			if (pocisk->bulletType >= BoomTypeAnimationOne)DrawSurface(info.screen, info.ex, pocisk->x, pocisk->y);
			pocisk = pocisk->next;
		} while (pocisk != NULL);
	}
}

void drawBoss(world &info,boss &beza)
{
	DrawSurface(info.screen, info.bossT, beza.x, beza.y);
	DrawRectangle(info.screen, HealthBarX, HealthBarY, (beza.health/beza.mHealth) * HealthBarL, HealthBarH, info.czarny, info.czerwony);
	if ((beza.health < 50) && (beza.health > 0))
	{
		if (beza.animation < beza.milliseconds)
		{
			beza.textre++;
			beza.textre = beza.textre % 2;
			if (beza.textre == 0) info.bossT = beza.half2;
			else info.bossT = beza.half1;
			beza.animation = rand() % (int)beza.health;
			beza.milliseconds = 0;
		}
		beza.milliseconds++;
	}
	else if (beza.health <= 0)info.bossT = beza.end;
}

void grade(thing &player,world& info)
{
	if (player.score <= 0) info.grade=info.gf;
	if (player.score > 0) info.grade = info.gd;
	if (player.score > 1000) info.grade = info.gc;
	if (player.score > 10000) info.grade = info.gb;
	if (player.score > 15000) info.grade = info.ga;
	if (player.score > 20000) info.grade = info.gs;
}

void drawbackground(world& info, background& ground,thing &player)
{
 DrawSurface(info.screen, info.tlo, ground.x, ground.y);
char text[128];
 if (info.tryb == GAMEOVER)
 {
	sprintf(text, " %li", player.score);
	DrawString(info.screen, info.screen->w / 1.32 - strlen(text) * 8 / 2, 480, text, info.charset);
	grade(player, info);
	DrawSurface(info.screen, info.grade, 550, 500);
	info.bonus = false;
 }
 if (info.tryb == WIN)
 {
	 char text[128];
	 sprintf(text, " %li", player.score);
	 DrawString(info.screen, info.screen->w /3.2 - strlen(text) * 8 / 2, 600, text, info.charset);
	 grade(player, info);
	 DrawSurface(info.screen, info.grade, 780, 400);
	 info.bonus = false;
 }
}

void drawNav(world& info, thing& player,background &ground)
{
	DrawSurface(info.screen, ground.nav, NAV_X, NAV_Y);

	for (int i = 0;i < 3;i++)
	{
		if(player.health>i) 	DrawSurface(info.screen, info.goodLife, NAV_X+50*i, NAV_Y+290);
		else 	DrawSurface(info.screen, info.badLife, NAV_X + 50 * i, NAV_Y +290);
	}
	DrawSurface(info.screen, info.screenTexture, 10,10);
	DrawRectangle(info.screen, 1069, 145, 80, 20, info.czarny, info.czarny);
}

void DrawPlayer(world &info,thing &player)
{
	if (player.inv == 0)
	{
		DrawSurface(info.screen, player.me, player.x, player.y);
	}
	else
	{
		player.invtimer++;
		if (player.inv % 2 == 0)
		{
			DrawSurface(info.screen, player.me, player.x, player.y);
		}
		if (player.invtimer >10)
		{
			player.invtimer = 0;
			player.inv++;
		}
		if (player.inv == 6) player.inv = 0;
	}
}

void setlvl1(boss& beza, background& ground, world& info)
{
	ground.fight1 = SDL_LoadBMP("./ph/tloF.bmp");
	info.bullet = SDL_LoadBMP("./ph/bullet.bmp");
	beza.maksHealth = SDL_LoadBMP("./ph/404.bmp");
	beza.half2 = SDL_LoadBMP("./ph/404_2.bmp");
	beza.half1 = SDL_LoadBMP("./ph/404_5.bmp");
	beza.end = SDL_LoadBMP("./ph/404_6.bmp");
	info.bossT = beza.maksHealth;
	beza.xCenter = BossFCenterX;
	beza.yCenter = BossFCenterY;
	beza.hitboxD = ErFD;
	beza.hitboxL = ErFL;
	beza.hitboxU = ErFU;
	beza.hitboxR = ErFR;
	info.trapdor = SDL_LoadBMP("./ph/trapdor.bmp");
	info.bulletdestroyF = SDL_LoadBMP("./ph/bulletdestroy1.bmp");
	info.bulletdestroyS = SDL_LoadBMP("./ph/bulletdestroy2.bmp");
	info.bulletdestroyT = SDL_LoadBMP("./ph/bulletdestroy3.bmp");
	info.bulletdestroy = info.bulletdestroyF;

	beza.health = 100;
	beza.mHealth = 100;
}




void setlvl2(boss &beza,background &ground,world &info)
{
	beza.maksHealth = SDL_LoadBMP("./ph/unerror.bmp");
	ground.fight1= SDL_LoadBMP("./ph/tloF2.bmp");
	info.bullet= SDL_LoadBMP("./ph/bullet2.bmp");
	info.bossT = beza.maksHealth;
	info.trapdor = SDL_LoadBMP("./ph/trapdor2.bmp");
	beza.half1 = SDL_LoadBMP("./ph/unerror_2.bmp");
	beza.half2 = SDL_LoadBMP("./ph/unerror_3.bmp");
	beza.end= SDL_LoadBMP("./ph/unerror_4.bmp");
	beza.hitboxU = 0;
	beza.hitboxD = 135;
	beza.hitboxR = 200;
	beza.hitboxL = -36;
	beza.repeats = 0;
	beza.xCenter = BossSCenterX;
	beza.yCenter = BossSCenterY;
	beza.BhitboxD = CFBulletHitboxD;
	beza.BhitboxL = CFBulletHitboxL;
	beza.BhitboxR = CFBulletHitboxR;
	beza.BhitboxU = CFBulletHitboxU;
	info.bulletdestroyF = SDL_LoadBMP("./ph/bulletdestroy1.bmp");
	info.bulletdestroyS = SDL_LoadBMP("./ph/bulletdestroy2.bmp");
	info.bulletdestroyT = SDL_LoadBMP("./ph/bulletdestroy3.bmp");
	beza.health = 130;
	beza.mHealth = 130;
}

void setlvl3(boss& beza, background& ground, world& info)
{
	beza.maksHealth = SDL_LoadBMP("./ph/face.bmp");
	ground.fight1 = SDL_LoadBMP("./ph/bluescreen.bmp");
	info.bullet = SDL_LoadBMP("./ph/bullet2.bmp");
	info.bossT = beza.maksHealth;
	beza.half1 = SDL_LoadBMP("./ph/face.bmp");
	beza.half2 = SDL_LoadBMP("./ph/face.bmp");
	beza.end = SDL_LoadBMP("./ph/face.bmp");
	info.bulletarea = SDL_LoadBMP("./ph/area1.bmp");
	info.bullethotarea = SDL_LoadBMP("./ph/area2.bmp");
	info.brim = SDL_LoadBMP("./ph/brim.bmp");
	info.bigbullet= SDL_LoadBMP("./ph/404.bmp");
	beza.hitboxU = 17;
	beza.hitboxD = 270;
	beza.hitboxR = 145;
	beza.hitboxL = -10;
	beza.repeats = 0;
	beza.xCenter = BossTCenterX;
	beza.comboTime = 0;
	beza.yCenter = BossTCenterY;
	info.bullet= SDL_LoadBMP("./ph/bluebullet.bmp");
	info.bulletdestroyF = SDL_LoadBMP("./ph/bluebulletdestroy1.bmp");
	info.bulletdestroyS = SDL_LoadBMP("./ph/bluebulletdestroy2.bmp");
	info.bulletdestroyT = SDL_LoadBMP("./ph/bluebulletdestroy3.bmp");
	beza.left = SDL_LoadBMP("./ph/face5.bmp");
	beza.right= SDL_LoadBMP("./ph/face4.bmp");
	beza.up = SDL_LoadBMP("./ph/face2.bmp");
	beza.down = SDL_LoadBMP("./ph/face3.bmp");
	beza.normal = beza.maksHealth;
	beza.health = 150;
	beza.mHealth = 150;
}





void nextlvl(world &info,boss &beza,thing &player,background &ground)
{
	if (info.lvl == 2)info.lvl = 3;
	if (info.lvl == 1) info.lvl = 2;
	info.bonus = false;
	if (info.bulletHead != NULL)
	{
		bullet_type* pocisk = info.bulletHead, * temp;
		do
		{
			temp = pocisk;
			pocisk = pocisk->next;
			free(temp);
		} while (pocisk != NULL);
		info.bulletHead = (bullet_type*)malloc(sizeof(bullet_type));
		info.bulletHead = NULL;
	}
	if (info.pociskiHead != NULL)
	{
		projectile_type* pocisk = info.pociskiHead, * temp;
		do
		{
			temp = pocisk;
			pocisk = pocisk->next;
			free(temp);
		} while (pocisk != NULL);
		info.pociskiHead = (projectile_type*)malloc(sizeof(projectile_type));
		info.pociskiHead = NULL;
	}
	settingBeza(beza);
	settingPlayer(player);
	settingBackground(ground);
	if (info.lvl == 2)setlvl2(beza,ground,info);
	if (info.lvl == 3)setlvl3(beza, ground, info);
}

void moveGround(thing &player,background &ground)
{
			if ((ground.x - player.ax) > 400)
		{
			if ((player.x + player.hitboxL + player.ax) > 0) player.x += player.ax;
			player.ax = 0;
		}
		if ((ground.y - player.ay) > 75)
		{
			if (player.y + player.hitboxU + player.ay > 0) player.y += player.ay;
			player.ay = 0;
		}
		if ((ground.x - player.ax) < -600)
		{
			if ((player.x + player.hitboxR + player.ax) < SCREEN_WIDTH) player.x += player.ax;
			player.ax = 0;
		}
		if ((ground.y - player.ay) < -426)
		{
			if (player.y + player.hitboxD + player.ay < SCREEN_HEIGHT) player.y += player.ay;
			player.ay = 0;
		}
		ground.x -= player.ax;
		ground.y -= player.ay;
}

void bossActions(boss &beza,thing &player,background &ground,world &info)
{
	if (beza.health > 0)
	{
		doBoss(player, info, beza);
		drawBullets(info);
		drawBoss(info, beza);
		if (playerColision(player, beza) && (player.inv == 0))
		{
			player.health--;
			player.inv = 1;
			player.score -= 100;
			player.bonus = 0;
			if (player.health < 0)
			{
				info.tryb = GAMEOVER;
				player.score -= 1000;
			}
		}
	}
	else
	{
		if (info.lvl == 3)
		{
			info.tryb = WIN;
		}
		if (beza.health < -50)
		{
			DrawSurface(info.screen, info.trapdor, beza.x, beza.y);
			if (playerColision(player, beza))
			{
				player.score += 1000;
				nextlvl(info, beza, player, ground);
			}
		}
		else
		{
			drawBoss(info, beza);
			beza.health--;
		}
	}
}

void doBonus(world& info, thing& player,boss beza)
{
	if ((info.bonusTimer > 10)&&(!info.bonus))
	{
		info.bonusTimer = 0;
		if (rand() % 2 == 0)
		{
			do 
			{
				info.bonusX = rand() % SCREEN_WIDTH;
				info.bonusY = rand() % SCREEN_HEIGHT;
			} while (sqrt(pow(info.bonusX - beza.x, 2) + pow(info.bonusY - beza.y, 2)) < 100);

			info.bonus = true;
		}
	}
	if (info.bonus)
	{
		info.bonusX -= player.ax;
		info.bonusY -= player.ay;
		DrawSurface(info.screen, info.bonusH, info.bonusX, info.bonusY);
		if (bonusColision(player,info))
		{
			if(player.health<3)player.health++;
			player.score += 200;
			info.bonusTimer = 0;
			info.bonus = false;
		}
	}
}

void fighting(thing& player,world &info,boss& beza, background  &ground)
{
	info.worldTime += info.delta;
	beza.passedTime += info.delta;
	char text[128];
	info.fpsTimer += info.delta;
	info.bonusTimer += info.delta;
	if (info.fpsTimer > 0.5) {
		info.fps = info.frames * 2;
		info.frames = 0;
		info.fpsTimer -= 0.5;
	}
	player.score -= info.delta*100;

		movePlayer(&player, info);
		moveGround(player, ground);
		doBonus(info,player,beza);
		bossActions(beza, player, ground, info);
		drawNav(info, player, ground);
		beza.x -= player.ax;
		beza.y -= player.ay;
		sprintf(text, " %.0lf fps", info.fps);
		DrawString(info.screen, info.screen->w / 1.06 - strlen(text) * 8 / 2, 10, text, info.charset);
		sprintf(text, "%.1lf s", info.worldTime);
		DrawString(info.screen, info.screen->w/1.08 - strlen(text) * 8 / 2, 150, text, info.charset);
		sprintf(text, " %li", player.score);
		DrawString(info.screen, info.screen->w / 1.08 - strlen(text) * 8 / 2, 550, text, info.charset);
		sprintf(text, " %li", player.bonus);
		DrawString(info.screen, info.screen->w / 1.08 - strlen(text) * 8 / 2, 750, text, info.charset);
		DrawPlayer(info, player);

		if (info.tryb == GAMEOVER|| info.tryb == WIN)
		{
			ground.x = BackgroundFightCenterX;
			ground.y = BackgroundFightCenterY;
		}
}

void startGame(world& info,thing &player,background &ground,boss &beza)
{
	info.tryb = FIGHT;
	if (info.bulletHead != NULL)
	{
		bullet_type* pocisk = info.bulletHead, * temp;
		do
		{
			temp = pocisk;
			pocisk = pocisk->next;
			free(temp);
		} while (pocisk != NULL);
		info.bulletHead = (bullet_type*)malloc(sizeof(bullet_type));
		info.bulletHead = NULL;
	}
	if (info.pociskiHead != NULL)
	{
		projectile_type* pocisk = info.pociskiHead, * temp;
		do
		{
			temp = pocisk;
			pocisk = pocisk->next;
			free(temp);
		} while (pocisk != NULL);
		info.pociskiHead = (projectile_type*)malloc(sizeof(projectile_type));
		info.pociskiHead = NULL;
	}
	settingPlayer(player);
	settingBeza(beza);
	settingInfo(info);
	settingBackground(ground);
	ground.x = FightSreenCenterX;
	ground.y = FightSreenCenterY;
}

void events(world& info, thing& player, boss& beza, background& ground)
{
	while (SDL_PollEvent(&info.event)) {
		switch (info.event.type) {
		case SDL_KEYDOWN:
			if (info.event.key.keysym.sym == SDLK_ESCAPE) info.quit = 1;
			if (info.event.key.keysym.sym == SDLK_m)
			{
				info.tryb = MENU;
				ground.x = BackgroundFightCenterX;
				ground.y = BackgroundFightCenterY;
			}
			if ((info.event.key.keysym.sym == SDLK_1) && (info.tryb==MENU))
			{
				startGame(info, player, ground, beza);
				info.lvl = 1;
				player.score = 1000;
				setlvl1(beza, ground, info);
			}
			if ((info.event.key.keysym.sym == SDLK_2) && (info.tryb == MENU))
			{
				startGame(info, player, ground, beza);
				info.lvl = 2;
				player.score = 1000;
				setlvl2(beza, ground, info);
			}
			if ((info.event.key.keysym.sym == SDLK_3) && (info.tryb == MENU))
			{
				startGame(info, player, ground, beza);
				info.lvl = 3;
				player.score = 1000;
				setlvl3(beza, ground, info);
			}
			if (info.event.key.keysym.sym == SDLK_n)
			{
				startGame(info, player, ground, beza);
				setlvl1(beza, ground, info);
				player.score = 1000;
				player.bonus = 1;
			}
			break;
		case SDL_QUIT:
			info.quit = 1;
			break;
		};
	};
}

void clear(world info, int ret)
{
	SDL_FreeSurface(info.charset);
	SDL_FreeSurface(info.screen);
	SDL_DestroyTexture(info.scrtex);
	SDL_DestroyWindow(info.window);
	SDL_DestroyRenderer(info.renderer);
	SDL_Quit();
	exit(ret);
}

void setting_all(thing &player,boss &beza,background &ground,world &info)
{
	settingPlayer(player);
	settingBeza(beza);
	settingBackground(ground);
	settingInfo(info);
	if (info.charset == NULL)printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
	if (player.texture == NULL)printf("SDL_LoadBMP(me.bmp) error: %s\n", SDL_GetError());
	if (info.bullet == NULL)printf("SDL_LoadBMP(bullet.bmp) error: %s\n", SDL_GetError());
	SDL_SetColorKey(info.charset, true, 0x000000);
	if (player.texture == NULL || info.charset == NULL || info.bullet == NULL)clear(info, 1);
	set_colors(info);
	info.tlo = ground.menu;
}

void rendering(world &info,thing &player,boss &beza,background &ground)
{
	SDL_UpdateTexture(info.scrtex, NULL, info.screen->pixels, info.screen->pitch);
	SDL_RenderCopy(info.renderer, info.scrtex, NULL, NULL);
	SDL_RenderPresent(info.renderer);
	events(info, player, beza, ground);
}

void fpslimit(world &info)
{
	if ((1000 / FPSLIM) > SDL_GetTicks() - info.t2) {
		SDL_Delay(1000 / FPSLIM - (SDL_GetTicks() - info.t2));
	}
}

void firsRender(world &info)
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit (1);
	}
	info.rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &info.window, &info.renderer);
	if (info.rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(1);
	};
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(info.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(info.renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(info.window, "Szablon do zdania drugiego 2017");
}

