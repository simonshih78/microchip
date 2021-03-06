/*Inclus�o do arquivo header (*.h)  para o microcontrolador utilizado*/
#include<16F876A.h>
 
#use delay (clock=8000000)                 /*Defini��o da frequ�ncia do cristal para c�lculo 
                                                             ..dos delays*/
#fuses HS, NOWDT, PUT,BROWNOUT, NOLVP  //Configura��o dos bits de controle

//Defini��es para a comunica��o serial
#use RS232 (Baud=4800, xmit = PIN_C6, rcv = PIN_C7)

//Derfini��es para o protocolo I2C
#use i2c(master, sda=PIN_C4, scl=PIN_C3)

//Fun��o para envio de dados para um endere�o da mem�ria EEPROM por I2C
void esc_ext_eeprom(long int end, int dado) //end = 1 dado = 'C'
{
I2C_start();            //Envia uma condi��o de 'start' ao barramento I2C  
I2C_Write(0xA0);        // 0b1010 000 0 Identifica dispositivo para escrita
I2C_Write(end);         //Identifica endere�o na mem�ria
I2C_Write(dado);        //Envia o dado a ser gravado
I2C_Stop();             //Envia uma condi��o de 'stop' ao barramento I2C
delay_ms(5);            //Atraso de 5ms
}

//Fun��o para leitura de um dado por I2C num endere�o da mem�ria EEPROM externa  
int le_ext_eeprom(long int end) // end = 0
{
int leitura;
I2C_Start();                    //Envia uma condi��o de 'start' ao barramento I2C  
I2C_Write(0xA0);      //Identifica dispositivo para escrita
I2C_Write(end);       //Identifica endere�o na mem�ria
I2C_Start();              //Condi��o de Restart
I2C_Write(0xA1);          //0b 1010 000 1 Identifica dispositivo para leitura
leitura = I2C_Read(0);        //Faz a leitura do dado (NO acknowledge)
I2C_Stop();                     //Envia uma condi��o de 'stop' ao barramento I2C
delay_ms(5);         //Atraso de 5ms
return leitura;         //retorna leitura
}

void main()
{
//Envia a string pela serial e muda o cursor para a pr�xima linha
printf("Microcontroladores PIC 16F876A.\r\n");

//Envia a string pela serial e muda o cursor para a pr�xima linha
printf("Utilizando o protocolo I2C...\r\n");

//Envia a string pela serial e muda o cursor para a pr�xima linha
printf("Pressione B1 para gravar e B2 para ver.\r\n");

while(true)
   {
   if (input(PIN_A2))        //Se o bot�o B0 for pressionado
      {
      //Envia a string pela serial e muda o cursor para a pr�xima linha
      printf("Gravando na memoria EEPROM externa...\r\n");

      esc_ext_eeprom(0,'A');     //Escreve no endere�o 0 da mem�ria o caractere 'A'
      esc_ext_eeprom(1,'C');     //Escreve no endere�o 1 da mem�ria o caractere 'C'
      esc_ext_eeprom(2,'E');     //Escreve no endere�o 2 da mem�ria o caractere 'E'
      esc_ext_eeprom(3,'P');     //Escreve no endere�o 3 da mem�ria o caractere 'P'
      esc_ext_eeprom(4,'I');     //Escreve no endere�o 4 da mem�ria o caractere 'I'
      esc_ext_eeprom(5,'C');     //Escreve no endere�o 5 da mem�ria o caractere 'C'

      delay_ms(500);          //Atraso de 500ms

      //Envia a string pela serial e muda o cursor para a pr�xima linha
      printf("Pressione B2 para ver.\r\n");
      }
   
   if (input(PIN_A4))         //Se o bot�o B1 for pressionado
      {
      /*Envia as strings pela serial formatadas com os valores lido dos enderecos
        0 at� 5 da mem�ria EEPROM externa*/
      printf("Endereco 00 da memoria: %c\r\n",le_ext_eeprom(0));
      printf("Endereco 01 da memoria: %c\r\n",le_ext_eeprom(1));
      printf("Endereco 02 da memoria: %c\r\n",le_ext_eeprom(2));
      printf("Endereco 03 da memoria: %c\r\n",le_ext_eeprom(3));
      printf("Endereco 04 da memoria: %c\r\n",le_ext_eeprom(4));
      printf("Endereco 05 da memoria: %c\r\n",le_ext_eeprom(5));
      }
   }
}
