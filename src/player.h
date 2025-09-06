#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <string> // player name
#include <SDL3_ttf/SDL_ttf.h>

/** 
 * @class Player
 * players represented entity on screen */
class Player
{
private:
    float x,y;
    float width,height;
    float speed;
    int opacity;
    int id;

    std::string name;
    TTF_TextEngine* textEngine;
    TTF_Font* font;
    TTF_Text* name_text = nullptr;
    SDL_Color color; // 4 ints rgba {0,0,0,0} 
    // TODO cache the name texture

public:
    Player (float startX, float startY, int id);
    void update(float deltaTime, SDL_Gamepad* gamepad);
    void render(SDL_Renderer* renderer);

    // getter functions
    float getX() const {return x;}
    float getY() const {return y;}
    float getWidth() const {return width;}
    float getHeight() const {return height;}
};

#endif