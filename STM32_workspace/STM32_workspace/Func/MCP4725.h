#ifndef __MCP4725_H__
#define __MCP4725_H__

#include <stdint.h>

void MCP4725_Write_FastMode(float AnalogValue);

void MCP4725_Write_NormalMode(float analog_Data);

void MCP4725_Stop(void);

#endif
