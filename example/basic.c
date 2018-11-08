//
// SDL_ext : basic.c
//

#include"SDL_ext.h"

static void idle(void) {
  SDL_EDA_UpdateWindow();
}

static void rendering(void) {
  SDL_Renderer* renderer = SDL_EDA_GetRenderer();
  if (renderer) {
    int x, y, w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    SDL_GetMouseState(&x, &y); x *= 2; y *= 2;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 50, 204, 18, 192);
    SDL_RenderDrawLine(renderer, 0, 0, x, y);
    SDL_RenderDrawLine(renderer, w-1, 0, x, y);
    SDL_RenderDrawLine(renderer, w-1, h-1, x, y);
    SDL_RenderDrawLine(renderer, 0, h-1, x, y);
  }
}

static void keyDown(int key) {
  printf("%s: %c\n", __FUNCTION__, key);
}
static void keyUp(int key) {
  printf("%s: %c\n", __FUNCTION__, key);
}

static void joyCxn(int id) {
	SDL_Joystick* joystick = SDL_JoystickFromInstanceID(id);
  printf("joystick added: %d, %p / now %d\n", id, joystick, SDL_NumJoysticks());
	printf("name: %s\n", SDL_JoystickName(joystick));
	printf("type: %d\n", SDL_JoystickGetType(joystick));
	printf("axes' number: %d\n", SDL_JoystickNumAxes(joystick));
	printf("button's number: %d\n", SDL_JoystickNumButtons(joystick));
	printf("balls's number: %d\n", SDL_JoystickNumBalls(joystick));
	printf("hat's number: %d\n", SDL_JoystickNumHats(joystick));
}
static void joyDiscxn(int id) {
	printf("joystick removed: %d / now %d\n", id, SDL_NumJoysticks());
}

static void joyAxis(int id, Uint8 axis, Sint16 value)
{
	printf("%s: %d, %d\n", __FUNCTION__, axis, value);
}
static void joyButtonDown(int id, Uint8 button)
{
  printf("%s: %d\n", __FUNCTION__, button);
}
static void joyButtonUp(int id, Uint8 button)
{
  printf("%s: %d\n", __FUNCTION__, button);
}
static void joyHat(int id, Uint8 hat, Uint8 value)
{
	printf("%s: %d, %d, ", __FUNCTION__, id, hat);
	if (SDL_HAT_UP & value) {
		printf("hat_up ");
	}
	if (SDL_HAT_DOWN & value) {
		printf("hat_down ");
	}
	if (SDL_HAT_LEFT & value) {
		printf("hat_left ");
	}
	if (SDL_HAT_RIGHT & value) {
		printf("hat_right ");
	}
	printf("\n");
}

static void activeMotion(int x, int y) {
  printf("%s: (%d\t%d)\n", __FUNCTION__, x, y);
}
static void passiveMotion(int x, int y) {
  printf("%s: (%d\t%d)\n", __FUNCTION__, x, y);
}

static void resize(int w, int h) {
  printf("%s: (%d\t%d)\n", __FUNCTION__, w, h);
}
static void cursor() {
  printf("%s\n", __FUNCTION__);
}

static Uint8 closeRequested(void)
{
  const SDL_MessageBoxButtonData buttons[] = {
    { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Close" },
    {                                       0, 1, "Cancel" },
  };
  const SDL_MessageBoxColorScheme colorScheme = {
    {
      { 255,   0,   0 },
      {   0, 255,   0 },
      { 255, 255,   0 },
      {   0,   0, 255 },
      { 255,   0, 255 },
    }
  };
  const SDL_MessageBoxData messageboxdata = {
    SDL_MESSAGEBOX_INFORMATION,
    NULL,
    "Close window",
    "Select button",
    SDL_arraysize(buttons),
    buttons,
    &colorScheme
  };
  int buttonid = -1;
  if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
    SDL_Log("error to show message box");
  }
	if (buttonid == 0) return SDL_TRUE;
  return SDL_FALSE;
}

static void atExit(void) {
  printf("%s\n", __FUNCTION__);
}

static void init(void) {
  printf("\n%s\n", __FUNCTION__);
}

int main(int ac, char* av[]) {
  SDL_EDA_Init(&ac, av);
  
  SDL_EDA_WindowPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_EDA_WindowSize(720, 720);
  SDL_EDA_WindowState(SDL_EXT_WINDOW_HIGHDPI);
  SDL_EDA_CreateWindow(u8"SDL EDA system");
  SDL_EDA_FrameRate(60);

  SDL_EDA_IdleFunc(idle);
  SDL_EDA_DisplayFunc(rendering);
  SDL_EDA_KeyDownFunc(keyDown);
  SDL_EDA_KeyUpFunc(keyUp);
	SDL_EDA_JoyButtonDownFunc(joyButtonDown);
	SDL_EDA_JoyButtonUpFunc(joyButtonUp);
	SDL_EDA_JoyAxisFunc(joyAxis);
	SDL_EDA_JoyHatFunc(joyHat);
	SDL_EDA_JoyAddedFunc(joyCxn);
	SDL_EDA_JoyRemovedFunc(joyDiscxn);
  SDL_EDA_MouseActiveMotionFunc(activeMotion);
  SDL_EDA_MousePassiveMotionFunc(passiveMotion);
  SDL_EDA_ResizeFunc(resize);
  SDL_EDA_CursorEnterFunc(cursor);
  SDL_EDA_CursorLeaveFunc(cursor);
	SDL_EDA_CloseFunc(closeRequested);
  SDL_EDA_ExitFunc(atExit);

	SDL_EDA_WindowSize(120, 120);
	SDL_EDA_WindowPosition(0, 0);
	SDL_EDA_CreateWindow(u8"SDL EDA システム");
  SDL_EDA_DisplayFunc(rendering);
	SDL_EDA_CloseFunc(closeRequested);

  SDL_EDA_WindowPosition(120, 0);
	SDL_EDA_CreateWindow("system");
  SDL_EDA_DisplayFunc(rendering);
	SDL_EDA_CloseFunc(closeRequested);
  init();
  SDL_EDA_EnterLoop();
  
  return 0;
}
