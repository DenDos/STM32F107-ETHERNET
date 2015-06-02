#include "stm32f10x.h"
#include "InitS.h"
#include "Set_angle.h"
//#include "client.c"
#include "Math.h"
#include "stm32_eth.h"
#include "netconf.h"
#include "main.h"
#include "Delay.h"
#include "netif.h"
#include "pbuf.h"
#include "udp.h"
#include "httpd.h"
#include "tftpserver.h"
#include "udp.h"
#include "tcp.h"
#include <string.h>
#define SYSTEMTICK_PERIOD_MS  10
void udp_client_callbacka(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
int counter=90;
int counter1=0;
int Array[16][4] = {
									 
									 {0,71,39,0},
                   {45,71,39,0},
                   {45,20,88,0},
                   {45,20,88,1},
                   {45,71,39,1},
                   {0,71,39,1},
                   {0,20,88,1},
                   {0,20,88,0},
                   {0,71,39,0},
                   {-45,71,39,0},
                   {-45,20,88,0},
                   {-45,20,88,1},
                   {-45,71,39,1},
                   {45,71,39,1},
                   {45,20,88,1},
                   {45,20,88,0},
};
/////////Îáüÿâëÿåì âñå ñòðóêòóðû à òàê æå áóôåð ñ òåñòîâûì çíà÷åíèåì/////////////
	 int angle1,angle2,angle3,angle4;
/////////////////////////////////
int main()
	{  
/* Setup STM32 system (clocks, Ethernet, GPIO, NVIC) and STM3210C-EVAL resources */
  System_Setup();

/*ÌÎß ÔÓÍÊÖÈß ÈÍÈÖÈÀËËÈÇÀÖÈÈ ÒÀÉÌÅÐÎÂ È ÑÅÐÂÎ*/
  InitAll(); 
	set_pos(0,0,0,0);		
 
/* Initilaize the LwIP satck */
  LwIP_Init();
	init_TCPclient_listen();
while(1)
{
	//set_pos(angle1,angle2,angle3,angle4);	
		System_Periodic_Handle();
}
}
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

/**
  * @brief  Handles the periodic tasks of the system
  * @param  None
  * @retval None
  */
void System_Periodic_Handle(void)
{
  /* Update the LCD display and the LEDs status */
  /* Manage the IP address setting */
  //Display_Periodic_Handle(LocalTime);
  /* LwIP periodic services are done here */
  LwIP_Periodic_Handle(LocalTime);
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

