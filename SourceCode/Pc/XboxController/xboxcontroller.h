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
        _controllerNum = number - 1;
    }

    int GetNumber()
    {
        return _controllerNum;
    }

    XINPUT_STATE GetState()
    {
        // Zeroise the state
        ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

        // Get the state
        XInputGetState(_controllerNum, &_controllerState);

        return this->_controllerState;
    }

    bool IsConnected()
    {
        // Zeroise the state
        ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

        // Get the state
        this->_dwResult = XInputGetState(_controllerNum, &_controllerState);

        if(_dwResult == ERROR_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void Vibrate(int leftVal = 0, int rightVal = 0)
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

private:
    DWORD _dwResult;
    XINPUT_STATE _controllerState;
    int _controllerNum = ERROR_DEVICE_NOT_CONNECTED;
};

#endif // XBOXCONTROLLER_H
