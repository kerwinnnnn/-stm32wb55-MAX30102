#include "algorithm.h"
#include <math.h>
#include <stdlib.h>
#include <float.h>

#define ALPHA 0.3  // 直流滤波系数
#define WINDOW_SIZE 5  // 均值滤波窗口大小
#define THRESHOLD_PERIODS 3  // 动态阈值计算的周期数

static float dc_remove(float x, float *prev_w) {
    float y = x - *prev_w + ALPHA * (*prev_w);
    *prev_w = x;
    return y;
}

static float moving_average_filter(float input, uint16_t index) {
    static float buffer[500] = {0};
    static float sum = 0;
    static uint16_t count = 0;

    if (count >= WINDOW_SIZE) {
        sum -= buffer[index % WINDOW_SIZE];
    } else {
        count++;
    }

    buffer[index % WINDOW_SIZE] = input;
    sum += input;

    return sum / count;
}

void calculate_hr_spo2(uint32_t* ir_data, uint32_t* red_data, uint16_t length,uint8_t* heart_rate, uint8_t* spo2, uint8_t Fs)
{


    static float filtered_ir[500];
    float ir_prev_w = 0;
    // 计算直流滤波+均值滤波
    for (uint16_t i = 0; i < length; i++) {
        float ir_filtered = dc_remove(ir_data[i], &ir_prev_w);
        filtered_ir[i] = moving_average_filter(ir_filtered, i);
    }


   //计算心率
    float ir_data_sum_aver;
    uint16_t temp,i;

   for(i=0;i<length;i++)
   {
	   ir_data_sum_aver+=filtered_ir[i];
   }
   ir_data_sum_aver=ir_data_sum_aver/length;

    //寻找 PPG 与动态阈值交点
    for (uint16_t i = 1; i < length; i++)
    {
        if ((filtered_ir[i - 1] < ir_data_sum_aver && filtered_ir[i] >=ir_data_sum_aver))
        {

            temp=i;
            break;
        }
    }
    i++;
    for (; i < length; i++)
    {
           if ((filtered_ir[i - 1] < ir_data_sum_aver && filtered_ir[i] >=ir_data_sum_aver))
           {

               temp=i-temp;
               break;
           }
    }
    if((temp>28)&&(temp<200))
    {
    	if(30<60*Fs/temp&&60*Fs/temp<200){
    	*heart_rate=60*Fs/temp;}
    }
    else
    {

    	return ;
    }

    // 计算 SpO₂


    // 获取DC/AC值
    float ir_dc = 0, ir_ac = 0;
    float max_ir = -FLT_MAX, min_ir = FLT_MAX;

       for (uint16_t i = 0; i < length; i++) {
           ir_dc += filtered_ir[i];

           if (filtered_ir[i] > max_ir) max_ir = filtered_ir[i];
           if (filtered_ir[i] < min_ir) min_ir = filtered_ir[i];
       }
       ir_dc /= length;
       ir_ac = max_ir - min_ir;

    float red_dc = 0, red_ac = 0;
    float max_red = 0, min_red = FLT_MAX;
    float red_prev_w = 0;

    for (uint16_t i = 0; i < length; i++) {
        float red_filtered = dc_remove(red_data[i], &red_prev_w);
        red_dc += red_filtered;

        if (red_filtered > max_red) max_red = red_filtered;
        if (red_filtered < min_red) min_red = red_filtered;
    }
    red_dc /= length;
    red_ac = max_red - min_red;

    float R = (red_ac / red_dc) / (ir_ac / ir_dc);
    if(((-15)*R*R+56*R+80)<70||((-15)*R*R+56*R+80)>100)
    {
    	return;
    }
    else
    {
    *spo2 = (uint8_t)((-15)*R*R+56*R+80);
    }
}
