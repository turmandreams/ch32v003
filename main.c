
//#include "debug.h"
#include "oled_segment.h"
#include "turmantrain.h"

#include "turman.h"

vu8 val;

#define BLACK false
#define WHITE true


void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}




void pintavias(int instantey,int instantetren,int opcion){

	//float val1,val2;
	//int vall1,vall2;
	//int x1,x2,x3,x4,x5,x6;

    drawLine(0,10,127,10,WHITE);  //Linea Horizonte

    drawLine(0,63,40,10,WHITE);  ///Linea Vertical
    drawLine(42,63,55,10,WHITE);
    drawLine(84,63,70,10,WHITE);
    drawLine(127,63,87,10,WHITE);

/*
    for(int i=11+instantey;i<63;i+=10){

    	val1=0.69*i;
    	val2=0.24*i;

    	vall1=(int)val1;
    	vall2=(int)val2;


    	x1=50-vall1;x2=54-vall2;
        x3=61-vall2;x4=66-vall2;
        x5=72-vall2;x6=79-vall1;


        if((i>instantetren)||(opcion==0)){ drawLine(x1,i,x2,i,WHITE);   }
        if((i>instantetren)||(opcion==1)){ drawLine(x3,i,x4,i,WHITE);   }
        if((i>instantetren)||(opcion==2)){ drawLine(x5,i,x6,i,WHITE);   }

    }
*/

}


void juegotren(){

	bool sigue=true;
    int posicion=1;

    pinta(logoturmantrain,0,0,128,64);
    OLED_DisplayBuffer();

    Delay_Ms(1000);

    while(sigue){

    	//Derecha
    	if(digitalReadPD4!=1) { sigue=false;  }

    	//Izquierda
    	if(digitalReadPC0!=1) { sigue=false;  }

    	//Arriba
    	if(digitalReadPC4!=1) {  sigue=false; }

    	//Abajo
        if(digitalReadPD0!=1) {  sigue=false; }

        //Boton 1
        if(digitalReadPD3!=1) {  sigue=false; }

        //Boton 2
       if(digitalReadPD2!=1) {   sigue=false; }


    }

    sigue=true;


    float instante=0;
    int instantey=0;
    int instantey2=0;
    int ins=(float)instante;
    float instantef=0.0;
    float velocidad=0.3;

    int opcion=rand() % 3;

    int puntos=0;

    int trentam=instantey*1100;

    int x1=0;

    int val2=0;

    int contadorteclas=0;

    bool moneda=true;

    //limpiabuffer();

    instantey=(int)instantef;

    while(sigue){

      limpiabuffer();

      instantey2=instantey/2;

      if(contadorteclas==0){
    	  if(digitalReadPD4!=1) {  //Derecha
    		  contadorteclas=5;
    		  posicion++;
    		  if(posicion>2){ posicion=2;}
    		  //printf("PD4\n");
    	  }
    	  if(digitalReadPC0!=1) {  //Izquierda
    		  contadorteclas=5;
    		  posicion--;
    		  if(posicion<0){ posicion=0;}
    		  //printf("PC0\n");
    	  }

      }else{
    	  contadorteclas--;
      }


      //if(digitalRead(25)==LOW){ posicion=2; }
      //else if(digitalRead(26)==LOW){ posicion=1; }
      //else if(digitalRead(27)==LOW){ posicion=0; }

      ins=(float)instante;

      pintavias(ins,instantey,opcion);

      trentam=instantey*1100;

      if(instantey<56){
		  if(opcion!=0){ x1=48-instantey; pintaescalado(tren,x1,5,20,20,trentam/1000,trentam/1000);}
		  if(opcion!=1){ x1=60-instantey2; pintaescalado(tren,x1,5,20,20,trentam/1000,trentam/1000);}
		  if(opcion!=2){ x1=75; pintaescalado(tren,x1,5,20,20,trentam/1000,trentam/1000);}
      }else{
    	  if(opcion!=0){ x1=48-instantey; pintaescaladodifuso(tren,x1,5,20,20,trentam/1000,trentam/1000);}
    	  if(opcion!=1){ x1=60-instantey2; pintaescaladodifuso(tren,x1,5,20,20,trentam/1000,trentam/1000);}
    	  if(opcion!=2){ x1=75; pintaescaladodifuso(tren,x1,5,20,20,trentam/1000,trentam/1000);}
      }



      if(moneda){
          if(opcion==0){  val2=55-instantey;  }
          else if(opcion==1){ val2=65-(instantey/4); }
          else if(opcion==2){ val2=75+instantey2;; }

    	  pintaescalado(dolar,val2,instantey,20,20,instantey2,instantey2);


      }


      if(posicion==0){ pinta(player,12,45,20,20); }
      else if(posicion==1){ pinta(player,53,45,20,20);}
      else if(posicion==2){ pinta(player,95,45,20,20);}

      //fillRectBlack(0,0,128,10);

      pintatexto("Puntos : ",0,0,false);
      pintaint(puntos,54,0,false);

      //OLED_print("Puntos : ");
      //OLED_printD(puntos,false);

      //printf("Puntos : %d\n",puntos);

      OLED_DisplayBuffer();

      instante+=(velocidad*2);
      if(instante>10.0) { instante=0; }

      instantef+=velocidad;

      instantey=(int)instantef;

      if(instantey>63) {
          opcion=rand() % 3;
          instantef=0.0;
          instantey=0;
          velocidad+=0.01;
          moneda=true;
      }



      if((posicion==opcion)&&(instantey>45)&&(moneda)){
          puntos+=10;
          moneda=false;
      }


      if((instantef>55)&&(opcion!=posicion)){
    	  limpiabuffer();
          pintatexto("GAME OVER",35,30,false);
          OLED_DisplayBuffer();
          Delay_Ms(5000);
          sigue=false;
      }


    }

}



int main(void)
{
    GPIO_Config();

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    pinMode(PC0,INPUT);
    pinMode(PD0,INPUT);

    pinMode(PC4,INPUT);
    pinMode(PD4,INPUT);

    pinMode(PD2,INPUT);
    pinMode(PD3,INPUT);

    USARTx_CFG();

    OLED_init();

    /*
    limpiabuffer();

    drawLine(0,0,127,0,true);
    drawLine(0,0,0,63,true);
    drawLine(0,63,127,63,true);
    drawLine(127,63,127,0,true);

    drawLine(0,0,127,63,true);
    drawLine(0,63,127,0,true);

    OLED_DisplayBuffer();

    Delay_Ms(1000);
*/

    while(1){

    	juegotren();

    }


}
