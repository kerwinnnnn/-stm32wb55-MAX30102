#ifndef TASK_H
#define TASK_H

#include "app_common.h"



#define CFG_TASK_SEND_DATA_ID      (1 << 0) 


#define CFG_TIM_PROC_ID_ISR        (0)


extern uint8_t send_data_timer_id;


void Task_Init(void);
void Start_Send_Data_Timer(uint32_t timeout_ms);
void Send_Data_Task(void);

#endif
