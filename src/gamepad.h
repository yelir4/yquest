#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <SDL3/SDL.h>

// forward declare
struct Player;

/**
 * @struct GamepadWrapper
 * @brief wrap around SDL_Gamepad, connecting with @Player class
 */
struct GamepadWrapper
{
    SDL_Gamepad* gamepad; // active connection
    Player* player; // assigned player
    SDL_JoystickID deviceID; // gamepad permanent ID, so we can handle disconnection
    bool isMoving;

    GamepadWrapper (SDL_Gamepad* gp, SDL_JoystickID id)
        : gamepad(gp)
        , deviceID(id)
        , player(nullptr)
        , isMoving(false)
    {}

    void update(float deltaTime);
    void render(SDL_Renderer* renderer, int index);
};

#endif