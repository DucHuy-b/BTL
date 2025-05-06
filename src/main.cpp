#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Utils.hpp"
#include "MainMenu.hpp"
#include "GamePhase.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "MainMenu.hpp"
#include "GamePhase.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
 {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("No name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("font/ARIAL.TTF", 24);

    bool quit = false;
    SDL_Event e;

    bool inMenu = true;
    bool inGame = false;

    MainMenu menu(renderer, font);
    GamePhase* game = nullptr;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            if (inMenu)
            {
                menu.handleEvent(e);
                if (menu.isPlaySelected())
                {
                    inMenu = false;
                    inGame = true;
                    menu.cleanup();
                    game = new GamePhase(renderer, font);
                }
                else if (menu.isQuitSelected())
                {
                    quit = true;
                }
            }
            else if (inGame)
            {
                game->handleEvent(e);
            }
        }

        if (inMenu)
        {
            menu.render();
        }
        else if (inGame)
        {
            game->update();
            game->render();

            if (game->isFinished())
            {
                delete game;
                game = nullptr;
                inGame = false;
                inMenu = true;
                menu.reset();
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (game) delete game;

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();

    return 0;
}

