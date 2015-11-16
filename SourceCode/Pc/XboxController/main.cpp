#include <iostream>
#include <windows.h>
#include "xboxcontroller.h"


XboxController* Controller1;

int main()
{
    COORD RT_Cursor = {30, 4};
    COORD LS_Cursor = {30, 5};

    Controller1 = new XboxController(1);

    std::cout << "[A] Vibrate Left Only" << std::endl;
    std::cout << "[B] Vibrate Right Only" << std::endl;
    std::cout << "[X] Vibrate Both" << std::endl;
    std::cout << "[Y] Vibrate Neither" << std::endl;
    std::cout << "[RS] Show Right Sticks State: " << std::endl;
    std::cout << "[LS] Show Left Trigger State: " << std::endl;
    std::cout << "[BACK] Exit" << std::endl;

    while(true)
    {
        if(Controller1->IsConnected())
        {
            if(Controller1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
            {
                Controller1->Vibrate(65535, 0);
            }

            if(Controller1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
            {
                Controller1->Vibrate(0, 65535);
            }

            if(Controller1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
            {
                Controller1->Vibrate(65535, 65535);
            }

            if(Controller1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
            {
                Controller1->Vibrate();
            }

            if(Controller1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
            {
                SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), LS_Cursor );
                std::cout << "      " << std::endl;
                SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), LS_Cursor );
                std::cout << ((int)Controller1->GetState().Gamepad.sThumbLX) << std::endl;
            }

            if(Controller1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
            {
                SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), RT_Cursor );
                std::cout << "      " << std::endl;
                SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), RT_Cursor );
                std::cout << Controller1->GetState().Gamepad.bRightTrigger << std::endl;
            }

            if(Controller1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
            {
                break;
            }
        }
        else
        {
            std::cout << "\n\tERROR! - XBOX 360 Controller #" << Controller1->GetNumber() + 1 << " Not Found!" << std::endl;
            std::cout << "Press Any Key To Exit." << std::endl;
            std::cin.get();
            break;
        }
    }

    delete(Controller1);

    return 0;
}