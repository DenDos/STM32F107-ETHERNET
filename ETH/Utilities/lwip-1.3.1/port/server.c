/**
  ******************************************************************************
  * @file    server.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   A sample UDP/TCP server application.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "pbuf.h"
#include "udp.h"
#include "tcp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "initS.h"
#include <string.h>
#include "Set_angle.h"
//#include "malloc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UDP_SERVER_PORT    7   /* define the UDP local connection port */
#define UDP_CLIENT_PORT    454   /* define 4 the UDP remote connection port */
#define TCP_PORT    6666	/* define the TCP connection port */

struct ip_addr BindTcpAddr;		
err_t close_tcp;
u8_t *state;
int len;

//volatile char *rdatatad;
char * rdatatad;
char *data = {"Connection established\n"};
int led_flag = 0;
int socket_flag = 0;
int mass[4];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void udp_server_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
err_t tcp_server_accept(void *arg, struct tcp_pcb *pcb, err_t err);
static err_t connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
err_t sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len);
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
void led_control(char data);
void receivedData(char *data, struct tcp_pcb *pcb);

/* Private functions ---------------------------------------------------------*/

/*Закрываем соединение*/
int socket_close()
{
	socket_flag=0;
}
/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void tcp_err_callback(void *arg, err_t err) 
{ 
mem_free(arg); 
} 	

err_t tcp_poll_callback(void *arg, struct tcp_pcb *pcb) 
{ 
	u8_t *state = (u8_t *)arg; 

if (tcp_close(pcb) == ERR_OK) 
{ 
if ((*state & 2) == 2) /* close received */ 
{
	tcp_err(pcb, NULL); 
	tcp_recv(pcb, NULL); 
	tcp_sent(pcb, NULL); 
	tcp_poll(pcb, NULL, 0); 
	mem_free(state); 
}
else /* close not yet received */ 
	*state = 255; 
} 
}
//===============================================================|===============
// 	@Описание: Инициаллизация TCP в режиме Listen

//==============================================================================
void init_TCPclient_listen()
{
		
		struct tcp_pcb *pcb;	
		IP4_ADDR(&BindTcpAddr, 192, 168, 1, 107 );//195, 151, 217, 149
	
		/* Create a new TCP control block  */
		pcb = tcp_new();
		//tcp_poll(pcb, tcp_poll_callback, 10);
	
		if(pcb !=NULL)
		{
			err_t err;	  
					
			/* Assign to the new pcb a local IP address and a port number */
			err = tcp_bind(pcb, IP_ADDR_ANY , TCP_PORT);
			
		if(err != ERR_USE)
		{
			/* Set the connection to the LISTEN state */
				pcb = tcp_listen(pcb);
			
				/* Specify the function to be called when a connection is established */
				tcp_accept(pcb, tcp_server_accept);
		}
		else
		{
			/* We enter here if a conection to the addr IP address already exists */
			/* so we don't need to establish a new one */
			close_tcp =tcp_close(pcb);
		}            
		}
   
}
////////////////////////////////////////

err_t server_init()
{
		struct udp_pcb *upcb; 
		struct tcp_pcb *pcba;	 
		struct ip_addr DestIPaddres;	 
		struct ip_addr DestIPaddr;
		err_t err_out;
    u8_t *state;
				
		if ((state = mem_malloc(1)) == NULL)  
			{ 
				return ERR_MEM; 
			} 
		/* Create a new UDP control block */
		pcba = tcp_new();
		
		IP4_ADDR(&DestIPaddres, 192, 168, 1, 107 );//195, 151, 217, 149
		
		if (pcba!=NULL)
				IP4_ADDR(&DestIPaddr, 192, 168, 1, 54 );//195, 151, 217, 149
		else 
		{
				mem_free(state); 
				return ERR_MEM; 
		}
		
		tcp_arg(pcba, state); 
    tcp_err(pcba, tcp_err_callback); 
    tcp_recv(pcba, tcp_server_recv); 
    tcp_sent(pcba, NULL); 
    tcp_poll(pcba, tcp_poll_callback, 10);//НЕ МОГУ ПОНЯТЬ НУЖНА ОНА ИЛИ НЕТ

		err_out = tcp_bind(pcba, IP_ADDR_ANY, 6644);
		if(err_out != ERR_USE)
		{
      pcba = tcp_listen(pcba);
      tcp_accept(pcba, tcp_server_accept);
		}
		else
		{
	  tcp_close(pcba);
		}    

  return ERR_OK;
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_server_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
  struct tcp_pcb *pcb;
  __IO uint8_t iptab[4];
  uint8_t iptxt[20];
  
  /* We have received the UDP Echo from a client */
  /* read its IP address */
  iptab[0] = (uint8_t)((uint32_t)(addr->addr) >> 24);
  iptab[1] = (uint8_t)((uint32_t)(addr->addr) >> 16);
  iptab[2] = (uint8_t)((uint32_t)(addr->addr) >> 8);
  iptab[3] = (uint8_t)((uint32_t)(addr->addr));

  sprintf((char*)iptxt, "is: %d.%d.%d.%d     ", iptab[3], iptab[2], iptab[1], iptab[0]);	                           	                             

 
  /* Connect to the remote client */
  udp_connect(upcb, addr,555);//UDP_CLIENT_PORT
    
  /* Tell the client that we have accepted it */
  udp_send(upcb, p);

  /* free the UDP connection, so we can accept new clients */
  udp_disconnect(upcb);
	
  /* Bind the upcb to IP_ADDR_ANY address and the UDP_PORT port*/
  /* Be ready to get a new request from another client */  
  udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT );//
	
  /* Set a receive callback for the upcb */
  udp_recv(upcb, udp_server_callback, NULL);    	
	
  /* Create a new TCP control block  */
  pcb = tcp_new();
	
  if(pcb !=NULL)
  {
    err_t err;	  
	      
    /* Assign to the new pcb a local IP address and a port number */
    err = tcp_bind(pcb, addr, TCP_PORT);
	  
	if(err != ERR_USE)
	{
	  /* Set the connection to the LISTEN state */
      pcb = tcp_listen(pcb);
    
      /* Specify the function to be called when a connection is established */
      tcp_accept(pcb, tcp_server_accept);
	}
	else
	{
	  /* We enter here if a conection to the addr IP address already exists */
	  /* so we don't need to establish a new one */
	  tcp_close(pcb);
	}            
  }

  /* Free the p buffer */
  pbuf_free(p);
   
}
////////////////////////////////////////
err_t sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
 return ERR_OK;
}
///////////////////////////////////////

//////////calback вызывается когда соединение утановлено,вроде как :о////
static err_t connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
        LWIP_UNUSED_ARG(arg);
				if(err == ERR_OK)
					{
						tcp_write(tpcb, data, strlen(data), 0);
						tcp_output(tpcb);
					}
					
  				return err;
}



/**
  * @brief  This funtion is called when a TCP connection has been established on the port TCP_PORT.
  * @param  arg	user supplied argument 
  * @param  pcb	the tcp_pcb which accepted the connection
  * @param  err error value
  * @retval ERR_OK
  */
err_t tcp_server_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{ 
	        LWIP_UNUSED_ARG(arg);
				if(err == ERR_OK)
					{
						tcp_write(pcb, data, strlen(data), 0);
						tcp_output(pcb);
					}
  /* Specify the function that should be called when the TCP connection receives data */
  tcp_recv(pcb, tcp_server_recv);

  return ERR_OK;  
}

/**
  * @brief  This function is called when a data is received over the TCP_PORT.
  *         The received data contains the number of the led to be toggled.
  * @param  arg	user supplied argument 
  * @param  pcb	the tcp_pcb which accepted the connection
  * @param  p the packet buffer that was received
  * @param  err error value
  * @retval ERR_OK
  */
void servo_cont(int angle1,int angle2,int angle3,int angle4)
{
	set_pos(angle1,angle2,angle3,angle4);
}
void led_cont(struct tcp_pcb *pcb,char *rdata)
{
	char *ALL_LED = {"on\r\n"};
	char *ALL_LED_OFF = {"off\r\n"};
	char *LED1 = {"led1\r\n"};
	char *LED2 = {"led2\r\n"};
	char *LED3 = {"led3\r\n"};
	char *LED4 = {"led4\r\n"};
	char *EXIT = {"exit\r\n"};
	led_flag++;

	if (led_flag>1) led_flag=0;
	
	if (!strncmp(rdata,ALL_LED,3)&& (led_flag==1))
	{
		GPIO_SetBits(GPIOB , LEDS);
		tcp_write(pcb, "LEDS ON\r\n",11, 0);
		tcp_output(pcb);
	}else if (!strncmp(rdata,ALL_LED_OFF,3)&& (led_flag==1))
	{
		GPIO_ResetBits(GPIOB , LEDS);
		tcp_write(pcb, "LEDS OFF\r\n",12, 0);
		tcp_output(pcb);
	}	else if (!strncmp(rdata,LED1,4)&& (led_flag==1))
	{
		STM_EVAL_LEDToggle(0);
	}else if (!strncmp(rdata,LED2,4)&& (led_flag==1))
	{
		STM_EVAL_LEDToggle(1);
	}else if (!strncmp(rdata,LED3,4)&& (led_flag==1))
	{
		STM_EVAL_LEDToggle(2);
	}else if (!strncmp(rdata,LED4,4)&& (led_flag==1))
	{
		STM_EVAL_LEDToggle(3);
	}else if (!strncmp(rdata,EXIT,4)&& (led_flag==1))
	{
		tcp_write(pcb, "Goodbye!\r\n",12, 0);
		tcp_output(pcb);
		tcp_close(pcb);
		GPIO_ResetBits(GPIOB , LEDS);
		init_TCPclient_listen();
	}
}
/*ФУНКЦИЯ ОБРАБОТНКИ ПРИНЯТЫХ ДАННЫХ*/
void receivedData(char *data, struct tcp_pcb *pcb)
{
	int i=2,bufMas=4,m=0;
	/*Проверяем пришли ли нужные нам данные*/
	if (!strncmp(data,"SET|",3))
	{
			while (bufMas<14)
	{
		if (data[bufMas]== ',') 
		{
			bufMas++;
			m++;
			continue;
		}
		if(data[bufMas]>='0'&&data[bufMas]<='9')
		{
			mass[m]=mass[m]*10+(data[bufMas]-'0');
		}
		bufMas++;
	}
		
	
		if(mass[3] == 1) mass[3] =25;
			else	mass[3] = 0;
		
		if (data[4] == '-')
				mass[0]*=-1;
	
		servo_cont(mass[0],mass[1],mass[2],mass[3]);
		tcp_write(pcb, "OK!",3, 0);
		tcp_output(pcb);
		bufMas=6;
		m=0;
	}
	
	if (!strncmp(data,"led",3)||!strncmp(data,"exit",4))
		{
		led_cont(pcb,data);
		}
	
	memset (mass, 0, sizeof(mass));//очищаем массив с принятыми данными
}
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *pm, err_t err)
{
	
	int l=0;
	int i=2,gh=4,m=0;
	rdatatad = pm->payload;
	Delay(400);
	
		//tcp_write(pcb, "OK!",3, 0);
	//	tcp_output(pcb);
 	receivedData(rdatatad,pcb);
	tcp_recved(pcb, pm->tot_len); 
	
	/* Free the p buffer */
	rdatatad = 0;
	pbuf_free(pm);


}



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
