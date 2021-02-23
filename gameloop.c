#include <SDL.h>
//#include <stdlib.h>
#include "globalvar.h"
#include "gameloop.h"
#include "render.h"
#include "gameproc.h"

void calcfps(void)
{
if (tickcount-tickcount2>1000)
{
iGameTime++;                         

fps=fps_done;fps_done=0;
tickcount2=tickcount;

}
     
}
/*
void makedelay(void)
{
ltickcount=tickcount;     
Uint32 delta=tickcount-ltickcount;
if (delta<50) SDL_Delay(50-delta);
}
*/

#if defined(GP2X)
void gameloop(void)
{
	while (GameLoopEnabled)
	{
		if (GameMode==NewGameMode)
		{
			Gameprocess();
			Render();
			fps_done++;
		}
		else
		{
			RenderFade();
		}

		switch (GameMode)
		{
			case LOADING_EDITOR: SDL_Delay(40); break;
			case CLOSING_CREDITS: SDL_Delay(40); break;
			case LOADING_GAME: SDL_Delay(40); break;
			case MAINMENU: SDL_Delay(40); break;
			case PLAYER_NAME: SDL_Delay(40); break;
			case CHARACTERMENU: SDL_Delay(40); break;
			case EDITOR_MENU: SDL_Delay(30); break;
			case INTRO: SDL_Delay(30); break;
			case EDITOR_AI: SDL_Delay(40); break;
			case EDITOR_PALETTE: SDL_Delay(40); break;
			case EDITOR_TEXTURE: SDL_Delay(40); break;
		}

		tickcount=SDL_GetTicks();
		count++;
		calcfps();
	}
}
#elif defined(PC)
static void limitfps(void)
{
	static Uint32 curTicks = 0;
	static Uint32 lastTicks = 0;
	const Uint32 delay = 120;
	Uint32 t;

	curTicks = SDL_GetTicks();
	if (lastTicks == 0)
	{
		lastTicks = curTicks;
		return;
	}

	t = curTicks - lastTicks;
	lastTicks = curTicks;

	if (t < delay)
		SDL_Delay(delay - t);
}

void gameloop(void)
{
	while (GameLoopEnabled)
	{
		if (GameMode==NewGameMode)
		{
			Gameprocess();
			Render();
			fps_done++;
		}
		else
		{
			RenderFade();
		}

		limitfps();

		tickcount=SDL_GetTicks();
		count++;
		calcfps();
 	}
}
#endif
