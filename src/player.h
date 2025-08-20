#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <string> // player name
#include <SDL3_ttf/SDL_ttf.h>

/** player class, represents entity on screen */
class Player
{
private:
    float x,y;
    float width,height;
    float speed;
    int opacity;
    int playerID;
    std::string name;
    SDL_Color color; // 4 ints rgba {0,0,0,0} 
    // TODO cache the name texture

public:
    Player (float startX, float startY, int id);
    void update(float deltaTime, SDL_Gamepad* gamepad);
    void render(SDL_Renderer* renderer, TTF_Font* font);

    // getter functions
    float getX() const {return x;}
    float getY() const {return y;}
    float getWidth() const {return width;}
    float getHeight() const {return height;}
};

#endif