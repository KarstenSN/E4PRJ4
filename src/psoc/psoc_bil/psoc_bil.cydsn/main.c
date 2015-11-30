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
#define RD_BUFFERSIZE 2u
#define fclk 400000 //400kHz timer counter clk

/*################### DISTANCESENSOR #############################*/
// DistanceSensor variable definition
// Define af adresser
uint8 addrFL = 0b1110000;	// 0x70 dec 112
uint8 addrFR = 0b1110001;	// 0x71 dec 113
uint8 addrRL = 0b1110011;	// 0x73 dec 115
uint8 addrRR = 0b1110110;	// 0x76 dec 118

// Initiering af distancer
uint16 distanceFL = 0;	
uint16 distanceFR = 0;
uint16 distanceRL = 0;
uint16 distanceRR = 0;

uint8 FLwrite = 0b11100000;
uint8 FRwrite = 0b11100010;	
uint8 RLwrite = 0b11100110;	
uint8 RRwrite = 0b11101100;	
uint8 FLread  = 0b11100001;
uint8 FRread  = 0b11100011;	
uint8 RLread  = 0b11100111;	
uint8 RRread  = 0b11101101;

// Commando for start RangeReading
uint8 StartReading   = 0b01010001;  // 0x51 dec 81  

// Wait till complete functions.
void checkWriteComplete(void){
    while(0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_WR_CMPLT));
    CyDelay(60);
}

void checkReadComplete(void){
    while(0u == (I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_RD_CMPLT));
}

// getDistance()
void getDistance(void){
    
    uint8 FLbuf[2];
    uint8 FRbuf[2];
    uint8 RLbuf[2];
    uint8 RRbuf[2];
    
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
    
 
// Read Range-Reading command
    // Front Left
    I2C_1_I2CMasterWriteBuf(addrFL, &FLread, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();
    I2C_1_I2CMasterReadBuf(addrFL, FLbuf, 2 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkReadComplete();
    distanceFL = (FLbuf[0] << 8) + FLbuf[1];

    // Front Right
    I2C_1_I2CMasterWriteBuf(addrFR, &FRread, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();
    I2C_1_I2CMasterReadBuf(addrFR, FRbuf, 2 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkReadComplete();
    distanceFR = (FRbuf[0] << 8) + FRbuf[1];

    // Rear Left
    I2C_1_I2CMasterWriteBuf(addrRL, &RLread, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();
    I2C_1_I2CMasterReadBuf(addrRL, RLbuf, 2 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkReadComplete();
    distanceFR = (RLbuf[0] << 8) + RLbuf[1];
    
    // Rear Right
    I2C_1_I2CMasterWriteBuf(addrRR, &RRread, 1 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkWriteComplete();
    I2C_1_I2CMasterReadBuf(addrRR, RRbuf, 2 ,I2C_1_I2C_MODE_COMPLETE_XFER  );
    checkReadComplete();
    distanceRR = (RRbuf[0] << 8) + RRbuf[1];
}

// I2C initiation 
void I2C_init(void){
    I2C_1_Start();
    I2C_1_I2CMasterClearStatus();
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
CYBIT newData = 0;

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

    newData = 1;
}

int main()
{
    Timer_1_Start();
    isr_1_StartEx(my_ISR);
    isr_1_ClearPending();
    Pin_1_ClearInterrupt();
    I2C_1_I2CMasterClearReadBuf();
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
        
            I2C_1_I2CMasterClearReadBuf(); //Reset read buffer pointer  (Resetter IKKE data...)
            newData = 0;
        }
    }
}
