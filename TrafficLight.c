// TrafficLight.c
// Runs TM4C123
// Index implementation of a Moore finite state machine to operate
// a traffic light.
// Written by: Alex Lopez, Jaehyun Han
// Created: 10/8/2019
// Last modified: 10/15/2019

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
	
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
	
#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))
	
//define states
#define G_Ivy &traffic[0]
#define Y_Ivy &traffic[1]
#define G_1st &traffic[2]
#define Y_1st &traffic[3] 
#define W_Ivy &traffic[4] 
#define W_1st &traffic[5] 
#define Ivy_3 &traffic[6] 

// Linked data structure
struct state{
	int output;
	int time;
	struct state *Next[16];
};
typedef struct state Stype;
Stype traffic[7] = {
										 {0x21, 700, {G_Ivy, G_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, W_1st, W_1st, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy}},
										 {0x22, 200, {G_1st, G_1st, G_1st, G_1st, W_Ivy, W_Ivy, W_Ivy, W_Ivy, W_1st, W_1st, W_1st, W_1st, W_Ivy, W_Ivy, W_Ivy, W_Ivy}},
										 {0x0C, 700, {Y_1st, Y_1st, G_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st, Y_1st}},
										 {0x14, 200, {G_Ivy, G_Ivy, G_Ivy, G_Ivy, W_1st, G_Ivy, G_Ivy, G_Ivy, W_1st, W_1st, W_1st, W_1st, W_Ivy, W_1st, W_1st, W_1st}},
										 {0xE4, 400, {G_Ivy, G_Ivy, G_1st, G_1st, W_Ivy, G_Ivy, G_1st, G_1st, W_1st, W_1st, G_1st, G_1st, W_Ivy, G_Ivy, G_1st, G_1st}},
										 {0xA1, 400, {Ivy_3, Ivy_3, Ivy_3, Ivy_3, Ivy_3, Ivy_3, Ivy_3, Ivy_3, W_1st, Ivy_3, Ivy_3, Ivy_3, Ivy_3, Ivy_3, Ivy_3, Ivy_3}},
										 {0x21, 300, {G_Ivy, G_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, W_1st, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy, Y_Ivy}}
									 };
									 
//Function Prototypes
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);
void PortB_Init(void);
void PortE_Init(void);

int main(void){ 
	unsigned long sensor;
	Stype *ptr;
	ptr = G_Ivy;							//Pointer to G_Ivy (First state)	

	SysTick_Init();							//Initiate SysTick
	PortB_Init();							//Initiate Port B
	PortE_Init();							//Initiate Port E
	
  while(1){
	  GPIO_PORTB_DATA_R = ptr -> output;		//Outputs of state
	  SysTick_Wait10ms(ptr -> time);			//Time delay depends on state
	  sensor = (GPIO_PORTE_DATA_R & 0x0F);		//Read input from Port E
	  ptr = ptr -> Next[sensor];				//Point to next state according to inputs (ex: cars waiting, ppl using crosswalk)
  }
}

void PortB_Init(void){	
	unsigned int delay;
	SYSCTL_RCGC2_R |=0x02;
	delay = 0x11;
	GPIO_PORTB_DIR_R |=0xFF;
	GPIO_PORTB_AFSEL_R &=0x00;
	GPIO_PORTB_AMSEL_R &=0x00;
	GPIO_PORTB_DEN_R |=0xFF;
}

void PortE_Init(void){ 
	unsigned int delay;	
	SYSCTL_RCGC2_R |=0x10;
	delay = 0x10;
	GPIO_PORTE_DIR_R &=0xF0;
	GPIO_PORTE_AFSEL_R &=0xF0;
	GPIO_PORTE_AMSEL_R &=0xF0;
	GPIO_PORTE_DEN_R |=0x0F;
}
