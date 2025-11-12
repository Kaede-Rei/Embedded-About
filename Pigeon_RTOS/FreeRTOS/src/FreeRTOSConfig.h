/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/*-----------------------------------------------------------
 * 										核心调度配置
 *----------------------------------------------------------*/
 
#define configUSE_PREEMPTION			1				// 启用抢占式调度
#define configUSE_TIME_SLICING    1       // 启用时间片轮转调度
#define configUSE_IDLE_HOOK				0				// 禁用空闲任务钩子
#define configUSE_TICK_HOOK				0				// 禁用系统节拍钩子
#define configCPU_CLOCK_HZ				( ( unsigned long ) 72000000 )// CPU时钟72MHz
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )				// 系统节拍1ms
#define configMAX_PRIORITIES			( 5 )		// 最大优先级5级
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )		// 空闲任务堆栈128字
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 17 * 1024 ) )	// 动态内存池17KB（单片机RAM资源有限,最好设置为17KB）（10~17）
#define configMAX_TASK_NAME_LEN		( 16 )	// 任务名称最大长度
#define configUSE_TRACE_FACILITY	0				// 关闭调试跟踪（启用会影响性能）
#define configUSE_16_BIT_TICKS		0				// 使用32位Tick计数器
#define configIDLE_SHOULD_YIELD		1				// 空闲任务让出CPU

/*-----------------------------------------------------------
 * 								内存分配（静态动态二选一）
 *----------------------------------------------------------*/

#define configSUPPORT_STATIC_ALLOCATION     0	  // 静态内存分配（需要自己分配）
#define configSUPPORT_DYNAMIC_ALLOCATION    1   // 启用动态内存分配（默认已启用）
/* 若使用动态分配，需定义堆管理器 */
#define configUSE_HEAP_SCHEME               4		// 使用堆管理文件heap_4.c
extern void* pvPortMalloc(size_t xWantedSize);
extern void vPortFree(void* pv);
#define configCHECK_FOR_STACK_OVERFLOW 			2		// 任务堆栈溢出检测
#define configUSE_MALLOC_FAILED_HOOK 				1		// 堆内存分配失败钩子

/*-----------------------------------------------------------
 * 											同步与通信
 *----------------------------------------------------------*/

#define configUSE_MUTEXES             	1       // 启用互斥量
#define configUSE_RECURSIVE_MUTEXES   	1       // 启用递归互斥量
#define configUSE_COUNTING_SEMAPHORES 	1       // 启用计数信号量
#define configUSE_APPLICATION_TASK_TAG	0   		// 禁用任务标签（可选）
#define configUSE_QUEUE_SETS         		1       // 启用队列集合
#define configUSE_TASK_NOTIFICATIONS  	1       // 开启任务通知（开启后每个任务多增加8字节RAM，但相比信号量解除阻塞，此可快45%）
#define configTASK_NOTIFICATION_ARRAY_ENTRIES 1	// 设置任务通知数组的索引数

/*-----------------------------------------------------------
 * 											软件定时器
 *----------------------------------------------------------*/

#define configUSE_TIMERS              1       // 启用软件定时器
#define configTIMER_TASK_PRIORITY     (configMAX_PRIORITIES - 1)  	// 定时器任务优先级
#define configTIMER_QUEUE_LENGTH      10      // 定时器命令队列长度
#define configTIMER_TASK_STACK_DEPTH  (configMINIMAL_STACK_SIZE * 2)// 定时器任务堆栈

/*-----------------------------------------------------------
 * 								中断优先级配置（Cortex-M）
 *----------------------------------------------------------*/
 
/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY         255     // 内核中断优先级最低（优先级15）
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    191     // FreeRTOS可管理的中断最高优先级（优先级11）
/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15      // 硬件NVIC优先级对应

/*-----------------------------------------------------------
 * 可选API函数包含
 *----------------------------------------------------------*/
 
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_xEventGroupSetBitsFromISR		1
#define INCLUDE_xSemaphoreGetMutexHodler 		1
#define INCLUDE_xTaskAbortDelay							1
#define INCLUDE_vTaskDelay              		1
#define INCLUDE_vTaskDelayUntil         		1
#define INCLUDE_vTaskDelete             		1
#define INCLUDE_xTaskGetCurrentTaskHandle   1
#define INCLUDE_xTaskGetHandle							1
#define INCLUDE_xTaskGetIdleTaskHandle			1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_uxTaskGetStackHighWaterMark	1
#define INCLUDE_vTaskPrioritySet        		1
#define INCLUDE_xTaskResumeFronISR					1
#define INCLUDE_eTaskGetState								1
#define INCLUDE_vTaskSuspend            		1
#define INCLUDE_xTimerPendFunctionCall			1
#define INCLUDE_uxTaskPriorityGet       		1

/*-----------------------------------------------------------
 * 						中断服务例程重映射（STM32标准库）
 *----------------------------------------------------------*/
 
#define vPortSVCHandler             SVC_Handler
#define xPortPendSVHandler          PendSV_Handler
#define xPortSysTickHandler         SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

