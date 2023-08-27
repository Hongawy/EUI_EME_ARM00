#include "DIO.h"
#define RED_LED 1
#define BLUE_LED 2
#define GREEN_LED 3

uint8_t flag=0;
void GPIO_PortF_Handler(void){
    for(int i=0;i<10000;i++);//DEBOUNCING DELAY
    /*If SW1 was pressed*/
    if(GPIO_PORTF_RIS_R&0x01){
        flag=1;
    }
    /*If SW2 was pressed*/
    else if(GPIO_PORTF_RIS_R&0x10){
        flag=2;
    }
    GPIO_PORTF_ICR_R|=(0x11);//CLEAR THE INTERRUPT FLAG FOR SW1 and SW2
}
void main(void)
{
    uint8_t prev='R';
    DIO_Init(PORTF,0X0E,0X11);//PIN0,4 INPUTS , PIN1,2,3 OUTPUTS
    NVIC_EN0_R |= (1<<30); //ENABLES INTERRUPT NUMBER 30 (GPIO PORT F)
    GPIO_PORTF_IS_R &=~(0x11);//EDGE SENSITIVE FOR SW1 and SW2
    GPIO_PORTF_IEV_R &=~(0x11);//FALLING EDGE SENSITIVE FOR SW1 and SW2
    GPIO_PORTF_IM_R|=(0x11);//WHEN INTERRUPT IS TRIGGERED, IT GOES TO THE ISR FOR SW1 and SW2
    GPIO_PORTF_ICR_R|=(0x11);//CLEAR THE INTERRUPT FLAG JUST IN CASE FOR SW1 and SW2
    __asm(" CPSIE  I "); //ENABLES GLOBAL INTERRUPTS
    SET_BIT(GPIO_PORTF_DATA_R,RED_LED);//TURNS ON RED LED

    while(1){
         //__asm("           wfi\n");
        if(flag==1 && prev=='R'){
            flag=0;
            CLEAR_BIT(GPIO_PORTF_DATA_R,RED_LED);
            SET_BIT(GPIO_PORTF_DATA_R,BLUE_LED);
            prev='B';
            continue;
        }
        else if(flag==1 && prev=='B'){
            flag=0;
            CLEAR_BIT(GPIO_PORTF_DATA_R,BLUE_LED);
            SET_BIT(GPIO_PORTF_DATA_R,GREEN_LED);
            prev='G';
            continue;
        }
        else if(flag==1 && prev=='G'){
            flag=0;
            CLEAR_BIT(GPIO_PORTF_DATA_R,GREEN_LED);
            SET_BIT(GPIO_PORTF_DATA_R,RED_LED);
            prev='R';
            continue;
        }
        else if(flag==2 && prev=='R'){
            flag=0;
            CLEAR_BIT(GPIO_PORTF_DATA_R,RED_LED);
            SET_BIT(GPIO_PORTF_DATA_R,GREEN_LED);
            prev='G';
            continue;
        }
        else if(flag==2 && prev=='G'){
            flag=0;
            CLEAR_BIT(GPIO_PORTF_DATA_R,GREEN_LED);
            SET_BIT(GPIO_PORTF_DATA_R,BLUE_LED);
            prev='B';
            continue;
        }
        else if(flag==2 && prev=='B'){
            flag=0;
            CLEAR_BIT(GPIO_PORTF_DATA_R,BLUE_LED);
            SET_BIT(GPIO_PORTF_DATA_R,RED_LED);
            prev='R';
            continue;
        }

    }
}

