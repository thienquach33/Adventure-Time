#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <map>

class TextureManager
{
    public:
        static TextureManager* GetInstance() { return s_Instance = (s_Instance != nullptr)? s_Instance : new TextureManager(); }

        bool Load(std::string id, std::string filename);
        bool LoadText(std::string text, SDL_Color color, TTF_Font* font);
        void Drop(std::string id);
        void Clean();

        void Draw(std::string id, double x, double y, int width, int height, double scale, SDL_RendererFlip flip=SDL_FLIP_NONE);
        void DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip=SDL_FLIP_NONE);
        void DrawText(std::string id, double x, double y, int width, int height);
        void DrawMouse(std::string id, double x, double y, int width, int height, SDL_Rect);
        void DrawButton(std::string id, double x, double y, int width, int height, double scale, SDL_RendererFlip flip=SDL_FLIP_NONE);
        void DrawBackground(std::string id, SDL_Rect rect);

    private:
        TextureManager(){};
        std::map<std::string, SDL_Texture*> m_TextureMap;
        static TextureManager* s_Instance;
};

#endif  // TEXTUREMANAGER_H