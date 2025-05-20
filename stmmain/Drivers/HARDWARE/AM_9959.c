
#include "AM_9959.h"


AM_Instance AM1,AM2;

void AM_Instance_Init(AM_Instance* ham, AD9959_HandleTypeDef* had9959, uint16_t chcw, uint16_t chlw, DAC_HandleTypeDef* hdacx, uint32_t dac_channel)
{
	ham->CH_CW=chcw;
	ham->CH_LW=chlw;
	ham->MDepth=0.5;
	ham->TDelay=0;
	ham->had9959=had9959;
	ham->hdac=hdacx;
	ham->CH_ADC=dac_channel;
}

uint8_t AM_Init(void)
{
	HAL_DAC_Init(&hdac);
	HAL_DACEx_DualSetValue(&AM_DAC_HANDLE,0,0,0);
	HAL_DACEx_DualStart(&AM_DAC_HANDLE);
	AM_Instance_Init(&AM1,&hAD9959,0,1,&hdac,DAC_CHANNEL_1);
	AM_Instance_Init(&AM2,&hAD9959,0,1,&hdac,DAC_CHANNEL_2);
	
	return HAL_OK;
}

uint8_t SetDAC(AM_Instance* hamx, uint16_t val)
{
	HAL_DAC_SetValue(hamx->hdac,hamx->CH_ADC,DAC_ALIGN_12B_R,val>4096?4096:val);
	return HAL_OK;
}





