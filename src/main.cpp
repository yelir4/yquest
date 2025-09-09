#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> // handle platform differences for main function
#include <SDL3_ttf/SDL_ttf.h> // text rendering
#include <map> // map
#include <utility> // pair
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // show transparency

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
    
    // TODO does this need a destructor?
    /// @players map (key-value pairs) of @Player, representing players on screen
    std::map<int, Player> players;
    // create new Player, using id as hash
    players.emplace(1, Player(400,400,1,"yelir",textEngine,gameFont));
    players.emplace(2, Player(500,400,2,"lori",textEngine,gameFont));
    players.emplace(3, Player(600,400,3,"tomtom",textEngine,gameFont));
    players.emplace(4, Player(700,400,4,"poncho",textEngine,gameFont));

    // SDL_Gamepad wrapper: handle gamepads
    std::map<int, GamepadWrapper> gamepads;

    // 60 fps target
    const int TARGET_FPS = 60;
    const float TARGET_FRAME_TIME = 1000.0f / TARGET_FPS; // 16.67 ms / frame
    bool running = true;
    Uint64 lastTime = SDL_GetTicks(); // ex: 15432ms

    // event loop: HANDLE EVENTS -> UPDATE GAME -> RENDER GAME
    // HANDLE EVENTS
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
                case SDL_EVENT_QUIT: {
                    running = false;
                    break;
                }
                // key presses
                case SDL_EVENT_KEY_DOWN: {
                    if (event.key.key == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                    break;
                }
                // connect gamepad
                case SDL_EVENT_GAMEPAD_ADDED: {
                    SDL_JoystickID deviceID = event.gdevice.which; // gamepad that triggered the event
                    SDL_Gamepad* gamepad = SDL_OpenGamepad(deviceID);
                    if (gamepad) // add connected gamepad to @gamepads
                    {
                        gamepads.emplace(deviceID, GamepadWrapper(gamepad, deviceID));
                        SDL_Log("Gamepad connected: %s, %d", SDL_GetGamepadName(gamepad), deviceID);
                    }
                    break;
                }
                // disconnect gamepad
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    SDL_JoystickID deviceID = event.gdevice.which; // gamepad that triggered the event
                    GamepadWrapper& wrapper = gamepads.at(deviceID);
                    if (wrapper.player != nullptr) wrapper.player->gamepadWrapper = nullptr; // unset any associated player

                    SDL_CloseGamepad(wrapper.gamepad);
                    gamepads.erase(deviceID);
                    break;
                }
                // todo gamepad button presses (cycle players)
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                    switch (event.gbutton.button)
                    {
                        case SDL_GAMEPAD_BUTTON_DPAD_LEFT: {
                            SDL_JoystickID deviceID = event.gdevice.which;
                            GamepadWrapper& wrapper = gamepads.at(deviceID);
                            Player* player = wrapper.player; // gamepad's associated player (or nullptr)
                            bool found = false;

                            // iterator
                            // it: first or last element (or specific player)
                            // stop_it: beyond first or last element
                            auto it = (player == nullptr) ? players.rbegin() : std::make_reverse_iterator(players.find(player->id));

                            // look for player with no gamepad
                            // while (it != players.end())
                            while (it != players.rend())
                            {
                                if (it->second.gamepadWrapper == nullptr)
                                {
                                    found = true;
                                    if (player != nullptr) player->gamepadWrapper = nullptr; // unset original player's gamepad
                                    wrapper.player = &it->second; // set this gamepad's @player pointer
                                    it->second.gamepadWrapper = &wrapper; // set the new player's gamepad
                                    break;
                                }
                                --it;
                            }
                            if (!found) // fail
                            {
                                player->gamepadWrapper = nullptr; // unset original player's gamepad
                                wrapper.player = nullptr; // this gamepad has no @player
                            }
                            break;
                        }
                        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: {
                            SDL_JoystickID deviceID = event.gdevice.which;
                            GamepadWrapper& wrapper = gamepads.at(deviceID);
                            Player* player = wrapper.player; // gamepad's associated player (or nullptr)
                            bool found = false;

                            // iterator
                            // it: first or last element (or specific player)
                            // stop_it: beyond first or last element
                            auto it = (player == nullptr) ? players.begin() : players.find(player->id);

                            // look for player with no gamepad
                            while (it != players.end())
                            {
                                if (it->second.gamepadWrapper == nullptr)
                                {
                                    found = true;
                                    if (player != nullptr) player->gamepadWrapper = nullptr; // unset original player's gamepad
                                    wrapper.player = &it->second; // set this gamepad's @player pointer
                                    it->second.gamepadWrapper = &wrapper; // set the new player's gamepad
                                    break;
                                }
                                ++it;
                            }
                            if (!found) // fail
                            {
                                player->gamepadWrapper = nullptr; // unset original player's gamepad
                                wrapper.player = nullptr; // this gamepad has no @player
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }

        // UPDATE GAME LOGIC
        for (auto& [id,player] : players)
        {
            player.update(deltaTime); // update player state
        }
        for (auto& [id,wrapper] : gamepads)
        {
            wrapper.update(deltaTime); // update gamepad state
        }

        // RENDER GAME
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        // fill window
        SDL_RenderClear(renderer);

        // bottom panel
        SDL_FRect rect = {0,760,800,40};
        SDL_SetRenderDrawColor(renderer,20,20,50,255);
        SDL_RenderFillRect(renderer,&rect);

        for (auto& [id,player] : players)
        {
            player.render(renderer); // render player
        }
        int index = 0;
        for (auto& [id,wrapper] : gamepads)
        {
            wrapper.render(renderer, index); // render gamepad (indicators)
            ++index;
        }

        // show result
        SDL_RenderPresent(renderer);

        // finish early -> wait x ms (limit frame rate)
        Uint64 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < TARGET_FRAME_TIME)
        {
            SDL_Delay((Uint32)(TARGET_FRAME_TIME - frameTime));
        }
        lastTime = currentTime;
    }

    // cleanup
    // close open gamepads
    for (auto& [id, wrapper] : gamepads)
    {
        SDL_CloseGamepad(wrapper.gamepad);
    }
    TTF_DestroyRendererTextEngine(textEngine);
    TTF_CloseFont(gameFont);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}