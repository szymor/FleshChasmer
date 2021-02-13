#include "render.h"
#include "sfont.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include "sound.h"
#include "zdialogs.h"
#include "monsters.h"
#include "player.h"
#include "globalvar.h"
#include "logger.h"
#define FPMUL(x,y)	((((x)>>6)*((y)>>6))>>4)


void LoadConfig(void)
{
FILE *fp;Uint8 c;
fp=fopen("data/config.cfg","rb");
if (fp)
{
volume=fgetc(fp);
SoundMode=fgetc(fp);
ControlMode=fgetc(fp);
CameraMode=fgetc(fp);
MonsterHpMode=fgetc(fp);
showmap=fgetc(fp);

}
fclose(fp);     

}
void SaveConfig(void)
{
FILE *fp;Uint8 c;
fp=fopen("data/config.cfg","wb");
fputc(volume,fp);
fputc(SoundMode,fp);
fputc(ControlMode,fp);
fputc(CameraMode,fp);
fputc(MonsterHpMode,fp);
fputc(showmap,fp);

fclose(fp);     
}

void LoadPalette(void)
{
#ifdef GP2X
FILE *fp;char c;Uint8 i=0,n=0;
fp=fopen("data/palette.ggg","rb");
Uint8 t;
for (int i=0;i<256;i++)
{
screen->format->palette->colors[i].r=fgetc(fp);
screen->format->palette->colors[i].g=fgetc(fp);
screen->format->palette->colors[i].b=fgetc(fp);


if (i<16)
{
palfade0[i]=i;         
palfade1[i]=i;         
palfade2[i]=i;         
}
else
{
palfade0[i]=i/8*8+i%8/2;    
palfade1[i]=i/8*8+i%8/3;    
palfade2[i]=i/8*8+i%8/4;    
    
}


}    
     
fclose(fp);     
SDL_SetColors(screen,screen->format->palette->colors,0,256);
#endif
#ifdef PC
FILE *fp;char c;Uint8 i=0,n=0;
fp=fopen("data/palette.ggg","rb");
Uint8 t;
for (int i=0;i<256;i++)
{
ipal[i][0]=fgetc(fp);
ipal[i][1]=fgetc(fp);
ipal[i][2]=fgetc(fp);
if (i<16)
{
palfade0[i]=i;         
palfade1[i]=i;         
palfade2[i]=i;         
}
else
{
palfade0[i]=i/8*8+i%8/2;    
palfade1[i]=i/8*8+i%8/3;    
palfade2[i]=i/8*8+i%8/4;    
    
}
}    
     
fclose(fp);     
#endif
}

void LoadTileSet(void)
{
FILE *fp;Uint8 c;Uint16 i=0,n=0;
fp=fopen("data/tileset.ggg","rb");
if (fp!=NULL)
{
int i;
 for (i=0;i<=65535;i++)
 {
  Tile[i]=fgetc(fp);
 }    
     
 fclose(fp);     
}
for (i=0;i<256;i++)
Tile_offset[i]=256*i;


fp=fopen("data/tiledata.ggc","rb");

for (int i=0;i<=255;i++)
 {
 c=fgetc(fp);
      tiledata_alpha[i]=(c & 1);
   tiledata_animated[i]=(c & 2) / 2;
      tiledata_block[i]=(c & 4) / 4;
  tiledata_breakable[i]=(c & 8) / 8;
 tiledata_finalbreak[i]=(c &16) /16;
 tiledata_sound[i]=c/32;
 }
     
fclose(fp);

}



#define FPDIV(x,y)	((((x)<<10)/((y)>>2))>>4)
void inittables(void)
{
FILE *fp;char c;Uint16 i=0,n=0;
fp=fopen("data/cashe.ggg","rb");
if (fp!=NULL)
{
/*int i;Sint32 n;char b[4];
 for (i=0;i<4096;i++)
 {
  b[0]=fgetc(fp);   
  b[1]=fgetc(fp);
  b[2]=0;
  b[3]=0;    
    n=(Sint32)(b);   
  f_sin[i]=n;
  f_cos[(i+1024)%4096]=n;
 }    
  */
  fread(&f_sin,4,4096,fp);
 for (i=0;i<4096;i++)
  f_cos[i]=f_sin[(i+1024)%4096];
     
 fclose(fp);     f_sin[0]=0;


 for (i=0;i<=1023;i++)
 f_tan[i]=FPDIV(f_sin[i],f_cos[i]);
 f_tan[1023]=0xFFFFFFF;
}
}
void initsounds(void)
{
Mix_AllocateChannels(4);     
Mix_Volume(-1,volume);
Uint16 i=0;
d_sound_count=0;  

//spath="data/s000.wav";
char spath[64];
     
 sprintf(spath,"data/s000.wav");

d_sound[i]=NULL;
d_sound[i]=Mix_LoadWAV(spath);
while (d_sound[i]!=NULL)
     {
     d_sound_count++;
     i++;                   
d_sound[i]=NULL;
sprintf(spath,"data/s%i%i%i.wav",i/100,i/10%10,i%10);
d_sound[i]=Mix_LoadWAV(spath);
                        
     }



for (i=0;i<50;i++) 
{
music_loaded[i]=0;   
music[i]=NULL;
}
//for (i=0;i<=50;i++) 
//{
//OpenMusicTrack(i);   
//ClrScr();
//l_textstring=sprintf(s_textstring,"%i",i);
//s_drawtext(5,60,14);
//scrbuf_draw();
//}
}

void loadmodelscale(void)
{
FILE *fp;char c;Sint32 i;
fp=fopen("data/modscale.gmg","rb");
if (fp!=NULL)
 {
 for (i=0;i<256;i++)
  {
  model_scalex[i]=32768+(fgetc(fp)*256+fgetc(fp));          
  model_scaley[i]=32768+(fgetc(fp)*256+fgetc(fp));          
  model_scalez[i]=32768+(fgetc(fp)*256+fgetc(fp));          
  }            
 for (i=0;i<256;i++)
 model_scalei[i]=fgetc(fp);      

 fclose(fp);            
 }
 else
{ for (i=0;i<256;i++)
  {
  model_scalex[i]=65535;        
  model_scaley[i]=65535;
  model_scalez[i]=65535;          
  }            
for (i=0;i<256;i++)
model_scalei[i]=8;      
}
}


void loadmodels(void)
{
loadmodelscale();
Uint16 v=0,f=0,i=0,v0=0;
Uint16 c=0,n;Uint8 ch;
FILE *fp;char spath[128];
sprintf(spath,"data/%i%i%i.mgl",i/100,i/10%10,i%10);

while ((fp=fopen(spath,"rb"))!=NULL)
 {
model_f0[i]=f;
model_v0[i]=v;
v0=v;      
ch=fgetc(fp);c=ch+256*fgetc(fp);
model_fs[i]=ch;// From Zero to ...
ch=fgetc(fp);c=ch+256*fgetc(fp);
model_vs[i]=ch;
ch=fgetc(fp);c=ch+256*fgetc(fp);
model_ani[i]=ch;
//debugval=ch;
for (n=0;n<model_vs[i]*model_ani[i];n++)
{
c=fgetc(fp)+256*fgetc(fp);    
m_vx[v]=(c-32768);
c=fgetc(fp)+256*fgetc(fp);    
m_vy[v]=(c-32768);
c=fgetc(fp)+256*fgetc(fp);    
m_vz[v]=(c-32768);

m_vx[v]=(FPMUL(m_vx[v],model_scalex[i])*model_scalei[i])>>3;
m_vy[v]=(FPMUL(m_vy[v],model_scaley[i])*model_scalei[i])>>3;
m_vz[v]=(FPMUL(m_vz[v],model_scalez[i])*model_scalei[i])>>3;

v++;
}
for (n=0;n<model_fs[i];n++)
{
c=fgetc(fp);    
f_0[f]=c;
c=fgetc(fp);    
f_1[f]=c;
c=fgetc(fp);    
f_2[f]=c;
c=fgetc(fp);    
f_c[f]=c;
f++;
}


fclose(fp);
i++;
sprintf(spath,"data/%i%i%i.mgl",i/100,i/10%10,i%10);
 }   
model_mmv=v;
model_mmf=f;   
model_mms=i;   
}

void LoadAI(void)
{
FILE *fp;
fp=fopen("data/monster.ai","rb");
for (int i=0;i<32;i++)
for (int x=0;x<5;x++)
for (int y=0;y<8;y++)
 {
t_monster_ai[i][x][y]=fgetc(fp);
 }
fclose(fp);     
}



void initblockmap(void)
{
blockmapstatus[1]=STATUS_ALIVE;
blockmapstatus[2]=STATUS_AI;

}



void InitAll(void)
{
	logmsg();
	s_initfont();
	logmsg();
	ClrScr();
	logmsg();
	LoadPalette();
	logmsg();
	l_textstring=sprintf(s_textstring,"Loading Genocide Generations Gaming engine...");
	logmsg();
	s_drawtext(5,0,15);
	logmsg();
	scrbuf_draw();
	logmsg();

	l_textstring=sprintf(s_textstring,"Loading TileSet...");
	s_drawtext(5,24,14);
	LoadTileSet();
	scrbuf_draw();
	l_textstring=sprintf(s_textstring,"Loading Models...");
	s_drawtext(5,36,14);
	scrbuf_draw();
	loadmodels();// ^.^
	l_textstring=sprintf(s_textstring,"Loading Sounds...");
	s_drawtext(5,48,14);
	scrbuf_draw();
	initsounds();
	l_textstring=sprintf(s_textstring,"Starting game...");
	s_drawtext(5,60,14);
	scrbuf_draw();
	inittables();
	initblockmap();
	InitMonsterTable();
	LoadAI();
	InitPlayerStatus();
	LoadPlayerStatus();
	LoadPortraits();
	LoadBNG();
	ClrScr();
	scrbuf_draw();

	fadecount=0;
	//NewGameMode=EDITOR_MENU;//enchance this

	//EditorOn=0;
	//NewGameMode=LOADING_GAME;//enchance this
	//GameMode=LOADING_GAME;

	//NewGameMode=CLOSING_CREDITS;
	logmsg();
}
