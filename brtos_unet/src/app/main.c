/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MCU and OS includes */
#include "BRTOS.h"

/* Config. files */
#include "AppConfig.h"
#include "NetConfig.h"
#include "BoardConfig.h"

#include "tasks.h"        /* for tasks prototypes */
#include "unet_api.h"   /* for UNET network functions */


BRTOS_TH TH_SYSTEM;
BRTOS_TH TH_NET_APP1;
BRTOS_TH TH_NET_APP2;
BRTOS_TH TH_TERMINAL;

//#define RUN_TEST_TERMINAL 0
#if RUN_TEST_TERMINAL
	extern void task_run_tests(void*);
	extern void terminal_test(void);
#endif


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main_app(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{

  #if RUN_TEST_TERMINAL && 0
	terminal_test();
	task_run_tests(NULL);
  #endif

  UserEnterCritical();

  // Initialize BRTOS
  BRTOS_Init();

  #if (BRTOS_PLATFORM == FRDM_KL25Z)
  	  // Iniciar UART para utilizar com printf
  	  Init_UART0(115200,128);
  #endif

  ////////////////////////////////////////////////////////

  if(InstallTask(&System_Time,"System Time",System_Time_StackSize,SystemTaskPriority, NULL, &TH_SYSTEM) != OK)
  {
    while(1){};
  };

#if(NETWORK_ENABLE == 1)
  UNET_Init();      /* Install uNET tasks: Radio, Link, Ack Up/Down, Router Up/Down */

#if (BRTOS_PLATFORM == FRDM_KL25Z)
#if (UNET_DEVICE_TYPE == PAN_COORDINATOR) && 0
  if(InstallTask(&pisca_led_net,"Blink LED Example",UNET_App_StackSize,APP2_Priority, NULL, &TH_NET_APP2) != OK)
  {
    while(1){};
  };
#else
  if(InstallTask(&UNET_App_1_Decode,"Decode app 1 profiles",UNET_App_StackSize,APP1_Priority, NULL, &TH_NET_APP1) != OK)
  {
    while(1){};
  };
#endif
#endif
#endif

#if (UNET_DEVICE_TYPE == PAN_COORDINATOR)
	#define TERM_PRIORITY Terminal_Priority
#else
	#define TERM_PRIORITY APP3_Priority
#endif

  #if TERM_ENABLE == 1
  if(InstallTask(&Terminal_Task,"Terminal Task",Terminal_StackSize,TERM_PRIORITY, NULL, &TH_TERMINAL) != OK)
  {
    while(1){};
  };
  #endif

  #if(NETWORK_ENABLE == 1) && 0
  if(InstallTask(&UNET_App_1_Decode,"Decode app 1 profiles",UNET_App_StackSize,APP1_Priority, NULL, &TH_NET_APP1) != OK)
  {
    while(1){};
  };

#if (UNET_DEVICE_TYPE == PAN_COORDINATOR)
  if(InstallTask(&pisca_led_net,"Blink LED Example",UNET_App_StackSize,APP2_Priority, NULL, &TH_NET_APP2) != OK)
  {
    while(1){};
  };
#endif

#if (UNET_DEVICE_TYPE == ROUTER)
#if (ROUTER_TYPE == ROUTER1)
  if(InstallTask(&pisca_led_net,"Blink LED Example",UNET_App_StackSize,APP2_Priority, NULL, &TH_NET_APP2) != OK)
  {
    while(1){};
  };
#endif

#if (ROUTER_TYPE == ROUTER2)
  if(InstallTask(&make_path,"Make up path",UNET_App_StackSize,APP2_Priority, NULL, &TH_NET_APP2) != OK)
  {
    while(1){};
  };
#endif
#endif
#endif

#if 0
  if(InstallTask(&led_activity,"Blink LED for activity",UNET_App_StackSize,20, NULL, NULL) != OK)
  {
    while(1){};
  };
#endif

#if 0
  if(InstallTask(&led_activity2,"Blink LED for activity 2",UNET_App_StackSize,21, NULL, NULL) != OK)
  {
    while(1){};
  };
#endif


#if 0
  if(InstallTask(&task_run_tests,"Tests",UNET_App_StackSize,2, NULL, NULL) != OK)
  {
    while(1){};
  };
#endif

#if LORA_NET == 1
  extern void Lora_Net_Task(void *p);
  if(InstallTask(&Lora_Net_Task,"Lora net",UNET_App_StackSize,10, NULL, NULL) != OK)
  {
    while(1){};
  };
#endif


#if BRTOS_PLATFORM == BOARD_ROTEADORCFV1
#if SMART_METER_ENABLE == 1
  #include "smartmeter.h"

  /* Initialize A/D Converter and bandgap reference */
  ADC_Setup(HighSpeed, LongSampleTime, 12);

  if(OSInstallTask(&EnergyMetering_Task,"Energy meter Task",EnergyMetering_StackSize,EnergyMetering_Task_Priority,NULL, NULL) != OK)
  {
    while(1){};  // Installation error: please check error type.
                 // Usually BUSY_PRIORITY or NO_MEMORY
  }
#endif
#endif

  // Start Task Scheduler
  if(BRTOSStart() != OK)
  {
    for(;;){};
  };  

  return 0;

}

/* END main */
/*!
** @}
*/
