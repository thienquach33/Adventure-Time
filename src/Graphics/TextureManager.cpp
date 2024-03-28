#include "TextureManager.h"
#include "../Core/Engine.h"
#include "../Camera/Camera.h"
#include <iostream>

TextureManager* TextureManager::s_Instance = nullptr;

bool TextureManager::Load(std::string id, std::string filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->getRenderer(), surface);
    m_TextureMap[id] = texture;
    return true;    
}

bool TextureManager::LoadText(std::string text, SDL_Color color, TTF_Font* font)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->getRenderer(), surface);
    Uint8 r, g, b;
    m_TextureMap[text] = texture;
    return true;
}

void TextureManager::Draw(std::string id, double x, double y, int width, int height, double scale, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0, 0, width, height};
    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, (double) width * scale, (double) height * scale};
    // SDL_SetTextureColorMod(texture, 128, 128, 128); // 50% brightness
    SDL_Texture* texture = m_TextureMap[id];
    if(Engine::GetInstance()->darker == true) {
        SDL_SetTextureColorMod(texture, 128, 128, 128); // 50% brightness
    }
    SDL_RenderCopyEx(Engine::GetInstance()->getRenderer(), texture, &srcRect, &dstRect, 0 /*angle*/, nullptr, flip /*lat*/);
}

void TextureManager::DrawMenu(std::string id, double x, double y, int width, int height, double scale, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0, 0, width, height};
    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, (double) width * scale, (double) height * scale};
    // SDL_SetTextureColorMod(texture, 128, 128, 128); // 50% brightness
    SDL_Texture* texture = m_TextureMap[id];
    SDL_RenderCopyEx(Engine::GetInstance()->getRenderer(), texture, &srcRect, &dstRect, 0 /*angle*/, nullptr, flip /*lat*/);
}

void TextureManager::DrawButton(std::string id, double x, double y, int width, int height, double scale, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0, 0, width, height};
    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, (double) width * scale, (double) height * scale};
    SDL_RenderCopyEx(Engine::GetInstance()->getRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0 /*angle*/, nullptr, flip /*lat*/);
} 

void TextureManager::DrawBackground(std::string id, SDL_Rect rect)
{
    SDL_RenderCopy(Engine::GetInstance()->getRenderer(), m_TextureMap[id], NULL, &rect);
}

void TextureManager::DrawText(std::string id, double x, double y, int width, int height)
{
    SDL_Rect srcRect = {x, y, width, height};
    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, width, height};
    SDL_Texture* texture = m_TextureMap[id];
    SDL_RenderCopy(Engine::GetInstance()->getRenderer(), texture, NULL, &srcRect);
}

void TextureManager::DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {tileSize * frame, tileSize * row, tileSize, tileSize};

    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, tileSize, tileSize};
    SDL_Texture* texture = m_TextureMap[tilesetID];
    if(Engine::GetInstance()->darker == true) {
        SDL_SetTextureColorMod(texture, 128, 128, 128); // 50% brightness
    }
    SDL_RenderCopyEx(Engine::GetInstance()->getRenderer(), texture, &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawMouse(std::string id, double x, double y, int width, int height, SDL_Rect rect)
{
    SDL_Rect srcRect = {x, y, width, height};
    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect dstRect = {rect.x - cam.X, rect.y - cam.Y, rect.w, rect.h};
    SDL_RenderCopy(Engine::GetInstance()->getRenderer(), m_TextureMap[id], &srcRect, &rect);
}

void TextureManager::Drop(std::string id)
{
    SDL_DestroyTexture(m_TextureMap[id]);
    m_TextureMap.erase(id);
}

void TextureManager::Clean()
{
    std::map<std::string, SDL_Texture*>::iterator it;
    for (it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
    {
        SDL_DestroyTexture(it->second);
    }
    m_TextureMap.clear();
}