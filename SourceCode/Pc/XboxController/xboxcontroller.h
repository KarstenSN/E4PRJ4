#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H
#include <xinput.h>
#include <windows.h>
#include <iostream>

class XboxController
{
public:
    XboxController(int number)
    {
        this->_controllerNum = number - 1;
    }

    int GetNumber()
    {
        return this->_controllerNum;
    }

    XINPUT_STATE GetState()
    {
        // Zeroise the state
        ZeroMemory(&this->_controllerState, sizeof(XINPUT_STATE));

        // Get the state
        XInputGetState(this->_controllerNum, &this->_controllerState);

        return this->_controllerState;
    }

    bool connect()
    {
        // Zeroise the state
        ZeroMemory(&this->_controllerState, sizeof(XINPUT_STATE));

        // Get the state
        this->_dwResult = XInputGetState(this->_controllerNum, &this->_controllerState);

        if(_dwResult == ERROR_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void vibrate(int leftVal = 0, int rightVal = 0)
    {
        // Create a Vibraton State
        XINPUT_VIBRATION Vibration;

        // Zeroise the Vibration
        ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

        // Set the Vibration Values
        Vibration.wLeftMotorSpeed = leftVal;
        Vibration.wRightMotorSpeed = rightVal;

        // Vibrate the controller
        XInputSetState(_controllerNum, &Vibration);

        return;
    }

    void getCtrData(char &leftStick, unsigned char &rightTrigger, unsigned char &leftTrigger, char &buttonX)
    {
        // Zeroise the state
        ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

        // Get the state
        XInputGetState(_controllerNum, &_controllerState);

        // Send the Left Stick [LS] state
        leftStick = static_cast<char>(this->_controllerState.Gamepad.sThumbLX >> 8);

        // Send the Right Trigger [RT] state
        rightTrigger = static_cast<unsigned char>(this->_controllerState.Gamepad.bRightTrigger);

        // Send the Left Trigger [LT] state
        leftTrigger = static_cast<unsigned char>(this->_controllerState.Gamepad.bLeftTrigger);

        // Send the X Button [X] state
        buttonX = static_cast<char>(this->_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X);
    }

private:
    DWORD _dwResult;
    XINPUT_STATE _controllerState;
    int _controllerNum = ERROR_DEVICE_NOT_CONNECTED;
};

#endif // XBOXCONTROLLER_H
