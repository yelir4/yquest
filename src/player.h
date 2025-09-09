#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string> // player name

// forward declare
struct GamepadWrapper;

/** 
 * @struct Player
 * players represented entity on screen */
struct Player
{
    float x,y;
    float width,height;
    float speed;
    int id;
    GamepadWrapper* gamepadWrapper;

    std::string name;
    TTF_TextEngine* textEngine;
    TTF_Font* font;
    TTF_Text* name_text = nullptr; // cached name texture
    SDL_Color rgba; // rgba{0,0,0,0}

    Player (float x, float y, int id, const std::string& name, TTF_TextEngine* te, TTF_Font* f);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    // getter functions
    float getX() const {return x;}
    float getY() const {return y;}
    float getWidth() const {return width;}
    float getHeight() const {return height;}
};

#endif