
#include "AM_9959.h"


AM_Instance AM1,AM2;

void AM_Instance_Init(AM_Instance* ham, AD9959_HandleTypeDef* had9959, uint16_t chcw, uint16_t chmw, DAC_HandleTypeDef* hdacx, uint32_t dac_channel)
{
	ham->CH_CW=chcw;
	ham->CH_MW=chmw;
	ham->MDepth=0.5;
	ham->TDelay=0;
	ham->had9959=had9959;
	ham->hdac=hdacx;
	ham->CH_ADC=dac_channel;
	ham->changeflag=0;
}

/*
初始化AM实例
*/
uint8_t AM_Init(void)
{
	HAL_DAC_Init(&hdac);
	HAL_DACEx_DualSetValue(&AM_DAC_HANDLE,0,0,0);
	HAL_DACEx_DualStart(&AM_DAC_HANDLE);
	AM_Instance_Init(&AM1,&hAD9959,0,1,&hdac,DAC_CHANNEL_1);
	AM_Instance_Init(&AM2,&hAD9959,2,3,&hdac,DAC_CHANNEL_2);
	
	return HAL_OK;

}


/*
同步并应用由AM_SetXXX产生的更改。使用同一个AD9959实例的AM示例将会在同步被设置。
hmax: AM_Instance指针数组
cnt:   数组长度
*/
uint8_t AM_ApplyChanges(AM_Instance* hmax[], uint16_t cnt)
{
	uint16_t data16;
	uint32_t data32;
	float datafloat;
	
	for(uint16_t i=0;i<cnt;i++)
	{
		
		datafloat=((double)(hmax[i]->had9959->freq[hmax[i]->CH_MW])*360*(hmax[i]->TDelay)/1000000000);
		AD9959_Set_Phase(hmax[i]->had9959,hmax[i]->CH_MW,&datafloat);

		datafloat=((double)(hmax[i]->had9959->freq[hmax[i]->CH_CW])*360*(hmax[i]->TDelay)/1000000000);
		AD9959_Set_Phase(hmax[i]->had9959,hmax[i]->CH_CW,&datafloat);
		
		data16=1023;
		AD9959_Set_Amp(hmax[i]->had9959, hmax[i]->CH_CW, &data16);
		data16=data16*(hmax[i]->MDepth);
		AD9959_Set_Amp(hmax[i]->had9959, hmax[i]->CH_MW, &data16);
		
		data32=2000000;
		AD9959_Set_Freq(hmax[i]->had9959, hmax[i]->CH_MW, &data32);
	}

	for(uint16_t i=0;i<cnt;i++)
	{
		if(hmax[i]->changeflag==1)
		{
			AD9959_Apply_Change(hmax[i]->had9959);
			hmax[i]->changeflag=0;
		}
		
	}
	
	
	
	
	return HAL_OK;
}



/*
设置hmax指向的AM实例的载波频率
*/
uint8_t AM_SetCarrierFreq(AM_Instance* hmax, uint32_t Cfreq)
{
	AD9959_Set_Freq(hmax->had9959,hmax->CH_CW,&Cfreq);
	hmax->changeflag=1;
	return HAL_OK;
}


/*
设置hmax指向的AM实例的基波频率
*/
uint8_t AM_SetModulationFreq(AM_Instance* hmax, uint32_t Mfreq)
{
	AD9959_Set_Freq(hmax->had9959,hmax->CH_MW,&Mfreq);
	hmax->changeflag=1;
	return HAL_OK;
}

/*
设置hmax指向的AM实例的调制度
*/
uint8_t AM_SetMDepth(AM_Instance* hmax, float MD)
{
	hmax->MDepth=MD;
	hmax->changeflag=1;
	return HAL_OK;
}

/*
设置hmax1, hmax2指向的AM实例的载波时间差
hmax1的时间偏移被设置为0，hmax2的时间偏移被设置为TD
TD:  延时，单位ns
*/
uint8_t AM_SetTDelay(AM_Instance* hmax1, AM_Instance* hmax2, float TD)
{
	hmax1->TDelay=0;
	hmax2->TDelay=TD;
	hmax1->changeflag=1;
	hmax2->changeflag=1;
	return HAL_OK;
}

uint8_t SetDAC(AM_Instance* hamx, uint16_t val)
{
	HAL_DAC_SetValue(hamx->hdac,hamx->CH_ADC,DAC_ALIGN_12B_R,val>4096?4096:val);
	DAC_Trigger(&hdac);
	return HAL_OK;
}





