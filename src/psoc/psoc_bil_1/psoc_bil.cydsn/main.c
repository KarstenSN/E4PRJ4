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

//Definer stoerrelse paa buffere - hhv. 1 og 2 bytes
#define RD_BUFFERSIZE 9
#define fclk 400000                 //400kHz timer counter clk

/*################### DISTANCESENSOR #############################*/
// DistanceSensor variable definition
// Define af adresser
uint8 addrFL = 0b1110000;	            // 0x70 dec 112
uint8 addrFR = 0b1110001;	            // 0x71 dec 113
uint8 addrRL = 0b1110011;	            // 0x73 dec 115
uint8 addrRR = 0b1110110;	            // 0x76 dec 118

// Initiering af distance-variabler
uint16 distanceFL   = 0;	
uint16 distanceFR   = 0;
uint16 distanceRL   = 0;
uint16 distanceRR   = 0;
uint8 sendBuffer[9] = {0};

uint8 FLwrite = 0b11100000;             // 0xE0 dec 224
uint8 FRwrite = 0b11100010;	            // 0xE2 dec 226
uint8 RLwrite = 0b11100110;	            // 0xE6 dec 230
uint8 RRwrite = 0b11101100;	            // 0xEC dec 236
uint8 FLread  = 0b11100001;             // 0xE1 dec 225
uint8 FRread  = 0b11100011;	            // 0xE4 dec 227
uint8 RLread  = 0b11100111;	            // 0xE7 dec 231
uint8 RRread  = 0b11101101;             // 0xED dec 237

// Kommando til start RangeReading
uint8 StartReading    = 0b01010001;     // 0x51 dec 81  

// Wait till complete functions.
uint8 checkWriteComplete(void){
    while(0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_WR_CMPLT));
    CyDelay(20);
    return 0;
}

void checkReadComplete(void){
    while(0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_RD_CMPLT));
}

// getDistance()
void getDistance(void){
    
    uint8 FLbuf[2] = {0};
    uint8 FRbuf[2] = {0};
    uint8 RLbuf[2] = {0};
    uint8 RRbuf[2] = {0};
    
// Write Range-Reading commands
    // Front Left
    I2C_1_I2CMasterWriteBuf(addrFL, &FLwrite, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    // check for transfor complete
    checkWriteComplete();
    I2C_1_I2CMasterWriteBuf(addrFL, &StartReading, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();

    // Front Right
    I2C_1_I2CMasterWriteBuf(addrFR, &FRwrite, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    // check for transfor complete
    checkWriteComplete();
    I2C_1_I2CMasterWriteBuf(addrFR, &StartReading, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();

    // Rear Left
    I2C_1_I2CMasterWriteBuf(addrRL, &FRwrite, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    // check for transfor complete
    checkWriteComplete();
    I2C_1_I2CMasterWriteBuf(addrRL, &StartReading, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();

    // Rear Right
    I2C_1_I2CMasterWriteBuf(addrRR, &FRwrite, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    // check for transfor complete
    checkWriteComplete();
    I2C_1_I2CMasterWriteBuf(addrRR, &StartReading, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();

    CyDelay(100);
 
// Read Range-Reading command
    // Front Left Sensor
    I2C_1_I2CMasterWriteBuf(addrFL, &FLread, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();
    I2C_1_I2CMasterReadBuf(addrFL, FLbuf, 2 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkReadComplete();
    sendBuffer[0] = FLbuf[0];
    sendBuffer[1] = FLbuf[1];
    //distanceFL = (FLbuf[0] << 8) + FLbuf[1];

    // Front Right Sensor
    I2C_1_I2CMasterWriteBuf(addrFR, &FRread, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();
    I2C_1_I2CMasterReadBuf(addrFR, FRbuf, 2 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkReadComplete();
    sendBuffer[2] = FRbuf[0];
    sendBuffer[3] = FRbuf[1];
    //distanceFR = (FRbuf[0] << 8) + FRbuf[1];

    // Rear Left Sensor
    I2C_1_I2CMasterWriteBuf(addrRL, &RLread, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();
    I2C_1_I2CMasterReadBuf(addrRL, RLbuf, 2 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkReadComplete();
    sendBuffer[4] = RLbuf[0];
    sendBuffer[5] = RLbuf[1];
    //distanceRL = (RLbuf[0] << 8) + RLbuf[1];
    
    // Rear Right Sensor
    I2C_1_I2CMasterWriteBuf(addrRR, &RRread, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();
    I2C_1_I2CMasterReadBuf(addrRR, RRbuf, 2 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkReadComplete();
    sendBuffer[6] = RRbuf[0];
    sendBuffer[7] = RRbuf[1];
    //distanceRR = (RRbuf[0] << 8) + RRbuf[1];
    
}

//############################### TACHOMETER ##########################
//Erklaering af variable til Tachometer
uint8 readBuffer[RD_BUFFERSIZE] = {0}; // Slave -> Master
double primaryTimer = 0;
double secondaryTimer = 0;
volatile double circumference = 0.1945; //omkreds maalt med skydelaere.
volatile double timeBetween = 0;
volatile double rpm = 0;
volatile double calcVelocity = 0;
uint32 timerPeriod;
CYBIT newDataTacho = 0;

// Tachometer eget interrupt
CY_ISR_PROTO(my_ISR);

//Deklarering af ISR.
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
}

// Initiation of components. 
void init(void){
    I2C_1_Start();
    I2C_1_I2CMasterClearStatus();
    Timer_1_Start();
    isr_1_StartEx(my_ISR);
    isr_1_ClearPending();
    Pin_1_ClearInterrupt();
    I2C_1_I2CMasterClearReadBuf();
    I2C_1_Start();
    timerPeriod = Timer_1_ReadPeriod();
    I2C_1_I2CSlaveInitReadBuf(sendBuffer, RD_BUFFERSIZE);
    CyGlobalIntEnable;
}

int main()
{   
    init();
    uint8 count = 0;
    for(;;){

        I2C_1_I2CSlaveClearReadBuf();
        //I2C_1_I2CSlaveClearReadStatus();
        while(!(I2C_1_I2CSlaveStatus() & I2C_1_I2C_SSTAT_RD_CMPLT)){} 
           
        if((I2C_1_I2CSlaveStatus() & I2C_1_I2C_SSTAT_RD_CMPLT)){
            sendBuffer[0] = 0;
            sendBuffer[1] = 10+count;
            sendBuffer[2] = 0; 
            sendBuffer[3] = 20+count;
            sendBuffer[4] = 0;
            sendBuffer[5] = 30+count;
            sendBuffer[6] = 0;
            sendBuffer[7] = 40+count;
            
            //getDistance();
            count++;
            if (calcVelocity < 25){
                sendBuffer[8] = (uint8)(calcVelocity*10);
            } 
            else { 
                sendBuffer[8] = 'X';
            }
            I2C_1_I2CSlaveClearReadStatus();
        }
    }
}

