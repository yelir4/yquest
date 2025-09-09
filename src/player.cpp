#include "player.h"
#include "gamepad.h"

// constructor with member initializer list (faster)
Player::Player (float x, float y, int id, const std::string& name, TTF_TextEngine* te, TTF_Font* f)
    : x(x)
    , y(y)
    , width(20)
    , height(40)
    , speed(250.0f)
    , id(id)
    , gamepadWrapper(nullptr)

    , name(name)
    , textEngine(te)
    , font(f)
{
    name_text = TTF_CreateText(textEngine, font, name.c_str(), name.length());

    // different player -> different color
    switch (id)
    {
        case 1: rgba = {0,0,200,100}; break; // blue
        case 2: rgba = {200,0,0,100}; break; // red
        case 3: rgba = {0,200,0,100}; break; // green
        case 4: rgba = {200,200,0,100}; break; // yellow
        default: rgba = {200,200,200,100}; break; // white
    }
}

// update variables
// handle movement
void Player::update (float deltaTime)
{
    // movement
    float moveX = 0.0f;
    float moveY = 0.0f;

    // if @player has a gamepad
    if (gamepadWrapper != nullptr)
    {
        // left stick: movement
        // floats range [-,] -> [-100, 100]
        float leftStickX = SDL_GetGamepadAxis(gamepadWrapper->gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f;
        float leftStickY = SDL_GetGamepadAxis(gamepadWrapper->gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;
        // right stick: aiming
        float rightStickX = SDL_GetGamepadAxis(gamepadWrapper->gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f;
        float rightStickY = SDL_GetGamepadAxis(gamepadWrapper->gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f;

        // stick input deadzone
        const float DEADZONE = 0.15f;
        if (SDL_fabsf(leftStickX) > DEADZONE)
        {
            // gamepadWrapper->isMoving = true;
            moveX += leftStickX;
        }
        if (SDL_fabsf(leftStickY) > DEADZONE)
        {
            // gamepadWrapper->isMoving = true;
            moveY += leftStickY;
        }

        // normalize diagonal movement
        float length = SDL_sqrtf(moveX*moveX + moveY*moveY);
        if (length > 1.0f)
        {
            moveX /= length;
            moveY /= length;
        }

        // apply movement
        x += moveX * speed * deltaTime;
        y += moveY * speed * deltaTime;
    }
    /*
    else // keyboard movement
    {
        opacity = 255;
        const bool* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W]) y -= speed * deltaTime;
        if (keys[SDL_SCANCODE_S]) y += speed * deltaTime;
        if (keys[SDL_SCANCODE_A]) x -= speed * deltaTime;
        if (keys[SDL_SCANCODE_D]) x += speed * deltaTime;
    }*/

    // bounds checking
    if (x<0) x=0;
    if (y<0) y=0;
    if (x+width > 800) x = 800-width;
    if (y+height > 800) y = 800-height;
    // TODO collision checking
}

// render player and other ui
void Player::render (SDL_Renderer* renderer)
{
    // color checking
    rgba.a = (gamepadWrapper == nullptr || gamepadWrapper->isMoving == false) ? 100 : 255;
    // TODO replace playerbox with sprite
    SDL_FRect rect = {x,y,width,height};
    // set color
    SDL_SetRenderDrawColor(renderer,rgba.r,rgba.g,rgba.b,rgba.a);
    // place player @rect in renderer
    SDL_RenderFillRect(renderer,&rect);

    // render @name_text centered to player
    int textWidth, textHeight;
    if (TTF_GetTextSize(name_text, &textWidth, &textHeight) == true)
    {
        float draw_x = x + (width * 0.5) - (textWidth * 0.5f);
        // todo render player name
        TTF_DrawRendererText(name_text, draw_x, y + height);
    }
    else
    {
        SDL_Log("TTF_GetTextSize failed: %s", SDL_GetError());
    }
}