#include <project.h>
#include "cyapicallbacks.h"
#include <stdbool.h>

#define PAYLOAD_SIZE    10
#define NRF_CHANNEL     76

volatile bool isrFlag = false;
volatile uint8_t pressCount = 0;
bool printFlag = false;
uint16_t ADCoutput;

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
    UART_Start();
    ADC_Start();

    CyDelay(50);
    
    UART_PutString("nRF Tx\n");
    
    NRF_INIT_t tx;
    tx.channel = NRF_CHANNEL;
    tx.isTX = true;
    tx.RF_SETUP_DR = NRF_RF_SETUP_RF_DR_1000;
    tx.RF_SETUP_PWR = NRF_RF_SETUP_RF_PWR_18;
    tx.SETUP_RETR_ARC = NRF_SETUP_RETR_ARC_15;
    tx.SETUP_RETR_ARD = NRF_SETUP_RETR_ARD_1500;
    
    /* Start the component before anything else */
    nRF_Tx_Start(&tx);
    
    /* Test Rx Payload */
    nRF_Tx_WriteSingleRegister(NRF_DYNPD, 0x01u);
    nRF_Tx_WriteSingleRegister(NRF_FEATURE, 0x06u);
    nRF_Tx_SetRxPayloadSize(NRF_RX_PW_P0, PAYLOAD_SIZE);
    
    nRF_Tx_SetRxAddress(ADDR, sizeof(ADDR));
    nRF_Tx_SetTxAddress(ADDR, sizeof(ADDR));

    for(;;){
        
        count++;
        if(10 == count){
            if(250 == pressCount){
                pressCount = 0;
            }
            if(250 == test){
                test = 0;
            }
            count = 0;
            test++;
            data[9] = test;
            data[0] = pressCount;
            ADCoutput = ADC_Read32();
            data[1] = (ADCoutput & 0xFF00) >> 8;
            data[2] = ADCoutput & 0xFF;
            nRF_Tx_TxTransmit(data, sizeof(data));
        }
        CyDelay(100);
        nRF_Tx_GetLostPackets(&test);
        if(0x0F == test){
            nRF_Tx_ResetStatusIRQ(NRF_STATUS_MAX_RT);
        }
        UART_PutHexByte(test);
        UART_PutCRLF();
        
        if(isrFlag){
            if(nRF_Tx_GetStatus() & NRF_STATUS_RX_DR_MASK){
                do{
                    nRF_Tx_RxPayload(RXdata, sizeof(RXdata));
                    nRF_Tx_ResetStatusIRQ(NRF_STATUS_RX_DR);
                    nRF_Tx_ReadSingleRegister(NRF_FIFO_STATUS, &status);
                }while(!(status & NRF_STATUS_TX_FIFO_FULL));
                printFlag = true;
            }else if(nRF_Tx_GetStatus() & NRF_STATUS_TX_DS_MASK){
                LED_Write(~LED_Read());
                nRF_Tx_ResetStatusIRQ(NRF_STATUS_TX_DS);
            }else if(nRF_Tx_GetStatus() & NRF_STATUS_MAX_RT_MASK){
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

/* [] END OF FILE */
