#include "GamePhase.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

GamePhase::GamePhase(SDL_Renderer* renderer, TTF_Font* font)
: renderer(renderer), font(font), inIntroText(true), inCombat(false), inEndText(false), hits(0), playerHits(0), finished(false)
{
   std::srand(std::time(0));
   utils.loadTextboxFromFile("text/Intro.txt");
   utils.showTextbox = true;

   enemyRect = {700, 200, 200, 300};
   menuRect = {50, 400, 300, 250};
}

void GamePhase::handleEvent(SDL_Event& e)
{
    if(inIntroText && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
    {
        if(utils.textboxFinished())
        {
            utils.resetTextbox();
            utils.showTextbox = false;
            startCombat();
        }
    }
    else if(inCombat && e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
            case SDLK_1: handleCombatAction(1); break;
            case SDLK_2: handleCombatAction(2); break;
            case SDLK_3: handleCombatAction(3); break;
            case SDLK_4: handleCombatAction(4); break;
        }
    }
    else if(inEndText && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
    {
        if(utils.textboxFinished())
        {
            finished = true;
        }
    }
}

void GamePhase::update()
{
    if(inIntroText||inEndText)
    {
        utils.updateTextbox();
    }
}

void GamePhase::render()
{
    if(inIntroText||inEndText)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        utils.renderTextbox(renderer, font);
    }
    if(inCombat)
    {
        utils.renderBackground(renderer);
        utils.renderImage(renderer, enemyRect);
        utils.renderImage(renderer, menuRect);

        if(!resultText.empty())
        {
            utils.renderText(renderer, font, resultText, 20, 20);
        }
    }
    if (inEndText)
    {
        utils.renderTextbox(renderer, font);
    }
}

void GamePhase::startCombat()
{
    inIntroText = false;
    inCombat = true;

    utils.loadBackground(renderer, "image/background.png");
    utils.loadImage(renderer, "image/enemy.png");
    utils.loadImage(renderer, "image/combatmenu.png");
    utils.playMusic = true;
    utils.playBackgroundMusic("battleMus.mp3");
    utils.showBackground = true;
    utils.showImage = true;
}

void GamePhase::handleCombatAction(int action)
{
    int chance = rand()%100;

    switch(action)
    {
        case 1:
            if (chance < 70)
            {
                hits++;
                resultText = "Bạn đã đánh trúng hắn!";
            }
            else {
                playerHits++;
                resultText = "Bạn đã trượt! Kẻ địch phản công bất ngờ.";
            }
            break;
        case 2:
            if (chance < 50)
            {
                resultText = "Bạn né được đòn của kẻ địch!";
            }
            else
            {
                playerHits++;
                resultText = "Bạn cố né tránh nhưng không thành công! Kẻ địch đánh trúng bạn.";
            }
            break;
        case 3:
            if (chance < 15)
            {
                resultText = "Bạn đã chạy thoát thành công!";
                endCombat(true);
                return;
            }
            else
            {
                resultText = "Bạn cố gắng chạy trốn nhưng không thành công!";
            }
            break;
         case 4:
            if (chance < 8)
            {
                resultText = "Bạn bắn trúng và hạ gục hắn!";
                endCombat(true);
                return;
            }
            else
            {
                playerHits++;
                resultText = "Phát bắn trượt! Bạn đã làm hắn nổi giận.";
            }
            break;
    }

    if (hits >= 3)
    {
        resultText = "Bạn đã thành công sống sót!";
        endCombat(true);
    }
    else if (playerHits >= 3)
    {
        resultText = "Hắn đã kết liễu được bạn. Thật là đáng tiêc!";
        endCombat(false);
    }
}

void GamePhase::endCombat(bool win)
{
    inCombat = false;
    inEndText = true;

    utils.cleanup();
    utils.loadTextboxFromFile(win ? "text/Victory.txt" : "text/Defeat.txt");
    utils.showTextbox = true;
}

bool GamePhase::isFinished()
{
    return finished;
}
