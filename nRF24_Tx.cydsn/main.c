#include <project.h>
#include "cyapicallbacks.h"
#include <stdbool.h>

#define PAYLOAD_SIZE    10
#define NRF_CHANNEL     76

volatile bool isrFlag = false;
volatile bool isrTimerFlag = false;
volatile uint8_t pressCount = 10;
bool printFlag = false;
uint32_t ADCoutput;

uint8_t status = 0x00u,
        count = 0x00u,
        test = 0,
        data[PAYLOAD_SIZE]  = {0x00u, 0x02u, 0x03u, 0x04u, 0x05u,
                               0xDEu, 0xADu, 0xBEu, 0xEFu, 0xFFu},
        RXdata[PAYLOAD_SIZE]= {0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                0x00u, 0x00u, 0x00u, 0x00u, 0x00u},
        ADDR[5] = {0x11u, 0x22u, 0x33u, 0x44u, 0x55u};

int main(){

    CyGlobalIntEnable;
    isrSW_Start();
    isr_Timer_Start();
    UART_Start();
    ADC_Start();
    ADC_StartConvert();

    CyDelay(50);
    
    UART_PutString("nRF Tx\r\n");
    
    NRF_INIT_t tx;
    tx.channel = NRF_CHANNEL;
    tx.isTX = true;
    tx.RF_SETUP_DR = NRF_RF_SETUP_RF_DR_1000;
    tx.RF_SETUP_PWR = NRF_RF_SETUP_RF_PWR_18;
    tx.SETUP_RETR_ARC = NRF_SETUP_RETR_ARC_15;
    tx.SETUP_RETR_ARD = NRF_SETUP_RETR_ARD_1500;
    
    /* Start the component before anything else */
    nRF_Tx_Start(&tx);
    
    /* Test Dynamic Payload */
    nRF_Tx_EnableDynamicPayload(NRF_DYNPD_DPL_P0);

    nRF_Tx_SetRxPayloadSize(NRF_RX_PW_P0, PAYLOAD_SIZE);
    
    nRF_Tx_SetRxAddress(ADDR, sizeof(ADDR));
    nRF_Tx_SetTxAddress(ADDR, sizeof(ADDR));
    
    Timer_Start();

    for(;;){
        
        if(true == isrTimerFlag){
            // Stop the Timer just for fun
            Timer_Stop();

            test++;
            data[0] = pressCount;
            data[9] = test;
            ADCoutput = ADC_Read32();
            UART_PutString("Resultado de la conversion del ADC: ");
            UART_PutHexInt((uint16_t)ADCoutput);
            UART_PutCRLF();
            data[1] = (ADCoutput & 0xFF00) >> 8;
            data[2] = ADCoutput & 0xFF;
            nRF_Tx_TxTransmit(data, sizeof(data));
            isrTimerFlag = false;

            // Start the Timer again
            Timer_Start();
        }

        if(isrFlag){
            
            if(nRF_Tx_GetStatus() & NRF_STATUS_RX_DR_MASK){
                UART_PutString("Status: ");
                UART_PutHexByte(nRF_Tx_GetStatus());
                UART_PutCRLF();
                UART_PutString("RX\r\n");
                do{
                    nRF_Tx_RxPayload(RXdata, sizeof(RXdata));
                    nRF_Tx_ResetStatusIRQ(NRF_STATUS_RX_DR);
                    nRF_Tx_ReadSingleRegister(NRF_FIFO_STATUS, &status);
                }while(!(status & NRF_STATUS_TX_FIFO_FULL));
                printFlag = true;
            }else if(nRF_Tx_GetStatus() & NRF_STATUS_TX_DS_MASK){
                UART_PutString("Status: ");
                UART_PutHexByte(nRF_Tx_GetStatus());
                UART_PutCRLF();
                UART_PutString("TX\r\n");
                LED_Write(~LED_Read());
                nRF_Tx_ResetStatusIRQ(NRF_STATUS_TX_DS);
            }else if(nRF_Tx_GetStatus() & NRF_STATUS_MAX_RT_MASK){
                UART_PutString("Status: ");
                UART_PutHexByte(nRF_Tx_GetStatus());
                UART_PutCRLF();
                UART_PutString("Max RT\r\n");
                MAX_Write(~MAX_Read());
                nRF_Tx_ResetStatusIRQ(NRF_STATUS_MAX_RT);
            }

            isrFlag = false;
        }
        
        if(printFlag){
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
            printFlag = false;
        }

    }
}

void isrSW_Interrupt_InterruptCallback(void){
    pressCount++;
    /* Clear the PICU interrupt */
    SW_ClearInterrupt();
}

void nRF_Tx_isrIRQ_Interrupt_InterruptCallback(void){
    isrFlag = true;
    /* Clear the PICU interrupt */
    IRQ_ClearInterrupt();
}

void isr_Timer_Interrupt_InterruptCallback(void){
    UART_PutString("Timer\r\n");
    /* Read and clear the Timer status register */
    Timer_STATUS;
    isrTimerFlag = true;
}

/* [] END OF FILE */
