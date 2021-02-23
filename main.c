#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <SDL.h>
#include <SDL_mixer.h>

//#include "globalvar.h"
#include "controlpad.h"
#include "gameloop.h"
#include "sfont.h"
#include "initall.h"

#include "logger.h"

#define DEBUGON 1

#ifdef GP2X
#include "sys/mman.h"
#include "fcntl.h"

volatile unsigned short *MEM_REG;
unsigned long   gp2x_dev=0;

void RamHack(void)//I't me! RamHack!
{
gp2x_dev = open("/dev/mem", O_RDWR);

MEM_REG=(unsigned short *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, gp2x_dev, 0xc0000000);     
	unsigned short v,timing;

//tRC
  timing=5;
	v = (unsigned short)(MEM_REG[0x3804>>1] & (~(0xF << 8)));
	MEM_REG[0x3804>>1] = ((timing & 0xF) << 8) | v;	

//tRAS
  timing=3;
	v = (unsigned short)(MEM_REG[0x3804>>1] & (~(0xF << 4)));
	MEM_REG[0x3804>>1] = ((timing & 0xF) << 4) | v;	

//tWR
  timing=0;
	v = (unsigned short)(MEM_REG[0x3804>>1] & (~(0xF)));
	MEM_REG[0x3804>>1] = (timing & 0xF) | v;	

//tMRD
  timing=0;

	v = (unsigned short)(MEM_REG[0x3802>>1] & (~(0xF << 12)));
	MEM_REG[0x3802>>1] = ((timing & 0xF) << 12) | v;	

//tRFC
  timing=0;
  
	v = (unsigned short)(MEM_REG[0x3802>>1] & (~(0xF << 8)));
	MEM_REG[0x3802>>1] = ((timing & 0xF) << 8) | v;	

//tRP
  timing=1;
  
	v = (unsigned short)(MEM_REG[0x3802>>1] & (~(0xF << 4)));
	MEM_REG[0x3802>>1] = ((timing & 0xF) << 4) | v;	
//tRCD
  timing=1;

	v = (unsigned short)(MEM_REG[0x3802>>1] & (~(0xF)));
	MEM_REG[0x3802>>1] = (timing & 0xF) | v;	

close(gp2x_dev);
}

#endif
                                                    ////


int main (int argc, char *argv[])
{
	logmsg();
#ifdef PC
fullscreen = false;
#endif

for (int i=1; i < argc; ++i) {
	if (!strcmp("--fullscreen", argv[i])) {
		fullscreen = true;
	} else
	if (!strcmp("--editor", argv[i])) {
		EditorOn = 1;
	} else {
		printf("%s [--fullscreen] [--editor]\n", argv[0]);
		return strcmp("--help", argv[i]);
	}
}
	logmsg();
#ifdef GP2X
RamHack();
//system("/sbin/insmod mmuhack.o");
#endif

	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) 
    {
		exit (1);
	}
	SDL_ShowCursor(SDL_DISABLE);
	logmsg();
if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
    exit(1);
  }
Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	logmsg();
#if defined(POCKETGO) || defined(RETROFW)
	screen = SDL_SetVideoMode (320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (screen == NULL) {
		exit (2);
	}
#elif defined(GP2X)
	screen = SDL_SetVideoMode (320, 240, 8, SDL_HWSURFACE || SDL_DOUBLEBUF);
	if (screen == NULL) {
		exit (2);
	}
#elif defined(PC)
SDL_WM_SetCaption("FleshChasmer","");
	screen = SDL_SetVideoMode (640, 480, 32, SDL_HWSURFACE | (fullscreen ? SDL_FULLSCREEN : 0));
	if (screen == NULL) {
		exit (2);
	}
#endif
	logmsg();
	//load all
	LoadConfig();
	logmsg();
	InitAll();
	logmsg();
	if (EditorOn)
	{
	GameMode=EDITOR_MENU;//enchance this
	NewGameMode=EDITOR_MENU;//enchance this
	}
	else
	{
	NewGameMode=LOADING_GAME;//enchance this
	GameMode=LOADING_GAME;
	}
CradleMode=0;
	logmsg();
	if (SDL_NumJoysticks() > 0) {
		joy = SDL_JoystickOpen(0);
	}
#ifdef GP2X
	if (SDL_NumJoysticks() > 1) 
    {
    CradleMode=1;
		USBjoy = SDL_JoystickOpen(1);
	}
#endif

	logmsg();
	gameloop();
	logmsg();
    Mix_CloseAudio();
	SDL_Quit();
	SaveConfig();
#ifdef GP2X
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif
	logmsg();
	return 0;
}
