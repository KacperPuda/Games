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

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	
	world info;
	thing player;
	boss beza;
	background ground;
	firsRender(info);
	setting_all(player,beza,ground,info);
	SDL_ShowCursor(SDL_DISABLE);

	while (!info.quit) {
		
		info.t2 = SDL_GetTicks();
		info.delta = (info.t2 - info.t1) * 0.001;
		info.t1 = info.t2;
		SDL_FillRect(info.screen, NULL, info.czarny);
		drawbackground(info, ground,player);
		if ((info.tryb == FIGHT) && (info.tlo != ground.fight1))
		{
			ground.x = FightSreenCenterX;
			ground.y = FightSreenCenterY;
			info.tlo = ground.fight1;
			info.worldTime = 0;
		}
		if (info.tryb == FIGHT)fighting(player,info,beza,ground);
		if (info.tryb == MENU) info.tlo = ground.menu;
		if ((info.tryb == GAMEOVER) && (info.tlo != ground.gameover)) info.tlo = ground.gameover;
		if ((info.tryb == WIN) && (info.tlo != ground.winscreen)) info.tlo = ground.winscreen;
		SDL_UpdateTexture(info.scrtex, NULL, info.screen->pixels, info.screen->pitch);
		SDL_RenderCopy(info.renderer, info.scrtex, NULL, NULL);
		SDL_RenderPresent(info.renderer);
		events(info,player,beza,ground);
		info.frames++;
		if ((MS / FPSLIM) > SDL_GetTicks() - info.t2) {
			SDL_Delay(MS / FPSLIM - (SDL_GetTicks() - info.t2));
		}
	}
	clear(info, 0);
	return 0;
	};
