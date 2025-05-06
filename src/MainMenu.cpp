#include "MainMenu.hpp"

MainMenu::MainMenu(SDL_Renderer* renderer, TTF_Font* font)
: renderer(renderer), font(font), playSelected(false), quitSelected(false)
{}

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
    SDL_RenderClear(renderer);

    utils.loadBackground(renderer, "image/menubackground.png");
    utils.showBackground = true;
    utils.renderBackground(renderer);

    utils.renderText(renderer, font, "1. Chơi game", 300, 250);
    utils.renderText(renderer, font, "2. Thoát game", 300, 310);
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




