#if !defined(INPUT_HPP)
#define INPUT_HPP

#include "aurora/utils/utils.hpp"

struct aim
{
    static void PollEvents(SDL_Event* event);
    static void ProcessKeyboardEvent(SDL_KeyboardEvent* event);
    static void ProcessGamepadDeviceEvent(SDL_ControllerDeviceEvent* event);
    static void ProcessGamepadButtonEvent(SDL_ControllerButtonEvent* event);
    static void ProcessGamepadAxisEvent(SDL_ControllerAxisEvent* event);
    

    struct aimKeyInfo
    {
        u32 keyinfo;
        u32 type;
    };

    struct aimButtonInfo
    {
        u32 type;
        u32 button;
    };

    struct aimGamepad
    {
        float deadZone;

        SDL_GameController* handler;
        SDL_Haptic* haptic;

        void Destroy();

        std::vector<aimButtonInfo> prevButtons;
        std::vector<aimButtonInfo> buttons;

        bool IsButtonDown(u32 button);
        bool IsButtonReleased(u32 button);
        bool IsButtonPressed(u32 button);

        void Refresh();

        vec2 leftStickAxis;
        vec2 rightStickAxis;
        vec2 triggerAxis;

        vec2 leftStickDelta;
        vec2 rightStickDelta;
        vec2 triggerDelta;

        int GetHandle();

    	void FixDrift(vec2& src);
    	float FixDrift1D(float src);

        void Rumble(float strength, u32 duration);
    };

    static aimGamepad* CreateGamepad(int handle);

    static bool IsKeyDown(u32 key);
    static bool IsKeyReleased(u32 key);
    static bool IsKeyPressed(u32 key);
    static void Refresh();

    static aimGamepad* GetGamepadFromController(SDL_GameController* controller);
    static void RemoveGamepad(int handle);
    static aimGamepad* GetGamepad(int handle);

private:
    inline static std::vector<aimKeyInfo> pressedInfo;
    inline static std::vector<aimKeyInfo> prevPressedInfo;
    inline static std::vector<aimGamepad*> gamepads;
};


#endif // INPUT_HPP
