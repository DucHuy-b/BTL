#ifndef GAMEPHASE_HPP
#define GAMEPHASE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <string>
#include <vector>

#include "Utils.hpp"

class GamePhase
{
    public:
        GamePhase(SDL_Renderer* renderer, TTF_Font* font);

        void handleEvent(SDL_Event& e);
        void update();
        void render();
        bool isFinished();

    private:
        SDL_Renderer* renderer;
        TTF_Font* font;
        Utils utils;

        int hits;
        int playerHits;

        std::string resultText;

        bool finished;

        bool inIntroText;
        bool inCombat;
        bool inEndText;

        SDL_Rect enemyRect;
        SDL_Rect menuRect;

        void startCombat();
        void handleCombatAction(int action);
        void endCombat(bool win);
};

#endif
