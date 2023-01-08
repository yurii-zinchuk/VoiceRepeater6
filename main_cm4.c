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

#define FREQ 8000
#define TIME 8
#define SAMPLES FREQ*TIME

int write_voice();
int get_codes();
int transform_codes();

int play_voice();
void no_recording_handler();

uint16_t average;
bool recorded = false;
int16_t codes[SAMPLES];
//int8_t voice[SAMPLES];


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
                if (recorded) play_voice();
                else no_recording_handler();
            }
        }
    }
}


int write_voice()
{
    Cy_GPIO_Write(BLUE_PORT, BLUE_NUM, 0);
    printf("Started voice recording...\r\n");
    
    get_codes();
    transform_codes();
    recorded = true;
    
    Cy_GPIO_Write(BLUE_PORT, BLUE_NUM, 1);
    printf("Finished voice recording.\r\n\n");
    
    return 0;
}


int get_codes()
{
    ADC_Start();
    ADC_StartConvert();
    
    for (int i = 0; i < SAMPLES; ++i)
    {
        ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT);
        uint16_t ADC_Result = ADC_GetResult16(0);

        codes[i] = ADC_Result;

        /*
        int16_t Mic_mV = ADC_CountsTo_mVolts(0, ADC_Result);
        voice[i] = (int8_t) Mic_mV - 1600;
        */
    }
    
    ADC_StopConvert();
    ADC_Stop();
    
    return 0;
}


int transform_codes()
{
    uint32_t sum = 0;
    for (int i = 0; i < SAMPLES; ++i)
        sum += codes[i];
        
    average = sum / SAMPLES;

    uint16_t max_amp = 0;
    for (int i = 0; i < SAMPLES; ++i)
    {
        codes[i] = codes[i] - average;

        if (codes[i] > 0 && codes[i] > max_amp)
            max_amp = codes[i];
        else if (codes[i] < 0 && -codes[i] > max_amp)
            max_amp = -codes[i];
    }
    
    for (int i = 0; i < SAMPLES; ++i)
        codes[i] = 2048 + (2047 * codes[i] / max_amp);
        
    return 0;
}


int play_voice()
{
    Cy_GPIO_Write(RED_PORT, RED_NUM, 0);
    printf("Started voice playing...\r\n");
    
    VDAC_1_Start();

    for (int i = 0; i < SAMPLES; ++i)
    {
        VDAC_1_SetValue(codes[i]);
        
        CyDelayUs(125);
        
        /*
        char8 text[8];
        sprintf(text, "%d", voice[idx]);
        printf("%s\r\n", text);

        int32_t value = 10*voice[idx] + 1650;
        int32_t value = 2048 + codes[idx];
        */
    }
    
    VDAC_1_Stop();
    
    Cy_GPIO_Write(RED_PORT, RED_NUM, 1);
    printf("Finished voice playing.\r\n\n");

    return 0;
}


void no_recording_handler()
{
    printf("No recording. Record voice first.\r\n\n");

    for (int i = 0; i < 3; ++i) {
        Cy_GPIO_Write(RED_PORT, RED_NUM, 0);
        CyDelay(150);
        
        Cy_GPIO_Write(RED_PORT, RED_NUM, 1);
        CyDelay(150);
    }
}

/* [] END OF FILE */
