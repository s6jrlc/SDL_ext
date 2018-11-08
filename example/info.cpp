//
// SDL_ext : info.cpp
//

#include"SDL_ext.h"
#include<SDL2/SDL_ttf.h>
#include<iomanip>
#include<iostream>
#include<sstream>
#include<vector>

namespace {
	char* path(nullptr);
  TTF_Font* font(nullptr);
  
  void idle() {
    SDL_EDA_UpdateWindow();
  }
  void rendering() {
    std::vector<std::stringstream> ss(4);
    SDL_Rect dst{};
    SDL_Surface* sfc(nullptr);
    SDL_Texture* txt(nullptr);
    SDL_Renderer* renderer(SDL_EDA_GetRenderer());
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

		ss.at(0) << "SDL EDA System @ " << SDL_GetPlatform() << ':';
		ss.at(0) << std::setw(3) << std::right << SDL_EDA_GetFrameRate() << "FPS";
		ss.at(1) << " Ticks: " << std::setw(10) << std::right << SDL_GetTicks();
    ss.at(2) << " Keyboard-Focusing window ";
    ss.at(2) << '(' << std::setw(14) << std::right << SDL_GetKeyboardFocus() << ')';
		ss.at(3) << " Mouse-Focusing window ";
		ss.at(3) << '(' << std::setw(14) << std::right << SDL_GetMouseFocus() << ')';

    for (auto& s : ss) {
      sfc = TTF_RenderUTF8_Blended(font, s.str().c_str(), {152, 174, 102, 255});
      txt = SDL_CreateTextureFromSurface(renderer, sfc);
      dst.w = sfc->w; dst.h = sfc->h;
      SDL_RenderCopy(renderer, txt, nullptr, &dst);
      SDL_DestroyTexture(txt);
      SDL_FreeSurface(sfc);
      dst.y += dst.h;
    }
  }
	Uint8 quitRequested()
	{
		const SDL_MessageBoxButtonData buttons[] = {
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Close" },
			{                                     0, 1, "Cancel" },
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
			"Quit Application?",
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
  void atExit() {
    TTF_CloseFont(font);
    SDL_free(path);
    TTF_Quit();
  }
  void init() {
    TTF_Init();
		path = SDL_GetBasePath();
    font = TTF_OpenFont("ipag.ttf", 32);
    if (!font) {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "font load error", TTF_GetError(), nullptr);
      SDL_EDA_AvoidLoop();
      return;
    }
    TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    
    for (int i=0; i<SDL_GetNumVideoDisplays(); ++i) {
      float hdpi, vdpi, ddpi;
      SDL_GetDisplayDPI(i, &ddpi, &hdpi, &vdpi);
      
      std::cout << "Display No. " << i << std::endl;
      std::cout << "\tName: " << SDL_GetDisplayName(i) << std::endl;
      std::cout << "\tDPI: h " << hdpi << ", v " << vdpi << ", d " << ddpi << std::endl;
      std::cout << std::endl;
    }
  }
}

int main(int ac, char* av[]) {
  SDL_EDA_Init(&ac, av);
  
  SDL_EDA_WindowPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_EDA_WindowSize(720, 540);
  SDL_EDA_WindowState(SDL_EXT_WINDOW_HIGHDPI);
  SDL_EDA_CreateWindow("SDL Information");
  SDL_EDA_FrameRate(60);
  
  SDL_EDA_IdleFunc(idle);
  SDL_EDA_DisplayFunc(rendering);
	SDL_EDA_QuitRequestedFunc(quitRequested);
  SDL_EDA_ExitFunc(atExit);
  
  init();
  SDL_EDA_EnterLoop();
  
  return 0;
}
