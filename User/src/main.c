#include "main.h"
#include "queue.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#define STR_MAX_LENGTH	100
#define TC_BIT			0x0040
#define TC_SHIFT_LEFT	6
#define RXNE_BIT		0x0020
#define RXNE_SHIFT_LEFT	5

void uart_interrupt_my_init(void);
void uart_my_send(void);
void uart_my_receive(void);
void from_receive_to_send(queue_t * send, queue_t * receive);
void queue_push_string(queue_t * Q, const char * string, const uint8_t length);
int get_data(queue_t q);
void input_operand(int *a, int *b);
void op2_print_result(char *result);
void plus(void);
void subtract(void);
void multiply(void);
void divide(void);
void module(void);
void home_screen_option(void);
void home_screen_option2(void);
void option1(void);
void option2(void);

extern volatile uint8_t b_receive_done;
// extern variable declare for using option
extern volatile uint8_t msgSend;

extern queue_t queue_sender;
extern queue_t queue_receiver;

char* choose = "Choose your option (1, 2, ..): \n";
char* op1 = 		"1. Student info\n";
char* op2 = 	  "2. Basic operation( a,b,..):\n";
char* option3 = "3. Simple led\n";
char* option4 = "4. Advance led\n";
char* option5 = "5. Audio\n";
char* useroption = "Your option: ";
char* escape = "ESC: return previous menu\n";
char* newline = "\n";


int main(){
	
	queue_init(&queue_sender);
	queue_init(&queue_receiver);
	
	// Uart interrupt init
	uart_interrupt_my_init();

	for(;;){
		// Send option & wait for user input
		home_screen_option();
		from_receive_to_send(&queue_sender, &queue_receiver);
		uart_my_send();
		queue_push_string(&queue_sender, newline, strlen(newline));
		uart_my_send();
		
		// Check input option
		// TODO: add other functions
		// TODO: check if user input orther keys
		switch (msgSend)
		{
			case '1':
				option1();
				break;
			case '2':
				option2();
				break;
			default:
				home_screen_option();
		}
	}
	return 0;
}

void option1()
{
	char* id = "ID: 14520555\n";
	char* name = "Full name: Nguyen Thanh Nam\n";
	
	queue_push_string(&queue_sender, newline, strlen(newline));
	queue_push_string(&queue_sender, op1, strlen(op1));
	queue_push_string(&queue_sender, id, strlen(id));
	queue_push_string(&queue_sender, name, strlen(name));
	queue_push_string(&queue_sender, escape, strlen(escape));
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();
	uart_my_receive();
}

void option2()
{
	queue_t queue_get_data;
	int op_operator;
	
	// Home screen option 2
	home_screen_option2();
	uart_my_receive();	
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);			
	uart_my_send();					
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();
	
	op_operator = queue_get_data.items[0];
	
	//TODO: ESC doesn't show the previous menu
	switch (op_operator)
	{
		case 'a':
			plus();
			break;
		case 'b':
			subtract();
			break;
		case 'c':
			multiply();
			break;
		case 'd':
			divide();
			break;
		case 'e':
			module();
			break;
		default:
			home_screen_option2();
	}	
}

void plus()
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char sum_convert[100];	
	int sum;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	sum = operand1 + operand2;
	
	// Convert back to string
	sprintf(sum_convert,"%d",sum);
	result = sum_convert;
	
	// Print result
	op2_print_result(result);
}

void subtract()
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char sub_convert[100];	
	int sub;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	sub = operand1 - operand2;
	
	// Convert back to string
	sprintf(sub_convert,"%d",sub);
	result = sub_convert;
	
	// Print result
	op2_print_result(result);
}

void multiply(void)
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char mul_convert[100];	
	int mul;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	mul = operand1 * operand2;
	
	// Convert back to string
	sprintf(mul_convert,"%d",mul);
	result = mul_convert;
	
	// Print result
	op2_print_result(result);	
}


void divide(void)
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char div_convert[100];	
	float div;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	div = (float)(operand1) / (float)(operand2);
	
	// Convert back to string
	sprintf(div_convert,"%0.2f",div);
	result = div_convert;
	
	// Print result
	op2_print_result(result);	
}

void module(void)
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char mol_convert[100];	
	float mol;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	mol = sqrt((float)(operand1 * operand1) + (float)(operand2 * operand2));
	
	// Convert back to string
	sprintf(mol_convert,"%0.2f",mol);
	result = mol_convert;
	
	// Print result
	op2_print_result(result);	
}

void op2_print_result(char *result)
{
	char* txtresult = "Result: ";	
	
	queue_push_string(&queue_sender, txtresult, strlen(txtresult));
	queue_push_string(&queue_sender, result, strlen(result));
	queue_push_string(&queue_sender, newline, strlen(newline));
	
	queue_push_string(&queue_sender, escape, strlen(escape));
	
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();
	uart_my_receive();
}

void input_operand(int *a, int *b)
{	
	int operand1, operand2;
	char* num1 = "Operand 1: ";
	char* num2 = "Operand 2: ";
	
	// queue for get data input
	queue_t queue_get_data;
	
	/* Process for operand 1	*/
	queue_push_string(&queue_sender, newline, strlen(newline));
	queue_push_string(&queue_sender, num1, strlen(num1));
	uart_my_send();
	uart_my_receive();
	
	// Get data input
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);			
	uart_my_send();					
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();
	
	// Convert to int
	operand1 = get_data(queue_get_data);
	*a = operand1;
	
	
	// Process for operand 2
	queue_push_string(&queue_sender, num2, strlen(num2));
	uart_my_send();
	uart_my_receive();
	
	// Get data input	
	queue_get_data = queue_receiver;
	
	// Print to terminal	
	from_receive_to_send(&queue_sender, &queue_receiver);
	uart_my_send();
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();
	
	// Convert to int
	operand2 = get_data(queue_get_data);
	*b = operand2;
}

void home_screen_option()
{
	queue_push_string(&queue_sender, choose, strlen(choose));
	queue_push_string(&queue_sender, op1, strlen(op1));
	queue_push_string(&queue_sender, op2, strlen(op2));
	queue_push_string(&queue_sender, option3, strlen(option3));
	queue_push_string(&queue_sender, option4, strlen(option4));
	queue_push_string(&queue_sender, option5, strlen(option5));
	queue_push_string(&queue_sender, escape, strlen(escape));
	queue_push_string(&queue_sender, useroption, strlen(useroption));
	uart_my_send();
	uart_my_receive();
}

void home_screen_option2()
{
	queue_push_string(&queue_sender, newline, strlen(newline));
	queue_push_string(&queue_sender, op2, strlen(op2));
	queue_push_string(&queue_sender, "a. Plus\n", strlen("a. Plus\n"));
	queue_push_string(&queue_sender, "b. Subtract\n", strlen("b. Subtract\n"));
	queue_push_string(&queue_sender, "c. Multiply\n", strlen("c. Multiply\n"));
	queue_push_string(&queue_sender, "d. Divide\n", strlen("d. Divide\n"));
	queue_push_string(&queue_sender, "e. Module\n", strlen("e. Module\n"));
	queue_push_string(&queue_sender, useroption, strlen(useroption));
	uart_my_send();
}

void uart_interrupt_my_init()
{
	GPIO_InitTypeDef gpio_init;
	USART_InitTypeDef usart_init;
	NVIC_InitTypeDef nvic_init;

	GPIO_StructInit(&gpio_init);
	USART_StructInit(&usart_init);
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpio_init);

	usart_init.USART_BaudRate = 9600;
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart_init.USART_Parity = USART_Parity_No;
	usart_init.USART_StopBits = USART_StopBits_1;
	usart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &usart_init);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	nvic_init.NVIC_IRQChannel = USART3_IRQn;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&nvic_init);

	USART_Cmd(USART3, ENABLE);
}

void queue_push_string(queue_t * Q, const char * string, const uint8_t length)
{
	int i;
	for(i = 0; i < length; i++)
	{
		queue_push(Q, string[i]);
	}
}

void uart_my_send()
{
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void uart_my_receive()
{
	b_receive_done = 0;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	while(b_receive_done == 0);
}

void from_receive_to_send(queue_t * send, queue_t * receive)
{
	uint8_t item;
	uint8_t b_success = 0;
	while(queue_is_empty(receive) == 0)
	{
		item = queue_pop(receive, &b_success);
		queue_push(send, item);
	}
}

int get_data(queue_t q)
{
	char temp[100];
	int i = 0;
	
	while (queue_is_empty(&q) == 0)
	{
		temp[i] = (char)(q.items[i]);
		q.capacity--;
		q.items[i] = q.items[i+1];
		i++;
	}
	
	return atoi(temp);
}
