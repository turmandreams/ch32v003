#define BLACK false
#define WHITE true


/* Global define */
#define HIGH   1
#define LOW    0

#define INPUT  0
#define OUTPUT 1

#define PC0    0
#define PC1    1
#define PC2    2
#define PC3    3
#define PC4    4
#define PC5    5
#define PC6    6
#define PC7    7
#define PD0    8
#define PD1    9
#define PD2   10
#define PD3   11
#define PD4   12
#define PD5   13
#define PD6   14
#define PD7   15  // RST
#define PA1   16  // X1
#define PA2   17  // X0



///DigitalRead

#define digitalReadPA1 ((GPIOA->INDR>>1)&1)
#define digitalReadPA2 ((GPIOA->INDR>>2)&1)

#define digitalReadPC0 ((GPIOC->INDR>>0)&1)
#define digitalReadPC1 ((GPIOC->INDR>>1)&1)
#define digitalReadPC2 ((GPIOC->INDR>>2)&1)
#define digitalReadPC3 ((GPIOC->INDR>>3)&1)
#define digitalReadPC4 ((GPIOC->INDR>>4)&1)
#define digitalReadPC5 ((GPIOC->INDR>>5)&1)
#define digitalReadPC6 ((GPIOC->INDR>>6)&1)
#define digitalReadPC7 ((GPIOC->INDR>>7)&1)

#define digitalReadPD0 ((GPIOD->INDR>>0)&1)
#define digitalReadPD1 ((GPIOD->INDR>>1)&1)
#define digitalReadPD2 ((GPIOD->INDR>>2)&1)
#define digitalReadPD3 ((GPIOD->INDR>>3)&1)
#define digitalReadPD4 ((GPIOD->INDR>>4)&1)
#define digitalReadPD5 ((GPIOD->INDR>>5)&1)
#define digitalReadPD6 ((GPIOD->INDR>>6)&1)
#define digitalReadPD7 ((GPIOD->INDR>>7)&1)

//DigitaWrite

#define enciendePC2 GPIOC->BSHR=(1<<2)
#define apagaPC2 GPIOC->BSHR=(1<<(16 + 2))


void cambiaPC2(){
    int val=(GPIOC->OUTDR>>2)&1;
    if(val==0){ enciendePC2; }
    else{ apagaPC2;}
}


int cont=0;

int recibidos[20];

int8_t estadopwm1=0;


void Delay_750ns(void) {
    __asm volatile (
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
    );
}

void Delay_250ns(void) {
    __asm volatile (
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
        "nop\n" // 1 ciclo
    );
}



/*****************************************************************************************
*/
void pinMode(u8 pin, u8 mode)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0}; // Variable used for the GPIO configuration

  switch (pin)
  {
    case PA1:  // X0
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // pin
      break;
    case PA2:  // X1
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // pin
      break;
    case PC0:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // pin
      break;
    case PC1:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // pin
      break;
    case PC2:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // pin
      break;
    case PC3:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // pin
      break;
    case PC4:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // pin
      break;
    case PC5:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // pin
      break;
    case PC6:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // pin
      break;
    case PC7:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; // pin
      break;
    case PD0:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // pin
      break;
    case PD1:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // pin
      break;
    case PD2:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // pin
      break;
    case PD3:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // pin
      break;
    case PD4:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // pin
      break;
    case PD5:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // pin
      break;
    case PD6:
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // pin
      break;
    case PD7:  // RST
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable the clock for Port
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; // pin
      break;
    default:
      break;
  }

  if (mode == OUTPUT)
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // Push-pull output
  }
  else
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // Pull-up input
  }

  // Speed
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  switch (pin)
  {
    case PA1:  // X0
    case PA2:  // X1
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      break;
    case PC0:
    case PC1:
    case PC2:
    case PC3:
    case PC4:
    case PC5:
    case PC6:
    case PC7:
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      break;
    case PD0:
    case PD1:
    case PD2:
    case PD3:
    case PD4:
    case PD5:
    case PD6:
    case PD7:  // RST
      GPIO_Init(GPIOD, &GPIO_InitStructure);
      break;
    default:
      break;
  }
}


void recibedato(){

    int contador=0;
    int contadortiempo=0;

    int valant;
    int datos[17];

    int val=LOW;

    valant=val;
    while(contador<17){
        contadortiempo++;

        //if(GPIOC->INDR!=255){
        //    printf("%d\r\n",GPIOC->INDR);
        //}

        val = digitalReadPC2;  ///Se supone que se lee la entrada PC2
        //val = digitalRead(PC2);

        if(val!=valant){
            valant=val;
            datos[contador]=contadortiempo;
            contadortiempo=0;
            contador++;
        }

        if(contadortiempo>100){
            printf("Fallo\r\n");
            return;
        }
    }

    //while(digitalRead(PC2)==LOW){}

    datos[0]+=3; /// La función Recibe tarda en entrar por eso hay que sumarle al valor.

    //for(int i=0;i<17;i++){  printf("%d,",datos[i]);  }   printf("\r\n");

    int ack=0;

    int lectura=0;
    if(datos[0]>=5) { lectura=1;ack=!ack;}



    int direccion=0;

    for(int i=1;i<6;i++){
            if(datos[i]>=5) {
                switch (i){
                    case 1:
                        direccion+=16;ack=!ack;break;
                    case 2:
                        direccion+=8;ack=!ack;break;
                    case 3:
                        direccion+=4;ack=!ack;break;
                    case 4:
                        direccion+=2;ack=!ack;break;
                    case 5:
                        direccion+=1;ack=!ack;break;
                  }
            }
     }

     int dato=0;

     for(int i=6;i<16;i++){
            if(datos[i]>=5) {
                switch (i){
                    case 6:
                        dato+=512;ack=!ack;break;
                    case 7:
                        dato+=256;ack=!ack;break;
                    case 8:
                        dato+=128;ack=!ack;break;
                    case 9:
                        dato+=64;ack=!ack;break;
                    case 10:
                        dato+=32;ack=!ack;break;
                    case 11:
                        dato+=16;ack=!ack;break;
                    case 12:
                        dato+=8;ack=!ack;break;
                    case 13:
                        dato+=4;ack=!ack;break;
                    case 14:
                        dato+=2;ack=!ack;break;
                    case 15:
                        dato+=1;ack=!ack;break;
                }
            }
     }

     int16_t datoenviar=dato;

     //datoenviar=rand()%1025;
     //datoenviar=32768;
     //datoenviar=255;
     //datoenviar=7;



     if(lectura==1){   ///Nos han solicitado un dato y se lo vamos a enviar, el dato puede ser 16 bits.

            int ack3=0;
            Delay_750ns();
            pinMode(PC2,OUTPUT);
            enciendePC2;
            Delay_750ns();

            for(int i=15;i>=0;i--){

                    cambiaPC2();

                    int estado_bit = (datoenviar >> i) & 1;

                    if(estado_bit==0){ Delay_250ns();}
                    else{ Delay_750ns();ack3=!ack3;}

                }


            ////Ya que los datos son pares finaliza en alto, por lo que hay que enviar un pulso final

            cambiaPC2();

            if(ack3==0){ Delay_250ns(); }
            else{ Delay_750ns(); }

            cambiaPC2();

            Delay_750ns();

            pinMode(PC2,INPUT);

     }

     //printf("%d -> %d -> %d\r\n",lectura,direccion,dato);

     int ack2=0;
     if(datos[16]>=5){ ack2=1; }

     if(ack==ack2){  recibidos[cont]=dato; }
     else{ recibidos[cont]=0; }


     if(lectura==1){
         printf("%d,%d\r\n",dato,datoenviar);
         //printf("Dato enviado : %d\r\n",datoenviar);
     }else{
         printf("%d\r\n",dato);
     }



}
