#include "input.hpp"

void aim::PollEvents(SDL_Event* event)
{

    switch( event->type ){
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        ProcessKeyboardEvent((SDL_KeyboardEvent*)event);
        break;
    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
        ProcessGamepadDeviceEvent((SDL_ControllerDeviceEvent*)event);
        break;
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
        ProcessGamepadButtonEvent((SDL_ControllerButtonEvent*)event);
        break;
    case SDL_CONTROLLERAXISMOTION:
        ProcessGamepadAxisEvent((SDL_ControllerAxisEvent*)event);
        break;

      default:
        break;
    }
}

void aim::ProcessKeyboardEvent(SDL_KeyboardEvent* event)
{
    pressedInfo.push_back({ cast(event->keysym.sym,u32), event->type });
}

void aim::ProcessGamepadDeviceEvent(SDL_ControllerDeviceEvent* event)
{
	switch (event->type)
	{
	case SDL_CONTROLLERDEVICEADDED:
        CreateGamepad(event->which);
        break;
	case SDL_CONTROLLERDEVICEREMOVED:
        RemoveGamepad(event->which);
        break;
    default:
        break;
	}
}

void aim::ProcessGamepadButtonEvent(SDL_ControllerButtonEvent* event)
{
    aimGamepad* gamepad = GetGamepad(event->which);
    gamepad->buttons.push_back({ event->state, event->button });
    std::cout << event->which << "," << event->button << std::endl;
}

void aim::ProcessGamepadAxisEvent(SDL_ControllerAxisEvent* event)
{
}

void aim::Refresh()
{
    pressedInfo.clear();

    for (auto gamepad : gamepads)
    {
        gamepad->Refresh();
    }
}

aim::aimGamepad* aim::GetGamepadFromController(SDL_GameController* controller)
{
    for (auto gamepad : gamepads)
    {
	    if (gamepad->handler == controller)
	    {
            return gamepad;
	    }
    }
    return nullptr;
}

void aim::RemoveGamepad(int handle)
{

    gamepads[handle]->Destroy();
    gamepads[handle] = nullptr;

    for (auto gamepad : gamepads)
    {
        if (!gamepad) {
            continue;
        }
	    if (gamepad->GetHandle() > handle)
	    {
            gamepads[gamepad->GetHandle()] = gamepad;
	    }
    }
}

aim::aimGamepad* aim::GetGamepad(int handle)
{
    for (auto gamepad : gamepads)
    {
	    if (gamepad->GetHandle() == handle)
	    {
            return gamepad;
	    }
    }
    return nullptr;
}

void aim::aimGamepad::Destroy()
{
    SDL_GameControllerClose(handler);
    SDL_HapticClose(haptic);
}

bool aim::aimGamepad::IsButtonDown(u32 button)
{
    for (const aimButtonInfo pressed_info : buttons)
    {
        if (pressed_info.type != SDL_PRESSED) {
            continue;
        }
        if (pressed_info.button != button) {
            continue;
        }
        return true;
    }
    return false;
}

bool aim::aimGamepad::IsButtonReleased(u32 button)
{
    for (aimButtonInfo pressed_info : buttons)
    {
        if (pressed_info.type != SDL_RELEASED)
            continue;

        if (pressed_info.button == button)
        {
            return true;
        }
    }
    return false;
}

bool aim::aimGamepad::IsButtonPressed(u32 button)
{
    for (aimButtonInfo pressed_info : prevButtons)
    {
        if (pressed_info.button == button)
        {
            return false;
        }
    }

    for (aimButtonInfo pressed_info : buttons)
    {
        if (pressed_info.type != SDL_PRESSED)
            continue;

        if (pressed_info.button == button)
        {
            return true;
        }
    }
    return false;
}

void aim::aimGamepad::Refresh()
{
    prevButtons = buttons;
    buttons.clear();

    vec2 lsa = leftStickAxis;
    vec2 rsa = rightStickAxis;
    vec2 ta = triggerAxis;

    leftStickAxis.x = flt -SDL_GameControllerGetAxis(handler, SDL_CONTROLLER_AXIS_LEFTX) / (float)INT16_MAX;
    leftStickAxis.y = flt -SDL_GameControllerGetAxis(handler, SDL_CONTROLLER_AXIS_LEFTY) / (float)INT16_MAX;

    rightStickAxis.x = flt -SDL_GameControllerGetAxis(handler, SDL_CONTROLLER_AXIS_RIGHTX) / (float)INT16_MAX;
    rightStickAxis.y = flt -SDL_GameControllerGetAxis(handler, SDL_CONTROLLER_AXIS_RIGHTY) / (float)INT16_MAX;

    triggerAxis.x = flt SDL_GameControllerGetAxis(handler, SDL_CONTROLLER_AXIS_TRIGGERLEFT) / (float)INT16_MAX;
    triggerAxis.y = flt SDL_GameControllerGetAxis(handler, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / (float)INT16_MAX;

    vec2 lsDelta = leftStickAxis - lsa;
    vec2 rsDelta = rightStickAxis - rsa;
    vec2 taDelta = triggerAxis - ta;

    leftStickDelta = lsDelta;
    rightStickDelta = rsDelta;
    triggerDelta = taDelta;

    FixDrift(leftStickAxis);
    FixDrift(rightStickAxis);
}

int aim::aimGamepad::GetHandle()
{
    return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(handler));
}

void aim::aimGamepad::FixDrift(vec2& src)
{
	src = { FixDrift1D(src.x), FixDrift1D(src.y) };
}

float aim::aimGamepad::FixDrift1D(float src)
{
    if (abs(src) < 0.025)
    {
        return 0;
    }
    float dz = deadZone / flt INT16_MAX;
    if (src > dz)
    {
        return dz;
    }
    if (src < -dz)
    {
        return -dz;
    }
    return 0;
}

void aim::aimGamepad::Rumble(float strength, u32 duration)
{
    SDL_GameControllerRumble(handler, 0xFFFF * strength, 0xFFFF * strength, duration);

    //SDL_HapticRumblePlay(haptic, strength, duration);
}

aim::aimGamepad* aim::CreateGamepad(int handle)
{
    if (SDL_NumJoysticks() < 1)
    {
        throw std::exception("No joysticks connected.");
    }

    aimGamepad* gamepad = new aimGamepad;

    if (!SDL_IsGameController(handle)) {
        return nullptr;
    }
    gamepad->handler = SDL_GameControllerOpen(handle);

    if (SDL_GameControllerHasRumble(gamepad->handler))
    {
        gamepad->haptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(gamepad->handler));
    }

    gamepad->deadZone = 8000;

    gamepads.push_back(gamepad);

    return gamepad;
}

bool aim::IsKeyDown(u32 key)
{
    for (const aimKeyInfo pressed_info : pressedInfo)
    {
        if (pressed_info.type != SDL_KEYDOWN) {
            continue;
        }
        if (pressed_info.keyinfo != key) {
            continue;
        }
        return true;
    }
    return false;
}

bool aim::IsKeyReleased(u32 key)
{
    for (aimKeyInfo pressed_info : pressedInfo)
    {
        if (pressed_info.type != SDL_KEYUP)
            continue;

        if (pressed_info.keyinfo == key)
        {
            return true;
        }
    }
    return false;
}

bool aim::IsKeyPressed(u32 key)
{

    for (aimKeyInfo pressed_info : prevPressedInfo)
    {
        if (pressed_info.keyinfo == key)
        {
            return false;
        }
    }

    for (aimKeyInfo pressed_info : pressedInfo)
    {
        if (pressed_info.type != SDL_KEYDOWN)
            continue;

        if (pressed_info.keyinfo == key)
        {
            return true;
        }
    }
    return false;
}
