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

#if !defined (`$INSTANCE_NAME`_FUNCS_H)
#define `$INSTANCE_NAME`_FUNCS_H

#include <cytypes.h>
#include <cyfitter.h>
    
#include "`$INSTANCE_NAME`_regs.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct{
    bool isTX;
    uint8_t RF_SETUP_DR;
    uint8_t RF_SETUP_PWR;
    uint8_t SETUP_RETR_ARD;
    uint8_t SETUP_RETR_ARC;
    uint8_t channel;
}NRF_INIT_t;

void `$INSTANCE_NAME`_TransmitPulse(void);
void `$INSTANCE_NAME`_Listen(bool listen);
uint8_t `$INSTANCE_NAME`_GetStatus(void);
void `$INSTANCE_NAME`_PowerDownMode(void);
void `$INSTANCE_NAME`_StandbyIMode(void);
void `$INSTANCE_NAME`_StandbyIIMode(void);
void `$INSTANCE_NAME`_RxMode(void);
void `$INSTANCE_NAME`_TxMode(void);
void `$INSTANCE_NAME`_Start(NRF_INIT_t* init);
void `$INSTANCE_NAME`_SetRxAddress(uint8_t* addr, size_t addrSize);
void `$INSTANCE_NAME`_SetRxPipeAddress(uint8_t pipeNo, uint8_t* addr, size_t addrSize);
void `$INSTANCE_NAME`_SetTxAddress(uint8_t* addr, size_t addrSize);
void `$INSTANCE_NAME`_SetRxPayloadSize(uint8_t dataPipe, size_t payloadSize);
void `$INSTANCE_NAME`_FillTxFIFO(uint8_t* data, size_t payloadSize);
void `$INSTANCE_NAME`_TxTransmit(uint8_t* data, size_t payloadSize);
bool `$INSTANCE_NAME`_DataReady(void);
void `$INSTANCE_NAME`_SetChannel(uint8_t channel);
void `$INSTANCE_NAME`_ResetStatusIRQ(uint8_t flags);
void `$INSTANCE_NAME`_SetAutoACK(uint8_t pipe, uint8_t set); /* set = 1 to enable, set = 0 to disable the AutoACK */
void `$INSTANCE_NAME`_FlushRx(void);
void `$INSTANCE_NAME`_FlushTx(void);
void `$INSTANCE_NAME`_RxPayload(uint8_t* payload, size_t payloadSize);

/* New Functions */

void `$INSTANCE_NAME`_EnableDynamicPayload(uint8_t pipe);
void `$INSTANCE_NAME`_TxTransmitWaitNoACK(uint8_t* data, size_t payloadSize);
void `$INSTANCE_NAME`_RxWritePayload(uint8_t pipe, uint8_t* data, size_t payloadSize);
void `$INSTANCE_NAME`_ReuseTxPayload(void);
void `$INSTANCE_NAME`_ReadFromDataPipe(uint8_t* pipe);
void `$INSTANCE_NAME`_ResetIRQSource(uint8_t* source);
void `$INSTANCE_NAME`_GetRxFIFOPacketCount(uint8_t pipe, uint8_t* count);
void `$INSTANCE_NAME`_GetRetransmissionsCount(uint8_t* count);
void `$INSTANCE_NAME`_GetLostPackets(uint8_t* lostPackets);
void `$INSTANCE_NAME`_ReceivedPowerDetector(uint8_t* power); /* use only in Rx Mode */

/* Primitive Functions */
uint8_t `$INSTANCE_NAME`_ReadBit(uint8_t reg, uint8_t bit);
void `$INSTANCE_NAME`_ReadSingleRegister(uint8_t reg, uint8_t* data);
void `$INSTANCE_NAME`_ReadMultipleRegister(uint8_t reg, uint8_t* bufIn , size_t bufSize);
void `$INSTANCE_NAME`_WriteBit(uint8_t reg, uint8_t bit, uint8_t value);
void `$INSTANCE_NAME`_WriteSingleRegister(uint8_t reg, uint8_t data);
void `$INSTANCE_NAME`_WriteMultipleRegister(uint8_t reg, uint8_t* bufIn, size_t bufSize);
void writeBufferTest(uint8_t reg, uint8_t* buffer, size_t bufferSize);

#endif /* `$INSTANCE_NAME`_FUNCS_H */

/* [] END OF FILE */