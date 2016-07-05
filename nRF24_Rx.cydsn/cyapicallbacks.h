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
#ifndef CYAPICALLBACKS_H
#define CYAPICALLBACKS_H
    
    /*Define your macro callbacks here */
    /*For more information, refer to the Macro Callbacks topic in the PSoC Creator Help.*/
    
    #define isr_SW_INTERRUPT_INTERRUPT_CALLBACK
    void isr_SW_Interrupt_InterruptCallback(void);
    
    #define nRF_Rx_isrIRQ_INTERRUPT_INTERRUPT_CALLBACK
    void nRF_Rx_isrIRQ_Interrupt_InterruptCallback(void);
    
#endif /* CYAPICALLBACKS_H */   
/* [] */
