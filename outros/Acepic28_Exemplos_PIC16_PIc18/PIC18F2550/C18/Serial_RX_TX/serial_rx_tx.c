/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Leitura dos canais anal�gicos AN0 e AN3 com envio serial.           *
*Obs.:     Ligar chave 1 do DIP DP1 para Trimpot ADC1 para conex�o ao canal    *
*          anal�gico AN0 e chave 3 do DIP conecta o Sensor de Temperatura LM35 *
*          ao canal anal�gico AN3.                                             *
********************************************************************************/
#include <p18F2550.h>
#include <delays.h>
#include <usart.h>
#include <stdio.h>

// Frequencia do oscilador = 8MHz
// Ciclo de m�quina  = 1/(Fosc/4) = 0,5us

#pragma config FOSC = HS       		//Configura o oscilador a cristal
#pragma config CPUDIV = OSC1_PLL2 	//Postscale clock do sistema 
#pragma config WDT = OFF  			//Desabilita o Watchdog Timer (WDT).
#pragma config PWRT = ON   			//Habilita o Power-up Timer (PWRT).
#pragma config BOR = ON				//Habilita Brown-out reset 
#pragma config BORV = 1	        	//Tens�o do BOR � 4,33V.
#pragma config LVP = OFF       		//Desabilita o Low Voltage Program.

unsigned long res_ad;	//vari�vel de armazenamento da convers�o AD
unsigned long res_temp;	//vari�vel de armazenamento da convers�o AD
unsigned long tensao;	//variavel de resultado em tens�o
unsigned long temp;     //vari�vel de resultado da temperatura

char rec[5];

void trata_serial();
void msg_inicial();

void trata_serial()
{
if (rec[0]=='A' && rec[1]=='D' && rec[2]=='0')
	{
	ADCON0 = 0b00000001;  		/*Seleciona Canal 0 para convers�o
                                  ...e habilita o conversor AD*/
    ADCON0bits.GO = 1;          //Inicializa a convers�o
    while(ADCON0bits.GO);       //Aguarda o t�rmino da convers�o
 
    res_ad = ADRES;             //atribui o valor convertido � vari�vel res_ad0
 
    tensao = (res_ad*5000) / 1023;    //Calcula o valor em mV para o resultado obtido
    
	printf("\r\nAN0 = %04lu mV.\r\n\r\n", tensao); 
    
	}

if (rec[0]=='T' && rec[1]=='M' && rec[2]=='P')
	{
	ADCON0 = 0b00001101;         /*Seleciona Canal 3 para convers�o        
                                   ...e habilita o conversor AD*/
    ADCON0bits.GO = 1;          //Inicializa a convers�o
    while(ADCON0bits.GO);       //Aguarda o t�rmino da convers�o
 
    res_ad = ADRES;             //atribui o valor convertido � vari�vel res_ad0
 
    temp = (res_ad*500) / 1023;    //Calcula o valor em mV para o resultado obtido
    
	printf("\r\nTemperatura = %02lu C.\r\n\r\n", temp);
    
	}


	
}


void msg_inicial()
{
printf("Kit de Desenvolvimento ACEPIC 28\r\n");
Delay10KTCYx(1);   	//Gera um delay de 5ms
printf("Microcontrolador: PIC 18F2550\r\n");
Delay10KTCYx(1);   	//Gera um delay de 5ms
printf("\r\n-Envie AD0 para retornar Conversao A/D do Canal 0.\r\n");
Delay10KTCYx(1);   	//Gera um delay de 5ms
printf("-Envie TMP para retornar Conversao A/D do Canal 3. (Temperatura)\r\n\r\n");

}

void main()
{
int i;

TRISAbits.TRISA0 = 1;   //Direciona o pino 0 da porta A como entrada
TRISAbits.TRISA3 = 1;   //Direciona o pino 3 da porta A como entrada
TRISCbits.TRISC2 = 0;	//Direciona o pino 2 da porta C como sa�da (Aquecimento)
TRISAbits.TRISA4 = 1;   //Direciona o pino 4 da porta A como entrada (B2) 

OpenUSART(USART_TX_INT_OFF		//desabilita interrup��o de transmiss�o
			& USART_RX_INT_OFF	//desabilita interrup��o de recep��o
			& USART_ASYNCH_MODE	//modo ass�ncrono
			& USART_EIGHT_BIT		//transmiss�o e recep��o em 8 bits
			& USART_BRGH_HIGH,	//Baud Rate em alta velocidade
			25);					//SPBRG p/ 19200 bps 
                   
ADCON2 = 0b10100001;     /*ADFM1 = 1 -> Resultado da convers�o AD
                                    ... justificado � direita
                                    -
                                    ****Velocidade de aquisi��o em 8TAD
                                    ACQT2 = 1
                                    ACQT1 = 0
                                    ACQT0 = 0
                                    ****Fonte de clock em Fosc/8
                                    ADCS2 = 0
                                    ADCS1 = 0
                                    ADCS0 = 1*/        
 
  
ADCON1 = 0b00001011;      /* -
                           	 -
                             VCFG1 = 0 -> Vref- = terra
                             VCFG0 = 1 -> Vref+ = VDD
                             *****Seleciona os canais AN0 e AN3 como anal�gicos
                             PCFG3 = 1
                             PCFG2 = 1
                             PCFG1 = 0
                             PCFG0 = 1*/   

msg_inicial();                                              


while(1)
	{
     while(!DataRdyUSART());  //Aguarda a chegada de um caractere no buffer de recep��o
          
	 rec[i] = getcUSART();	    //recebe o caractere e armazena no �ndice n_dado da 
								//matriz n_dado

	 putcUSART(rec[i]);
	 Delay10KTCYx(1);   	//Gera um delay de 5ms
     i++;

	 if(rec[i-1]==0x0D)
		{
		trata_serial();
		i=0;
		msg_inicial();
		} 
 
     
    
     }  
}


