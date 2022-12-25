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
#include "project.h"
#include <stdio.h>

#define UART_HW ((CySCB_Type *) UART_SCB__HW)

#define FREQ 12000
#define TIME 10
#define SAMPLES FREQ*TIME

int write_voice();
int play_voice();

int8_t voice[SAMPLES];

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    
    UART_Start();
    CapSense_Start();
    setvbuf(stdin, NULL, _IONBF, 0);
    
    printf("\x1b[2J\x1b[H");
    printf("------------------- CapSense VoiceRepeater log -------------------\r\n\n");

    for(;;)
    {
        CapSense_ScanAllWidgets();
        if (!CapSense_IsBusy()) {
            CapSense_ProcessAllWidgets();
            if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)) {
                write_voice();
            }
            if (CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)) {
                play_voice();
            }
        }
    }
}

int write_voice()
{
    Cy_GPIO_Write(BLUE_PORT, BLUE_NUM, 0);
    printf("Started voice recording...\r\n");
    
    ADC_Start();
    ADC_StartConvert();
    
    for(int idx = 0; idx < SAMPLES; ++idx){
        ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT);
        int16_t ADC_Result = ADC_GetResult16(0);
        int16_t Mic_mV = ADC_CountsTo_mVolts(0, ADC_Result);
        voice[idx] = (int8_t) Mic_mV - 1611;
    }
    
    ADC_Stop();
    ADC_StopConvert();
    
    Cy_GPIO_Write(BLUE_PORT, BLUE_NUM, 1);
    printf("Finished voice recording.\r\n\n");
    
    return 0;
}

int play_voice()
{
    Cy_GPIO_Write(RED_PORT, RED_NUM, 0);
    printf("Started voice playing...\r\n");

    for(int idx = 0; idx < SAMPLES; idx++) {
        char8 text[8];
        sprintf(text, "%d", voice[idx]);
        printf("%s\r\n", text);
    }
    
    Cy_GPIO_Write(BLUE_PORT, RED_NUM, 1);
    printf("Finished voice playing.\r\n\n");

    return 0;
}

/* [] END OF FILE */
