#include "player.h"

// constructor
Player::Player (float startX, float startY, int id)
{
    x = startX;
    y = startY;
    width = 20;
    height = 40;
    speed = 250.0f;
    opacity = 255;
    playerID = id;
    name = "yelir";

    // different player -> different color
    switch (id)
    {
        case 1: color = {0,0,200,255}; break; // blue
        case 2: color = {200,0,0,255}; break; // red
        case 3: color = {0,200,0,255}; break;// green
        case 4: color = {200,200,0,255}; break;// yellow
        default: color = {200,200,200,200}; break;// white
    }
}

// update variables
// handle movement
void Player::update (float deltaTime, SDL_Gamepad* gamepad)
{
    float moveX = 0.0f;
    float moveY = 0.0f;

    // if user is assigned gamepad
    if (gamepad)
    {
        opacity = 100;

        // left stick movement
        // floats become -100 to 100
        float stickX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f;
        float stickY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;

        // stick input deadzone
        const float DEADZONE = 0.15f;
        if (SDL_fabsf(stickX) > DEADZONE) moveX += stickX;
        if (SDL_fabsf(stickY) > DEADZONE) moveY += stickY;

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
    else // keyboard movement
    {
        opacity = 255;

        const bool* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W]) y -= speed * deltaTime;
        if (keys[SDL_SCANCODE_S]) y += speed * deltaTime;
        if (keys[SDL_SCANCODE_A]) x -= speed * deltaTime;
        if (keys[SDL_SCANCODE_D]) x += speed * deltaTime;
    }

    // bounds checking
    if (x<0) x=0;
    if (y<0) y=0;
    if (x+width > 800) x = 800-width;
    if (y+height > 800) y = 800-height;
    // TODO collision checking
}

// render player and other ui
void Player::render (SDL_Renderer* renderer, TTF_Font* font)
{
    // rectangle represents player
    // TODO replace with sprite
    SDL_FRect rect = {x,y,width,height};
    // set color
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,opacity); // blue players
    // place the player "rect" in the renderer
    SDL_RenderFillRect(renderer,&rect);

    // render player NAME
    // for ints, use std::to_string(5);
    SDL_Surface* surface = TTF_RenderText_Solid(font,name.c_str(),name.length(),color);
    if (surface)
    {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        if (texture)
        {
            float text_width, text_height;
            // how large is the text
            SDL_GetTextureSize(texture, &text_width, &text_height);
            // create square of text's dimensions
            SDL_FRect dest_rect = {
                (float)(x+(width*0.5))-(text_width/2), // center text to player sprite
                y+height,
                float(text_width),
                float(text_height)
            };
            // render texture
            SDL_RenderTexture(renderer,texture,NULL,&dest_rect);

            // destroy after submitting to renderer
            SDL_DestroyTexture(texture);
        }
        SDL_DestroySurface(surface);
    }

    // render player indicator
    SDL_FRect indicator = {(float)0+20*playerID,750,10,10};
    SDL_RenderFillRect(renderer,&indicator);
}