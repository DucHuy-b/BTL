#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <string>
#include <vector>

class Utils
{
    public:
        bool showTextbox = false;
        bool showBackground = false;
        bool showImage = false;
        bool playMusic = false;

        void loadBackground(SDL_Renderer* renderer, const std::string& path);
        void renderBackground(SDL_Renderer* renderer);
        void unloadBackground();

        void loadImage(SDL_Renderer* renderer, const std::string& path);
        void renderImage(SDL_Renderer* renderer, SDL_Rect dst);
        void unloadImage();

        void playBackgroundMusic(const std::string& path);
        void stopMusic();

        void loadTextboxFromFile(const std::string& path);
        void updateTextbox();
        void renderTextbox(SDL_Renderer* renderer, TTF_Font* font);
        bool textboxFinished();
        void resetTextbox();

        void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);

        void cleanup();

        Utils();
        virtual ~Utils();
    private:
        SDL_Texture* background = nullptr;
        SDL_Texture* image = nullptr;
        Mix_Music* music = nullptr;

        int currentLine = 0;
        std::vector<std::string> textboxLines;
        Uint32 lastLineTime = 0;
        const Uint32 lineDelay = 1000;


};

#endif
