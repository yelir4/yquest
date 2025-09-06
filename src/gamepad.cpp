#include <vector>


struct GamepadWrapper
{
    SDL_Gamepad* gamepad; // active connection
    int playerID; // assigned player
    SDL_JoystickID deviceID; // gamepad permanent ID, so we can handle disconnection

    GamepadInfo (SDL_Gamepad* gp, SDL_JoystickID id)
        : gamepad(gp)
        , deviceID(id)
        , assignedPlayerID(-1)
    {}
};

/**
 * @class GamepadManager
 * datastructure to manage connected gamepads
 */
class GamepadManager
{
private:
    // vector of gamepad wrappers
    std::vector<GamepadWrapper> gamepads;

public:
    void addGamepad (SDL_JoystickID deviceID)
    {
        // open the gamepad
        SDL_Gamepad* gamepad = SDL_OpenGamepad(deviceID);
        // place in vector
        if (gamepad)
        {
            gamepads.emplace_back(gamepad, deviceID);
        }

        SDL_Log("Gamepad connected: %s (Total: %d)", SDL_GetGamepadName(gamepad), (int)gamepads.size());
    }

    void removeGamepad (SDL_JoystickID deviceID)
    {
        // iterate through @gamepads to find gamepad to clear
        for (int i=0; i<(int)gamepads.size(); ++i)
        {
            if (gamepads[i].deviceID == deviceID)
            {
                SDL_Log("Gamepad disconnected (Player %d)", it->playerID);
                SDL_CloseGamepad(it->gamepad);
                gamepads.erase(it);
            }
        }
    }

    void cyclePlayers (int gamepadIndex)
    {
        // just making sure we're in bounds i guess
        // also check if there's a player to cycle through
        if (gamepadIndex >= 0
            && gamepadIndex < (int)gamepads.size())
            && 
        {
            // should check if there's even an open player right?
        }
    }
};