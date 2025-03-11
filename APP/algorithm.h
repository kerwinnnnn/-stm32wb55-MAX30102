#ifndef HR_SPO2_ALGORITHM_H
#define HR_SPO2_ALGORITHM_H

#include <stdint.h>



#define MIN_BPM 40
#define MAX_BPM 180
#define FLT_MAX 0x3FFFF





void calculate_hr_spo2(uint32_t* ir_data, uint32_t* red_data, uint16_t length, uint8_t* heart_rate, uint8_t* spo2,uint8_t Fs) ;

#endif
