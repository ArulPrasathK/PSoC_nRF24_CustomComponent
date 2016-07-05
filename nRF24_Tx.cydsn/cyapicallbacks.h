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
    
    /* Callback for the interrupt on the board switch */
    #define isrSW_INTERRUPT_INTERRUPT_CALLBACK
    void isrSW_Interrupt_InterruptCallback(void);
    
    /**
    * Intern isr callback function, this is triggered on a 
    * falling edge on the IRQ pin
    **/
    #define nRF_Tx_isrIRQ_INTERRUPT_INTERRUPT_CALLBACK
    void nRF_Tx_isrIRQ_Interrupt_InterruptCallback(void);
    
#endif /* CYAPICALLBACKS_H */   
/* [] */
