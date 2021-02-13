#include "controlpad.h"

#define AXIS_DEADZONE 7000

static const Sint8 angle_detection[9]=
{7, 0, 1,
 6,-1, 2,
 5, 4, 3};
int i_keyb[19];
#if defined(POCKETGO)
static const SDLKey sd_keyb[19]={SDLK_UP,SDLK_e,SDLK_RIGHT,SDLK_c,SDLK_DOWN,SDLK_z,SDLK_LEFT,SDLK_q,
SDLK_LCTRL,SDLK_LALT,SDLK_SPACE,SDLK_LSHIFT,SDLK_PAGEUP,SDLK_PAGEDOWN,SDLK_TAB,SDLK_BACKSPACE,SDLK_ESCAPE,SDLK_RETURN,SDLK_RCTRL
};
#else
static const SDLKey sd_keyb[19]={SDLK_w,SDLK_e,SDLK_d,SDLK_c,SDLK_x,SDLK_z,SDLK_a,SDLK_q,
SDLK_f,SDLK_g,SDLK_h,SDLK_t,SDLK_r,SDLK_y,SDLK_4,SDLK_5,SDLK_b,SDLK_n,SDLK_s
};
#endif
static const int sd_key_ref[11]=
{
PAD_A,PAD_B,PAD_X,PAD_Y,PAD_L1,PAD_R1,PAD_L2,PAD_R2,PAD_SELECT,PAD_START,PAD_Z 
      
};
void ControlPadUpdate(void)
{
     
 SDL_JoystickUpdate();
 controlpad.button[PAD_A]=SDL_JoystickGetButton(joy,VK_FA);   
 controlpad.button[PAD_X]=SDL_JoystickGetButton(joy,VK_FX);   
 controlpad.button[PAD_B]=SDL_JoystickGetButton(joy,VK_FB);   
 controlpad.button[PAD_Y]=SDL_JoystickGetButton(joy,VK_FY);   
 controlpad.button[PAD_Z]=SDL_JoystickGetButton(joy,VK_FZ);

 controlpad.button[PAD_L1]=SDL_JoystickGetButton(joy,VK_FL);   
 controlpad.button[PAD_R1]=SDL_JoystickGetButton(joy,VK_FR);   
 controlpad.button[PAD_L2]=SDL_JoystickGetButton(joy,VK_VOL_DOWN);   
 controlpad.button[PAD_R2]=SDL_JoystickGetButton(joy,VK_VOL_UP);   

 controlpad.button[PAD_START]=SDL_JoystickGetButton(joy,VK_START);
 controlpad.button[PAD_SELECT]=SDL_JoystickGetButton(joy,VK_SELECT);

 controlpad.angle=-1;
#ifdef PC
Sint16 axis=SDL_JoystickGetAxis(joy,0);
Uint8 x=1;
if (axis<-AXIS_DEADZONE) x=0;
else
if (axis>AXIS_DEADZONE) x=2;
axis=SDL_JoystickGetAxis(joy,1);
Uint8 y=1;
if (axis<-AXIS_DEADZONE) y=0;
else
if (axis>AXIS_DEADZONE) y=2;

 controlpad.angle=angle_detection[x+y*3];
//Keyboard 
		SDL_Event event;

		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
                     GameLoopEnabled=0;
                     break;
                case SDL_KEYDOWN:
                     for (int i=0;i<19;i++)
					 if (event.key.keysym.sym==sd_keyb[i])
                     i_keyb[i]=1;
					break;
                case SDL_KEYUP:
                     for (int i=0;i<19;i++)
					 if (event.key.keysym.sym==sd_keyb[i])
                     i_keyb[i]=0;
					break;
				
            }
        }
for (int i=0;i<8;i++)
 if (i_keyb[i]) controlpad.angle=i;

for (int i=8;i<19;i++)
 if (i_keyb[i]) controlpad.button[i-8]=1;

  
#endif    
#ifdef GP2X
if (SDL_JoystickGetButton(joy,VK_UP)) controlpad.angle=0;
else
if (SDL_JoystickGetButton(joy,VK_UP_RIGHT)) controlpad.angle=1;
else
if (SDL_JoystickGetButton(joy,VK_RIGHT)) controlpad.angle=2;
else
if (SDL_JoystickGetButton(joy,VK_DOWN_RIGHT)) controlpad.angle=3;
else
if (SDL_JoystickGetButton(joy,VK_DOWN)) controlpad.angle=4;
else
if (SDL_JoystickGetButton(joy,VK_DOWN_LEFT)) controlpad.angle=5;
else
if (SDL_JoystickGetButton(joy,VK_LEFT)) controlpad.angle=6;
else
if (SDL_JoystickGetButton(joy,VK_UP_LEFT)) controlpad.angle=7;

if (CradleMode)
{
Sint16 axis=SDL_JoystickGetAxis(USBjoy,0);
Uint8 x=1;
if (axis<-AXIS_DEADZONE) x=0;
else
if (axis>AXIS_DEADZONE) x=2;
axis=SDL_JoystickGetAxis(USBjoy,1);
Uint8 y=1;
if (axis<-AXIS_DEADZONE) y=0;
else
if (axis>AXIS_DEADZONE) y=2;

 controlpad.angle=angle_detection[x+y*3];


 controlpad.button[PAD_A]=SDL_JoystickGetButton(USBjoy,USBVK_FA);   
 controlpad.button[PAD_X]=SDL_JoystickGetButton(USBjoy,USBVK_FX);   
 controlpad.button[PAD_B]=SDL_JoystickGetButton(USBjoy,USBVK_FB);   
 controlpad.button[PAD_Y]=SDL_JoystickGetButton(USBjoy,USBVK_FY);   


 controlpad.button[PAD_L1]=SDL_JoystickGetButton(USBjoy,USBVK_VOL_DOWN);   
 controlpad.button[PAD_R1]=SDL_JoystickGetButton(USBjoy,USBVK_VOL_UP);   
 controlpad.button[PAD_Z]=0;
 if (controlpad.button[PAD_L1]) if (controlpad.button[PAD_R1])  controlpad.button[PAD_Z]=1;

 controlpad.button[PAD_START]=SDL_JoystickGetButton(USBjoy,USBVK_START);
 controlpad.button[PAD_SELECT]=SDL_JoystickGetButton(USBjoy,USBVK_SELECT);


}

#endif
}
