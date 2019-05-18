#include<stdint.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define SET_BIT(PORT,BIT) PORT|= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT&= ~(1<<BIT)
struct {

  volatile unsigned int FLAG_ISR1:1;

}FLAG_BIT;
int dis;
int main()
{
  Serial.begin(9600);
    CLR_BIT(DDRD,PD3); // engine switch input
    CLR_BIT(DDRD,PD4); // Echo pin input
    SET_BIT(DDRB,PB4);  // Motor output
    SET_BIT(DDRD,PD6);  // blue led
    SET_BIT(DDRD,PD7);  //Red led
    SET_BIT(DDRB,PB0);  //Green led

    SET_BIT(PORTD,PD3); //Pull up engine switch
   
    
    extint1();

   while(1)
  {
    if(FLAG_BIT.FLAG_ISR1==1)
     {
       dis=ultra_sonicread();

       output();

     }
   else
     {
       
        CLR_BIT(PORTD,PD6); //BLUE
        CLR_BIT(PORTD,PD7); //RED
        CLR_BIT(PORTB,PB0); //GREEN
        CLR_BIT(PORTB,PB4); //MOTOR

     }

  }



}
void extint1()
{
  EICRA|=(1<<ISC10);
    EICRA&=~(1<<ISC11);
    EIMSK|=(1<<INT1);
  sei();
}
ISR(INT1_vect)
{

  FLAG_BIT.FLAG_ISR1=1;

}
float ultra_sonicread()
{
 
  SET_BIT(DDRD,PD4);
  SET_BIT(PORTD,PD4);
  _delay_ms(5);
  CLR_BIT(PORTD,PD4);
  CLR_BIT(DDRD,PD4);
  int time_duration = pulseIn(PD4,HIGH);
  float distance=0.343*time_duration*0.5*0.1;
  Serial.println(distance);
  return(distance);
}
void output()
{
   if(dis<=40)
       {
           CLR_BIT(PORTB,PB0); //GREEN
           SET_BIT(PORTD,PD7); //RED
           CLR_BIT(PORTD,PD6); //BLUE
           SET_BIT(PORTB,PB4); // MOTOR
       }
       else if(dis>40 && dis<=60) //check if distance is in between 40 to 60 cm
        {
            CLR_BIT(PORTB,PB0); //GREEN
            CLR_BIT(PORTD,PD7); //RED
            SET_BIT(PORTD,PD6); //BLUE
            CLR_BIT(PORTB,PB4); // MOTOR
         }
         else
        {
            SET_BIT(PORTB,PB0); //GREEN
            CLR_BIT(PORTD,PD7); //RED
            CLR_BIT(PORTD,PD6); //BLUE
            CLR_BIT(PORTB,PB4); // MOTOR
        }
}
 
