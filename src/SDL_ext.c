//
//  SDL_ext.c
//

#include"SDL_ext.h"

/* definition of key strings */
#define SDL_EDA_KEY_PREV "prev"
#define SDL_EDA_KEY_NEXT "next"

#define SDL_EDA_KEY_GLCONTEXT "context"

#define SDL_EDA_KEY_FUNC_MOVED "moved function"
#define SDL_EDA_KEY_FUNC_RESIZE "resize function"
#define SDL_EDA_KEY_FUNC_SIZE_CHANGED "size-changed function"
#define SDL_EDA_KEY_FUNC_CURSOR_ENTER "cursor-enter function"
#define SDL_EDA_KEY_FUNC_CURSOR_LEAVE "cursor-leave function"
#define SDL_EDA_KEY_FUNC_MOUSE_BUTTON "mouse-button function"
#define SDL_EDA_KEY_FUNC_ACTIVE_MOTION "active-motion function"
#define SDL_EDA_KEY_FUNC_PASSIVE_MOTION "passive-motion function"
#define SDL_EDA_KEY_FUNC_DROP "drop function"
#define SDL_EDA_KEY_FUNC_CLOSE "close function"
#define SDL_EDA_KEY_FUNC_DISPLAY "display function"

/* type definition of functions */
typedef Uint8 (*SDL_EdaFuncEventRequested)(void);
typedef void (*SDL_EdaFuncAtTimes)(void);
typedef void (*SDL_EdaFuncDisplay)(void);
typedef void (*SDL_EdaFuncJoyCxn)(Sint32);
typedef void (*SDL_EdaFuncJoyAxis)(Sint32, Uint8, Sint16);
typedef void (*SDL_EdaFuncJoyButton)(Sint32, Uint8);
typedef void (*SDL_EdaFuncJoyhat)(Sint32, Uint8, Uint8);
typedef void (*SDL_EdaFuncKeyboard)(Sint32);
typedef void (*SDL_EdaFuncMouseButton)(Uint8, Uint8, Sint32, Sint32);
typedef void (*SDL_EdaFuncMouseMotion)(Sint32, Sint32);
typedef void (*SDL_EdaFuncWindowWithData)(Sint32, Sint32);
typedef void (*SDL_EdaFuncWindow)(void);
typedef void (*SDL_EdaFuncDrop)(char*);

/* definition of static variables */
#define COUNT_PER_SPEED 1000
#define DEFAULT_FRAMERATE 60
#define DEFAULT_COEF_COUNT 3000

static Uint32 coef_count_ = DEFAULT_COEF_COUNT;
static Uint32 actual_framerate_ = DEFAULT_FRAMERATE;
static Uint32 coef_ticks_ = DEFAULT_COEF_COUNT / COUNT_PER_SPEED;
static Uint32 wait_ticks_ = DEFAULT_COEF_COUNT / DEFAULT_FRAMERATE;

static int x_ = SDL_WINDOWPOS_CENTERED, y_ = SDL_WINDOWPOS_CENTERED;
static int w_ = 480, h_ = 480;
static Uint32 window_flag_ = SDL_WINDOW_SHOWN;

static Uint32 n_windows_ = 0;

static Uint8 system_active_ = SDL_TRUE;

static SDL_EdaFuncEventRequested func_quit_ = NULL;
static SDL_EdaFuncAtTimes func_at_exit_ = NULL;
static SDL_EdaFuncAtTimes func_idle_ = NULL;
static SDL_EdaFuncJoyCxn func_joy_added_ = NULL;
static SDL_EdaFuncJoyCxn func_joy_removed_ = NULL;
static SDL_EdaFuncJoyAxis func_joy_axis_ = NULL;
static SDL_EdaFuncJoyButton func_joy_button_down_ = NULL;
static SDL_EdaFuncJoyButton func_joy_button_up_ = NULL;
static SDL_EdaFuncJoyhat func_joy_hat_ = NULL;
static SDL_EdaFuncKeyboard func_keydown_ = NULL;
static SDL_EdaFuncKeyboard func_keyup_ = NULL;

static SDL_Window* window_head_ = NULL;
static SDL_Renderer* renderer_ = NULL;

/*
 definition of SDL Extension functions
 */

Uint8 SDL_EXT_KeyIsPressed(SDL_Keycode key)
{
	return SDL_GetKeyboardState(NULL)[SDL_GetScancodeFromKey(key)];
}

SDL_Joystick* SDL_EXT_JoystickFromDeviceIndex(int device_index)
{
	return SDL_JoystickFromInstanceID(SDL_JoystickGetDeviceInstanceID(device_index));
}

/*
 definition of SDL EDA functions
 */

void SDL_EDA_Init(int* acp, char** av)
{
	Uint32 init_flags = SDL_INIT_EVERYTHING;
	for (--(*acp); (*acp) > 0; --(*acp))
	{
		if (SDL_strcmp(av[*acp], "--no-audio") == 0)
		{
			init_flags &= ~(SDL_INIT_AUDIO);
		}
		else if (SDL_strcmp(av[*acp], "--no-haptic") == 0)
		{
			init_flags &= ~(SDL_INIT_HAPTIC);
		}
		//else if (SDL_strcmp(av[*acp], "--no-joystick") == 0)
		//{
		//	flags &= ~(SDL_INIT_JOYSTICK);
		//}
		//else if (SDL_strcmp(av[*acp], "--no-gamecontroller") == 0)
		//{
		//	flags &= ~(SDL_INIT_GAMECONTROLLER);
		//}
		//else if (SDL_strcmp(av[*acp], "--no-timer") == 0)
		//{
		//	flags &= ~(SDL_INIT_TIMER);
		//}
	}
	if (SDL_Init(init_flags))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, __FUNCTION__, SDL_GetError(), NULL);
		system_active_ = SDL_FALSE;
		return;
	}
}
void SDL_EDA_ExitLoop(void)
{
	system_active_ = SDL_FALSE;
}

void SDL_EDA_FrameRate(Uint32 framerate)
{
	if (framerate != 0 && framerate <= 1000) {
		Uint32 m = COUNT_PER_SPEED, n = framerate, k = 0;
		while (n) {
			k = m % n;
			m = n;
			n = k;
		}
		coef_count_ = COUNT_PER_SPEED * framerate / m;
		coef_ticks_ = coef_count_ / COUNT_PER_SPEED;
		wait_ticks_ = coef_count_ / framerate;
	}
}
Uint32 SDL_EDA_GetFrameRate(void)
{
	return actual_framerate_;
}

void SDL_EDA_UpdateWindow(void)
{
	SDL_Window* w;
	for (w = window_head_; w; w = SDL_GetWindowData(w, SDL_EDA_KEY_NEXT))
	{
		SDL_Renderer* renderer = SDL_GetRenderer(w);
		SDL_GLContext ctx = SDL_GetWindowData(w, SDL_EDA_KEY_GLCONTEXT);
		
		if (renderer)
		{
			SDL_RenderPresent(renderer);
		}
		if (ctx)
		{
			SDL_GL_MakeCurrent(w, ctx);
			SDL_GL_SwapWindow(w);
		}
	}
}

void SDL_EDA_WindowPosition(int x, int y)
{
	x_ = x; y_ = y;
}
void SDL_EDA_WindowSize(int w, int h)
{
	w_ = w; h_ = h;
}
void SDL_EDA_WindowState(Uint32 flag)
{
	window_flag_ = flag;
}

Uint32 SDL_EDA_CreateWindow(const char* title)
{
	SDL_Window* new_window = SDL_CreateWindow(title, x_, y_, w_, h_, window_flag_);
	if (window_flag_ & SDL_WINDOW_OPENGL)
	{
		SDL_GLContext ctx = SDL_GL_CreateContext(new_window);
		SDL_SetWindowData(new_window, SDL_EDA_KEY_GLCONTEXT, ctx);
		SDL_GL_MakeCurrent(new_window, ctx);
	}
	else if (!(window_flag_ & SDL_WINDOW_VULKAN))
	{
		SDL_CreateRenderer(new_window, -1, SDL_RENDERER_PRESENTVSYNC);
	}
	
	if (window_head_) // push front
	{
		SDL_Window* w = window_head_;
		SDL_SetWindowData(w, SDL_EDA_KEY_PREV, new_window);
		SDL_SetWindowData(new_window, SDL_EDA_KEY_NEXT, w);
		window_head_ = new_window;
	}
	else
	{
		window_head_ = new_window;
	}
	
	return ++n_windows_;
}

static SDL_INLINE void SDL_edaDestroyWindow(SDL_Window* window) // change non-static function?
{
	if (window)
	{
		SDL_Renderer* renderer = SDL_GetRenderer(window);
		SDL_GLContext ctx = SDL_GetWindowData(window, SDL_EDA_KEY_GLCONTEXT);
		
		if (renderer)
		{
			SDL_DestroyRenderer(renderer);
			renderer = 0;
		}
		if (ctx)
		{
			SDL_GL_DeleteContext(ctx);
			ctx = 0;
		}
		
		if (window == window_head_) // arg is head window
		{
			window_head_ = SDL_GetWindowData(window_head_, SDL_EDA_KEY_NEXT);
			if (window_head_)
			{
				SDL_SetWindowData(window_head_, SDL_EDA_KEY_PREV, NULL);
			}
		}
		else if (!SDL_GetWindowData(window, SDL_EDA_KEY_NEXT)) // arg is tail window
		{
			SDL_Window* w = SDL_GetWindowData(window, SDL_EDA_KEY_PREV);
			if (w)
			{
				SDL_SetWindowData(w, SDL_EDA_KEY_NEXT, NULL);
			}
		}
		else
		{
			SDL_Window* w_prev = SDL_GetWindowData(window, SDL_EDA_KEY_PREV);
			SDL_Window* w_next = SDL_GetWindowData(window, SDL_EDA_KEY_NEXT);
			SDL_SetWindowData(w_prev, SDL_EDA_KEY_NEXT, w_next);
			SDL_SetWindowData(w_next, SDL_EDA_KEY_PREV, w_prev);
		}
		
		SDL_DestroyWindow(window);
		--n_windows_;
	}
}

SDL_FORCE_INLINE Uint8 SDL_edaCallWCFunc(SDL_WindowEvent* ev_window, const char* str)
{
	SDL_EdaFuncEventRequested f = SDL_GetWindowData(SDL_GetWindowFromID(ev_window->windowID), str);
	if (f) return f();
	return SDL_TRUE;
}
SDL_FORCE_INLINE void SDL_edaCallWFunc(SDL_WindowEvent* ev_window, const char* str)
{
	SDL_EdaFuncWindow f = SDL_GetWindowData(SDL_GetWindowFromID(ev_window->windowID), str);
	if (f) f();
}
SDL_FORCE_INLINE void SDL_edaCallWDFunc(SDL_WindowEvent* ev_window, const char* str)
{
	SDL_EdaFuncWindowWithData f = SDL_GetWindowData(SDL_GetWindowFromID(ev_window->windowID), str);
	if (f) f(ev_window->data1, ev_window->data2);
}

static SDL_INLINE void SDL_edaProcessWindowsEvents(SDL_WindowEvent* ev_window)
{
	switch (ev_window->event)
	{
		case SDL_WINDOWEVENT_CLOSE:
			if (SDL_edaCallWCFunc(ev_window, SDL_EDA_KEY_FUNC_CLOSE))
			{
        SDL_FlushEvent(SDL_QUIT);
        if (n_windows_ > 1 || !func_quit_ || func_quit_())
        {
          SDL_edaDestroyWindow(SDL_GetWindowFromID(ev_window->windowID));
        }
        if (n_windows_ == 0) system_active_ = SDL_FALSE;
			}
			break;
		case SDL_WINDOWEVENT_MOVED:
			SDL_edaCallWDFunc(ev_window, SDL_EDA_KEY_FUNC_MOVED);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			SDL_edaCallWDFunc(ev_window, SDL_EDA_KEY_FUNC_RESIZE);
			break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDL_edaCallWDFunc(ev_window, SDL_EDA_KEY_FUNC_SIZE_CHANGED);
			break;
		case SDL_WINDOWEVENT_ENTER:
			SDL_edaCallWFunc(ev_window, SDL_EDA_KEY_FUNC_CURSOR_ENTER);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			SDL_edaCallWFunc(ev_window, SDL_EDA_KEY_FUNC_CURSOR_LEAVE);
			break;
			
		default:
			break;
	}
}

SDL_FORCE_INLINE void SDL_edaCallDropFunc(const SDL_DropEvent* ev_drop)
{
	char* str = ev_drop->file;
	SDL_Window* w = SDL_GetWindowFromID(ev_drop->windowID);
	if (w)
	{
		SDL_EdaFuncDrop f = SDL_GetWindowData(w, SDL_EDA_KEY_FUNC_DROP);
		if (f) f(str);
	}
	SDL_free(str);
}

SDL_FORCE_INLINE void SDL_edaCallJoyCxnFunc(const SDL_JoyDeviceEvent* ev_jdev)
{
	SDL_Joystick* joystick = SDL_JoystickOpen(ev_jdev->which);
	if (func_joy_added_) func_joy_added_(SDL_JoystickInstanceID(joystick));
}
SDL_FORCE_INLINE void SDL_edaCallJoyDiscxnFunc(const SDL_JoyDeviceEvent* ev_jdev)
{
	if (func_joy_removed_) func_joy_removed_(ev_jdev->which);
	SDL_JoystickClose(SDL_JoystickFromInstanceID(ev_jdev->which));
}

SDL_FORCE_INLINE void SDL_edaCallMBFunc(const SDL_Event* ev)
{
	SDL_Window* w = SDL_GetWindowFromID(ev->window.windowID);
	SDL_EdaFuncMouseButton f = SDL_GetWindowData(w, SDL_EDA_KEY_FUNC_MOUSE_BUTTON);
	if (f) f(ev->button.button, ev->button.state, ev->button.x, ev->button.y);
}
SDL_FORCE_INLINE void SDL_edaCallMMFunc(const SDL_Event* ev)
{
	SDL_Window* w = SDL_GetWindowFromID(ev->window.windowID);
	SDL_EdaFuncMouseMotion af = SDL_GetWindowData(w, SDL_EDA_KEY_FUNC_ACTIVE_MOTION);
	SDL_EdaFuncMouseMotion pf = SDL_GetWindowData(w, SDL_EDA_KEY_FUNC_PASSIVE_MOTION);
	if (af && ev->motion.state)
	{
		af(ev->motion.x, ev->motion.y);
	}
	else if (pf && !ev->motion.state)
	{
		pf(ev->motion.x, ev->motion.y);
	}
}

static SDL_INLINE void SDL_edaProcessEvents(SDL_Event* ev)
{
	while (SDL_PollEvent(ev))
	{
		switch (ev->type)
		{
			case SDL_QUIT:
				if (n_windows_ > 0 && (!func_quit_ || func_quit_()))
				{
					SDL_Window* w;
					SDL_Window* w_next;
					for (w = window_head_; w; w = w_next)
					{
						SDL_EdaFuncEventRequested f = SDL_GetWindowData(w, SDL_EDA_KEY_FUNC_CLOSE);
						w_next = SDL_GetWindowData(w, SDL_EDA_KEY_NEXT);
						if (!f || f()) SDL_edaDestroyWindow(w);
					}
				}
				if (n_windows_ == 0) system_active_ = SDL_FALSE;
				break;
			case SDL_KEYDOWN:
				if (func_keydown_) func_keydown_(ev->key.keysym.sym);
				break;
			case SDL_KEYUP:
				if (func_keyup_) func_keyup_(ev->key.keysym.sym);
				break;
			case SDL_JOYAXISMOTION:
				if (func_joy_axis_) func_joy_axis_(ev->jaxis.which, ev->jaxis.axis, ev->jaxis.value);
				break;
			case SDL_JOYBUTTONDOWN:
				if (func_joy_button_down_) func_joy_button_down_(ev->jbutton.which, ev->jbutton.button);
				break;
			case SDL_JOYBUTTONUP:
				if (func_joy_button_up_) func_joy_button_up_(ev->jbutton.which, ev->jbutton.button);
				break;
			case SDL_JOYHATMOTION:
				if (func_joy_hat_) func_joy_hat_(ev->jhat.which, ev->jhat.hat, ev->jhat.value);
				break;
			case SDL_JOYDEVICEADDED:
				SDL_edaCallJoyCxnFunc(&ev->jdevice);
				break;
			case SDL_JOYDEVICEREMOVED:
				SDL_edaCallJoyDiscxnFunc(&ev->jdevice);
				break;
			case SDL_DROPFILE:
			case SDL_DROPTEXT:
				SDL_edaCallDropFunc(&ev->drop);
				break;
			case SDL_WINDOWEVENT:
				SDL_edaProcessWindowsEvents(&ev->window);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				SDL_edaCallMBFunc(ev);
				break;
			case SDL_MOUSEMOTION:
				SDL_edaCallMMFunc(ev);
				break;
			default:
				break;
		}
	}
	
	if (func_idle_)
	{
		func_idle_();
	}
}

SDL_FORCE_INLINE void SDL_edaSetRenderer(SDL_Renderer* renderer)
{
	renderer_ = renderer;
}
SDL_Renderer* SDL_EDA_GetRenderer(void)
{
	return renderer_;
}

static SDL_INLINE void SDL_edaDisplay(void)
{
	SDL_Window* w;
	for (w = window_head_; w; w = SDL_GetWindowData(w, SDL_EDA_KEY_NEXT))
	{
		SDL_EdaFuncDisplay func = SDL_GetWindowData(w, SDL_EDA_KEY_FUNC_DISPLAY);
		if (func)
		{
			SDL_Renderer* renderer = SDL_GetRenderer(w);
			SDL_GLContext ctx = SDL_GetWindowData(w, SDL_EDA_KEY_GLCONTEXT);
			if (renderer)
			{
				SDL_edaSetRenderer(renderer);
				func();
				SDL_edaSetRenderer(NULL);
			}
			if (ctx)
			{
				SDL_GL_MakeCurrent(w, ctx);
				func();
			}
			if (SDL_GetWindowFlags(w) & SDL_WINDOW_VULKAN) {
				func();
			}
		}
	}
}

static SDL_INLINE void SDL_edaQuit(void)
{
	int i;
	while (window_head_ != NULL)
	{
		SDL_edaDestroyWindow(window_head_);
	}
	
	for (i=0; i<SDL_NumJoysticks(); ++i)
	{
		SDL_JoystickClose(SDL_EXT_JoystickFromDeviceIndex(i));
	}
	
	if (func_at_exit_)
	{
		func_at_exit_();
	}
	SDL_Quit();
}

SDL_FORCE_INLINE Uint32 SDL_edaGetTicks(void)
{
	return SDL_GetTicks() * coef_ticks_;
}

void SDL_EDA_EnterLoop(void)
{
	SDL_Event ev;
	Uint32 now_ticks = 0, frame_ticks = 0;
	
	if (system_active_)
	{
		now_ticks = SDL_edaGetTicks();
	}
	
	while (system_active_)
	{
		SDL_edaProcessEvents(&ev);
		SDL_edaDisplay();
		
		frame_ticks = wait_ticks_ - (SDL_edaGetTicks() - now_ticks);
		if ((Sint32)frame_ticks > 0) SDL_Delay(frame_ticks / coef_ticks_);
		actual_framerate_ = coef_count_ / (SDL_edaGetTicks() - now_ticks);
		now_ticks = SDL_edaGetTicks();
	}
	
	SDL_edaQuit();
}

void SDL_EDA_QuitRequestedFunc(SDL_EdaFuncEventRequested f)
{
	func_quit_ = f;
}
void SDL_EDA_ExitFunc(SDL_EdaFuncAtTimes f)
{
	func_at_exit_ = f;
}
void SDL_EDA_IdleFunc(SDL_EdaFuncAtTimes f)
{
	func_idle_ = f;
}
void SDL_EDA_KeyDownFunc(SDL_EdaFuncKeyboard f)
{
	func_keydown_ = f;
}
void SDL_EDA_KeyUpFunc(SDL_EdaFuncKeyboard f)
{
	func_keyup_ = f;
}
void SDL_EDA_JoyAddedFunc(SDL_EdaFuncJoyCxn f)
{
	func_joy_added_ = f;
}
void SDL_EDA_JoyRemovedFunc(SDL_EdaFuncJoyCxn f)
{
	func_joy_removed_ = f;
}
void SDL_EDA_JoyAxisFunc(SDL_EdaFuncJoyAxis f)
{
	func_joy_axis_ = f;
}
void SDL_EDA_JoyButtonDownFunc(SDL_EdaFuncJoyButton f)
{
	func_joy_button_down_ = f;
}
void SDL_EDA_JoyButtonUpFunc(SDL_EdaFuncJoyButton f)
{
	func_joy_button_up_ = f;
}
void SDL_EDA_JoyHatFunc(SDL_EdaFuncJoyhat f)
{
	func_joy_hat_ = f;
}
void SDL_EDA_MouseButtonFunc(SDL_EdaFuncMouseButton f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_MOUSE_BUTTON, f);
}
void SDL_EDA_MouseActiveMotionFunc(SDL_EdaFuncMouseMotion f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_ACTIVE_MOTION, f);
}
void SDL_EDA_MousePassiveMotionFunc(SDL_EdaFuncMouseMotion f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_PASSIVE_MOTION, f);
}
void SDL_EDA_DisplayFunc(SDL_EdaFuncDisplay f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_DISPLAY, f);
}
void SDL_EDA_ResizeFunc(SDL_EdaFuncWindowWithData f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_RESIZE, f);
}
void SDL_EDA_SizeChangedFunc(SDL_EdaFuncWindowWithData f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_SIZE_CHANGED, f);
}
void SDL_EDA_MovedFunc(SDL_EdaFuncWindowWithData f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_MOVED, f);
}
void SDL_EDA_CursorEnterFunc(SDL_EdaFuncWindow f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_CURSOR_ENTER, f);
}
void SDL_EDA_CursorLeaveFunc(SDL_EdaFuncWindow f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_CURSOR_LEAVE, f);
}
void SDL_EDA_DropFunc(SDL_EdaFuncDrop f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_DROP, f);
}
void SDL_EDA_CloseFunc(SDL_EdaFuncEventRequested f)
{
	SDL_SetWindowData(window_head_, SDL_EDA_KEY_FUNC_CLOSE, f);
}
