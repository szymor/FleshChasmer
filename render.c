#include <SDL.h>
#include <SDL_mixer.h>
#include "fields.h"
#include "shop.h"
#include "types.h"
#include "triggers.h"
#include "zmath.h"
#include "gpu.h"
#include "game.h"
#include "interface.h"
#include "sound.h"
#include "sectoreffect.h"

#include "sfont.h"
#include "editor.h"
#include "video.h"
#include "globalvar.h"
#define FPMUL(x,y)	((((x)>>6)*((y)>>6))>>4)
#define FPDIV(x,y)	((((x)<<6)/((y)>>6))>>4)


Uint8 zlcolor;
Sint8 lastlevelsight=-1;
Uint8 map_ogb33p[10]={9,14,13,9,9,9,9,9,9,15};
Uint8 map_ogb33f[10]={0,0,0,0,0,0,0,0,0,4};

Sint32 r_modelx[64],r_modely[64],r_modelz[64],r_modeltx[64],r_modelty[64],r_modeltz[64];
Uint8 r_modelf0[64],r_modelf1[64],r_modelanim[64],r_modelanimcount[64];
Uint8 r_modeli[64],r_modelcolor[64];
Uint8 r_models;

Sint32 nx,ny,nz;

//army of sprites
Uint8 r_sprites,r_spritei[48];
Sint32 r_spritex[48],r_spritey[48],r_spritez[48],r_spritew[48];

/*

*/
void PrepareSprites(void)
{
Uint8 i=0;
if (movie_mode!=1)
for (Uint8 n=0;n<16;n++)
if (loot_count[n]!=0)
{
r_spritex[i]=loot_rx[n];
r_spritey[i]=loot_ry[n];
r_spritez[i]=loot_rz[n];

r_spritei[i]=160+loot_item[n];
r_spritew[i]=10000;
i++;
}


for (Uint8 n=0;n<32;n++)
if (field_ingame[n])
{
r_spritex[i]=field_x[n];
r_spritey[i]=field_y[n];
r_spritez[i]=field_z[n];

r_spritei[i]=field_index[n];
r_spritew[i]=field_w[n];
i++;
}




r_sprites=i;
}

void RenderSprites(void)
{
Sint32 z,s,x0,x1,y0,y1,x,y,xt,yt,ci,cy,t,x00,y00;
Uint8 c;
PrepareSprites();     
if (r_sprites)
{
zlvertex=0;
 for (Uint8 i=0;i<r_sprites;i++)
 {vbufferz[0]=0;
  zlpoint(r_spritex[i],r_spritez[i],r_spritey[i]);
  z=vbufferz[0];
  if (z>50)
  {
  texture=r_spritei[i];
  s=FPMUL(r_spritew[i],dist);
  s=FPDIV(s,z+dist);
  x0=vbufferx[0]-s;
  x1=vbufferx[0]+s;
  y0=vbuffery[0]-s;
  y1=vbuffery[0]+s;//If dis shalt art ported to DS be happy
  x00=x0;y00=y0;
  if (s>0)
  {
  t=65536*16/(2*s);
  if ((x0<160)&(x1>0)&(y0<120)&(y1>0))
   {//actual draw
    if (x0<0)x0=0;
    if (x1>160)x1=160;
    if (y0<0)y0=0;
    if (y1>120)y1=120;
    for (y=y0;y<y1;y++)
    {
    cy=y*160;
    for (x=x0;x<x1;x++)
    {
    ci=cy+x;
    if (z<zbuffer0[ci])
     { 
                            
     xt=FPMUL((x-x00)*65536,t)>>16;
     yt=FPMUL((y-y00)*65536,t)>>16;
     c=Tile[Tile_offset[texture]+xt+yt*16];                

     if (c)
       {
       zbuffer0[ci]=z;
       scrbuf0[ci]=c;    
       }   
     }
    }
    }//actual draw
   }
   }
  }
 }
}     
}


void zlline(Sint32 vx,Sint32 vy,Sint32 vz,Sint32 vx0,Sint32 vy0,Sint32 vz0)
{
zlvertex=0;
zlpoint(vx,vy,vz);
zlvertex=1;
zlpoint(vx0,vy0,vz0);

Sint32 lastx,x,x0,y,y0,y1,c=0;
if (vvis[0]&&vvis[1])
{
vbufferx[1]=vbufferx[1]*2;
vbuffery[1]=vbuffery[1]*2;
vbufferx[0]=vbufferx[0]*2;
vbuffery[0]=vbuffery[0]*2;

if (vbuffery[0]<vbuffery[1]) {y0=vbuffery[0];y1=vbuffery[1];}else{y0=vbuffery[1];y1=vbuffery[0];}

if (!((y0>screen_y1)||(y1<screen_y0)))
{
if (y0<screen_y0) y0=screen_y0;
if (y1>screen_y1) y1=screen_y1;
if (vbuffery[0]==vbuffery[1])
{
if (vbufferx[0]<vbufferx[1]) {lastx=vbufferx[0];x0=vbufferx[1];}else{lastx=vbufferx[1];x0=vbufferx[0];}

   for (x=lastx;x<x0;x++)     
      if ((x>=screen_x0)&&(x<=screen_x1))
      scrbuf[x+y1*320]=zlcolor;
            
           
}

else
 for (y=y0;y<=y1;y++)
 {
  x0=vbufferx[0] + (y - vbuffery[0]) * (vbufferx[1] - vbufferx[0]) / (vbuffery[1] - vbuffery[0]);
  if (c>0)
  {
   if (x0>lastx)
    {
    for (x=lastx;x<x0;x++)     
      if ((x>=screen_x0)&(x<screen_x1))
      scrbuf[x+y*320]=zlcolor;
      }else
    for (x=lastx;x>x0;x--)     
      if ((x>=screen_x0)&(x<=screen_x1))
      scrbuf[x+y*320]=zlcolor;
      
  }
  if ((x0>=screen_x0)&(x0<=screen_x1))
  scrbuf[x0+y*320]=zlcolor;
  lastx=x0;c++;
 }

}
}     
}    


void scrbuf_draw(void)
{
	SDL_LockSurface(screen);
#if defined(POCKETGO) || defined(RETROFW)
	for (int x = 0; x < 320; x++)
		for (int y = 0; y < 240; y++)
		{
			Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x*4;
			Uint8 c = scrbuf[x+y*320];
			*(Uint32 *)p =ipal[c][2] + 256*ipal[c][1] + 65536*ipal[c][0];
		}
#elif defined(GP2X)
if (CradleMode)
for (int i=3200;i<73600;i++)
{
Uint8 *p = (Uint8 *)screen->pixels +i;
*(Uint8 *)p =scrbuf[i];
}
else
for (int i=0;i<76800;i++)
{
Uint8 *p = (Uint8 *)screen->pixels +i;
*(Uint8 *)p =scrbuf[i];
}
#elif defined(PC)
Uint16 x1,y1;
for (int x=0;x<640;x++)
for (int y=0;y<480;y++)
{
    x1=x/2;
    y1=y/2;
Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x*4;
Uint8 c;
c=scrbuf[x1+y1*320];
*(Uint32 *)p =ipal[c][2]+256*ipal[c][1]+65536*ipal[c][0];
}
#endif

SDL_UnlockSurface(screen);
       SDL_Flip(screen);
     
}
void ClrScr(void)
{
for (int i=0;i<76800;i++)
scrbuf[i]=16;   
}
void recalculatesight(void)
{
Uint8 i;
if (levelsight<4) levelsight=4;
else
if (levelsight>9) levelsight=9;

Sint32 range=levelsight*57000;
for (i=0;i<160;i++)
{
fog3[i]=range+(-65536+f_sin[i*13])/8;
fog2[i]=fog3[i]-32000;     
fog1[i]=fog2[i]-26000;     
fog0[i]=fog1[i]-22000;     
}
lastlevelsight=levelsight;     
}

void renderlevel(void)
{
Sint32 mas[21][21][9],cell;
Sint32 t,t0;
Uint8 vl,obj;

Sint32 cx,cy,cz,x,y,z,x0,x1,z0,z1,ix,iy,iz,dx,dy,dz;
cx=rcamera[0]>>16;ix=cx-9;
cy=rcamera[1]>>16;iy=0;
cz=rcamera[2]>>16;iz=cz-9;     

x0=9-levelsight;x1=9+levelsight;
z0=9-levelsight;z1=9+levelsight;

t=cx-9+x0;
if (t<0) {x0=x0-t;}

t=cz-9+z0;
if (t<0) {z0=z0-t;}

t=cx-9+x1;
if (t>levelx) {x1=1+x1-((t-levelx));}

t=cz-9+z1;
if (t>levely) {z1=1+z1-((t-levely));}

if ((x0<=x1)&&(z0<=z1))
{
x1=x1;
z1=z1;
nx=f_sin[camera[4]];
nz=f_cos[camera[4]];

ny=f_sin[camera[3]];
nx=FPMUL(nx,f_cos[camera[3]]);
nz=FPMUL(nz,f_cos[camera[3]]);
for (y=0;y<=levelz+1;y++)
for (z=z0;z<=z1;z++)
for (x=x0;x<=x1;x++)
{
t=(x+ix)+(z+iz)*128+(y+iy)*16384;
t0=FPMUL(nx,meshx[t]-camera[0])
  +FPMUL(nz,meshy[t]-camera[2])
  +FPMUL(ny,meshz[t]-camera[1]);
if (t0>-100000)
{
zlvertex=4+x+z*20+y*400;

if (mapobj[t]>=33)
if (mapobj[t]>=43)
{
SectorEffect(x+ix,z+iz,y+iy);
}else
   {
r_modelcolor[r_models]=0;
r_modelx[r_models]=meshx[t]+32666;
r_modely[r_models]=(meshz[t]+meshz[t+1]+meshz[t+128]+meshz[t+129])>>2;
r_modelz[r_models]=meshy[t]+32666;
r_modeltx[r_models]=0;
r_modelty[r_models]=0;
r_modeltz[r_models]=0;
//r_modelty[r_models]=(count*10)%4096;
//+1
Uint8 ti=mapobj[t]-33;

r_modeli[r_models]=map_ogb33p[ti];

r_modelf0[r_models]=map_ogb33f[ti];
if (ti==9) r_modelf0[r_models]=4+count/6%4;
r_modelf1[r_models]=r_modelf0[r_models];
r_models++;


//////END OF THEM   
   }//visual objects
zlpoint(meshx[t],meshz[t],meshy[t]);    
}
mas[x][z][y]=t0;
}

for (y=0;y<=levelz;y++)
for (z=z0;z<z1;z++)
for (x=x0;x<x1;x++)
if (mas[x][z][y]>-25536)
{
dx=x+ix;
dy=y;
dz=z+iz;
cell=(dx)+(dz)*128+(dy)*16384;
if (cy>=dy)
 {
if (mapt0[cell])
  {
t=4+(x)+(z)*20+y*400;
vbufferx[3]=vbufferx[t];
vbuffery[3]=vbuffery[t];
vbufferz[3]=vbufferz[t];
vvis[3]=vvis[t];

t0=t+1;
vbufferx[2]=vbufferx[t0];
vbuffery[2]=vbuffery[t0];
vbufferz[2]=vbufferz[t0];
vvis[2]=vvis[t0];

t0=t+21;
vbufferx[1]=vbufferx[t0];
vbuffery[1]=vbuffery[t0];
vbufferz[1]=vbufferz[t0];
vvis[1]=vvis[t0];

t0=t+20;
vbufferx[0]=vbufferx[t0];
vbuffery[0]=vbuffery[t0];
vbufferz[0]=vbufferz[t0];
vvis[0]=vvis[t0];


vl=mapt0[cell];
if (tiledata_animated[vl]) vl+=counttextureanimation;
texture=vl;
if (tiledata_alpha[vl])
zltexquada();else    
zltexquad();

//DELETE THIS
Textured_Faces+=2;
//DELETE THIS

  }
 }//floor
if (cx>=dx)
 {
if (mapt1[cell])
  {
t=4+(x)+(z)*20+y*400;
vbufferx[3]=vbufferx[t];
vbuffery[3]=vbuffery[t];
vbufferz[3]=vbufferz[t];
vvis[3]=vvis[t];

t0=t+20;
vbufferx[2]=vbufferx[t0];
vbuffery[2]=vbuffery[t0];
vbufferz[2]=vbufferz[t0];
vvis[2]=vvis[t0];

t0=t+420;
vbufferx[1]=vbufferx[t0];
vbuffery[1]=vbuffery[t0];
vbufferz[1]=vbufferz[t0];
vvis[1]=vvis[t0];

t0=t+400;
vbufferx[0]=vbufferx[t0];
vbuffery[0]=vbuffery[t0];
vbufferz[0]=vbufferz[t0];
vvis[0]=vvis[t0];

vl=mapt1[cell];
if (tiledata_animated[vl]) vl+=counttextureanimation;
texture=vl;
if (tiledata_alpha[vl])
zltexquada();else    
zltexquad();
//DELETE THIS
Textured_Faces+=2;
//DELETE THIS

  }
 }//west
if (cx<=dx)
 {
if (mapt2[cell])
  {
t=5+(x)+(z)*20+y*400;
vbufferx[2]=vbufferx[t];
vbuffery[2]=vbuffery[t];
vbufferz[2]=vbufferz[t];
vvis[2]=vvis[t];

t0=t+20;
vbufferx[3]=vbufferx[t0];
vbuffery[3]=vbuffery[t0];
vbufferz[3]=vbufferz[t0];
vvis[3]=vvis[t0];

t0=t+420;
vbufferx[0]=vbufferx[t0];
vbuffery[0]=vbuffery[t0];
vbufferz[0]=vbufferz[t0];
vvis[0]=vvis[t0];

t0=t+400;
vbufferx[1]=vbufferx[t0];
vbuffery[1]=vbuffery[t0];
vbufferz[1]=vbufferz[t0];
vvis[1]=vvis[t0];

vl=mapt2[cell];
if (tiledata_animated[vl]) vl+=counttextureanimation;
texture=vl;
if (tiledata_alpha[vl])
zltexquada();else    
zltexquad();
//DELETE THIS
Textured_Faces+=2;
//DELETE THIS

  }
 }//east
if (cz>=dz)
 {
if (mapt3[cell])
  {
t=4+(x)+(z)*20+y*400;
vbufferx[2]=vbufferx[t];
vbuffery[2]=vbuffery[t];
vbufferz[2]=vbufferz[t];
vvis[2]=vvis[t];

t0=t+1;
vbufferx[3]=vbufferx[t0];
vbuffery[3]=vbuffery[t0];
vbufferz[3]=vbufferz[t0];
vvis[3]=vvis[t0];

t0=t+401;
vbufferx[0]=vbufferx[t0];
vbuffery[0]=vbuffery[t0];
vbufferz[0]=vbufferz[t0];
vvis[0]=vvis[t0];

t0=t+400;
vbufferx[1]=vbufferx[t0];
vbuffery[1]=vbuffery[t0];
vbufferz[1]=vbufferz[t0];
vvis[1]=vvis[t0];

vl=mapt3[cell];
if (tiledata_animated[vl]) vl+=counttextureanimation;
texture=vl;
if (tiledata_alpha[vl])
zltexquada();else    
zltexquad();
//DELETE THIS
Textured_Faces+=2;
//DELETE THIS


  }
 }//south
if (cz<=dz)
 {
if (mapt4[cell])
  {

t=24+(x)+(z)*20+y*400;
vbufferx[3]=vbufferx[t];
vbuffery[3]=vbuffery[t];
vbufferz[3]=vbufferz[t];
vvis[3]=vvis[t];

t0=t+1;
vbufferx[2]=vbufferx[t0];
vbuffery[2]=vbuffery[t0];
vbufferz[2]=vbufferz[t0];
vvis[2]=vvis[t0];

t0=t+401;
vbufferx[1]=vbufferx[t0];
vbuffery[1]=vbuffery[t0];
vbufferz[1]=vbufferz[t0];
vvis[1]=vvis[t0];

t0=t+400;
vbufferx[0]=vbufferx[t0];
vbuffery[0]=vbuffery[t0];
vbufferz[0]=vbufferz[t0];
vvis[0]=vvis[t0];

vl=mapt4[cell];
if (tiledata_animated[vl]) vl+=counttextureanimation;
texture=vl;
if (tiledata_alpha[vl])
zltexquada();else    
zltexquad();
//DELETE THIS
Textured_Faces+=2;
//DELETE THIS

  }
 }//north
if (cy<=dy)
 {
if (mapt5[cell])
  {
t=404+(x)+(z)*20+y*400;
vbufferx[3]=vbufferx[t];
vbuffery[3]=vbuffery[t];
vbufferz[3]=vbufferz[t];
vvis[3]=vvis[t];

t0=t+1;
vbufferx[2]=vbufferx[t0];
vbuffery[2]=vbuffery[t0];
vbufferz[2]=vbufferz[t0];
vvis[2]=vvis[t0];

t0=t+21;
vbufferx[1]=vbufferx[t0];
vbuffery[1]=vbuffery[t0];
vbufferz[1]=vbufferz[t0];
vvis[1]=vvis[t0];

t0=t+20;
vbufferx[0]=vbufferx[t0];
vbuffery[0]=vbuffery[t0];
vbufferz[0]=vbufferz[t0];
vvis[0]=vvis[t0];

vl=mapt5[cell];
if (tiledata_animated[vl]) vl+=counttextureanimation;
texture=vl;
if (tiledata_alpha[vl])
zltexquada();else    
zltexquad();
//DELETE THIS
Textured_Faces+=2;
//DELETE THIS

  }
 }//celling

}

}

}
Sint16 fade_ef0[8]={-1,0,11,329,3,4,325,10};
void RenderFade(void)

{
if (fadecount==0) {volume_delay=0;fadecount=10;GameMode=NewGameMode;}
else 
 {
fadecount--;
Uint32 i,k;
 

// for (i=0;i<76800;i++)
// scrbuf[i]=32+((scrbuf[i]-1)&7);   

 for (i=640;i<76160;i++)
 scrbuf[i]=scrbuf[i+fade_ef0[i&7]];   


 
 if (fadecount>4)
 { 
  if (fadecount%2==0)     
  {
 for (i=0;i<76800;i++)
 scrbuf[i]=palfade0[scrbuf[i]];   
  }                     
 }
 else
 {
 if (fadecount==4)
 for (i=0;i<76800;i++)
 scrbuf[i]=16;   
 }    



scrbuf_draw();
 }
}


void rendermodels(void)
{
if (r_models)
for (int i=0;i<r_models;i++)
/*
if (FPMUL(nx,r_modely[i]-camera[0])
  +FPMUL(nz,r_modelz[i]-camera[2])
  +FPMUL(ny,r_modely[i]-camera[1])>165535)
*/
 {
modelcolor=r_modelcolor[i];  
    zlpushmatrix();
    zltranslate(r_modelx[i],r_modely[i],r_modelz[i]);
    if (r_modelty[i]) zlrotatey(r_modelty[i]);
    if (r_modeltz[i]) zlrotatez(r_modeltz[i]);
    if (r_modeltx[i]) zlrotatex(r_modeltx[i]);
    if (r_modelf0[i]==r_modelf1[i])
    rendermodel(r_modeli[i],r_modelf0[i]);
    else
    renderanimmodel(r_modeli[i],r_modelf0[i],r_modelf1[i],r_modelanimcount[i],r_modelanim[i]);
    zlpopmatrix();              

             
 }     
     
}
void rendermovie(void)
{
Uint8 mp=movie_playing;
for (Uint8 i=1;i<=movie_num_layers[mp];i++)
if (c_movie_object[i][0])
 {
 r_modelcolor[r_models]=0;
r_modelx[r_models]=c_movie_layer[i][0];
r_modelz[r_models]=c_movie_layer[i][2];
r_modely[r_models]=c_movie_layer[i][1];


r_modeltx[r_models]=c_movie_layer[i][3];
r_modelty[r_models]=c_movie_layer[i][4];
r_modeltz[r_models]=c_movie_layer[i][5];

r_modeli [r_models]=c_movie_object[i][0]-1;
if (c_movie_object[i][2])
{
r_modelf0[r_models]=c_movie_object[i][1]+(count/c_movie_object[i][2])%4;
r_modelf1[r_models]=c_movie_object[i][1]+(count/c_movie_object[i][2]+1)%4;
r_modelanim[r_models]=c_movie_object[i][2];
r_modelanimcount[r_models]=count % c_movie_object[i][2];
}
else
{
r_modelf0[r_models]=c_movie_object[i][1];
r_modelf1[r_models]=r_modelf0[r_models];
r_modelanim[r_models]=0;
r_modelanimcount[r_models]=0;
}

r_models++;

    
    
 }
     
}
void rendermonsters(void)
{
if (monsters)
for (Uint8 i=0;i<16;i++)
if (monster_ingame[i])
if (monster_visible[i])
 {
r_modelcolor[r_models]=monster_modelcolor[i];
r_modelx[r_models]=monster_x[i];
r_modelz[r_models]=monster_y[i];
r_modely[r_models]=monster_z[i];
r_modeltx[r_models]=monster_anglex[i];
r_modelty[r_models]=monster_angle[i];
r_modeltz[r_models]=monster_anglez[i];
r_modeli [r_models]=monster_model[i];;
r_modelf0[r_models]=monster_frame0[i];;
r_modelf1[r_models]=monster_frame1[i];
r_modelanim[r_models]=monster_anim[i];
r_modelanimcount[r_models]=monster_animcount[i];
r_models++;
if (monster_hasted[i])
if (count/2%2)
  {
r_modelcolor[r_models]=monster_modelcolor[i];
r_modelx[r_models]=monster_x[i]-2000+random(4000)-monster_ix[i]*8;
r_modelz[r_models]=monster_y[i]-2000+random(4000)-monster_iy[i]*8;
r_modely[r_models]=monster_z[i];
r_modeltx[r_models]=monster_anglex[i];
r_modelty[r_models]=monster_angle[i];
r_modeltz[r_models]=monster_anglez[i];
r_modeli [r_models]=monster_model[i];;
r_modelf0[r_models]=monster_frame0[i];;
r_modelf1[r_models]=monster_frame1[i];
r_modelanim[r_models]=monster_anim[i];
r_modelanimcount[r_models]=monster_animcount[i];
r_models++;
                    
  }

 }     
}
char indic[4][40]={"Level Up!","Cool","Poisoned","Boss"};
Uint8 indic_c[4]={31,31,31,31};

void RenderIndicators(void)
{
Uint32 x,y;
zlvertex=0;
Uint8 color;
for (Uint8 i=0;i<4;i++)
if (indicator_count[i])
 {
  zlpoint(indicator_x[i],indicator_z[i],indicator_y[i]);    


if (indicator_arg[i]<=-10000)
{
l_textstring=sprintf(s_textstring,"+%i",-10000-indicator_arg[i]);
color=87;
}
else
if (indicator_arg[i]<0)
{
l_textstring=sprintf(s_textstring,"%i",indicator_arg[i]);
color=39-indicator_count[i]/4;
}
else
if (indicator_arg[i]==0)
{
l_textstring=sprintf(s_textstring,"Guard");
color=29;
}
else
if (indicator_arg[i]<10000)
{
l_textstring=sprintf(s_textstring,"+%i",indicator_arg[i]);
color=55;
}
else
//if (indicator_arg[i]>=10000)
{
l_textstring=sprintf(s_textstring,"%s",indic[indicator_arg[i]-10000]);
color=indic_c[indicator_arg[i]-10000];
}



x=vbufferx[0]*2-l_textstring/2*10;
y=vbuffery[0]*2-12;

s_drawtext2(x,y-1,16);
s_drawtext2(x+1,y,16);
s_drawtext2(x-1,y,16);
s_drawtext2(x,y+1,16);
s_drawtext2(x,y,color);
indicator_z[i]+=1500;                       

  indicator_count[i]++;
  if (indicator_count[i]==25)
   indicator_count[i]=0; 
 }
 
}
void renderhand()
{
Sint32 ix=f_cos[monster_angle[0]];     
Sint32 iy=f_sin[monster_angle[0]];     
if (monster_action[0]==2)
 {
if (player_arm[0])
if (monster_frame0[0]!=t_monster_meleeframe0[0])
{
r_modelx[r_models]=monster_x[0];
r_modelz[r_models]=monster_y[0];
r_modely[r_models]=monster_z[0];
r_modeltx[r_models]=monster_anglex[0];
r_modelty[r_models]=monster_angle[0];
r_modeltz[r_models]=monster_anglez[0];

r_modelx[r_models]+=ix*5/12;
r_modelz[r_models]+=iy*5/12;
r_modelcolor[r_models]=0;

switch (player_arm[0])
{
case 32:r_modeli [r_models]=21;break;//knife
case 33:r_modeli [r_models]=22;break;//sword
case 34:r_modeli [r_models]=23;break;//katane
case 35:r_modeli [r_models]=24;break;//rod
case 36:r_modeli [r_models]=25;break;//hammer
case 37:
r_modeli [r_models]=22;
if (count%2)
r_modelcolor[r_models]=37;

break;//glass sword
case 38:r_modeli [r_models]=26;break;//scythe
case 39:
r_modeli [r_models]=27;
if (count%2)
r_modelcolor[r_models]=31;
break;//chaingsaw

}
r_modelf0[r_models]=0;
r_modelf1[r_models]=0;
r_modelanim[r_models]=0;
r_modelanimcount[r_models]=0;

r_models++;
}
 }    
else
if (monster_frame0[0]==t_monster_shootframe[0]& handpistol>0)
 {
r_modelcolor[r_models]=0;
r_modelx[r_models]=monster_x[0];
r_modelz[r_models]=monster_y[0];
r_modely[r_models]=monster_z[0];
r_modeltx[r_models]=monster_anglex[0];
r_modelty[r_models]=monster_angle[0];
r_modeltz[r_models]=monster_anglez[0];
/*
if (monster_action[0]!=5)
{
r_modelx[r_models]=monster_x[0]-ix/30*monster_actioncount[0];
r_modelz[r_models]=monster_y[0]-iy/30*monster_actioncount[0];
}
*/


r_modeli [r_models]=17+t_monster_shoottype[0];
switch (t_monster_shoottype[0])
{
case 1:
r_modely[r_models]+=4000;
break;       
}
r_modelf0[r_models]=0;
r_modelf1[r_models]=0;
r_modelanim[r_models]=0;
r_modelanimcount[r_models]=0;


r_models++;
 }    

}

void rendermap(void)
{
for (Uint8 i=0;i<16;i++)
if (monster_ingame[i])
{
//scrbuf[(50+monster_cx[i]*2)+320*(50+monster_cy[i]*2)]=48;
}
}

void renderscene(void)
{
//DELETE ME
Model_Faces=0;
Textured_Faces=0;
//DELETE ME
ProcFields();



r_models=0;     
if (lastlevelsight!=levelsight) recalculatesight();     


int i;
for (i=0;i<76800;i++)
zbuffer[i]=0xFFFFFFF;   

int for0,for1;
for0=screen0_y0*160;
for1=screen0_y1*160;
for (i=0;i<=for0;i++)
{
zbuffer0[i]=0;
scrbuf0[i]=16;
}
for (i=for0;i<=for1;i++)
{
zbuffer0[i]=0xFFFFFFF;
scrbuf0[i]=fogcolor;
}
for (i=for1;i<=19199;i++)
{
zbuffer0[i]=0;
scrbuf0[i]=16;
}

/*
for (i=0;i<=19199;i++)
{
zbuffer0[i]=0xFFFFFFF;
scrbuf0[i]=16;
}
*/

matrix[0]=65536;matrix[1]=0;matrix[2]=0;matrix[3]=0;
matrix[4]=0;matrix[5]=65536;matrix[6]=0;matrix[7]=0;
matrix[8]=0;matrix[9]=0;matrix[10]=65536;matrix[11]=0;
matrix[12]=0;matrix[13]=0;matrix[14]=0;matrix[15]=65536;

zltranslate(0,0,-dist);
zlrotatez(rcamera[5]);
zlrotatex(rcamera[3]);
zlrotatey(rcamera[4]);
zltranslate(-rcamera[0],-rcamera[1],-rcamera[2]);

txbuffer[0]=500;    tybuffer[0]=500;
txbuffer[1]=65035;   tybuffer[1]=500;
txbuffer[2]=65035;   tybuffer[2]=65035;
txbuffer[3]=500;    tybuffer[3]=65035;

renderlevel();

if (movie_playing<16) rendermovie();
if (movie_mode!=1){rendermonsters();
if (level_tag==0) renderhand();}

rendermodels();//:)


RenderSprites();

i=0;Uint16 k,c;
k=0;c=0;

Uint8 col;Sint32 z;
i=0;
while (i<76800)
{
col=scrbuf0[k];
z=zbuffer0[k];
if (z>fog3[c]) col=fogcolor;
else
if (z>fog2[c]) col=palfade2[col];
else
if (z>fog1[c]) col=palfade1[col];
else
if (z>fog0[c]) col=palfade0[col];

scrbuf[i]=col;
scrbuf[i+1]=col;

zbuffer[i]=z;
zbuffer[i+1]=z;
c++;
i+=2;k++;     
if (c==160) {c=0;i+=320;}
}


for (i=320+screen_y0*320;i<(screen_y1+1)*320;i++)
{
scrbuf[i]=scrbuf[i-320];
zbuffer[i]=zbuffer[i-320];
c++;     
if (c==320) {c=0;i+=320;}
}


//agressive cam detection
agressive_cam=0;
//if camera watches player
{
zlvertex=0;
zlpoint(monster_x[0],monster_z[0]+20000,monster_y[0]);    
//debugval0=vbufferx[0];
//debugval1=vbuffery[0];
if ((vbufferx[0]<40)|(vbufferx[0]>120))
agressive_cam=1;
}
//

RenderIndicators();

//if ((count>40)&(count<100))
/*
{
                           
l_textstring=sprintf(s_textstring,"fps:%i",fps);
s_drawtext(281,screen_y0+1,16);        
s_drawtext(280,screen_y0,14);        
l_textstring=sprintf(s_textstring,"%i",debugval0);
s_drawtext(281,screen_y0+13,16);        
if (debugval0==0)
s_drawtext(280,screen_y0+12,14);
else
s_drawtext(280,screen_y0+12,15);

l_textstring=sprintf(s_textstring,"%i",debugval1);
s_drawtext(281,screen_y0+25,16);        
if (debugval1==0)
s_drawtext(280,screen_y0+24,14);
else
s_drawtext(280,screen_y0+24,15);
}
*/
/*l_textstring=sprintf(s_textstring,"color faces:%i",Model_Faces);
s_drawtext(241,161,16);        
s_drawtext(240,160,14);        

l_textstring=sprintf(s_textstring,"  textured :%i",Textured_Faces);
s_drawtext(241,173,16);        
s_drawtext(240,172,14);        
*/
/*
l_textstring=sprintf(s_textstring,"%i",handpistol);
s_drawtext(241,173,16);        
s_drawtext(240,172,14);        
*/
}

void Render(void)
{
int i;
ClrScr();
if (EditorOn) RenderEditor();
else
switch (GameMode)
{
case LOADING_GAME:gameload();       break;
case GAME:gamerender();       break;       
case SHOP:ShopProc();       break;       
case MAINMENU:mainmenu();       break;       
case CHARACTERMENU:CharacterMenu();       break;       
case PLAYER_NAME:PlayerNameMenu(); break;       
case CLOSING_CREDITS:creditsproc();       break;
case INTRO:renderintro();       break;

}
if (GameMode==GAME)
if ((1==movie_mode)&(c_movie_step<3))
ClrScr();

if (count<4)
ClrScr();

if (volume_delay)
 {
if (volume_delay>10)                 
{                 
if (volume==128)
l_textstring=sprintf(s_textstring,"Volume MAX",volume);
else
if (volume)
l_textstring=sprintf(s_textstring,"Volume %i/128",volume);
else
l_textstring=sprintf(s_textstring,"Sound OFF");
s_drawtext2(161-l_textstring*10/2,201,16);
s_drawtext2(160-l_textstring*10/2,200,31);
}
else
{
if (volume==128)
l_textstring=sprintf(s_textstring,"Volume MAX",volume);
else
if (volume)
l_textstring=sprintf(s_textstring,"Volume %i/128",volume);
else
l_textstring=sprintf(s_textstring,"Sound OFF");
s_drawtext2(161-l_textstring*10/2,211-volume_delay,16);
s_drawtext2(160-l_textstring*10/2,210-volume_delay,17+volume_delay);

}
Mix_Volume(-1,volume);

Uint32 vol=100;
  if (Music_Fade_Count)
  if (Music_Fade_Count>25) vol=(Music_Fade_Count-25)*4;
  else
  vol=100-Music_Fade_Count*4;
vol=vol*volume/100;

vol=(vol*Music_Volume)/100;
              
Mix_Volume(0,vol);               
volume_delay--;                
 }  



scrbuf_draw();  
}


