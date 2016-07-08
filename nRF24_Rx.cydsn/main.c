#include <project.h>
#include "cyapicallbacks.h"

#include <stdbool.h>
#include <stdio.h>

#define PAYLOAD_SIZE    10
#define NRF_CHANNEL     76

volatile bool isrFlag = false;
volatile bool printFlag = false;

uint16_t adcResult;
char8 str[12];
float32 adcVolt;

uint8_t status = 0x00u,
        count = 0x00u,
        data[PAYLOAD_SIZE] = {0x01u, 0x02u, 0x03u, 0x04u, 0x05u},
        RXdata[PAYLOAD_SIZE] = {0x00u},
        RXPayload[PAYLOAD_SIZE] = {0xDEu, 0xADu, 0xBEu, 0xEFu, 0xC0u, 0xFFu, 0xEEu, 0xBAu, 0x0BE, 0xFFu},
        ADDR[5] = {0x11u, 0x22u, 0x33u, 0x44u, 0x55u};

int main()
{
    CyGlobalIntEnable;
    
    /* Start the UART for printing to console */
    UART_Start();
    /* Start the SW interrupt routine */
    isr_SW_Start();
    
    CyDelay(50);
    
    UART_PutString("nRF24 Receiver\n");
    
    /* Struct to configure the nRF24 */
    NRF_INIT_t rx;
    /* nRF24 working channel */
    rx.channel = NRF_CHANNEL;
    rx.isTX = false;
    /* Data rate */
    rx.RF_SETUP_DR = NRF_RF_SETUP_RF_DR_1000;
    /* Power */
    rx.RF_SETUP_PWR = NRF_RF_SETUP_RF_PWR_18;
    /**/
    rx.SETUP_RETR_ARC = NRF_SETUP_RETR_ARC_15;
    /**/
    rx.SETUP_RETR_ARD = NRF_SETUP_RETR_ARD_1500;

    /* Start the component, passing as parameter
    a NRF_INIT_t struct */
    nRF_Rx_Start(&rx);
    
    /* Test Rx Payload */
    nRF_Rx_EnableDynamicPayload(NRF_DYNPD_DPL_P0);

    nRF_Rx_SetRxPayloadSize(NRF_RX_PW_P0, PAYLOAD_SIZE);
    nRF_Rx_SetTxAddress(ADDR, sizeof(ADDR));
    nRF_Rx_SetRxAddress(ADDR, sizeof(ADDR));

    for(;;){
        if(isrFlag){
            /* We read the Status register and see if the cause of the IRQ is because data is ready */
            if(nRF_Rx_GetStatus() & NRF_STATUS_RX_DR_MASK){
                /* Stop listening */
                nRF_Rx_Listen(false);
                LED_Write(~LED_Read());
                do{
                    nRF_Rx_RxPayload(RXdata, sizeof(RXdata));
                    nRF_Rx_ResetStatusIRQ(NRF_STATUS_RX_DR);
                    nRF_Rx_ReadSingleRegister(NRF_FIFO_STATUS, &status);
                }while(!(status & NRF_STATUS_TX_FIFO_FULL));
                nRF_Rx_RxWritePayload(0, RXPayload, sizeof(RXPayload));
                nRF_Rx_Listen(true);
                printFlag = true;
            }else if(nRF_Rx_GetStatus() & NRF_STATUS_TX_DS_MASK){
                MAX_Write(~MAX_Read());
                nRF_Rx_ResetStatusIRQ(NRF_STATUS_TX_DS);
            }else if(nRF_Rx_GetStatus() & NRF_STATUS_MAX_RT_MASK){
                nRF_Rx_ResetStatusIRQ(NRF_STATUS_MAX_RT);
            }

            isrFlag = false;
        }
        
        if(printFlag){
            adcResult = (RXdata[1] << 8) | RXdata[2];
            adcVolt = (5 / 65536) * adcResult;
            sprintf(str, "%.4f   ", adcVolt);
            UART_PutHexByte(RXdata[0]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[1]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[2]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[3]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[4]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[5]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[6]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[7]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[8]);
            UART_PutString("\r\n");
            UART_PutHexByte(RXdata[9]);
            UART_PutString("\r\n");
            UART_PutString(str);
            UART_PutString("\r\n");
            printFlag = false;
        }

    }
}

void isr_SW_Interrupt_InterruptCallback(void){
    /* Clear PICU interrupt */
    SW_ClearInterrupt();  
}

void nRF_Rx_isrIRQ_Interrupt_InterruptCallback(void){
    isrFlag = true;
    /* Clear PICU interrupt */
    IRQ_ClearInterrupt();
}

/* [] END OF FILE */
