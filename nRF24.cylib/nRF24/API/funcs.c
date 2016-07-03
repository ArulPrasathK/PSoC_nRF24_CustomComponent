/****************************************************************************
* This file is part of the PSoC Creator nRF24 Custom Component project.
*
* Copyright (C) 2016 Carlos Diaz <carlos.santiago.diaz@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
****************************************************************************/

#include <project.h>
#include <cytypes.h>

#include "`$INSTANCE_NAME`_funcs.h"
#include "`$INSTANCE_NAME`_regs.h"

void `$INSTANCE_NAME`_TransmitPulse(void){
    `$INSTANCE_NAME`_Ctrl_CE_Write(1);
    CyDelayUs(15);
    `$INSTANCE_NAME`_Ctrl_CE_Write(0);
}

void `$INSTANCE_NAME`_Listen(bool listen){
    if(listen){
        /* SET_CE */
        `$INSTANCE_NAME`_Ctrl_CE_Write(1);
    }else{
        `$INSTANCE_NAME`_Ctrl_CE_Write(0);
    }
}

/**
 * @brief Funcion para obtener el registro STATUS.
 * @return  Valor del registro STATUS.
 */
uint8_t `$INSTANCE_NAME`_GetStatus(void){
    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteByte(NRF_NOP);
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
    
	return (uint8_t)`$INSTANCE_NAME`_SPI_ReadRxData();
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_PowerDownMode(void){
	`$INSTANCE_NAME`_WriteBit(NRF_CONFIG, 1, 0);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_StandbyIMode(void){
	`$INSTANCE_NAME`_Listen(false);
	`$INSTANCE_NAME`_WriteBit(NRF_CONFIG, 1, 0);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_StandbyIIMode(void){
    `$INSTANCE_NAME`_Listen(true);
	`$INSTANCE_NAME`_WriteBit(NRF_CONFIG, 1, 1);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_RxMode(void){
    `$INSTANCE_NAME`_FlushRx();
    `$INSTANCE_NAME`_FlushTx();
    /*=========================================================
    * To enter this mode, nRF24 must have PWR_UP bit set high,
    * PRIM_RX bit set high and the CE pin set high
    =========================================================*/
    `$INSTANCE_NAME`_WriteSingleRegister(NRF_CONFIG, 0x07u);
    `$INSTANCE_NAME`_Listen(true);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_TxMode(void){
    `$INSTANCE_NAME`_FlushRx();
    `$INSTANCE_NAME`_FlushTx();
    /*=========================================================
    * To enter this mode, nRF24 must have PWR_UP bit set high,
    * PRIM_RX bit set low and the CE pin set high
    =========================================================*/
    `$INSTANCE_NAME`_WriteSingleRegister(NRF_CONFIG, 0x06u);
    `$INSTANCE_NAME`_Listen(true);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_Start(NRF_INIT_t* init){
    `$INSTANCE_NAME`_SetChannel(init->channel);
    `$INSTANCE_NAME`_WriteSingleRegister(NRF_RF_SETUP, init->RF_SETUP_DR | init->RF_SETUP_PWR);
    `$INSTANCE_NAME`_WriteSingleRegister(NRF_SETUP_RETR, init->SETUP_RETR_ARD | init->SETUP_RETR_ARC);
    if(init->isTX){
        `$INSTANCE_NAME`_TxMode();
    }else{
        `$INSTANCE_NAME`_RxMode();
    }
    `$INSTANCE_NAME`_SPI_Start();
    return;
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_SetRxAddress(uint8_t* addr, size_t addrSize){
    `$INSTANCE_NAME`_WriteMultipleRegister(NRF_RX_ADDR_P0, addr, addrSize);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_SetRxPipeAddress(uint8_t pipeNo, uint8_t* addr, size_t addrSize){
    uint8_t reg = 0x00u;
    
    switch (pipeNo){
        case 0:
            reg = NRF_RX_ADDR_P0;
        break;
        case 1:
            reg = NRF_RX_ADDR_P1;
        break;
        case 2:
            reg = NRF_RX_ADDR_P2;
            addrSize = 1;
        break;
        case 3:
            reg = NRF_RX_ADDR_P3;
            addrSize = 1;
        break;        
        case 4:
            reg = NRF_RX_ADDR_P4;
            addrSize = 1;
        break;
        case 5:
            reg = NRF_RX_ADDR_P5;
            addrSize = 1;
        break;
        default:
            reg = NRF_RX_ADDR_P0;
        break;
        
    }
        
    `$INSTANCE_NAME`_WriteMultipleRegister(reg, addr, addrSize);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_SetTxAddress(uint8_t* addr, size_t addrSize){
    `$INSTANCE_NAME`_WriteMultipleRegister(NRF_TX_ADDR, addr, addrSize);
}

/**
 * @brief Configuramos la cantidad de informacion que recibira el Rx.
 * @detail Solo usar en el RX
 * @param dataPipe  Numero del pipe a elegir.
 * @param payloadSize   Cantidad de informacion que vamos a recibir en dicho pipe.
 */
void `$INSTANCE_NAME`_SetRxPayloadSize(uint8_t dataPipe, size_t payloadSize){
    `$INSTANCE_NAME`_WriteSingleRegister(dataPipe, (payloadSize & 0x3Fu));
}

/*
  @brief Llenamos el TX FIFO con la informacion a mandar.
  @param data  Puntero al array que contiene la informacion.
  @param payloadSize   Tamaño del array a mandar.
*/

void `$INSTANCE_NAME`_FillTxFIFO(uint8_t* data, size_t payloadSize){
    uint8_t i = 0;

    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_W_TX_PAYLOAD);
    for(; i < payloadSize; i++){
        `$INSTANCE_NAME`_SPI_WriteTxData(data[i]);
    }
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}


/**
 * @brief Funcion para mandar un array.
 * @param data  Puntero al array que contiene la informacion a mandar.
 * @param payloadSize   Tamaño del array a mandar.
 */
void `$INSTANCE_NAME`_TxTransmit(uint8_t* data, size_t payloadSize){
    uint8_t i = 0;

    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_W_TX_PAYLOAD);
    for(; i < payloadSize; i++){
        `$INSTANCE_NAME`_SPI_WriteTxData(data[i]);
    }
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
    `$INSTANCE_NAME`_TransmitPulse();
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
bool `$INSTANCE_NAME`_DataReady(void){
    uint8_t data;
    `$INSTANCE_NAME`_ReadSingleRegister(NRF_STATUS, &data);
    if(NRF_STATUS_DATA_IS_RDY == data){
        return true;
    }else{
        return false;    
    }
}

/**
 * @brief Elegimos el canal sobre el cual trabajaremos.
 * @param channel   Elegimos el canal sobre el que vamos a trabajar.
 */
void `$INSTANCE_NAME`_SetChannel(uint8_t channel){
    `$INSTANCE_NAME`_WriteSingleRegister(NRF_RF_CH, (channel & 0x7Fu));
}

/**
 * @brief Limpiamos el IRQflag.
 * @param flags Flag que vamos a limpiar.
 */
void `$INSTANCE_NAME`_ResetStatusIRQ(uint8_t flags){
        `$INSTANCE_NAME`_WriteBit(NRF_STATUS, flags, 1);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_SetAutoACK(uint8_t pipe, uint8_t set){ /* set = 1 to enable, set = 0 to disable the AutoACK */
    `$INSTANCE_NAME`_WriteBit(NRF_EN_AA, pipe, set);
}

/**
 * @brief Vaciamos el RX FIFO.
 */
void `$INSTANCE_NAME`_FlushRx(void){
    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_FLUSH_RX);
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}

/**
 * @brief Vaciamos el TX FIFO.
 */
void `$INSTANCE_NAME`_FlushTx(void){
    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_FLUSH_TX);
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}

/**
 * @brief Pin event handler prototype.
 * @param pin    Pin that triggered this event.
 * @param action Action that lead to triggering this event.
 */
void `$INSTANCE_NAME`_RxPayload(uint8_t* payload, size_t payloadSize){
    `$INSTANCE_NAME`_ReadMultipleRegister(NRF_R_RX_PAYLOAD, payload, payloadSize);
}

/* New Functions ======================*/

void `$INSTANCE_NAME`_EnableDynamicPayload(uint8_t pipe){
    `$INSTANCE_NAME`_WriteSingleRegister(NRF_DYNPD, pipe);
}

/* EN_DYN_ACK bit on FEATURE register must be set high to enables
*  the W_TX_PAYLOAD_NOACK command. */

void `$INSTANCE_NAME`_TxTransmitWaitNoACK(uint8_t* data, size_t payloadSize){
    uint8_t i = 0;

    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_W_TX_PAYLOAD_NOACK);
    for(; i < payloadSize; i++){
        `$INSTANCE_NAME`_SPI_WriteTxData(data[i]);
    }
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
    `$INSTANCE_NAME`_TransmitPulse();
}

void `$INSTANCE_NAME`_RxWritePayload(uint8_t pipe, uint8_t* data, size_t payloadSize){
    uint8_t i = 0;

    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_W_ACK_PAYLOAD | (pipe & 0x7));
    for(; i < payloadSize; i++){
        `$INSTANCE_NAME`_SPI_WriteTxData(data[i]);
    }
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}

void `$INSTANCE_NAME`_ReuseTxPayload(void){
    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_REUSE_TX_PL);
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}

void `$INSTANCE_NAME`_ReadFromDataPipe(uint8_t* pipe){
    `$INSTANCE_NAME`_ReadSingleRegister(NRF_STATUS, pipe);
    *pipe = (*pipe & 0x0Eu) >> 1;
}

void `$INSTANCE_NAME`_ResetIRQSource(uint8_t* source){
    
    if(`$INSTANCE_NAME`_GetStatus() & NRF_STATUS_RX_DR_MASK){ /* RX_DR: Data Received */
        *source = NRF_STATUS_RX_DR;
        `$INSTANCE_NAME`_WriteSingleRegister(NRF_STATUS, NRF_STATUS_RX_DR_MASK);
    }else if(`$INSTANCE_NAME`_GetStatus() & NRF_STATUS_TX_DS_MASK){ /* TX_DS: Data Sent */
        *source = NRF_STATUS_TX_DS;
        `$INSTANCE_NAME`_WriteSingleRegister(NRF_STATUS, NRF_STATUS_TX_DS_MASK);
    }else if(`$INSTANCE_NAME`_GetStatus() & NRF_STATUS_MAX_RT_MASK){ /* MAX_RT: Retry Timeout */
        *source = NRF_STATUS_MAX_RT;
        `$INSTANCE_NAME`_WriteSingleRegister(NRF_STATUS, NRF_STATUS_MAX_RT_MASK);
    }
}

/* Get the number of packets residing in the Rx FIFO on the specific pipe */
void `$INSTANCE_NAME`_GetRxFIFOPacketCount(uint8_t pipe, uint8_t* count){
    
    switch(pipe){
        case 0:
            pipe = NRF_RX_PW_P0;
        break;
        case 1:
            pipe = NRF_RX_PW_P1;
        break;
        case 2:
            pipe = NRF_RX_PW_P2;
        break;
        case 3:
            pipe = NRF_RX_PW_P3;
        break;
        case 4:
            pipe = NRF_RX_PW_P4;
        break;
        case 5:
            pipe = NRF_RX_PW_P5;
        break;
        default:
            pipe = NRF_RX_ADDR_P0;
        break;
    }
        
    `$INSTANCE_NAME`_ReadSingleRegister(pipe, count);
}

/**
 * @brief 
 * @param 
 * @param 
 */
void `$INSTANCE_NAME`_ReceivedPowerDetector(uint8_t* power){
    `$INSTANCE_NAME`_ReadSingleRegister(NRF_RPD, power); 
}

/**
 * @brief 
 * @param 
 * @param 
 */
void `$INSTANCE_NAME`_GetRetransmissionsCount(uint8_t* count){
    `$INSTANCE_NAME`_ReadSingleRegister(NRF_OBSERVE_TX, count);
    *count &= NRF_OBSERVE_TX_ARC_CNT_MASK;
}

/**
 * @brief 
 * @param 
 * @param 
 */
void `$INSTANCE_NAME`_GetLostPackets(uint8_t* lostPackets){
    `$INSTANCE_NAME`_ReadSingleRegister(NRF_OBSERVE_TX, lostPackets);
    *lostPackets &= NRF_OBSERVE_TX_PLOS_CNT_MASK;
    *lostPackets >>= 4;
}

/* Primitive Functions */

uint8_t `$INSTANCE_NAME`_ReadBit(uint8_t reg, uint8_t bit){
    if(!((0 <= bit) && (bit <= 7))){
        return 0x02;    
    }
	uint8_t temp = 0x00u;
    
    `$INSTANCE_NAME`_ReadSingleRegister(reg, &temp);
    if(!(temp & (1 << bit))){
        return 0x00u;
    }else{
        return 0x01u;
    }
}

void `$INSTANCE_NAME`_ReadSingleRegister(uint8_t reg, uint8_t* data){
    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_R_REGISTER | reg);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_NOP);
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
    (void)`$INSTANCE_NAME`_SPI_ReadRxData(); /* Dummy read, this is the STATUS Register */
    *data = `$INSTANCE_NAME`_SPI_ReadRxData();
}

void `$INSTANCE_NAME`_ReadMultipleRegister(uint8_t reg, uint8_t* bufIn , size_t bufSize){
	uint8_t i = 0, j = 0;
	
    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_R_REGISTER | reg);
    for(; i < bufSize; i++){
        `$INSTANCE_NAME`_SPI_WriteTxData(NRF_NOP);
    }
    (void)`$INSTANCE_NAME`_SPI_ReadRxData(); /* Dummy read, this is the STATUS Register */
    for(; j < bufSize; j++){
        *(bufIn + j) = `$INSTANCE_NAME`_SPI_ReadRxData();
    }
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}

void `$INSTANCE_NAME`_WriteBit(uint8_t reg, uint8_t bit, uint8_t value){
    if(!((0 <= bit) && (bit <= 7))){
        return;    
    }

	uint8_t temp = 0x00u;
    
    `$INSTANCE_NAME`_ReadSingleRegister(reg, &temp);
    if(value){
        temp |= (0x01u << bit);    
    }else{
        temp &= ~(0x01u << bit);
    }
    `$INSTANCE_NAME`_WriteSingleRegister(reg, temp);
}

void `$INSTANCE_NAME`_WriteSingleRegister(uint8_t reg, uint8_t data){
    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_W_REGISTER | reg);
    `$INSTANCE_NAME`_SPI_WriteTxData(data);
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}

void `$INSTANCE_NAME`_WriteMultipleRegister(uint8_t reg, uint8_t* bufIn, size_t bufSize){
	uint8_t i = 0;

    `$INSTANCE_NAME`_SPI_ClearRxBuffer();
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_WriteTxData(NRF_W_REGISTER | reg);
    for(; i < bufSize; i++){
        `$INSTANCE_NAME`_SPI_WriteTxData(bufIn[i]);
    }
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}


void writeBufferTest(uint8_t reg, uint8_t* bufIn, size_t bufSize){
    `$INSTANCE_NAME`_SPI_ClearRxBuffer();

    uint8_t auxBuffer[(bufSize + 1)];
    
    for(; bufSize > 0; bufSize--, bufSize--){
        auxBuffer[bufSize] = bufIn[bufSize];
    }
    
    auxBuffer[0] = (uint8_t)(NRF_W_REGISTER | reg);
    `$INSTANCE_NAME`_Ctrl_SS_Write(0);
    `$INSTANCE_NAME`_SPI_PutArray(auxBuffer, (bufSize + 1));
    while(!(`$INSTANCE_NAME`_SPI_ReadTxStatus() & `$INSTANCE_NAME`_SPI_STS_SPI_IDLE));
    `$INSTANCE_NAME`_Ctrl_SS_Write(1);
}

/* [] END OF FILE */
