/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <string.h>
#include <stdio.h>

//#define DEBUGGING_MODE
//Definer stoerrelse paa buffere - hhv. 1 og 2 bytes
#define RD_BUFFERSIZE 2u
    
//Erklaering af buffer arrays til I2C
uint8 readBuffer[RD_BUFFERSIZE] = {0}; // Slave -> Master

#define DEBUGGING_MODE
#define fclk 400000 //400kHz timer counter clk
double primaryTimer = 0;
double secondaryTimer = 0;
volatile double circumference = 0.1945; //omkreds maalt med skydelaere.
volatile double timeBetween = 0;
volatile double rpm = 0;
volatile double calcVelocity = 0;
uint32 timerPeriod;
CYBIT newData = 0;
/*
uint8 count = 0;
uint8 test[100];
*/

CY_ISR_PROTO(my_ISR);

CY_ISR(my_ISR){
    secondaryTimer = (double)primaryTimer;
    primaryTimer = (double)Timer_1_ReadCounter();
    isr_1_ClearPending();
    Pin_1_ClearInterrupt();
    
    if (secondaryTimer < primaryTimer){
        timeBetween = ((secondaryTimer + timerPeriod) - primaryTimer)/fclk;
    } else {
        timeBetween = (secondaryTimer - primaryTimer)/fclk;
    }
   
    rpm = 60 / (timeBetween*5);
    calcVelocity = ((circumference*rpm) / 60)*3.6;

    newData = 1;
    
    /* Debugging! */
    /*
    if (count >= 95){
        count = 0;
    }
    
    count++;
    */
}

int main()
{
    Timer_1_Start();
    UART_1_Start();
    isr_1_StartEx(my_ISR);
    isr_1_ClearPending();
    Pin_1_ClearInterrupt();
    I2C_1_I2CSlaveInitReadBuf(readBuffer, RD_BUFFERSIZE);
    I2C_1_Start();
    timerPeriod = Timer_1_ReadPeriod();
    
     CyGlobalIntEnable;

    for(;;)
    {   
        if (newData){
            if (calcVelocity < 25){
                readBuffer[0] = (uint8)(calcVelocity*10);
                //test[count] = readBuffer[0];
                readBuffer[1] = 0;
            } else { 
                readBuffer[0] = 'X';
                readBuffer[1] = 'X';
            }
        
            I2C_1_I2CSlaveClearReadBuf(); //Reset read buffer pointer  (Resetter IKKE data...)
            newData = 0;
        }
    }
}
