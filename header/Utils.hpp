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
        bool Sound = false;

        int currentBatchIndex = 0;
        std::vector<std::string> currentBatch;

        void loadBackground(SDL_Renderer* renderer, const std::string& path);
        void renderBackground(SDL_Renderer* renderer);
        void unloadBackground();

        void loadImages(SDL_Renderer* renderer, const std::string& path);
        void renderImages(SDL_Renderer* renderer, size_t index, SDL_Rect dst);
        void unloadImages();

        void playBackgroundMusic(const std::string& path);
        void stopMusic();

        void playSound(const std::string& path);

        void loadTextboxFromFile(const std::string& filename);
        void updateTextbox(Uint32 currentTime);
        void renderTextbox(SDL_Renderer* renderer, TTF_Font* font);
        void nextTextboxFrame();
        void resetTextbox();
        bool textboxFinished();

        void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);

        void cleanup();

        Utils();
        virtual ~Utils();
    private:
        SDL_Texture* background = nullptr;
        std::vector<SDL_Texture*> images;
        Mix_Music* music = nullptr;
        Mix_Chunk* chunk = nullptr;

        std::vector<std::string> textboxLines;
        std::vector<std::string> linesToShow;
        int currentLineIndex = 0;
        Uint32 lastLineTime = 0;
        Uint32 lineTimer = 800;
};

#endif
