#include "gamepad.h"
#include "player.h"

void GamepadWrapper::update(float deltaTime)
{
    isMoving = false;
    float leftStickX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f;
    float leftStickY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;
    // right stick aiming
    float rightStickX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f;
    float rightStickY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f;

    // stick input deadzone
    const float DEADZONE = 0.15f;
    if (SDL_fabsf(leftStickX) > DEADZONE
        || SDL_fabsf(leftStickY) > DEADZONE)
    {
        isMoving = true;
    }
    if (isMoving) SDL_Log("yes");
}

// create gamepad indicators
// color white if no player, otherwise copy player color
void GamepadWrapper::render(SDL_Renderer* renderer, int index)
{
    if (isMoving) SDL_Log("here");
    SDL_Color rgba;
    if (player == nullptr)
    {
        rgba = {255,255,255,255};
        rgba.a = (isMoving) ? 255 : 100;
    }
    else
    {
        rgba = player->rgba;
        // rgba.a = (isMoving) ? 255 : 100;
    }
    SDL_SetRenderDrawColor(renderer,rgba.r,rgba.g,rgba.b,rgba.a);

    SDL_FRect indicator = {(float)10+20*index,775,10,10}; //x,y,w,h
    SDL_RenderFillRect(renderer,&indicator);
    // render which gamepad this thing belongs to
    // std::string padIndex = (gamepad == nullptr) ? std::to_string(0) : std::to_string(gamepadWrapper->deviceID);
    // TTF_Text* gamepad_text = TTF_CreateText(textEngine, font, padIndex.c_str(), padIndex.length());
    // TTF_DrawRendererText(gamepad_text, (float)0+20*id, 740);
}