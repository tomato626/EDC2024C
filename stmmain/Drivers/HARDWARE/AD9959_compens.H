#ifndef _AD9959_COMPENS_H_
#define _AD9959_COMPENS_H_

#include "main.h"


#define AD9959_COMP_Q2F(q) ((((float)(q))/INT16_MAX)/2)


/*
对于AD9959的定点数增益数组
步进1MHz/数据，从0开始
浮点数增益 ampF=AD9959_COMP_Q2F( 定点数增益q )
ampF在0-2之间
*/
extern const uint16_t AD9959_AMP_MAP[100];




#endif


