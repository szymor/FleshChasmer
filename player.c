#include <player.h>
#include <globalvar.h>
#include <zmath.h>
#include <sfont.h>
#include <render.h>
#include <game.h>
#include <storage.h>
#include <sound.h>
#include <skills.h>
#include <interface.h>
#include <items.h>
#include <SDL.h>

void damage_meleeweapon(void)
{
if (level_tag==0)
{
Sint16 damage=1+random(5);
if (player_arm[0]==37) damage+=100+random(50);
if (damage>=weapon_points[player_arm[0]-32])
 {
 weapon_points[player_arm[0]-32]=1000;
 WearItem(player_arm[0],0);                                          
 LoseItem(player_arm[0]);
 player_arm[0]=0;
 sprintf(new_message,"Your weapon has just broken!");
 NewMessage();UpdatePlayer();
 }
 else weapon_points[player_arm[0]-32]-=damage;
}
}
void damage_missleweapon(void)
{
if (level_tag==0)
{
Sint16 damage=1+random(5);
if (damage>=weapon_points[player_arm[1]-32])
 {
 weapon_points[player_arm[1]-32]=1000;
 WearItem(player_arm[1],0);                                          
 LoseItem(player_arm[1]);
 player_arm[1]=0;
 sprintf(new_message,"You broke your gun!");
 NewMessage();UpdatePlayer();
 }
 else weapon_points[player_arm[1]-32]-=damage;
}
}

void UpdatePlayer(void)
{
if (level_tag==0)
{
monster_mhp[0]=44+(t_monster_str[0]+4*t_monster_con[0])/4*ch_level;
t_monster_mn[0]=10+(2*t_monster_int[0]+4*t_monster_wis[0])/18*ch_level;
t_monster_mv[0]=25+(t_monster_str[0]+t_monster_con[0])/3*ch_level;
t_monster_speed[0]=2*t_monster_dex[0]+275;
monster_speed[0]=t_monster_speed[0];
t_monster_speed1[0]=t_monster_speed[0]*3/4;
t_monster_speed0[0]=t_monster_speed[0]/2;

t_monster_damage[0][0]=4+t_monster_str[0]*ch_level/14;
t_monster_damage[0][1]=t_monster_dex[0]*ch_level/26;

t_monster_shootdamage[0][0]=8+t_monster_dex[0]*ch_level/14;
t_monster_shootdamage[0][1]=6+t_monster_int[0]*ch_level/25;

t_monster_mdamage[0][0]=2+t_monster_int[0]+lvlclass[2]*3/2;
t_monster_mdamage[0][1]=t_monster_int[0]/3+t_monster_wis[0]/2;

t_monster_regen[0][0]=32;
t_monster_regen[0][1]=t_monster_con[0]/5;
t_monster_regen[0][2]=44;
t_monster_regen[0][3]=t_monster_int[0]/10;
t_monster_regen[0][4]=t_monster_con[0]/10;
t_monster_armor[0]=t_monster_con[0]/10;
t_monster_resist[0]=t_monster_wis[0]/10;
switch (player_arm[2])
{
case 44:
t_monster_armor[0]+=20;
t_monster_resist[0]+=20;
break;//ring of protection

case 52:
t_monster_armor[0]+=25;
break;//leather
case 53:
t_monster_armor[0]+=42;
break;//bronze
case 54:
t_monster_armor[0]+=60;
break;//red

}

if (monster_mhp[0]>9999)monster_mhp[0]=9999;
if (t_monster_mn[0]>9999)t_monster_mn[0]=9999;
if (t_monster_mv[0]>2500)t_monster_mv[0]=2500;

if (monster_hp[0]>monster_mhp[0]) monster_hp[0]=monster_mhp[0];
if (monster_mn[0]>t_monster_mn[0]) monster_mn[0]=t_monster_mn[0];
if (monster_mv[0]>t_monster_mv[0]) monster_mv[0]=t_monster_mv[0];

t_monster_mv1[0]=t_monster_mv[0]/2;
t_monster_mv0[0]=t_monster_mv1[0]/2;

switch (t_monster_shoottype[0])
{
case 1:t_monster_shootdamage[0][0]+=10;break;       
case 2:t_monster_shootdamage[0][0]+=1500;break;       
case 3:t_monster_shootdamage[0][0]+=100;break;       
}

switch (player_arm[0])
{
case 32://knife
t_monster_damage[0][0]+=20;
break;
case 33://sword
t_monster_damage[0][0]+=50;
break;
case 34://katana
t_monster_damage[0][0]+=75;
break;
case 35://rod
t_monster_mdamage[0][0]+=50;
break;
case 36://hammer
t_monster_damage[0][0]+=60;
break;
case 37://glass
t_monster_damage[0][0]+=1000;
break;
case 38://scythe
t_monster_damage[0][0]+=500;
break;
case 39://chaingsaw
t_monster_damage[0][0]+=250;
break;

}

}
}

void NextLevel(void)
{
CurrentLevel++;
if (CurrentLevel>11)
{
GameMode=CLOSING_CREDITS;
fadecount=10;
count=0;
NewGameMode=CLOSING_CREDITS;
}
else
{
SavePlayerStatus();
Game_LoadNewLevel();
}
//GameLoopEnabled=0;    
     
}

void InitPlayerStatus(void)
{
Uint8 i;
for (i=0;i<64;i++)
 {
 item_p[i]=64;
 item_c[i]=0;
// item_p[i]=i;
// item_c[i]=i+1;
 }     
for (i=0;i<32;i++)
 {
 skill_p[i]=32;
 skill_l[i]=0;
 }     
for (i=0;i<12;i++)
weapon_points[i]=1000;     

AddItem(61,418);
AddItem(2,5);
AddItem(6,3);

AddSkill(0);
AddSkill(1);
AddSkill(30);


lvlclass[0]=0;
lvlclass[1]=0;
lvlclass[2]=0;
lvlclass[3]=0;
ch_class=0;
ch_level=0;
ch_exp=0;
CurrentLevel=0;
player_arm[0]=0;
player_arm[1]=0;
player_arm[2]=0;

t_monster_shoottype[0]=5;

t_monster_str[0]=6;
t_monster_dex[0]=10;
t_monster_con[0]=5;
t_monster_int[0]=12;
t_monster_wis[0]=4;
t_monster_cha[0]=2;

t_monster_action[0][0]=0;
t_monster_action[0][1]=1;
t_monster_action[0][2]=0;
t_monster_action[0][3]=1;
UpdatePlayer();
sprintf(t_monster_name[0],"Code 88");
iGameTime=0;
}
void LoadPlayerStatus(void)
{// LOADInG beGiNZ!
FILE *fp;Uint8 c;
fp=fopen(getSavecardPath(),"rb");

if (fp)
{
iGameTime=16777216*fgetc(fp)+65536*fgetc(fp)+256*fgetc(fp)+fgetc(fp);

CurrentLevel=fgetc(fp);
ch_level=fgetc(fp);
ch_class=fgetc(fp);
lvlclass[0]=fgetc(fp);
lvlclass[1]=fgetc(fp);
lvlclass[2]=fgetc(fp);
lvlclass[3]=fgetc(fp);
ch_exp=16777216*fgetc(fp)+65536*fgetc(fp)+256*fgetc(fp)+fgetc(fp);

player_arm[0]=fgetc(fp);
player_arm[1]=fgetc(fp);
player_arm[2]=fgetc(fp);


t_monster_str[0]=fgetc(fp);
t_monster_dex[0]=fgetc(fp);
t_monster_con[0]=fgetc(fp);
t_monster_int[0]=fgetc(fp);
t_monster_wis[0]=fgetc(fp);
t_monster_cha[0]=fgetc(fp);

WearItem(player_arm[0],1);
WearItem(player_arm[1],1);
WearItem(player_arm[2],1);

for (Uint8 i=0;i<64;i++)
{
item_c[i]=fgetc(fp)*256+fgetc(fp);
item_p[i]=fgetc(fp);
}
for (Uint8 i=0;i<32;i++)
{
skill_l[i]=fgetc(fp);
skill_p[i]=fgetc(fp);
}
for (Uint8 i=0;i<12;i++)
{
weapon_points[i]=256*fgetc(fp)+fgetc(fp);
}
for (Uint8 i=0;i<4;i++)
t_monster_action[0][i]=fgetc(fp);
for (Uint8 i=0;i<8;i++)
t_monster_name[0][i]=fgetc(fp);



fclose(fp);     
}

//LOADIng dies
}
Uint8 bngz0[76800];
void SavePlayerStatus(void)
{
//SetMusic(25);

Uint32 i;
ClrScr();
FILE *fp;
fp=fopen("data/load.bng","rb");
 for (i=0;i<76800;i++) bngz0[i]=fgetc(fp);
 fclose(fp);       

if (CradleMode)
{
 for (i=0;i<20*320;i++) bngz0[i]=16;
 for (i=220*320;i<240*320;i++) bngz0[i]=16;
}

for (i=0;i<76800;i++) scrbuf[i]=bngz0[i];

scrbuf_draw();
SDL_Delay(1000);
sPlaySound(28,50,50);

for (Uint32 y=150;y<210;y++)
for (Uint32 x=y%2;x<319;x+=2)
scrbuf[x+y*320]=16;
for (Uint32 x=0;x<320;x++)
{
scrbuf[x+150*320]=16;
scrbuf[x+210*320]=16;
}

l_textstring=sprintf(s_textstring,"Checking SD card in slot 1...");
s_drawtext2(21,161,19);
s_drawtext2(20,160,30);
scrbuf_draw();
SDL_Delay(2000);
ClrScr();
sPlaySound(28,58,68);
for (i=0;i<76800;i++) scrbuf[i]=bngz0[i];
for (Uint32 y=150;y<210;y++)
for (Uint32 x=y%2;x<319;x+=2)
scrbuf[x+y*320]=16;
for (Uint32 x=0;x<320;x++)
{
scrbuf[x+150*320]=16;
scrbuf[x+210*320]=16;
}

l_textstring=sprintf(s_textstring,"Card found. Saving Game...");
s_drawtext2(41,161,19);
s_drawtext2(40,160,30);
scrbuf_draw();
SDL_Delay(1000);
sPlaySound(28,128,128);
l_textstring=sprintf(s_textstring,"Note: Please do not eat your SD card!");
s_drawtext(41,185,19);
s_drawtext(40,184,28);
scrbuf_draw();
SDL_Delay(3000);
ClrScr();
sPlaySound(29,128,128);
for (i=0;i<76800;i++) scrbuf[i]=bngz0[i];
for (Uint32 y=150;y<210;y++)
for (Uint32 x=y%2;x<319;x+=2)
scrbuf[x+y*320]=16;
for (Uint32 x=0;x<320;x++)
{
scrbuf[x+150*320]=16;
scrbuf[x+210*320]=16;
}


//SAVI/ng already
Uint8 c;
fp=fopen(getSavecardPath(),"wb");
fputc(iGameTime/16777216%256,fp);
fputc(iGameTime/65536%256,fp);
fputc(iGameTime/256%256,fp);
fputc(iGameTime%256,fp);

fputc(CurrentLevel,fp);
fputc(ch_level,fp);
fputc(ch_class,fp);
fputc(lvlclass[0],fp);
fputc(lvlclass[1],fp);
fputc(lvlclass[2],fp);
fputc(lvlclass[3],fp);
fputc(ch_exp/16777216%256,fp);
fputc(ch_exp/65536%256,fp);
fputc(ch_exp/256%256,fp);
fputc(ch_exp%256,fp);

Uint8 c0=player_arm[0];
Uint8 c1=player_arm[1];
Uint8 c2=player_arm[2];

fputc(player_arm[0],fp);WearItem(player_arm[0],0);
fputc(player_arm[1],fp);WearItem(player_arm[1],0);
fputc(player_arm[2],fp);WearItem(player_arm[2],0);


fputc(t_monster_str[0],fp);
fputc(t_monster_dex[0],fp);
fputc(t_monster_con[0],fp);
fputc(t_monster_int[0],fp);
fputc(t_monster_wis[0],fp);
fputc(t_monster_cha[0],fp);

WearItem(c0,1);
WearItem(c1,1);
WearItem(c2,1);

for (Uint8 i=0;i<64;i++)
{
fputc(item_c[i]/256,fp);
fputc(item_c[i]%256,fp);
fputc(item_p[i],fp);
}
for (Uint8 i=0;i<32;i++)
{
fputc(skill_l[i],fp);
fputc(skill_p[i],fp);
}
for (Uint8 i=0;i<12;i++)
{
fputc(weapon_points[i]/256,fp);
fputc(weapon_points[i]%256,fp);
}
for (Uint8 i=0;i<4;i++)
fputc(t_monster_action[0][i],fp);

for (Uint8 i=0;i<8;i++)
fputc(t_monster_name[0][i],fp);

fclose(fp);     
//SAVI/ng already

l_textstring=sprintf(s_textstring,"Data Saved!");
s_drawtext2(111,161,19);
s_drawtext2(110,160,31);
scrbuf_draw();
SDL_Delay(2000);
ClrScr();
sPlaySound(20,128,128);
for (i=0;i<76800;i++) scrbuf[i]=bngz0[i];

scrbuf_draw();
SDL_Delay(1500);
     
//SetMusic(50);

}


void AddLevel(void)
{
if (ch_level==0)
GiveExp(exp_lvl[1]);
else
if (ch_level<99)
GiveExp(exp_lvl[ch_level]-exp_lvl[ch_level-1]+1);
}



///////

void LevelGain(void)
{

Uint8 i=ch_class;
Uint8 l=lvlclass[ch_class];
switch (i)
{
case 0://fighter
t_monster_str[0]+=1;
t_monster_con[0]+=1;     
if (l%5<3)t_monster_dex[0]+=1;     
if (l%5<2)t_monster_int[0]+=1;     
if (l%5<1)t_monster_wis[0]+=1;     
if (l%3)t_monster_cha[0]+=1;     
if (random(2)) t_monster_str[0]++;else t_monster_con[0]++;

if (l==10) AddSkill(28);
if (l==25) AddSkill(29);

break;       
case 1://thief
t_monster_dex[0]+=1;
t_monster_cha[0]+=1;     
if (l%5<2)t_monster_int[0]+=1;     
if (l%5<2)t_monster_con[0]+=1;     
if (l%3)t_monster_wis[0]+=1;     
if (random(2)) t_monster_dex[0]++;else t_monster_str[0]++;


if (l==6) AddSkill(23);
if (l==18) AddSkill(26);

if (l==15) AddSkill(4);
if (l==25) AddSkill(3);
if (l==35) AddSkill(5);

break;       
case 2://Mage
t_monster_int[0]+=1;
t_monster_wis[0]+=1;     
if (l%5<4)t_monster_cha[0]+=1;     
if (l%5==0)t_monster_con[0]+=1;     
if (l%2)t_monster_int[0]+=1;     
if (random(2)) t_monster_int[0]++;else t_monster_wis[0]++;

if (l==1) AddSkill(14);
if (l==6) AddSkill(17);
if (l==11) AddSkill(20);
if (l==16) AddSkill(15);
if (l==22) AddSkill(18);
if (l==30) AddSkill(21);

break;       
case 3://Acolyte
t_monster_cha[0]+=1;
t_monster_wis[0]+=1;     
if (l%5<4)t_monster_int[0]+=1;     
if (l%3==2)t_monster_wis[0]+=1;     

if (l%2==0)t_monster_con[0]+=1;     
if (l%3==1)t_monster_str[0]+=1;     
if (random(2)) t_monster_cha[0]++;else t_monster_wis[0]++;

if (l==1) AddSkill(11);
if (l==5) AddSkill(6);
if (l==13) AddSkill(7);
if (l==12) AddSkill(9);
if (l==15) AddSkill(10);

if (l==16) AddSkill(24);

if (l==18) AddSkill(26);

if (l==20) AddSkill(12);
if (l==25) AddSkill(13);
if (l==26) AddSkill(25);

if (l==30) AddSkill(8);
if (l==40) AddSkill(2);


break;       
       
       
}
UpdatePlayer();
     monster_mv[0]=t_monster_mv[0];

switch (ch_class)
{
case 0:
     monster_hp[0]=monster_mhp[0];
     break;
case 1:
     if (monster_hasted[0]<160)
     monster_hasted[0]=160;
     break;
case 2:
     monster_mn[0]=t_monster_mn[0];
     break;
case 3:
     if (random(2)) 
     {
     if (monster_magicarmor[0]<320)
     monster_magicarmor[0]=320;
     }
     else
     {
     if (monster_shielded[0]<320)
     monster_shielded[0]=320;
     }
     break;
}
     
}


void TryLevelUp(void)
{
if (ch_exp>=exp_lvl[ch_level])
{
sPlaySound(2,128,128);

ch_level++;                              
lvlclass[ch_class]++;

if (lvlclass[ch_class]%10==0)
{
sprintf(new_message,"You have been promoted to %s!",class_s[ch_class*10+lvlclass[ch_class]/10]);
NextMusicTrack(32);Music_Fade_Count=25;
}
else
sprintf(new_message,"You have gained a level of experience!");

NewMessage();
LevelGain();

indicator_x[indicator_i]=monster_x[0];
indicator_y[indicator_i]=monster_y[0];
indicator_z[indicator_i]=monster_z[0]+15000;
indicator_arg[indicator_i]=10000;
indicator_count[indicator_i]=1;
indicator_i=(indicator_i+1)%4;

}     
     
     
}
void GiveExp(unsigned int exppoints)
{
if (level_tag==0) 
 {
 ch_exp+=exppoints;
 exp_bonus+=exppoints;
 exp_count=0;
 if (ch_level<99)        
 TryLevelUp();             
 }     
}
void FullRevive(void)
{
monster_hp[0]=monster_mhp[0];
monster_mn[0]=t_monster_mn[level_tag];    
monster_mv[0]=t_monster_mv[level_tag];    
}
