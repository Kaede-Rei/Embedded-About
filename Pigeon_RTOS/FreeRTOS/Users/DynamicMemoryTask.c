#include "main.h"

/*  全局变量  */
uint8_t flag = 0;
uint8_t n = 0;

/**
  *	@Name 		Handle_Init
  *	@Include	Task1~6、Semaphore1、EventGroup4、AllTask
  *	@Brief		定义所有相关句柄
  */
/*  任务1句柄  */
TaskHandle_t Task1_Handle;
/*  任务1信号量句柄  */
SemaphoreHandle_t Task1_BinaryHandle = NULL;
/*  任务2句柄  */
TaskHandle_t Task2_Handle;
/*  任务3句柄  */
TaskHandle_t Task3_Handle;
/*  任务4句柄  */
TaskHandle_t Task4_Handle;
/*  任务4事件组句柄  */
EventGroupHandle_t Task23Cmplt4Start_Handle;
/*  任务5句柄  */
TaskHandle_t Task5_Handle;
///*  任务6句柄  */
//TaskHandle_t Task6_Handle;
/*  总任务句柄  */
static TaskHandle_t AppTaskCreate_Handle;

/**
  *	@TaskNum		1
  *	@TaskName		接收并开始
  *	@Brief			接收到信号后开始作业
	*	@Word				132Word = 528Byte
  */
static void RevdAndStart1(void)
{
		while(1)
		{
				/* 等待信号 */
				if(flag == 0 && xSemaphoreTake(Task1_BinaryHandle,portMAX_DELAY) == pdPASS && Serial_RxPacket[2] == 0x01)
				{
						flag = 1;
						//	通知任务2和任务3开始作业
						xTaskNotify(Task2_Handle, 0x01, eSetValueWithOverwrite);
						xTaskNotify(Task3_Handle, 0x01, eSetValueWithOverwrite);
						//	发送剩余栈大小
//						UBaseType_t stack_remain = uxTaskGetStackHighWaterMark(NULL);
//						Serial_Printf("Task1 Stack Remained: %d Bytes \r\n", stack_remain * sizeof(StackType_t));
				}
		}
}

/**
  *	@TaskNum		2
  *	@TaskName		取下旧料盒
  *	@Brief			鸽笼开始更换饲料，该任务负责将旧料盒取下来
	*	@Word				256Word = 1024Byte
  */
static void TakeOld2(void)
{
		uint32_t Data = 0x00;
		while(1)
		{
				/* 等待作业通知 */
				xTaskNotifyWait(0x00, 0x01, &Data, portMAX_DELAY);
						
				if(Data == 0x01)
				{
						//取旧料盒的过程：
						while(Serial_RxPacket[3]==0x01) 
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_5);
							//0是向着尾端，1是向着铭牌方向
							Motorx_Run(1,10,1000);
							LED1_OFF();
							if(Serial_RxPacket[3]==0x00)
							{
								break;
							}
							//	防止死循环，需要有时间让出CPU进行调度
							vTaskDelay(pdMS_TO_TICKS(50));
						}
						GPIO_ResetBits(GPIOF, GPIO_Pin_5);
								
							
						//y轴步进电机运动一定的高度
						//0是向着尾端，1是向着铭牌方向
						//所有的圈数*5/2，即为以mm为单位的走的长度，所以我们就是用已知的长度(mm)*2/5即可，需要更改的就是中间圈数这个变量
							
						uint16_t highdistance = Serial_RxPacket[5];
						uint16_t quanshu = highdistance*4;
							
						if(flag == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_2);
							//0是向着尾端，1是向着铭牌方向
							Motory_Run(1,quanshu,1000);
						} 
						else {
							GPIO_ResetBits(GPIOF, GPIO_Pin_2);		
						}	
							
							
						//这里电推杆的时间可以根据串口发送的数据来定
				
						uint16_t pushRodPosition = Serial_RxPacket[4]; 		// 例如 0x89
							
						uint16_t sum = pushRodPosition*10;
						uint16_t time = sum/15;
						//		while(pushRodPosition>0) 
						//		{
						//			sum=sum*16+(pushRodPosition%10);
						//			pushRodPosition/=10;
						//		}
			
						controlelow();
						//Delay_s(time);
						vTaskDelay(pdMS_TO_TICKS(time*1000));
						
						controlestop();
								
						//Delay_s(3);
						vTaskDelay(pdMS_TO_TICKS(3000));
						
						
						//下面两行代码就是夹爪往下夹
						Servoc_SetAngle(70);
						Servoc_SetAngle(0);
						//Delay_ms(2000);
						vTaskDelay(pdMS_TO_TICKS(2000));

 
						//这里加上一个平台上来一点点的代码：
						if(flag == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_2);
							//0是向着尾端，1是向着铭牌方向
							Motory_Run(1,6,1000);
						} 
						else {
							GPIO_ResetBits(GPIOF, GPIO_Pin_2);		
						}
						
			
			
						controlehigh();
						//Delay_s(time);
						vTaskDelay(pdMS_TO_TICKS(time*1000));
						
						controlestop();
						// //Delay_ms(3000);
						
						
						//夹爪松开
						Servoc_SetAngle(70);
						
						
						//y轴步进电机运动一定的高度,这个高度可以改
						
						quanshu+=6;
						if(flag == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_2);
							//0是向着尾端，1是向着铭牌方向
							Motory_Run(0,quanshu,1000);
						} 
						else {
							GPIO_ResetBits(GPIOF, GPIO_Pin_2);		
						}
			
			
						//这里要补充步进电机运行到最边边的下料的地方
						while(LightSensor_Get1() == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_5);
							//0是向着尾端，1是向着铭牌方向
							Motorx_Run(0,1,1000);	
						}
						GPIO_ResetBits(GPIOF, GPIO_Pin_5);
						
					
						controldlow();
						//Delay_s(15);
						vTaskDelay(pdMS_TO_TICKS(15000));
						
						controldstop();
						//Delay_ms(3000);
						vTaskDelay(pdMS_TO_TICKS(3000));
						
						//然后就是两个舵机开始运行
						
							//夹爪抓起废弃料盒
						Servoa_SetAngle(40);//第一个舵机抓取时的角度
						Servob_SetAngle(80);//第二个舵机抓取时的角度
						//Delay_ms(1000);
						vTaskDelay(pdMS_TO_TICKS(1000));
						
						controldhigh();
						//Delay_s(15);
						vTaskDelay(pdMS_TO_TICKS(15000));
						
						controldstop();
						
						//最后夹爪松开
						Servoa_SetAngle(100);//第一个舵机松抓的角度
						Servob_SetAngle(20);//第二个舵机松抓的角度
						
						
						//步进电机去到上料点
						if(flag == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_5);
							//0是向着尾端，1是向着铭牌方向
							//所有的圈数*5/2，即为以mm为单位的走的长度，所以我们就是用已知的长度(mm)*2/5即可，需要更改的就是中间圈数这个变量
							Motorx_Run(1,74,1000);
						}
						else {
							GPIO_ResetBits(GPIOF, GPIO_Pin_5);		
						}
						
						//	完成作业并设置事件组位掩码
						xEventGroupSetBits(Task23Cmplt4Start_Handle, 0x01);
						//	发送剩余栈大小
//						UBaseType_t stack_remain = uxTaskGetStackHighWaterMark(NULL);
//						Serial_Printf("Task2 Stack Remained: %d bytes \r\n", stack_remain * sizeof(StackType_t));
				}
		}
}

/**
  *	@TaskNum		3
  *	@TaskName		下料
  *	@Brief			鸽笼开始更换饲料，该任务负责投放饲料
	*	@Word				192Word = 768Byte
  */
static void DropFeed3(void)
{
		uint32_t Data = 0x00;
		while(1)
		{
				/* 等待作业通知 */
				xTaskNotifyWait(0x00, 0x01, &Data, portMAX_DELAY);
				if(Data == 0x01)
				{
						//开始上料盒过程
			
						if(flag == 1 && n<=6)
						{
				
							GPIO_SetBits(GPIOF, GPIO_Pin_8);
							//1是向着尾端，0是向着铭牌方向
							//所有的圈数*5/2，即为以mm为单位的走的长度，所以我们就是用已知的长度(mm)*2/5即可，需要更改的就是中间圈数这个变量
							Motors_Run(0,8,1000);
							n++;
						}
						if(flag == 1 && n==7) {
							GPIO_SetBits(GPIOF, GPIO_Pin_8);
							//1是向着尾端，0是向着铭牌方向
							//所有的圈数*5/2，即为以mm为单位的走的长度，所以我们就是用已知的长度(mm)*2/5即可，需要更改的就是中间圈数这个变量
							Motors_Run(1,48,1000);
							n=0;
						}
						else {
							GPIO_ResetBits(GPIOF, GPIO_Pin_8);		
						}
				
			
			
						//电机翻转180°回到步进电机的位置，开始抓取空盒子
						controlalow();
						//Delay_ms(2600);
						vTaskDelay(pdMS_TO_TICKS(2600));
						
						//开始返回平台
						controlahigh();
						//Delay_ms(2600);
						vTaskDelay(pdMS_TO_TICKS(2600));
						
						controlastop();
						
						//下料
						controlbhigh();
						//Delay_ms(5000);
						vTaskDelay(pdMS_TO_TICKS(5000));
						
						//下料停止
						controlbstop();
						// //Delay_ms(2000);		
					
						//	完成作业并设置事件组位掩码
						xEventGroupSetBits(Task23Cmplt4Start_Handle, 0x02);
						//	发送剩余栈大小
//						UBaseType_t stack_remain = uxTaskGetStackHighWaterMark(NULL);
//						Serial_Printf("Task3 Stack Remained: %d bytes \r\n", stack_remain * sizeof(StackType_t));
				}
		}
}

/**
  *	@TaskNum		4
  *	@TaskName		推出新料盒
  *	@Brief			更换饲料完毕后将新饲料盒推到传送带面前
	*	@Word				128Word = 512Byte
  */
static void PushNew4(void)
{
		while(1)
		{
				/* 等待任务2和任务3均完成作业 */
				if(xEventGroupWaitBits(Task23Cmplt4Start_Handle, 0x03, pdTRUE, pdTRUE, portMAX_DELAY) == 0x03)
				{
						controlchigh();
						//Delay_ms(8000);	
						vTaskDelay(pdMS_TO_TICKS(7600));
					
						controlclow();
						//Delay_ms(8000);						
						vTaskDelay(pdMS_TO_TICKS(7600));
						
						controlcstop();
						// //Delay_ms(3000);
						
						//	通知任务5开始作业
						xTaskNotify(Task5_Handle, 0x01, eSetValueWithOverwrite);
						//	发送剩余栈大小
//						UBaseType_t stack_remain = uxTaskGetStackHighWaterMark(NULL);
//						Serial_Printf("Task4 Stack Remained: %d bytes \r\n", stack_remain * sizeof(StackType_t));
				}
		}
}

/**
  *	@TaskNum		5
  *	@TaskName		传送
  *	@Brief			将新的饲料盒传送到应该放的位置
	*	@Stack			320Word = 1280Byte
  */
static void TransmitNew5(void)
{
		uint32_t Data = 0x00;
		while(1)
		{
				/* 等待作业通知 */
				xTaskNotifyWait(0x00, 0x01, &Data, portMAX_DELAY);
				if(Data == 0x01)
				{
						//下面两行代码就是夹爪往下夹
						Servoc_SetAngle(70);
						Servoc_SetAngle(0);
						// //Delay_ms(2000);
			
						while(Serial_RxPacket[3]==0x01) 
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_5);
							//0是向着尾端，1是向着铭牌方向
							Motorx_Run(0,10,1000);
							LED1_OFF();
							if(Serial_RxPacket[3]==0x00)
							{
								break;
							}
//							//	防止死循环，需要有时间让出CPU进行调度
//							vTaskDelay(pdMS_TO_TICKS(50));
						}
						GPIO_ResetBits(GPIOF, GPIO_Pin_5);
			

			
						//y轴步进电机运动一定的高度
						uint16_t highdistances = Serial_RxPacket[5];
						uint16_t quanshus = highdistances*4;
						
						if(flag == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_2);
							//0是向着尾端，1是向着铭牌方向
							Motory_Run(1,quanshus,1000);
						} 
						else {
							GPIO_ResetBits(GPIOF, GPIO_Pin_2);		
						}
			
			
			
						//这里电推杆的时间可以根据串口发送的数据来定
			
						uint16_t pushRodPositions = Serial_RxPacket[4]; 		// 例如 0x89
						
						uint16_t sums = pushRodPositions*10;
						uint16_t times = sums/15;
						
						controlelow();
						//Delay_s(times);
						vTaskDelay(pdMS_TO_TICKS(times*1000));
						
						//Delay_s(2);
						vTaskDelay(pdMS_TO_TICKS(2000));
						
			
						//这里加上一个平台下来一点点的代码：168-177
						if(flag == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_2);
							//0是向着尾端，1是向着铭牌方向
							Motory_Run(0,6,1000);
						} 
						else {
							GPIO_ResetBits(GPIOF, GPIO_Pin_2);		
						}
						
			
						controlestop();
						// //Delay_ms(3000);
			
						Servoc_SetAngle(70);
						//Delay_ms(2000);
						vTaskDelay(pdMS_TO_TICKS(2000));
						
						controlehigh();
						//Delay_s(times);
						vTaskDelay(pdMS_TO_TICKS(times*1000));
						
						controlestop();
						// //Delay_ms(3000);
						
			
						quanshus-=6;
						
						//y轴步进电机运动一定的高度
						if(flag == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_2);
							//0是向着尾端，1是向着铭牌方向
							Motory_Run(0,quanshus,1000);
						} 
						else {
							GPIO_ResetBits(GPIOF, GPIO_Pin_2);		
						}
						
						
						Serial_TxPacket[0] = 0x01 ;
						Serial_SendPacket();
			
						
						//这里要补充步进电机运行到最边边的下料的地方
						while(LightSensor_Get1() == 1)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_5);
							//0是向着尾端，1是向着铭牌方向
							Motorx_Run(0,1,1000);	
						}
						GPIO_ResetBits(GPIOF, GPIO_Pin_5);
						
//						xTaskNotify(Task6_Handle, 0x01, eSetValueWithOverwrite);
						
						
						GPIO_ResetBits(GPIOF, GPIO_Pin_5);
						GPIO_ResetBits(GPIOF, GPIO_Pin_8);
						GPIO_ResetBits(GPIOF, GPIO_Pin_2);
						
						controlastop();
						controlbstop();
						controlcstop();
						controldstop();
						controlestop();
						
						Servoa_SetAngle(0);  // 第一个舵机初始角度
						Servob_SetAngle(20); // 第二个舵机初始角度
						Servoc_SetAngle(0);  // 夹爪舵机初始角度
				
						flag = 0;
									
						//	发送剩余栈大小
//						UBaseType_t stack_remain = uxTaskGetStackHighWaterMark(NULL);
//						Serial_Printf("Task5 Stack Remained: %d bytes \r\n", stack_remain * sizeof(StackType_t));
				}
		}
}

///**
//  *	@TaskNum		6
//  *	@TaskName		停止作业
//  *	@Brief			未接收到信号时或作业结束时默认停止作业
//	*	@Stack			78Word = 312Byte
//  */
//static void StopWork6(void)
//{
//		uint32_t Data = 0x00;
//		while(1)
//		{
//				if(Serial_RxPacket[2]==0x00) 
//				{
//						GPIO_ResetBits(GPIOF, GPIO_Pin_5);
//						GPIO_ResetBits(GPIOF, GPIO_Pin_8);
//						GPIO_ResetBits(GPIOF, GPIO_Pin_2);
//						
//						controlastop();
//						controlbstop();
//						controlcstop();
//						controldstop();
//						controlestop();
//						
//						Servoa_SetAngle(0);  // 第一个舵机初始角度
//						Servob_SetAngle(20); // 第二个舵机初始角度
//						Servoc_SetAngle(0);  // 夹爪舵机初始角度
//				}		
//				// 每200ms检测一次
//				vTaskDelay(pdMS_TO_TICKS(100));
//				xTaskNotifyWait(0x00, 0x01, &Data, pdMS_TO_TICKS(100));
//		}
//}

/**
  *	@TaskNum		ALL
  *	@TaskName		总任务
  *	@Brief			用于创建所有子任务的总任务
	*	@Stack			160Word = 640Byte
  */
static void AppTaskCreate(void)
{
		/* 进入临界区 */
		taskENTER_CRITICAL();           		
 
		/* 创建任务1信号量 */
		Task1_BinaryHandle = xSemaphoreCreateBinary();
		/* 创建事件组 */
		Task23Cmplt4Start_Handle = xEventGroupCreate();
	
		/* 创建子任务 */
		xTaskCreate((TaskFunction_t)RevdAndStart1,	//任务函数
								(const char*)"RevdAndStart1",		//任务名称
								(uint32_t)132,									//任务堆栈大小
								(void*)NULL,										//传递给任务函数的参数
								(UBaseType_t)4, 								//任务优先级
								&Task1_Handle);									//任务句柄
		xTaskCreate((TaskFunction_t)TakeOld2,				//任务函数
								(const char*)"TakeOld2",				//任务名称
								(uint32_t)256,									//任务堆栈大小
								(void*)NULL,										//传递给任务函数的参数
								(UBaseType_t)4, 								//任务优先级
								&Task2_Handle);									//任务句柄
		xTaskCreate((TaskFunction_t)DropFeed3,			//任务函数
								(const char*)"DropFeed3",				//任务名称
								(uint32_t)192,									//任务堆栈大小
								(void*)NULL,										//传递给任务函数的参数
								(UBaseType_t)4, 								//任务优先级
								&Task3_Handle);									//任务句柄
		xTaskCreate((TaskFunction_t)PushNew4,				//任务函数
								(const char*)"PushNew4",				//任务名称
								(uint32_t)128,									//任务堆栈大小
								(void*)NULL,										//传递给任务函数的参数
								(UBaseType_t)4, 								//任务优先级
								&Task4_Handle);									//任务句柄
		xTaskCreate((TaskFunction_t)TransmitNew5,		//任务函数
								(const char*)"TransmitNew5",		//任务名称
								(uint32_t)320,									//任务堆栈大小
								(void*)NULL,										//传递给任务函数的参数
								(UBaseType_t)4, 								//任务优先级
								&Task5_Handle);									//任务句柄
//		xTaskCreate((TaskFunction_t)StopWork6,			//任务函数
//								(const char*)"StopWork6",				//任务名称
//								(uint32_t)78,										//任务堆栈大小
//								(void*)NULL,										//传递给任务函数的参数
//								(UBaseType_t)4, 								//任务优先级
//								&Task6_Handle);									//任务句柄
								
		/* 删除AppTaskCreate任务 */
		vTaskDelete(AppTaskCreate_Handle); 	
		/* 退出临界区 */
		taskEXIT_CRITICAL();            		
}

/**	
  *	@Name 	CreateAllTask_Dynamic
  *	@Param	void
	* @Ret		创建成功返回0，创建失败返回-1
	* @Brief	动态分配内存创建所有子任务的总任务
  */
int CreateAllTask_Dynamic(void)
{
		xTaskCreate((TaskFunction_t)AppTaskCreate,//任务函数
								(const char*)"AppTaskCreate",	//任务名称
								(uint32_t)160,								//任务堆栈大小
								(void*)NULL,									//传递给任务函数的参数
								(UBaseType_t)5, 							//任务优先级
								&AppTaskCreate_Handle);				//任务句柄
    if(AppTaskCreate_Handle != NULL)					//创建成功
				return 0;
		else																			//创建失败
				return -1;
}

/**	
  *	@Name 	StopWork
  *	@Param	void
	* @Ret		void
	* @Brief	停止所有工作，用于栈溢出时紧急停止
  */
void StopWork(void)
{
		GPIO_ResetBits(GPIOF, GPIO_Pin_5);
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		GPIO_ResetBits(GPIOF, GPIO_Pin_2);
		
		controlastop();
		controlbstop();
		controlcstop();
		controldstop();
		controlestop();
		
		Servoa_SetAngle(0);  // 第一个舵机初始角度
		Servob_SetAngle(20); // 第二个舵机初始角度
		Servoc_SetAngle(0);  // 夹爪舵机初始角度
}
