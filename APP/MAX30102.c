#include"i2c.h"
#include<stdio.h>
#include"MAX30102.h"
#include"algorithm.h"
#include"gpio.h"
extern I2C_HandleTypeDef hi2c1;

uint32_t ir_buffer[500];
int32_t buffer_length=500;
uint32_t red_buffer[500];
int32_t n_sp02;
int8_t ch_spo2_valid;
int32_t n_heart_rate;
int8_t  ch_hr_valid;
int32_t n_brightness;



#define MAX_BRIGHTNESS 255

#define MAX_LED_REGISTER 200
#define MAX_LED_INC      10
#define MIN_LED_REGISTER 20

uint8_t current=72;




void adjust_Current(uint8_t n_brightness)
{
	uint8_t current_inc=0;
	current_inc=(255-n_brightness)*MAX_LED_REGISTER/255;
	if(current_inc>MAX_LED_INC)current_inc=MAX_LED_INC;
	current+=current_inc;
	if(current> MAX_LED_REGISTER)current=MAX_LED_REGISTER;
	max30102_Reg_Write(REG_LED1_PA, current);

}

void max30102_Reg_Write(uint8_t Register_Address,uint8_t Data)
{
	uint8_t buf[2];
	buf[0]=Register_Address;
	buf[1]=Data;
	HAL_I2C_Mem_Write(&hi2c1,I2C_WRITE_ADDR, buf[0], I2C_MEMADD_SIZE_8BIT,&buf[1], 2, HAL_MAX_DELAY);
}

void max30102_Reg_Read(uint8_t Register_Address,uint8_t *Data)
{
	uint8_t buf[2];
	buf[0]=Register_Address;
	HAL_I2C_Mem_Read(&hi2c1,I2C_READ_ADDR, buf[0], I2C_MEMADD_SIZE_8BIT,&buf[1], 1, HAL_MAX_DELAY);
	*Data=buf[1];
}

void max30102_FIFO_ReadBytes(uint32_t *Red_Data,uint32_t *IR_Data)
{
	uint8_t data[6];
	uint8_t temp;
	max30102_Reg_Read(REG_INTR_STATUS_1,&temp);
	max30102_Reg_Read(REG_INTR_STATUS_2,&temp);
	HAL_I2C_Mem_Read(&hi2c1,I2C_READ_ADDR, REG_FIFO_DATA, I2C_MEMADD_SIZE_8BIT,data, 6, HAL_MAX_DELAY);

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
	max30102_Reg_Write(REG_LED1_PA,0x24);   	//Choose value for ~ 7mA for LED1
	max30102_Reg_Write(REG_LED2_PA,0x24);   	// Choose value for ~ 7mA for LED2
	max30102_Reg_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED
}

void max30102_Safety(void)
{
	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)>0);
	int i=0;
	for(i=0;i<500;i++)
	  {
		  max30102_FIFO_ReadBytes((ir_buffer+i),(red_buffer+i));

	  }
	  maxim_heart_rate_and_oxygen_saturation(ir_buffer, buffer_length, red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
}

void max30102_Calculate_HR_BO_Value(int32_t* HR_Value,int8_t* HR_Valid,int32_t* BO_Value,int8_t* BO_Valid)
{
	int i;
	int32_t prev_data;

	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)>0);
	uint32_t min=0x3FFFF;
	uint32_t max=0;
	for(i=0;i<500;i++)
		{
			max30102_FIFO_ReadBytes((ir_buffer+i),(red_buffer+i));
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
			int32_t f_temp=red_buffer[i]-prev_data;
			if(f_temp>0)
			  {
				if(max==min)
				{
					f_temp=0;
				}
				else
				{
					f_temp/=max-min;
					f_temp*=MAX_BRIGHTNESS;
				}
			  }
			else
			  {
				if(max==min)
					{
						f_temp=0;
					}
				else{
					f_temp/=min-max;
					f_temp*=MAX_BRIGHTNESS;

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
			 adjust_Current(n_brightness);
		  }
		  maxim_heart_rate_and_oxygen_saturation(ir_buffer, buffer_length, red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		  *HR_Value=n_heart_rate;
		  *HR_Valid=ch_hr_valid;
		  *BO_Value=n_sp02;
		  *BO_Valid=ch_spo2_valid;

}
