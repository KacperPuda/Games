#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"bosses.h"
#include"structs.h"
#include"funkcjeDoS.h"
#include"definitions.h"
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

bool bulletColision(bullet_type* bullet, thing player)
{
	SDL_Rect box1, box2;
	box1.x = bullet->x+bullet->hitboxL;
	box1.y = bullet->y+bullet->hitboxU;
	box1.h = bullet->hitboxD - bullet->hitboxU;
	box1.w = bullet->hitboxR - bullet->hitboxL;
	box2.x = player.x + player.hitboxL;
	box2.y = player.y + player.hitboxU;
	box2.h = player.hitboxD - player.hitboxU;
	box2.w = player.hitboxR - player.hitboxL;
	if (SDL_HasIntersection(&box1, &box2)) return true;
	return false;
}

void setBulletDirection(double &px,double &py,double xb,double yb,thing &player,int direct)
{
	int g = 1;
	double x, y,xx=1,yy=1;
	if (direct < CircleBulets)
	{	
		if (direct > CircleBuletsR)
		{
			direct -= 2*CircleBuletsR;
			g = (-1);
		}
		x =direct;
		y = sqrt(100 - (x * x))+yb;
		x += xb;
		xx =x-xb;
		yy = y-yb;
	}
	if (direct == BulletsOnPlayer)
	{
		x = player.x;
		y = player.y;
		xx = (x - xb);
		yy = (y - yb);
	}
	if (xx == 0 || yy == 0)
	{
		py  = 1;
		px = 1;
		if(xx<0)px= -1;
		if (yy < 0)py = -1;
		if (xx == 0) px = 0;
		if (yy == 0)	py = 0;
	}
	else
	{
		py = yy;
		px = xx;
		while (((px * px) + (py * py)) > 0.99)
		{
			px = px / 1.1;
			py = py / 1.1;
		}
	}
	py = py * g;
}

void setbullet(bullet_type*& pocisk, world& info, thing& player, boss& beza, double px, double py, double x, double y)
{
	if (x == 0)
	{
		pocisk->x = beza.x + beza.xCenter;
		pocisk->y = beza.y + beza.yCenter;
	}
	else
	{
		pocisk->x = x;
		pocisk->y = y;
	}
	pocisk->plusX = px;
	pocisk->plusY = py;
	pocisk->bulletType = beza.shootinfo;
	pocisk->hitboxD = beza.BhitboxD;
	pocisk->hitboxU = beza.BhitboxU;
	pocisk->hitboxR = beza.BhitboxR;
	pocisk->hitboxL = beza.BhitboxL;
	pocisk->angle = 0;
	pocisk->timer = 0;
	if (beza.shootinfo == SpiralBullet) pocisk->speed = 3;
	else pocisk->speed = 6;
	if (beza.shootinfo == Laser)pocisk->speed = 10;
	if (beza.shootinfo == SpiralBullet)pocisk->r = 100;
	else pocisk->r = 0;
}

void shootBullet(world& info, thing& player, boss& beza, double px, double py,double x,double y)
{
	if (info.bulletHead == NULL)
	{
		info.bulletHead = (bullet_type*)malloc(sizeof(bullet_type));
		setbullet(info.bulletHead, info, player, beza, px, py, x, y);
		info.bulletHead->next = NULL;
	}
	else
	{
		bullet_type* pocisk;
		pocisk = (bullet_type*)malloc(sizeof(bullet_type));
		setbullet(pocisk, info, player, beza, px, py, x, y);
		pocisk->next = info.bulletHead;
		info.bulletHead = pocisk;
	}
}

void removeBullet(bullet_type *&pocisk,world &info, bullet_type*& prev, bullet_type*& tmp)
{
	if (pocisk == info.bulletHead)
	{
		bullet_type* temp = NULL;
		temp = info.bulletHead->next;
		free(info.bulletHead);
		info.bulletHead = temp;
		pocisk = info.bulletHead;
	}
	else
	{
		prev->next = pocisk->next;
		free(pocisk);
		pocisk = prev;
	}
}

void cilcularM(boss beza, bullet_type*& bullet,world &info,thing player)
{
	bullet->angle += bullet->speed * info.delta;
	bullet->x = beza.x+ beza.xCenter + bullet->r * cos(bullet->angle);
	bullet->y = beza.y+ beza.yCenter + bullet->r * sin(bullet->angle);
	bullet->r+=2;
}

void waveM(boss beza, bullet_type*& bullet, world& info, thing player)
{

	bullet->x +=(bullet->speed * info.delta* DeltaToSecond - player.ax*bullet->plusX)*bullet->plusX;
	bullet->y = DeltaToSecond *sin(fabs((bullet->x-(beza.x + beza.xCenter)+player.ax)/100))+beza.y + beza.yCenter + fabs((bullet->x - beza.x-beza.xCenter + player.ax)) * (bullet->bulletType - 3);
	if (bullet->plusY == (-1)) bullet->y = beza.y + beza.yCenter + (beza.y + beza.yCenter - bullet->y);
}

void boomM(boss &beza, bullet_type*& bullet, world& info, thing player)
{
	if (bullet->timer < 1.2)
	{
		bullet->y += bullet->plusY * bullet->speed * info.delta * DeltaToSecond - player.ay;
		bullet->x += bullet->plusX * bullet->speed * info.delta * DeltaToSecond - player.ax;
	}
	else
	{
		bullet->bulletType = BoomDoneType;
	}
}

void dobooms(thing& player, boss beza, world& info)
{
	bullet_type* bullet = info.bulletHead;
	bullet_type* tmp, * prev;
	int j;
	double px, py;
	bool boom = false;
	prev = bullet;
	while (bullet != NULL)
	{
		if (bullet->bulletType == BoomDoneType)
		{
			bullet->bulletType = BoomTypeAnimationOne;
			j = (-CircleBuletsR);
			for (int i = 0;i < 40;i++)
			{
				setBulletDirection(px, py, bullet->x, beza.y + bullet->y, player, j);
				shootBullet(info, player, beza, px, py, bullet->x, bullet->y);
				j += 2;
			}
			bullet = info.bulletHead;
		}
		prev = bullet;
		if (bullet == NULL) break;
		else bullet = bullet->next;
	}
}

void homingM(boss beza, bullet_type*& bullet, world& info, thing player)
{
	if (player.y > bullet->y) bullet->plusY+=0.01;
	else bullet->plusY -= 0.01;
	if (player.x > bullet->x) bullet->plusX += 0.01;
	else bullet->plusX -= 0.01;
	if (((bullet->plusX * bullet->plusX) + (bullet->plusY * bullet->plusY)) > 0.99)
	{
		bullet->plusX = bullet->plusX / sqrt(((bullet->plusX * bullet->plusX) + (bullet->plusY * bullet->plusY)));
		bullet->plusY = bullet->plusY / sqrt(((bullet->plusX * bullet->plusX) + (bullet->plusY * bullet->plusY)));
	}
}

void animation(bullet_type*& bullet, world& info, thing player)
{
	bullet->x -= player.ax;
	bullet->y -= player.ay;
	if (bullet->timer > 0.06)
	{
		switch (bullet->bulletType)
		{
			case CBulletAnimatonOne:
			{
				info.bulletdestroy = info.bulletdestroyS;
				break;
			}
			case CBulletAnimatonTwo:
			{
				info.bulletdestroy = info.bulletdestroyT;
				break;
			}
			case BoomTypeAnimationOne:
			{
				info.ex = info.ex2;
				break;
			}
			case BoomTypeAnimationTwo:
			{
				info.ex = info.ex3;
				break;
			}
			case BoomTypeAnimationThree:
			{
				info.ex = info.ex4;
				break;
			}
			case BoomTypeAnimationFour:
			{
				info.ex = info.ex5;
				break;
			}
		}
	 bullet->bulletType++;
	 bullet->timer = 0;
	
	}
}

void move(thing* player,boss beza,world &info)
{
	bullet_type* bullet = info.bulletHead;
	bullet_type* tmp, * prev;
	int i = 0;
	bool boom = false;
	prev = bullet;
	while (bullet != NULL)
	{
		bullet->timer += info.delta;
		if (bullet->bulletType== CommonBullet || bullet->bulletType == Laser || bullet->bulletType == HomingBullet)
		{
			bullet->y += bullet->plusY * bullet->speed * info.delta * 100 - player->ay;
			bullet->x += bullet->plusX * bullet->speed * info.delta * 100 - player->ax;
		}
		if (bullet->bulletType == HomingBullet) homingM(beza, bullet, info, *player);
		if (bullet->bulletType == SpiralBullet) cilcularM(beza,bullet,info,*player);
		if (bullet->bulletType == WaveBulletFirst || bullet->bulletType == WaveBulletSecond) waveM(beza, bullet, info, *player);
		if (bullet->bulletType == BoomBullet) boomM(beza, bullet, info, *player);
		if (bullet->bulletType >= CBulletAnimatonOne) animation(bullet, info,*player);
		if (bullet->bulletType == WarningArea || bullet->bulletType == HotArea)
		{
			bullet->y -= player->ay;
			bullet->x -=  player->ax;
			if (bullet->timer > 1)bullet->bulletType = HotArea;
		}
		if (bulletColision(bullet,*player)&&(bullet->bulletType!=7) && (bullet->bulletType <20))
		{
			if (player->inv == InvOff)
			{
				player->health--;
				player->inv = InvOn;
				player->score -= 100;
				player->bonus = 0;
				if ((bullet->bulletType != HotArea) && (bullet->bulletType != Laser) && (bullet->bulletType != CBulletAnimatonOne) && (bullet->bulletType != CBulletAnimatonTwo))
				{
					bullet->bulletType = CBulletAnimatonOne;
					info.bulletdestroy = info.bulletdestroyF;
					bullet->timer = 0;
				}
			}
			if (player->health < 0)
			{
				info.tryb = GAMEOVER;
				player->score -= 1000;
			}
		}
		else
		{
			if ((bullet->x + bullet->hitboxR > SCREEN_WIDTH + AdditionalLength || bullet->y + bullet->hitboxD > SCREEN_HEIGHT + AdditionalLength || bullet->x < 0 + bullet->hitboxL - AdditionalLength || bullet->y + bullet->hitboxU < 0 - AdditionalLength)&&(bullet->timer>2))
			{
				if(bullet->bulletType==1)removeBullet(bullet, info, prev, tmp);
				else
				{
					if(bullet->timer>6)removeBullet(bullet, info, prev, tmp);
				}
			}
		}
			if (bullet != NULL)	if ((bullet->bulletType > CBulletAnimatonTwo)&& (bullet->bulletType < BoomTypeAnimationOne))removeBullet(bullet, info, prev, tmp);
			if (bullet != NULL)	if (bullet->bulletType > BoomTypeAnimationFour)removeBullet(bullet, info, prev, tmp);
			if (bullet != NULL) if ((bullet->bulletType == HomingBullet) && (bullet->timer > 4))removeBullet(bullet, info, prev, tmp);
			if (bullet != NULL) if ((bullet->bulletType == HotArea) && (bullet->timer > 2))removeBullet(bullet, info, prev, tmp);
			if (bullet != NULL) if (bullet->bulletType == TrashBullet)removeBullet(bullet, info, prev, tmp);
			if (bullet != NULL) if (bullet->bulletType == BoomDoneType)boom = true;
		
		prev = bullet;
		if (bullet == NULL) break;
		else bullet = bullet->next;
	}
	if (boom) dobooms(*player, beza, info);

}

void bossAttac1(thing player,boss &beza, world& info)
{
	double px, py;
	int j = (-CircleBuletsR);
	for (int i = 0;i < 8;i++)
	{
		setBulletDirection(px,py, beza.x + beza.xCenter, beza.y + beza.yCenter, player,j);
		shootBullet(info,player,beza,px,py,0,0);
		j+=5;
	}
}

void bossAttac1_2(thing player, boss& beza,world &info)
{
	double px, py;
	int j = (-CircleBuletsR);
	j += 2;
	for (int i = 0;i < 8;i++)
	{
		if (j == 14)j = 12;
		setBulletDirection(px,py, beza.x+ beza.xCenter, beza.y + beza.yCenter, player,j);
		shootBullet(info,player,beza,px,py, 0, 0);

		if(j==(-8)||j==(2)|| j == (12)|| j == (22))j += 6;
		else j += 4;
	}
}

void bossAttac2(thing &player, boss& beza, world& info)
{
	double px, py;
	setBulletDirection(px, py, beza.x + beza.xCenter, beza.y + beza.yCenter, player, 50);
	shootBullet(info, player, beza, px, py, 0, 0);
}

void bossAttac3(thing player, boss& beza, world& info)
{
	double px, py;
	int j = (-CircleBuletsR);
	for (int i = 0;i < 40;i++)
	{
		setBulletDirection(px, py, beza.x + beza.xCenter, beza.y + beza.yCenter, player, j);
		shootBullet(info, player, beza, px, py, 0, 0);
		j ++;
	}
}

void bossAttac4(thing player, boss& beza, world& info)
{
	double px, py;
	setBulletDirection(px, py, beza.x + beza.xCenter, beza.y + beza.yCenter, player, 50);
	shootBullet(info, player, beza, px, py, 0, 0);
}

void bossAttac5(thing player, boss& beza, world& info)
{
	double px=1, py=1;
	for (int i = 0;i < 8;i++)
	{
		px = px * (-1);
		if ((i % 2 == 0) && (i > 0)) py *= -1;
		if ((i % 4 == 0) && (i > 0)) beza.shootinfo = WaveBulletSecond;
		shootBullet(info, player, beza, px, py, 0, 0);

	}
	int j = 0;
	beza.shootinfo = CommonBullet;
	for (int i = 0;i < 2;i++)
	{
			setBulletDirection(px, py, beza.x + beza.xCenter, beza.y + beza.yCenter, player, j);
			shootBullet(info, player, beza, px, py, 0, 0);
			j+=20;
	}
}

void bossAttac6(thing player, boss& beza, world& info)
{
	double px = 1, py = 1;
	setBulletDirection(px, py, beza.x + beza.xCenter, beza.y + beza.yCenter, player, 50);
	shootBullet(info, player, beza, px, py, 0, 0);
	
}

void bossAttac7(thing player, boss& beza, world& info)
{
	shootBullet(info, player, beza, 0, 0, player.x - AreaCenterLengthTwo, player.y - AreaCenterLength);
	shootBullet(info, player, beza, 0, 0, player.x + AreaCenterLength, player.y - AreaCenterLength);
	shootBullet(info, player, beza, 0, 0, player.x - AreaCenterLength, player.y - AreaCenterLengthTwo);
	shootBullet(info, player, beza, 0, 0, player.x - AreaCenterLength, player.y + AreaCenterLength);
	shootBullet(info, player, beza, 0, 0, player.x - AreaCenterLength, player.y - AreaCenterLength);
}

void bossAttac7_2(thing player, boss& beza, world& info)
{
	shootBullet(info, player, beza, 0, 0, player.x - AreaCenterLength, player.y - AreaCenterLength);
}

void bossAttac8(thing player, boss& beza, world& info)
{
	double px, py;
	int j = (-CircleBuletsR);
	for (int i = 0;i < 4;i++)
	{
		setBulletDirection(px, py, beza.x + beza.xCenter, beza.y + beza.yCenter, player, j);
		shootBullet(info, player, beza, px, py, 0, 0);
		j += 10;
	}
}

