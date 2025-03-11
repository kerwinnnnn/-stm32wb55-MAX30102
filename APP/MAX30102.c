#include"i2c.h"
#include<stdio.h>
#include"MAX30102.h"
#include"algorithm.h"
#include"gpio.h"
extern I2C_HandleTypeDef hi2c1;

uint32_t ir_buffer[500];
uint32_t buffer_length=500;
uint32_t red_buffer[500];
uint8_t n_sp02;
uint8_t n_heart_rate;
uint32_t n_brightness;



#define MAX_BRIGHTNESS 255

#define MAX_LED_REGISTER 200
#define MAX_LED_INC      10
#define MIN_LED_REGISTER 20

#define SAMPLE_RATE 100

//uint8_t current=72;



//void adjust_Current(uint8_t n_brightness)
//{
//	uint8_t current_inc=0;
//	current_inc=(255-n_brightness)*MAX_LED_REGISTER/255;
//	if(current_inc>MAX_LED_INC)current_inc=MAX_LED_INC;
//	current+=current_inc;
//	if(current> MAX_LED_REGISTER)current=MAX_LED_REGISTER;
//	max30102_Reg_Write(REG_LED1_PA, current);
//
//}

void max30102_Reg_Write(uint8_t Register_Address,uint8_t Data)
{
	uint8_t buf[2];
	buf[0]=Register_Address;
	buf[1]=Data;
	if(HAL_I2C_Mem_Write(&hi2c1,I2C_WRITE_ADDR, buf[0], I2C_MEMADD_SIZE_8BIT,&buf[1], 2, HAL_MAX_DELAY)!=HAL_OK)
		printf("IIC write error");
}

void max30102_Reg_Read(uint8_t Register_Address,uint8_t *Data)
{
	uint8_t buf[2];
	buf[0]=Register_Address;
	if(HAL_I2C_Mem_Read(&hi2c1,I2C_READ_ADDR, buf[0], I2C_MEMADD_SIZE_8BIT,&buf[1], 1, HAL_MAX_DELAY)!=HAL_OK)
		printf("IIC read error");
	*Data=buf[1];
}

void max30102_FIFO_ReadBytes(uint32_t *Red_Data,uint32_t *IR_Data)
{
	uint8_t data[6];
	uint8_t temp;
	max30102_Reg_Read(REG_INTR_STATUS_1,&temp);
	max30102_Reg_Read(REG_INTR_STATUS_2,&temp);
	if(HAL_I2C_Mem_Read(&hi2c1,I2C_READ_ADDR, REG_FIFO_DATA, I2C_MEMADD_SIZE_8BIT,data, 6, HAL_MAX_DELAY)!=HAL_OK)
		printf("FIFO read error");

	*Red_Data = ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[2];
	*IR_Data = ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 8) | (uint32_t)data[5];


}
void max30102_Init(void)
{
	max30102_Reg_Write(REG_MODE_CONFIG,0x40);//MAX30102reset

	max30102_Reg_Write(REG_INTR_ENABLE_1,0xc0);	// INTR setting
	max30102_Reg_Write(REG_INTR_ENABLE_2,0x00);
	max30102_Reg_Write(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	max30102_Reg_Write(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	max30102_Reg_Write(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	max30102_Reg_Write(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
	max30102_Reg_Write(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	max30102_Reg_Write(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)
	max30102_Reg_Write(REG_LED1_PA,0x2f);   	//Choose value for ~ 7mA for LED1
	max30102_Reg_Write(REG_LED2_PA,0x2f);   	// Choose value for ~ 7mA for LED2
	max30102_Reg_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED
}



void max30102_Calculate_HR_BO_Value(uint8_t* HR_Value,uint8_t* BO_Value)
{
	int i;
	int32_t prev_data;

	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)>0);
	uint32_t min=0x3FFFF;
	uint32_t max=0;
	for(i=100;i<500;i++)
		{
			ir_buffer[i-100]=ir_buffer[i];//保持数据的连续性
			red_buffer[i-100]=red_buffer[i];
			if(min>red_buffer[i])
				{
				  min=red_buffer[i];
				};
			if(max<red_buffer[i])
			  	{
				  max=red_buffer[i];
			  	};

		 }


	for(i=400;i<500;i++)
		{
			max30102_FIFO_ReadBytes((ir_buffer+i),(red_buffer+i));
			prev_data=red_buffer[i-1];
			float f_temp=red_buffer[i]-prev_data;
			if(f_temp>0)
			  {
				if(max==min)
				{
					f_temp=0;
				}
				else
				{
					f_temp*=MAX_BRIGHTNESS;
					f_temp/=max-min;
				}
			  }
			else
			  {
				if(max==min)
					{
						f_temp=0;
					}
				else{
					f_temp*=MAX_BRIGHTNESS;
					f_temp/=min-max;
				    }

			  }
			 n_brightness=f_temp;
			 if(n_brightness<0)
				  {
					 n_brightness=0;
				  }
			 if(n_brightness>MAX_BRIGHTNESS)
				  {
					 n_brightness=MAX_BRIGHTNESS;
				  }
			 //adjust_Current(n_brightness);
		  }


		 calculate_hr_spo2(ir_buffer, red_buffer, buffer_length, &n_heart_rate, &n_sp02,SAMPLE_RATE);
		  *HR_Value=n_heart_rate;
		  *BO_Value=n_sp02;

}
