#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> // handle platform differences for main function
#include <SDL3_ttf/SDL_ttf.h> // text rendering
#include "player.h"

// TODO dpi scaling
/**
 * yquest entry point
 */
int main(int argc, char* argv[])
{

    // initialize video (includes keyboard), audio, and gamepads (controllers)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD) == false)
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // initialize text
    if(TTF_Init() == false)
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    TTF_Font* game_font = TTF_OpenFont("../assets/roboto/Roboto-regular.ttf", 14);
    if (!game_font)
    {
        SDL_Log("Could not load font: %s", SDL_GetError());
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    // create window
    SDL_Window* window = SDL_CreateWindow("yquest",800,800,SDL_WindowFlags(0));
    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        TTF_Quit();
        TTF_CloseFont(game_font);
        return 1;
    }

    // create renderer (draws graphics, can use any backend)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        TTF_Quit();
        TTF_CloseFont(game_font);
        SDL_DestroyWindow(window);
        return 1;
    }

    // handle gamepads
    SDL_Gamepad* gamepad = NULL;
    // check for gamepads
    int numGamepads = 0;
    SDL_JoystickID* gamepads = SDL_GetGamepads(&numGamepads);
    if (numGamepads > 0)
    {
        gamepad = SDL_OpenGamepad(gamepads[0]); // first gamepad
        if (gamepad) SDL_Log("Gamepad connected: %s", SDL_GetGamepadName(gamepad));
    }
    if (gamepads) SDL_free(gamepads); // free any allocated memory


    // CONSTANT VARIABLES
    const int TARGET_FPS = 60;
    const float TARGET_FRAME_TIME = 1000.0f / TARGET_FPS; // 16.67 ms / frame
    
    Player player(400,400,1); // define new player id=1
    Player p2(500,400,2); // id 2 
    Player p3(600,400,3); // id 3
    Player p4(700,400,4); // id 4

    bool running = true;
    Uint64 lastTime = SDL_GetTicks(); // ex: 15432ms

    // event loop
    // HANDLE EVENTS -> UPDATE GAME -> RENDER GAME
    while (running)
    {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // seconds

        // HANDLE EVENTS
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;

                // key presses
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                    break;

                // connect/disconnect gamepad
                case SDL_EVENT_GAMEPAD_ADDED:
                    if (!gamepad)
                    {
                        gamepad = SDL_OpenGamepad(event.gdevice.which);
                        if (gamepad) SDL_Log("Gamepad connected: %s", SDL_GetGamepadName(gamepad));
                    }
                    break;

                case SDL_EVENT_GAMEPAD_REMOVED:
                    if (!gamepad)
                    {
                        // todo wrong
                        // if (gamepad && event.gdevice.which == SDL_GetGamepadInstanceID(gamepad))
                        {
                            SDL_Log("Gamepad disconnected");
                            SDL_CloseGamepad(gamepad);
                            gamepad = NULL;
                        }
                        if (gamepad) SDL_Log("Gamepad connected: %s", SDL_GetGamepadName(gamepad));
                    }
                    break;

                // gamepad button presses

                default:
                    break;
            }
        }

        // UPDATE GAME LOGIC
        // updateGame(deltaTime);
        player.update(deltaTime, gamepad);

        // RENDER GAME
        // render outside border
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        // fill window
        SDL_RenderClear(renderer);

        // bottom panel
        SDL_FRect rect = {0,760,800,40};
        SDL_SetRenderDrawColor(renderer,20,20,50,255);
        SDL_RenderFillRect(renderer,&rect);
        // renderGame(renderer);

        // NOTE passing font POINTER
        player.render(renderer, game_font);
        // show result
        SDL_RenderPresent(renderer);

        // FINISH EARLY -> wait x ms
        // limit frame rate
        Uint64 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < TARGET_FRAME_TIME)
        {
            SDL_Delay((Uint32)(TARGET_FRAME_TIME - frameTime));
        }

        lastTime = currentTime;
    }

    // cleanup
    TTF_CloseFont(game_font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}