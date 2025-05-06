#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

#include "Utils.hpp"

class MainMenu
{
    public:
        MainMenu(SDL_Renderer* renderer, TTF_Font* font);
        void handleEvent(SDL_Event& e);
        void render();

        bool isPlaySelected();
        bool isQuitSelected();

        void reset();
        void cleanup();

    private:
        SDL_Renderer* renderer;
        TTF_Font* font;
        Utils utils;

        bool playSelected;
        bool quitSelected;
};

#endif
