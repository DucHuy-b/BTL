#include "GamePhase.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

GamePhase::GamePhase(SDL_Renderer* renderer, TTF_Font* font)
: renderer(renderer), font(font),  hits(0), playerHits(0), finished(false), inIntroText(true), inCombat(false), inEndText(false)
{
   std::srand(std::time(0));
   utils.loadTextboxFromFile("text/Intro.txt");
   utils.showTextbox = true;

   enemyRect = {400, 200, 400, 600};
   menuRect = {50, 400, 300, 250};
}

void GamePhase::handleEvent(SDL_Event& e)
{
    if (inIntroText && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
    {
        if (utils.textboxFinished())
        {
            utils.resetTextbox();
            utils.showTextbox = false;
            startCombat();
        }
        else if (utils.currentBatchIndex >= utils.currentBatch.size())
        {
            utils.nextTextboxFrame();
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
    else if (inEndText && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
    {
        if (utils.textboxFinished())
        {
            finished = true;
        }
        else
        {
            utils.nextTextboxFrame();
        }
    }
}

void GamePhase::update()
{
    if (inIntroText || inEndText)
    {
        utils.updateTextbox(SDL_GetTicks());
        if (utils.currentBatchIndex >= utils.currentBatch.size() && !utils.textboxFinished())
        {
            utils.nextTextboxFrame();
        }
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
        utils.renderImages(renderer, 0, enemyRect);
        utils.renderImages(renderer, 1, menuRect);

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
    utils.loadImages(renderer, "image/enemy.png");
    utils.loadImages(renderer, "image/combatmenu.png");
    utils.playMusic = true;
    utils.playBackgroundMusic("audio/battleMus.mp3");
    utils.showBackground = true;
    utils.showImage = true;
}

void GamePhase::handleCombatAction(int action)
{
    int chance = rand()%100;
    int enemyAtk = rand()%100;
    int enemyDodge = rand()%100;

    switch(action)
    {
        case 1:
            if (chance < 70 && enemyDodge>40)
            {
                utils.Sound = true;
                utils.playSound("audio/punch.wav");
                hits++;
                resultText = "Bạn đã đánh trúng hắn " + std::to_string(hits) + " lần!";
            }
            else if(enemyDodge<40 && enemyAtk<80)
            {
                utils.Sound = true;
                utils.playSound("audio/punch.wav");
                playerHits++;
                resultText = "Bạn đã trượt! Kẻ địch phản công bất ngờ. Bạn bị đánh trúng " + std::to_string(playerHits) + " lần!" ;
            }
            else if(enemyDodge<40 && enemyAtk>80)
            {
                utils.Sound = true;
                utils.playSound("audio/punch.wav");
                playerHits++;
                resultText = "Bạn đã đánh trượt!";
            }
            break;
        case 2:
            if (chance<50 && enemyAtk<80)
            {
                resultText = "Bạn né được đòn của kẻ địch!";
            }
            else if(chance>50 && enemyAtk<80)
            {
                utils.Sound = true;
                utils.playSound("audio/punch.wav");
                playerHits++;
                resultText = "Bạn cố né tránh nhưng không thành công! Hắn đánh trúng bạn" + std::to_string(playerHits) + " lần!";
            }
            else if(chance>50 && enemyAtk>80)
            {
                resultText = "Hắn ta chỉ đứng đó không làm gì cả.";
            }
            break;
        case 3:
            if (chance < 15)
            {
                resultText = "Bạn đã chạy thoát thành công!";
                endCombat(true);
            }
            else
            {
                resultText = "Bạn cố gắng chạy trốn nhưng không thành công!";
            }
            break;
         case 4:
            utils.Sound = true;
            utils.playSound("audio/gunshot.wav");
            if (chance < 8)
            {
                resultText = "Bạn bắn trúng và hạ gục hắn!";
                endCombat(true);
            }
            else
            {
                playerHits++;
                resultText = "Phát bắn trượt! Bạn đã làm hắn nổi giận. Hắn đánh trúng bạn "+ std::to_string(playerHits) + " lần!";
            }
            break;
    }

    if (hits >= 3)
    {
        endCombat(true);
    }
    else if (playerHits >= 3)
    {
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
