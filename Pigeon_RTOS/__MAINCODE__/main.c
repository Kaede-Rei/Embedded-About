#include "main.h"

int main(void)
{
	
/**
	*	@Name 	Init
	*	@Brief	工程初始化
	*/
		controla_Init();
		controlb_Init();
		controlc_Init();
		controld_Init();
		controle_Init();
		Motors_Init();
		Motorx_Init();
		Motory_Init();
		Serial_Init();
		Servoc_Init();
		Servoab_Init();
		LightSensor_Init();
		LED_Init();
		Servoa_SetAngle(0);		//第一个舵机的初始角度
		Servob_SetAngle(20);	//第二个舵机的初始角度，与第一个舵机角度镜像，摇臂镜像
	
/**
	*	@Name 	Start
	*	@Brief	创建任务并开启调度
	*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		if(!CreateAllTask_Dynamic())
				vTaskStartScheduler();
		while(1);
		
}
