
#include "task.h"
#include"usart.h"
#include"math.h"
#include "stm32_seq.h"
#include "stm32_lpm.h"
#include"app_conf.h"
#include"dbg_trace.h"
#include"stdio.h"




uint8_t send_data_timer_id;
extern UART_HandleTypeDef huart1;

void Send_Data_Task(void)
{
    float data[100];
    float PI = 3.14;
    uint8_t buffer[10];
    for (uint8_t i = 0; i < 100; i++) {
        data[i] = sin(2 * PI * i / 99);
        snprintf((char *)buffer, sizeof(buffer), "%.2f", data[i]);
        HAL_UART_Transmit(&huart1, buffer, strlen((char *)buffer), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t *)"\n", 1, HAL_MAX_DELAY);
        HAL_Delay(1);
    }

}


void Send_Data_Timer_Callback(void)
{
    UTIL_SEQ_SetTask(CFG_TASK_SEND_DATA_ID, CFG_SCH_PRIO_0);
    printf("Timer triggered! Setting task\n\r");
}


void Start_Send_Data_Timer(uint32_t timeout_ms)
{
    uint32_t timeout_ticks = timeout_ms * 1000 / CFG_TS_TICK_VAL;
    HW_TS_Start(send_data_timer_id, timeout_ticks);
    printf("Timer start!\n\r");
 }


void Task_Init(void)
{

	 UTIL_SEQ_RegTask(CFG_TASK_SEND_DATA_ID, UTIL_SEQ_RFU, Send_Data_Task) ;
	if( HW_TS_Create(CFG_TIM_PROC_ID_ISR, &send_data_timer_id, hw_ts_Repeated, Send_Data_Timer_Callback)==(hw_ts_Failed))
	 {
//		APP_DBG_MSG("Timer failed creating !\n");
	 }
	 Start_Send_Data_Timer(1000);
}


