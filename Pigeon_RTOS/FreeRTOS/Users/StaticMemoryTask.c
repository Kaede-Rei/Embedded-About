#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.H"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

#if configSUPPORT_STATIC_ALLOCATION == 1

/*  总任务句柄  */
static TaskHandle_t AppTaskCreate_Handle;
///*  LED_A0任务句柄  */
//static TaskHandle_t LED_A0_Task_Handle;
///*  LED_A2任务句柄  */
//static TaskHandle_t LED_A2_Task_Handle;
/*  LED_A4任务句柄  */
static TaskHandle_t LED_A4_Task_Handle;
///*  消息队列句柄  */
//static QueueHandle_t Test_Queue = NULL;
///*  二值信号量句柄  */
//static SemaphoreHandle_t BinarySemphr_Handle = NULL;
///*  计数信号量句柄  */
//static SemaphoreHandle_t CountSemphr_Handle = NULL;
///*  互斥量句柄  */
//static SemaphoreHandle_t MutexSemphr_Handle = NULL;
///*  递归互斥量句柄  */
////static SemaphoreHandle_t MutexRecurSemphr_Handle = NULL;
///*  事件组句柄  */
//static EventGroupHandle_t EventGroup_Handle = NULL;
/*  软件定时器句柄  */
static TimerHandle_t Timer_Handle = NULL;

/*  总任务堆栈  */
static StackType_t AppTaskCreate_Stack[128];
/*  LED_A0任务堆栈  */
static StackType_t LED_A0_Task_Stack[128];
/*  LED_A2任务堆栈  */
static StackType_t LED_A2_Task_Stack[128];
/*  LED_A4任务堆栈  */
static StackType_t LED_A4_Task_Stack[128];
/*  空闲任务任务堆栈  */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/*  定时器任务堆栈  */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/*  总任务控制块  */
static StaticTask_t AppTaskCreate_TCB;
/*  LED_A0任务控制块  */
static StaticTask_t LED_A0_Task_TCB;
/*  LED_A2任务控制块  */
static StaticTask_t LED_A2_Task_TCB;
/*  LED_A4任务控制块  */
static StaticTask_t LED_A4_Task_TCB;
/*  空闲任务控制块  */
static StaticTask_t Idle_Task_TCB;	
/*  定时器任务控制块  */
static StaticTask_t Timer_Task_TCB;

///*  消息队列内容缓存区  */
//static uint8_t QueueStorage[4 * 4];

///*  消息队列存储区  */
//static StaticQueue_t Queue_Buffer;
///*  二值信号量存储区  */
//static StaticSemaphore_t BinarySemphr_Buffer;
///*  计数信号量存储区  */
//static StaticSemaphore_t CountSemphr_Buffer;
///*  互斥量存储区  */
//static StaticSemaphore_t MutexSemphr_Buffer;
///*  递归互斥量存储区  */
//static StaticSemaphore_t MutexRecurSemphr_Buffer;
///*  事件组存储区  */
//static StaticEventGroup_t EventGroup_Buffer;
/*  软件定时器存储区  */
static StaticTimer_t Timer_Buffer;

///*  递归次数计数与状态标志符  */
//static uint8_t RecursiveCount = 0;
//static uint8_t RecursiveFlag = 0;

/*  获取空闲任务的任务堆栈和任务控制块内存  */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
		*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;									//任务控制块内存
		*ppxIdleTaskStackBuffer=Idle_Task_Stack;							//任务堆栈内存
		*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;				//任务堆栈大小
}
/*  获取定时器任务的任务堆栈和任务控制块内存  */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
		*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;								//任务控制块内存
		*ppxTimerTaskStackBuffer=Timer_Task_Stack;						//任务堆栈内存
		*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;	//任务堆栈大小
}

///*  分任务-纯LED  */
//static void LED_A0_Task(void)
//{
//		while(1){
//				GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A2_Task(void)
//{
//		while(1){
//				GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)));
//				vTaskDelay(pdMS_TO_TICKS(2000));
//		}
//}
///*  分任务-消息队列  */
//static void LED_A0_Task(void)
//{
//		uint32_t data = 1;
//		while(1){
//				xQueueSend(Test_Queue, &data, 0);
//				GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A2_Task(void)
//{
//		uint32_t data;
//		while(1){
//				xQueueReceive(Test_Queue, &data, portMAX_DELAY);
//				if(data == 1){
//						GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)));
//				}
//		}
//}
///*  分任务-二值信号量  */
//static void LED_A0_Task(void)
//{
//		while(1){
//				xSemaphoreGive(BinarySemphr_Handle);
//				GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A2_Task(void)
//{
//		while(1){
//				if(xSemaphoreTake(BinarySemphr_Handle, portMAX_DELAY) == pdTRUE){
//						GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)));
//				}
//		}
//}
///*  分任务-计数信号量  */
//static void LED_A0_Task(void)
//{
//		while(1){
//				if(uxSemaphoreGetCount(CountSemphr_Handle) == 4)		//满了二值设为1
//						xSemaphoreGive(BinarySemphr_Handle);						
//				if(uxSemaphoreGetCount(BinarySemphr_Handle) == 0){	//空了释放直到满	
//						xSemaphoreGive(CountSemphr_Handle);
//						GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));	
//				}
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A2_Task(void)
//{
//		while(1){
//				if(uxSemaphoreGetCount(CountSemphr_Handle) == 0){		//空了二值设为0
//						xSemaphoreTake(BinarySemphr_Handle, 0);
//						GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)1);	//灯灭
//				}
//				if(uxSemaphoreGetCount(BinarySemphr_Handle) == 1){	//满了获取直到空
//						xSemaphoreTake(CountSemphr_Handle, 0);
//						GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)0);	//灯亮
//				}
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
///*  分任务-二值信号量  */
//static void LED_A0_Task(void)
//{
//		while(1){
//				xSemaphoreGive(MutexSemphr_Handle);
//				GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A2_Task(void)
//{
//		while(1){
//				if(xSemaphoreTake(MutexSemphr_Handle, portMAX_DELAY) == pdTRUE){
//						GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)));
//				}
//		}
//}
///*  分任务-递归互斥量  */
//static void LED_A0_Task(void)
//{
//		while(1){
//				if(RecursiveFlag == 1){
//						xSemaphoreGiveRecursive(MutexRecurSemphr_Handle);
//						RecursiveCount--;
//						GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));
//				}
//				if(RecursiveCount <= 0){
//						RecursiveFlag = 0;
//				}
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A2_Task(void)
//{
//		while(1){
//				if(RecursiveFlag == 0){
//						xSemaphoreTakeRecursive(MutexRecurSemphr_Handle, portMAX_DELAY);
//						RecursiveCount++;
//						GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)));
//				}
//				if(RecursiveCount >= 5){
//						RecursiveFlag = 1;
//				}
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
///*  分任务-事件组  */
//static void LED_A0_Task(void)
//{
//		while(1){
//				xEventGroupSetBits(EventGroup_Handle, 0x01);
//				GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A2_Task(void)
//{
//		while(1){
//				xEventGroupSetBits(EventGroup_Handle, 0x02);
//				GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)));
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A4_Task(void)
//{
//		while(1){
//				if(xEventGroupWaitBits(EventGroup_Handle, 0x03, pdTRUE, pdTRUE, portMAX_DELAY) == 0x03){
//						GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)));
//				}
//		}
//}
///*  分任务-任务通知  */
//static void LED_A0_Task(void)
//{
//		while(1){
//				xTaskNotify(LED_A4_Task_Handle, 0x00, eIncrement);
//				GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A2_Task(void)
//{
//		while(1){
//				xTaskNotify(LED_A4_Task_Handle, 0x00, eIncrement);
//				GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)));
//				vTaskDelay(pdMS_TO_TICKS(1000));
//		}
//}
//static void LED_A4_Task(void)
//{
//		uint32_t Data = 0x00;
//		while(1){
//				Data = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//				if(Data == 0x02){
//						GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)));
//				}
//		}
//}
/*  分任务-软件定时器  */
static void LED_A0_Task(void)
{
		while(1){
				xTaskNotify(LED_A4_Task_Handle, 0x01, eSetBits);
				GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)));
				vTaskDelay(pdMS_TO_TICKS(1000));
		}
}
static void LED_A2_Task(void)
{
		while(1){
				xTaskNotify(LED_A4_Task_Handle, 0x02, eSetBits);
				GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)));
				vTaskDelay(pdMS_TO_TICKS(2000));
		}
}
static void LED_A4_Task(void)
{
		uint32_t Data = 0x00;
		while(1){
				xTaskNotifyWait(0x00, 0x03, &Data, portMAX_DELAY);
				if(Data == 0x03){
						GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)));
				}
		}
}
/*  软件定时器回调函数  */
static void Timer_IRQHandler(TimerHandle_t Timer_Handle)
{
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)));
}

/*  总任务  */
static void AppTaskCreate(void)
{
		taskENTER_CRITICAL();           		//进入临界区
		
//		/* 创建消息队列 */
//		Test_Queue = xQueueCreateStatic(4, 4, QueueStorage, &Queue_Buffer);
//		/* 创建二值信号量 */
//		BinarySemphr_Handle = xSemaphoreCreateBinaryStatic(&BinarySemphr_Buffer);
//		/* 创建计数信号量 */
//		CountSemphr_Handle = xSemaphoreCreateCountingStatic(4, 0, &CountSemphr_Buffer);
//		/* 创建互斥量 */
//		MutexSemphr_Handle = xSemaphoreCreateMutexStatic(&MutexSemphr_Buffer);
//		/* 创建递归互斥量 */
//		MutexRecurSemphr_Handle = xSemaphoreCreateRecursiveMutexStatic(&MutexRecurSemphr_Buffer);
//		/* 创建事件组 */
//		EventGroup_Handle = xEventGroupCreateStatic(&EventGroup_Buffer);
		
		/* 创建软件定时器任务 */
		Timer_Handle = xTimerCreateStatic("Timer_Task",
																	pdMS_TO_TICKS(2000),
																	pdTRUE,
																	0,
																	Timer_IRQHandler,
																	&Timer_Buffer);
		
		/* 开启软件定时器 */
		xTimerStart(Timer_Handle, portMAX_DELAY);
		
		/* 创建LED_Task任务 */
//		LED_A0_Task_Handle = 
		xTaskCreateStatic((TaskFunction_t)LED_A0_Task,			//任务函数
											(const char*)"LED_A0_Task",				//任务名称
											(uint32_t)128,										//任务堆栈大小
											(void*)NULL,											//传递给任务函数的参数
											(UBaseType_t)4, 									//任务优先级
											(StackType_t*)LED_A0_Task_Stack,	//任务堆栈
											(StaticTask_t*)&LED_A0_Task_TCB);	//任务控制块
//		LED_A2_Task_Handle =
		xTaskCreateStatic((TaskFunction_t)LED_A2_Task,			//任务函数
											(const char*)"LED_A2_Task",				//任务名称
											(uint32_t)128,										//任务堆栈大小
											(void*)NULL,											//传递给任务函数的参数
											(UBaseType_t)4, 									//任务优先级
											(StackType_t*)LED_A2_Task_Stack,	//任务堆栈
											(StaticTask_t*)&LED_A2_Task_TCB);	//任务控制块 
		LED_A4_Task_Handle =
		xTaskCreateStatic((TaskFunction_t)LED_A4_Task,			//任务函数
											(const char*)"LED_A4_Task",				//任务名称
											(uint32_t)128,										//任务堆栈大小
											(void*)NULL,											//传递给任务函数的参数
											(UBaseType_t)3, 									//任务优先级
											(StackType_t*)LED_A4_Task_Stack,	//任务堆栈
											(StaticTask_t*)&LED_A4_Task_TCB);	//任务控制块   
	
		vTaskDelete(AppTaskCreate_Handle); 	//删除AppTaskCreate任务
	
		taskEXIT_CRITICAL();            		//退出临界区
}

int CreateAllTask_Static(void)
{
		AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,			//任务函数
																						(const char*)"AppTaskCreate",				//任务名称
																						(uint32_t)128,											//任务堆栈大小
																						(void*)NULL,												//传递给任务函数的参数
																						(UBaseType_t)5, 										//任务优先级
																						(StackType_t*)AppTaskCreate_Stack,	//任务堆栈
																						(StaticTask_t*)&AppTaskCreate_TCB);	//任务控制块
    if(AppTaskCreate_Handle != NULL)		//创建成功
				return 0;
		else																//创建失败
				return -1;
}

#endif
