#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> // handle platform differences for main function
#include <SDL3_ttf/SDL_ttf.h> // text rendering
#include "player.h"
#include "gamepad.h"

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
    TTF_Font* gameFont = TTF_OpenFont("../assets/roboto/Roboto-regular.ttf", 14);
    if (!gameFont)
    {
        SDL_Log("Could not load font: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // create window
    SDL_Window* window = SDL_CreateWindow("yquest",800,800,SDL_WindowFlags(0));
    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        TTF_CloseFont(gameFont);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // create renderer (draws graphics, can use any backend)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(gameFont);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // text renderer engine (caching, dynamic text)
    TTF_TextEngine* textEngine = TTF_CreateRendererTextEngine(renderer);
    if (!textEngine)
    {
        SDL_Log("TTF_TextEngine failed: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(gameFont);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // define players
    std::vector<Player> players;
    players.emplace_back(400,400,1,"yelir",textEngine,gameFont);
    players.emplace_back(500,400,2,"lori",textEngine,gameFont);
    players.emplace_back(400,400,3,"tomtom",textEngine,gameFont);
    players.emplace_back(400,400,4,"poncho",textEngine,gameFont);

    // place gamepads in their gamepad handler
    // SDL_Gamepad pointer: handle gamepads
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

    // 60 fps target
    const int TARGET_FPS = 60;
    const float TARGET_FRAME_TIME = 1000.0f / TARGET_FPS; // 16.67 ms / frame
    
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
        player.render(renderer);
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

    // todo should close open gamepads?
    TTF_DestroyTextEngine(textEngine);
    // cleanup
    TTF_CloseFont(game_font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}