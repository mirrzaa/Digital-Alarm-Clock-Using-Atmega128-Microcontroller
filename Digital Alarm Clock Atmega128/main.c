/*
 * Project_4.c
 *
 *  Team WaduDev 16-3
 *	u1610146 -> Mirzashomol Karshiev (Team Leader)
 *  u1610131 -> Madiyor Abdukhashimov
 *  u1610137 -> Mardon Zarifjonov 
 *
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "_main.h"
#include "lcd_n.h"



//Timers variables
int cnt;
char sec,min,hour;

//Variables for Stop watch
unsigned char bool=0;
unsigned char s_sec,s_min;
unsigned char s_hour;
unsigned char ch=0;


//Variables for setting time
unsigned char t_hour=0;
unsigned char t_min=0;
unsigned char t_sec=0;
unsigned char t_bool=0;
unsigned char set_bool=0;


//Variables for setting alarm
unsigned char alarm_bool=6;
unsigned char a_hour=0;
unsigned char a_min=0;
unsigned char a_sec=0;
unsigned char a_swith=0;
unsigned char sw_counter=0;
char a_day[3];


//stop watch boolean to stop
unsigned char stp_bool=0;

char str[]="2020 1/5";
char AM[]="AM ";
char PM[]="PM ";
char ON[]="STOPWATCH:ON";
char OFF[]="STOPWATCH:OFF";
unsigned char stop_watch_bool=0;
unsigned char day_chooser=0;
char days[3];



//<----------Initialize Timer0----------->
void Init_timer0(void)
{
	OCR0=99;			//count 0 to 99
	TIMSK=0x02;			//time0/counter compare interrupt enable
}


//<----------PD1(INT0) For Switching Between Modes----------->
SIGNAL(INT0_vect)
{
	TCCR0=0x0f;
	bool++;
	stop_watch_bool=0;
	
		
	if (bool==3)
	{
		t_bool=1;
		bool=0;
	}

	if (bool==2)
	{
		sw_counter++;
		t_bool=0;
		alarm_bool=0;
		day_chooser=0;
		t_hour=t_min=t_sec=0;
	}
	
	if (bool==1)
	   {
		   s_hour=s_min=s_sec=0;
		   stp_bool=1;
	   }
	   
}



//<----------PD2(INT2) For Setting Timer----------->
SIGNAL(INT2_vect)
{
	//Setting the Time
	if(set_bool==0)
	{
		t_hour++;
		if (t_hour==24)
			t_hour=0;
	}
	 if(set_bool==1)
	{
		t_min++;
		if (t_min==60)
			t_min=0;
	}
	 if(set_bool==2)
	{
		t_sec++;
		if (t_sec==60)
			t_sec=0;
	}
	if(set_bool==3)
	{
		switch(day_chooser)
		{
			case 0:
				days[0]='M'; days[1]='o';days[2]='n';		//Monday
				break;
			case 1:
				days[0]='T';days[1]='u';days[2]='e';		//Tuesday
				break;
			case 2:
				days[0]='W';days[1]='e';days[2]='d';		//Wednesday
				break;
			case 3:
				days[0]='T';days[1]='h';days[2]='u';		//Thursday
				break;
			case 4:
				days[0]='F';days[1]='r';days[2]='i';		//Friday
				break;
			case 5:
				days[0]='S';days[1]='a';days[2]='t';		//Saturday
				break;
			case 6:
				days[0]='S';days[1]='u';days[2]='n';		//Sunday
				break;
			case 7:
				days[0]='M';days[1]='o';days[2]='n';		//Monday
				day_chooser=0;
				break;	
		}
		day_chooser++;
	}
	
	//Setting the Alarm
	if(alarm_bool==0)
	{
		t_hour++;
		if (t_hour==24)
			t_hour=0;
	}
	if(alarm_bool==1)
	{
		t_min++;
		if (t_min==60)
			t_min=0;
	}
	if(alarm_bool==2)
	{
		t_sec++;
		if (t_sec==60)
			t_sec=0;
	}
	if(alarm_bool==3)
	{
		switch(day_chooser)
		{
			case 0:
			days[0]='M'; days[1]='o';days[2]='n';		//Monday
			break;
			case 1:
			days[0]='T';days[1]='u';days[2]='e';		//Tuesday
			break;
			case 2:
			days[0]='W';days[1]='e';days[2]='d';		//Wednesday
			break;
			case 3:
			days[0]='T';days[1]='h';days[2]='u';		//Thursday
			break;
			case 4:
			days[0]='F';days[1]='r';days[2]='i';		//Friday
			break;
			case 5:
			days[0]='S';days[1]='a';days[2]='t';		//Saturday
			break;
			case 6:
			days[0]='S';days[1]='u';days[2]='n';		//Sunday
			break;
			case 7:
			days[0]='M';days[1]='o';days[2]='n';		//Monday
			day_chooser=0;
			break;
		}
		day_chooser++;
	}
	
}

//<----------PD3(INT3) For Stopping Timer----------->
SIGNAL(INT3_vect)
{
	if (stp_bool==1 && bool==1)
	{
		LCD_Clear();
		stop_watch_bool=1;
		TCCR0=0x00;				//stop the timer
	}
	
	//for setting the time
	if (set_bool==0)
	{
		hour=t_hour;
		set_bool=1;
	}
	else if (set_bool==1)
	{
		min=t_min;
		set_bool++;
	}
	else if (set_bool==2)
	{
		
		sec=t_sec;
		set_bool++;
	}else if (set_bool==3)
	{
		set_bool++;
		TCCR0=0x0f;
		t_bool=1;
	}
	
	
	//For setting the alarm
	if (alarm_bool==0)
	{
		a_hour=t_hour;
		alarm_bool++;
	}
	else if (alarm_bool==1)
	{
		a_min=t_min;
		alarm_bool++;
	}
	else if (alarm_bool==2)
	{
		a_sec=t_sec;
		alarm_bool++;
	}else if (alarm_bool==3)
	{
		alarm_bool++;
		
		TCCR0=0x0f;
		t_bool=1;
		bool=0;
	}
	
}

//<----------Timer0/Counter----------->
ISR(TIMER0_COMP_vect)
{
	
	cnt++;
	if(cnt==144)
	{
		cnt=0;
		sec++;
		s_sec++;
		if(sec>=60)
		{
			min++;
			s_min++;
			sec=0;
			s_sec=0;
		}
		if(min>=60)
		{
			hour++;
			s_hour++;
			min=0;
			s_min=0;
		}
		if (hour>=24)
		{
			hour=0;
			s_hour=0;
		}
	}
}


//<----------Normal Time Function----------->
void Time()
{
	LCD_pos(0,0);	//Move position to 0,0
	LCD_STR(str);	//OUTPUT: TIME FUNCTION
	
	LCD_pos(9,0);
	LCD_STR(days);
	
	 if (a_hour==hour && a_min==min && a_sec==sec)
	{
		for (char i=0;i<2;i++)
		{
			PORTB=0x00;		//LED blinking
			 _delay_ms(1000);
			PORTB=0xff;
			 _delay_ms(1000);
		 }
	 }
		  
	
	if (hour>12)
	{
		LCD_pos(0,1);
		LCD_STR(PM);
		LCD_pos(3,1);
		LCD_CHAR((hour-12)/10+'0');
		LCD_CHAR((hour-12)%10+'0');
		LCD_CHAR(':');
	}
	else
	{
		LCD_pos(0,1);
		LCD_STR(AM);
		LCD_pos(3,1);
		LCD_CHAR((hour)/10+'0');
		LCD_CHAR((hour)%10+'0');
		LCD_CHAR(':');
	}
	LCD_pos(6,1);
	LCD_CHAR((min/10)+'0');
	LCD_CHAR((min%10)+'0');
	LCD_CHAR(':');
	
	LCD_pos(9,1);
	LCD_CHAR((sec/10)+'0');
	LCD_CHAR((sec%10)+'0');
	
	LCD_CHAR(' ');
	LCD_pos(12,1);
	LCD_CHAR(' ');
	LCD_CHAR(' ');
	LCD_CHAR(' ');
	
}
	
//<----------Stopwatch Function----------->
void Stop_Watch(void)
{
		if (stop_watch_bool==1)
		{
			LCD_pos(0,0);
			LCD_STR(OFF);
		}else{
			LCD_pos(0,0);
			LCD_STR(ON);
		}
		
		LCD_pos(0,1);
		LCD_CHAR((s_hour)/10+'0');
		LCD_CHAR((s_hour)%10+'0');
		LCD_CHAR(':');
		
		LCD_pos(3,1);		  //shows min 00:
		LCD_CHAR((s_min/10)+'0');
		LCD_CHAR((s_min%10)+'0');
		LCD_CHAR(':');
		
		LCD_pos(6,1);		  //shows sec 00
		LCD_CHAR((s_sec/10)+'0');
		LCD_CHAR((s_sec%10)+'0');
		
		LCD_CHAR('.');
		LCD_pos(9,1);
		LCD_CHAR(((cnt/10)%10)+'0');
		LCD_CHAR((cnt%10)+'0');
}	
	
	
	
//<----------Alarm Function----------->
void Alarm(void)
{
	LCD_Clear();
	
	while(1)
	{
		if (t_bool==1)
		{
			LCD_Clear();
			break;
		}
		
		else{
			LCD_pos(0,0);
			LCD_STR("SETTING ALARM:");
			
			LCD_pos(0,1);
			LCD_CHAR((t_hour)/10+'0');
			LCD_CHAR((t_hour)%10+'0');
			LCD_CHAR(':');
			
			LCD_pos(3,1);		  //shows min 00:
			LCD_CHAR((t_min/10)+'0');
			LCD_CHAR((t_min%10)+'0');
			LCD_CHAR(':');
			
			LCD_pos(6,1);		  //shows sec 00
			LCD_CHAR((t_sec/10)+'0');
			LCD_CHAR((t_sec%10)+'0');
			
			LCD_pos(10,1);
			LCD_STR(days);}
	}}

	
	
//<----------Initializes all functions----------->
void Init(void)
{
	//Normal Time Initialization
	cnt=0;
	sec=min=0;
	hour=12;			
	bool=0;
	
	//StopWatch Time Initialization
	s_sec=0;
	s_min=0;
	s_hour=0;
	ch=0;
	
	//Initialize Interrupt
	cli();				//Clear Interrupts
	EIMSK = 0x0f;		//ENABLE INT0 for SWITCH
	EICRA = 0xAA;		//INT0-->falling edge
	sei();				//Set Interrupt bit
	
	//Initialize LCD
	PortInit();
	DDRD = 0x00;	//PORTD is input (SWITCH)
	DDRB = 0xff;	//LED output
	PORTB=0xff;
	LCD_Init();		//Initialize LCD by LCD_Comm
	LCD_Clear();	//Clear the Display
	LCD_pos(0,0);	//Move position to 0,0
	LCD_STR(str);	//OUTPUT: TIME FUNCTION
	
	//Initialize Timer
	Init_timer0();}


//<----------Setting Normal Time----------->
void Set_Time()
{	LCD_Clear();
	while(1)
	{
		if (t_bool ==1)
		{	
			LCD_Clear();
			_delay_ms(300);
			break;
		}
		else{
		LCD_pos(0,0);
		LCD_STR("SETTING TIME:");
		
		LCD_pos(0,1);
		LCD_CHAR((t_hour)/10+'0');
		LCD_CHAR((t_hour)%10+'0');
		LCD_CHAR(':');
		
		LCD_pos(3,1);		  //shows min 00:
		LCD_CHAR((t_min/10)+'0');
		LCD_CHAR((t_min%10)+'0');
		LCD_CHAR(':');
		
		LCD_pos(6,1);		  //shows sec 00
		LCD_CHAR((t_sec/10)+'0');
		LCD_CHAR((t_sec%10)+'0');
		
		LCD_pos(10,1);
		LCD_STR(days);
}}}

//<----------Welcoming Page----------->
void WelcomeDisplay()
{	
	LCD_Clear();
	for(char i=0;i<3;i++)
	{
		LCD_pos(3,0);
		LCD_STR("WELCOME TO");
		LCD_pos(2,1);
		LCD_STR("WaduDeV 16-3");
		_delay_ms(500);
		
		LCD_Clear();
		_delay_ms(500);
			
	}
}

int main(void)
{
	Init();					//Initializes all Interrupts, Ports, LCD and variables.
	WelcomeDisplay();		//Welcoming Message
	Set_Time();				//Sets the Timer initially.
	
	while (1)
	{
		switch(bool)		//Based on Switch (PD0) it will switch between modes
		{
			case 0:
				Time();		//Normal Time mode
				break;
			case 1:
				Stop_Watch();//Stop watch mode
				break;
			case 2:
				Alarm();	//Alarm mode
				break;
			default:
				break;
		}
		
	}
}