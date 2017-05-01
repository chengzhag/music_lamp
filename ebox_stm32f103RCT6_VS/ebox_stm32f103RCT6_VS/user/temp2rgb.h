#ifndef __TEMP2RGB
#define __TEMP2RGB

#include "color_convert.h"

#define TEMP_START 1300
#define TEMP_COUNT 391
#define TEMP_INCREASE 100
#define TEMP_END (TEMP_START+(TEMP_COUNT-1)*TEMP_INCREASE)

COLOR_RGB temp2rgb(uint16_t temp);


#endif