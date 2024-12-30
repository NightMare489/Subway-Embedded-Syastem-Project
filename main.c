/*
 * embedded.c
 *
 * Created: 11/4/2024 4:43:35 PM
 * Author : yaser
 */ 


#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>


#define LCD_Dir DDRB          /* Define LCD data port direction */
#define LCD_Port PORTB        /* Define LCD data port */
#define RS PA0                /* Define Register Select pin */
#define EN PA1                /* Define Enable signal pin */

#define LCD_Control_Dir DDRA  /* Define LCD control port direction */
#define LCD_Control_Port PORTA /* Define LCD control port */


void LCD_Command(unsigned char cmnd)
{
	LCD_Port = cmnd;                     /* Send full 8-bit command */
	LCD_Control_Port &= ~(1 << RS);      /* RS=0, command register */
	LCD_Control_Port |= (1 << EN);       /* Enable pulse */
	_delay_us(3);
	LCD_Control_Port &= ~(1 << EN);
	_delay_ms(3);
}

void LCD_Char(unsigned char data)
{
	LCD_Port = data;                     /* Send full 8-bit data */
	LCD_Control_Port |= (1 << RS);       /* RS=1, data register */
	LCD_Control_Port |= (1 << EN);       /* Enable pulse */
	_delay_us(3);
	LCD_Control_Port &= ~(1 << EN);
	_delay_ms(3);
}

void LCD_Init(void)
{
	LCD_Dir = 0xFF;                      /* Set PORTB as output for data */
	LCD_Control_Dir |= (1 << RS) | (1 << EN); /* Set PA0 and PA1 as output */
	_delay_ms(20);                       /* LCD Power ON delay always >15ms */

	LCD_Command(0x38);                   /* Initialize in 8-bit mode, 2 line, 5x7 matrix */
	LCD_Command(0x0C);                   /* Display ON, Cursor OFF */
	LCD_Command(0x01);                   /* Clear display */
	_delay_ms(2);
}


void LCD_String (char *str)		/* Send string to LCD function */
{
int i;
for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
{
LCD_Char (str[i]);
}
}



void LCD_Char_xy (char row, char pos, char str)	/* Send string to LCD with xy position */
{
	if ((row == 0) && (pos<16) && (pos>=0))
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if ((row == 1) && (pos<16) && (pos>=0))
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	else
		return;
	LCD_Char(str);		/* Call LCD char function */
}

void LCD_Clear()
{
LCD_Command (0x01);		/* Clear display */
_delay_ms(2);
LCD_Command (0x80);		/* Cursor at home position */
}

const int Bird_pos = 4;
int Bird_Row = 0;
unsigned int seed = 1234; // Change this seed for different sequences

int obs_x[4];
int obs_y[4];
int num_obs =0;




// Function to generate pseudo-random numbers
unsigned int pseudo_random() {
	seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return seed  & 1; // Return a random bit (0 or 1)
}

char lcd_Buffer[2][16]; // Buffer for 2 rows of 16 characters
char rewrite_Buffer[2][16];

int restart=0;
int endgame=0;


int score =0;


int main()
{
	DDRA |= (1<<2);
	TCCR1B |= (1<<CS11) | (1<<CS10);
	TCCR0|= (1<<CS02) | (1<<CS00);
	
	sei();
	TIMSK |= (1<<TOIE1);
	TCNT1 = 53816;
	
	
	
	TIMSK |= (1<< TOIE0);
	TCNT0 = 60;
	
	
	MCUCR |= (1<<ISC00) | (1<<ISC01);
	GICR |= (1<<INT0);
	
	
LCD_Init();

	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 16; col++) {
			lcd_Buffer[row][col] = ' ';
			rewrite_Buffer[row][col] = 1;
		}
	}
	
lcd_Buffer[Bird_Row][Bird_pos] = 'A';



while(1){

	if(endgame){
		break;
	}

	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 16; col++) {
			if(rewrite_Buffer[row][col]){			
			LCD_Char_xy(row, col, lcd_Buffer[row][col]);
			rewrite_Buffer[row][col] =0;
			}
		}
	}


		
	if(endgame){
		break;
	}
		
	
	_delay_ms(15);
	
}

LCD_Clear();
char buff[15];
sprintf(buff,"You Lost: %d",score);
LCD_String(buff);

while(1);

}

ISR(TIMER1_OVF_vect){
	
		
		int rand_y = pseudo_random();
		if(num_obs<4){
			obs_x[num_obs] = 14;
			obs_y[num_obs] = rand_y;
			num_obs++;
			}else{
			for(int i=0;i<num_obs;i++){
				if(obs_x[i]<=0){
					lcd_Buffer[obs_y[i]][obs_x[i]] = ' ';
					rewrite_Buffer[obs_y[i]][obs_x[i]] = 1;
					obs_x[i] = 14;
					
				}
			}
			
		}
	
	
	TCNT1 = 53816;
	
}

ISR(TIMER0_OVF_vect){


	
	for(int i=0;i<num_obs;i++){
		if(obs_x[i]>0){
			
			lcd_Buffer[obs_y[i]][obs_x[i]-1] = 'B';
			rewrite_Buffer[obs_y[i]][obs_x[i]-1] = 1;
			
			lcd_Buffer[obs_y[i]][obs_x[i]] = ' ';
			rewrite_Buffer[obs_y[i]][obs_x[i]] = 1;
			
			obs_x[i]--;
			if((obs_x[i] == Bird_pos) && (obs_y[i] == Bird_Row)){
				endgame = 1;
				break;
			}
			
			if(obs_x[i] == Bird_pos){
				score++;
			}
			
		}
		
	}
	


	TCNT0 = 60;
}


ISR(INT0_vect){
	
	if(!endgame){
		
    lcd_Buffer[Bird_Row][Bird_pos] = ' '; // Clear old position in the buffer
	rewrite_Buffer[Bird_Row][Bird_pos] =1;
    Bird_Row = !Bird_Row;               // Toggle row
	
		if(lcd_Buffer[Bird_Row][Bird_pos]=='B'){
			endgame=1;
			score--;
			}else{
	
		lcd_Buffer[Bird_Row][Bird_pos] = 'A'; // Update new position in the buffer
		rewrite_Buffer[Bird_Row][Bird_pos] =1;
		}
	
	
	}
	
	
}
