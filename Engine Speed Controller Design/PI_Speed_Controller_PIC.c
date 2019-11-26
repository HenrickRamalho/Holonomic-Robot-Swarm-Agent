
int t_100ms = 0;      //Acumulador para Timer1
float MV = 0;         //Variável Manipulada - PWM
float PV = 0;         //Variável de Processo - Encoder
int SP = 0;          //Range 0 a 100 Pulsos por 0,1 Segundos
float ER = 0;         //Erro
float Er_i = 0;       //Erro Integral
float Kp = 0.95;      //Ganho Proporcional
float Ki = 0.35;      //Ganho Integral

int uart_r = 0;
int flag = 0;

void main()
{
 TRISA = 0b00000000;
 PORTA = 0b00000000;
 TRISB = 0b01000010;
 PORTB = 0b00000000;

 INTCON = 0b11100000;
 CMCON = 0b00000111;
 TMR0 = 0;
 OPTION_REG = 0b00000110;   //0,032768 segundos por estouro - 3 estouros para 0,1 segundo

 TMR1L = 0;
 TMR1H = 0;
 T1CON = 0b00000011;

 PWM1_Init(3000);
 PWM1_Start();

 UART1_Init(9600);
 Delay_ms(10);

 while(1){
  if(UART1_Data_Ready()){
   uart_r = UART1_Read();
   if(uart_r == 255){
    uart_r = 0;
    flag = 1;
    while(flag == 1){
     if(UART1_Data_Ready()){
      SP = UART1_Read();
      flag = 2;
      while(flag == 2){
       if(UART1_Data_Ready()){
        uart_r = UART1_Read();
        if(uart_r == 'b'){
         PORTB.F4 = 0;
         PORTB.F5 = 1;
         flag = 0;
        }
        if(uart_r == 'f'){
         PORTB.F4 = 1;
         PORTB.F5 = 0;
         flag = 0;
        }
       }
      }
     }
    }
   }
  }
 }
}

void interrupt(void){
 if(INTCON.T0IF == 1){
  INTCON.T0IF = 0;
  TMR0 = 0;
  t_100ms++;

  if(t_100ms == 3){
   T1CON = 0b00000010;
   t_100ms = 0;
   PORTB.F0 = !PORTB.F0;
   PV = TMR1L;
   UART1_Write(PV);
   ER = SP - PV;
   ER_i = ER + ER_i;

   MV = (Kp * ER) + (ki * ER_i);
   
   if(MV > 255){
    MV = 255;
   }
   if(MV < 0){
    MV = 0;
   }
   PWM1_Set_Duty(MV);
   TMR1L = 0;
   TMR1H = 0;
   T1CON = 0b00000011;
  }
 }
}