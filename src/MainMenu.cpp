#include "MainMenu.hpp"

MainMenu::MainMenu(SDL_Renderer* renderer, TTF_Font* font)
: renderer(renderer), font(font), playSelected(false), quitSelected(false), utils(utils)
{
    utils.loadBackground(renderer, "image/menuBackground");
    utils.showBackground = true;
}

void MainMenu::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_1)
        {
            playSelected = true;
        }
        else if (e.key.keysym.sym == SDLK_2)
        {
            quitSelected = true;
        }
    }
}

void MainMenu::render()
{
    utils.renderBackground(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    utils.renderText(renderer, font, "1. Choi game", 412, 300);
    utils.renderText(renderer, font, "2. Thoat game", 412, 360);
}

bool MainMenu::isPlaySelected()
{
    return playSelected;
}

bool MainMenu::isQuitSelected()
{
    return quitSelected;
}

void MainMenu::reset()
{
    playSelected = false;
    quitSelected = false;
}

void MainMenu::cleanup()
{
    utils.cleanup();
}




