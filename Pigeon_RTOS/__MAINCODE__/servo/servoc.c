#include "stm32f10x.h"                  // Device header
#include "PWMC.h" 

void Servoc_Init(void)
{
	PWMC_Init();
}

void Servoc_SetAngle(float Angle)
{
	PWMC_SetCompare2(Angle/ 270 * 2000 + 500);
}
