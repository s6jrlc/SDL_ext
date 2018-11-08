//
//  SDL_ext.h
//

#ifndef SDL_EDA_H
#define SDL_EDA_H

#include<SDL2/SDL.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	SDL_EXT_WINDOW_DEFAULT = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
	SDL_EXT_WINDOW_HIGHDPI = SDL_EXT_WINDOW_DEFAULT | SDL_WINDOW_ALLOW_HIGHDPI,
	SDL_EXT_WINDOW_BORDERLESS = SDL_EXT_WINDOW_DEFAULT | SDL_WINDOW_BORDERLESS,
	SDL_EXT_WINDOW_OPENGL = SDL_EXT_WINDOW_DEFAULT | SDL_WINDOW_OPENGL,
	SDL_EXT_WINDOW_VULKAN = SDL_EXT_WINDOW_DEFAULT | SDL_WINDOW_VULKAN,
} SDL_EXT_WindowFlags;

/**
 This function initializes SDL EDA system

 @param argcp the pointer of the number of command line arguments
 @param argv the pointer of command-line-arguments array
 */
extern DECLSPEC void SDLCALL SDL_EDA_Init(int* argcp, char** argv);

/**
 This function exits main loop
 */
extern DECLSPEC void SDLCALL SDL_EDA_ExitLoop(void);

/**
 This function avoids main loop for your intialization error
 */
extern DECLSPEC void SDLCALL SDL_EDA_AvoidLoop(void);

/**
 This function enters main loop of SDL EDA system
 */
extern DECLSPEC void SDLCALL SDL_EDA_EnterLoop(void);

/**
 This function changes framerate (maybe each window later)
 
 @param framerate framerate of window
 */
extern DECLSPEC void SDLCALL SDL_EDA_FrameRate(Uint32 framerate);

/**
 This function returns framerate

 @return framerate of system
 */
extern DECLSPEC Uint32 SDLCALL SDL_EDA_GetFrameRate(void);

/**
 This function changes flag of a new window

 @param window_flag window mode flag
 */
extern DECLSPEC void SDLCALL SDL_EDA_WindowState(Uint32 window_flag);


/**
 This function creates a window with OpenGL context

 @param title window title
 @return the number of windows
 */
extern DECLSPEC Uint32 SDLCALL SDL_EDA_CreateWindow(const char* title);

/**
 This function changes position of a new window

 @param x x-coordinate position
 @param y y-coordinate position
 */
extern DECLSPEC void SDLCALL SDL_EDA_WindowPosition(int x, int y);

/**
 This function changes size of a new window

 @param w window width
 @param h window height
 */
extern DECLSPEC void SDLCALL SDL_EDA_WindowSize(int w, int h);

/**
 This function updates windows
 */
extern DECLSPEC void SDLCALL SDL_EDA_UpdateWindow(void);


/**
 This function sets a function executed when requested quit event, returning 1 to quit application

 @param func quit-requested function
 */
extern DECLSPEC void SDLCALL SDL_EDA_QuitRequestedFunc(Uint8 (*func)(void));

/**
 This function sets function executed when exit

 @param func at-exit a function
 */
extern DECLSPEC void SDLCALL SDL_EDA_ExitFunc(void (*func)(void));

/**
 This function sets a function executed when process is idle
 
 @param func idle function
 */
extern DECLSPEC void SDLCALL SDL_EDA_IdleFunc(void (*func)(void));

/**
 This function sets a function executed when key is pressed

 @param func key-up function
 */
extern DECLSPEC void SDLCALL SDL_EDA_KeyDownFunc(void (*func)(Sint32));

/**
 This function sets a function executed when key is released

 @param func key-down function
 */
extern DECLSPEC void SDLCALL SDL_EDA_KeyUpFunc(void (*func)(Sint32));


/**
 This function sets a function executed when joystick is connected

 @param func function for joystick connection
 */
extern DECLSPEC void SDLCALL SDL_EDA_JoyAddedFunc(void (*func)(Sint32));

/**
 This function sets a function executed when joystick is disconnected
	 
 @param func function for joystick disconnection
 */
extern DECLSPEC void SDLCALL SDL_EDA_JoyRemovedFunc(void (*func)(Sint32));

/**
 This function sets a function executed when joystick-axis-state is changed
 
 @param func joystick-axis function
 */
extern DECLSPEC void SDLCALL SDL_EDA_JoyAxisFunc(void (*func)(Sint32, Uint8, Sint16));

/**
 This function sets a function executed when joystick-button is pressed

 @param func joystick-button-down function

 */
extern DECLSPEC void SDLCALL SDL_EDA_JoyButtonDownFunc(void (*func)(Sint32, Uint8));

/**
 This function sets a function executed when joystick-button is released

 @param func joystick-button-up function
 */
extern DECLSPEC void SDLCALL SDL_EDA_JoyButtonUpFunc(void (*func)(Sint32, Uint8));

/**
 This function sets a function executed when joystick-hat is changed

 @param func joystick-hat function
 */
extern DECLSPEC void SDLCALL SDL_EDA_JoyHatFunc(void (*func)(Sint32, Uint8, Uint8));
	
	
/**
 This function sets a function executed when mouse button state changes

 @param func mouse button function
 */
extern DECLSPEC void SDLCALL SDL_EDA_MouseButtonFunc(void (*func)(Uint8, Uint8, Sint32, Sint32));

/**
 This function sets a function executed when active mouse cursor moves

 @param func active mouse motion function
 */
extern DECLSPEC void SDLCALL SDL_EDA_MouseActiveMotionFunc(void (*func)(Sint32, Sint32));

/**
 This function sets a function executed when passive mouse cursor moves

 @param func passive mouse motion function
 */
extern DECLSPEC void SDLCALL SDL_EDA_MousePassiveMotionFunc(void (*func)(Sint32, Sint32));

/**
 This function sets a displaying function

 @param func displaying function
 */
extern DECLSPEC void SDLCALL SDL_EDA_DisplayFunc(void (*func)(void));

/**
 This function sets a function executed when window size is changed

 @param func window-size-changed function
 */
extern DECLSPEC void SDLCALL SDL_EDA_SizeChangedFunc(void (*func)(Sint32, Sint32));

/**
 This function sets a function executed when window is resized

 @param func window-resized function
 */
extern DECLSPEC void SDLCALL SDL_EDA_ResizeFunc(void (*func)(Sint32, Sint32));

/**
 This function sets a function executed when window is moved

 @param func window-moved function
 */
extern DECLSPEC void SDLCALL SDL_EDA_MovedFunc(void (*func)(Sint32, Sint32));

/**
 This function sets a function executed when mouse cursor strides across window border
 
 @param func mouse-cursor function
 */
extern DECLSPEC void SDLCALL SDL_EDA_CursorEnterFunc(void (*func)(void));

/**
 This function setsa  function executed when mouse cursor strides across window border
 
 @param func mouse-cursor function
 */

extern DECLSPEC void SDLCALL SDL_EDA_CursorLeaveFunc(void (*func)(void));

/**
 This function sets a function executed when some drops into window

 @param func drop function
 */
extern DECLSPEC void SDLCALL SDL_EDA_DropFunc(void (*func)(char*));

/**
 This function sets a function executed when window is closed, returning 1 to close window

 @param func window close function
 */
extern DECLSPEC void SDLCALL SDL_EDA_CloseFunc(Uint8 (*func)(void));


/**
 This function must be called in SDL-rendering function to change renderer condition
 
 @return pointer of renderer
 */
extern DECLSPEC SDL_Renderer* SDLCALL SDL_EDA_GetRenderer(void);


/**
 This function returns whether a specified key is pressed

 @param key keycode
 @return 1 if the key is pressed, otherwise 0
 */
extern DECLSPEC Uint8 SDLCALL SDL_EXT_KeyIsPressed(SDL_Keycode key);

/**
 This function returns the SDL_Joystick associated with device instance

 @param device_index device instance index
 @return SDL_Joystick associated with device instance
 */
extern DECLSPEC SDL_Joystick* SDL_EXT_JoystickFromDeviceIndex(int device_index);


#ifdef __cplusplus
}
#endif

#endif // SDL_EDA_H
