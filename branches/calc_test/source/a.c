#include <stdio.h>
#include <stdlib.h>
/*
case SIN : push_eval( sin( RD(value_frist)) ); 
break; 
case COS : push_eval( cos( RD(value_frist)) ); 
break; 
case TAN : push_eval( tan( RD(value_frist)) ); 
*/
#define MAX 100

typedef enum {
	blank,num, eos, plus, minus, times, divide, mod, pow, sqrt, fac, lparen, rparen, comma, fun_x
} priority;

typedef struct number{
	int num_flag;	// 0 이면 정수, 1이면 실수 
	union type{
		int i;
		float f;
	}t;
}number;
static number stack[MAX];
static number *num1, *num2, *result;
// 숫자 스택

// 연산자 스택
int oper[MAX];

// 수식 스택
//char exp[MAX];

static int p_top = -1, topStk = -1, topOp = -1; // 스택 포인터
static int float_flag=0;	// 0이면 정수 1이면 실수 

void postfix(char* str,int x);
void operation(int flag);
priority get_token(char* symbol);

// 스택
void push(int *top, number item)
{
	if(*top >= MAX-1) {
		printf("Stack is full!!\n");
		return;
	}
	
	if(float_flag == 0){
		stack[++(*top)].t.i = item.t.i;
		stack[(*top)].num_flag = item.num_flag;
	}else{
		stack[++(*top)].t.f = item.t.f;
		stack[(*top)].num_flag = item.num_flag;
	}
}

number pop(int *top)
{
	if( (*top) == -1) {
		printf("Stack is empty\n");
		exit(0);
	}

	return stack[(*top)--];
}

void push_op(int *top, priority item)
{
	if(*top >= MAX-1) {
		printf("Operation Stack is full!!\n");
		return;
	}

	oper[++(*top)] = item;
}

priority pop_op(int *top)
{
	if( (*top) == -1) {
		printf("Operation Stack is empty\n");
		return 0;
	}

	return oper[(*top)--];
}

priority get_token(char* symbol)
{
	switch( *symbol) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '*': return times;
	case '/': return divide;
	case '%': return mod;
	case '^': return pow;
	case 'R': return sqrt;
	case '!': return fac;
	case '\0': return eos;
	case ' ' : return blank;
	case '.' : return comma;
	case 'x' : return fun_x;
	default: return num;
	}
}

/*-----------------------------------------------------
	실수 연산 
-----------------------------------------------------*/
float add_f(float num1,float num2) { return num1+num2; }
float sub_f(float num1,float num2) { return num1-num2; }
float mul_f(float num1,float num2) { return num1*num2; }
//float modul_f(float num1,float num2) { return num1%num2; }
float divi_f(float num1,float num2)
{ 
	if(num2 !=0)
		return num1/num2;
	else 
		return 0;
}



/*-----------------------------------------------------
	정수 연산 
-----------------------------------------------------*/
int fac_i(int num1)
{ 
	int i, res = 1;

	for(i = num1; i > 0 ; i--) 
		res *= i;
	return res;
}

int sqrt_i(int x)
{
    int op, res, one;

    op = x;
    res = 0;

    one = 1UL << (32 - 2);
    while (one > op)
        one >>= 2;

    while (one != 0) {
        if (op >= res + one) {
            op = op - (res + one);
            res = res +  2 * one;
        }
        res /= 2;
        one /= 4;
    }
    return res;
}

 
int pow_i(int x, int t)
{
    int i;
    int  op, res;

    op = res = x;
    
    for (i = 0; i < t - 1; i++) {
        res = res * op;
    }

    return res;
}

int add_i(int num1,int num2) { return num1+num2; }
int sub_i(int num1,int num2) { return num1-num2; }
int mul_i(int num1,int num2) { return num1*num2; }
int modul_i(int num1,int num2) { return num1%num2; }
int divi_i(int num1,int num2)
{ 
	if(num2 !=0)
		return num1/num2;
	else 
		return 0;
}



void operation2(int flag)	// 스택에서 pop한 후 연산하고 결과를 push (피연산자가 1개인 연산) 
{
	priority token;
	int state;
	number n;

	if(flag == 1)
		p_top--;

	token = pop_op(&topOp);
	num1 = &stack[topStk--];
	
	state = num1->num_flag; 
	if (state > 0 )
		n.num_flag = 1;
	else
		n.num_flag = 0;

	switch(token) {
		case sqrt:
			switch (state){
				case 0:
					n.t.i = sqrt_i(num1->t.i); break;
				//case 1:
				//	n.t.f = sqrt_f(num1->t.f); break;
			}
			break;
		case fac:
			switch (state){
				case 0:
					n.t.i = fac_i(num1->t.i); break;
			}
			break;
		default : 
			break;
	}
	push(&topStk,n);
}


void operation(int flag)	// 스택에서 pop한 후 연산하고 결과를 push (피연산자가 2개인 연산) 
{
	priority token;
	int state;
	number n;

	if(flag == 1)
		p_top--;

	token = pop_op(&topOp);
	num2 = &stack[topStk--];
	num1 = &stack[topStk--];
	
	state = (num1->num_flag * 2 ) + (num2->num_flag);
	if (state >0 )
		n.num_flag = 1;
	else
		n.num_flag = 0;

	switch(token) {
		case plus:
			switch (state){
				case 0:
					n.t.i = add_i(num1->t.i,num2->t.i); break;
				case 1:
					n.t.f = add_f((float)num1->t.i,num2->t.f); break;
				case 2:
					n.t.f = add_f(num1->t.f,(float)num2->t.i); break;
				case 3: 
					n.t.f = add_f(num1->t.f,num2->t.f); break;
			}
			break;
		case minus:
			switch (state){
				case 0:
					n.t.i = sub_i(num1->t.i,num2->t.i); break;
				case 1:
					n.t.f = sub_f((float)num1->t.i,num2->t.f); break;
				case 2:
					n.t.f = sub_f(num1->t.f,(float)num2->t.i); break;
				case 3: 
					n.t.f = sub_f(num1->t.f,num2->t.f); break;
			}
			break;
		case times:
			switch (state){
				case 0:
					n.t.i = mul_i(num1->t.i,num2->t.i); break;
				case 1:
					n.t.f = mul_f((float)num1->t.i,num2->t.f); break;
				case 2:
					n.t.f = mul_f(num1->t.f,(float)num2->t.i); break;
				case 3: 
					n.t.f = mul_f(num1->t.f,num2->t.f); break;
			}
			break;
		case divide:
			switch (state){
				case 0:
					n.t.i = divi_i(num1->t.i,num2->t.i); break;
				case 1:
					n.t.f = divi_f((float)num1->t.i,num2->t.f); break;
				case 2:
					n.t.f = divi_f(num1->t.f,(float)num2->t.i); break;
				case 3: 
					n.t.f = divi_f(num1->t.f,num2->t.f); break;
				}
			break;
		case pow:
			switch (state){
				case 0:
					n.t.i = pow_i(num1->t.i,num2->t.i); break;
				//case 1:
				//	n.t.f = pow_f((float)num1->t.i,num2->t.f); break;
				//case 2:
				//	n.t.f = pow_f(num1->t.f,(float)num2->t.i); break;
				//case 3: 
				//	n.t.f = pow_f(num1->t.f,num2->t.f); break;
			}
			break;
		case mod:
			switch (state){
				case 0:
					n.t.i = modul_i(num1->t.i,num2->t.i); 
					break;
				default : 
					//n.t.f = divi_f(num1->t.f,num2->t.f); 
					break;
				}
			break;	
		default : 
			break;
	}
	push(&topStk,n);
}

void num_to_stack(char* temp,int *j)	// temp의 숫자 스트링을 스택에 푸쉬
{
		number n;

		temp[*j]='\0';
		if(temp[0]!='\0'){
			switch(float_flag){
				case 0:
					n.num_flag = 0;
					n.t.i = atoi(temp);
					break;
				case 1:
					n.num_flag = 1;
					n.t.f =(float)atof(temp);
					break;
				default:
					break;
			}
			push(&topStk,n);
			temp[0]='\0'; *j=0; float_flag=0;
		}
}

void plot(char* str,int start, int end)		// f(x) 함수에서 [start,end]범위의 그래프 그리기 
{
	int i; 
	printf("%d %d \n",start,end);
	for(i=start; i <= end ; i++){
		postfix(str,i);
		result = &stack[topStk];
		if(result->num_flag == 0)
			printf("i=%d,  result=%d\n",i,result->t.i);
		else 
			printf("i=%d,  result=%f\n",i,result->t.f);

		// TODO F(x)의 (x,y)좌표에 따라 그래프 출력 
	}
}

void postfix(char* str,int x)
{
	char temp[MAX]="\0"; // 임시 저장 공간
	int i = 0, j = 0; // string 포인터
	int token;	// 현재 토큰의 종류 
	int flag = 0;	//  '('이 스택에 있는지 여부 
	
	number fx;	// f(x)에서 x의 값을 받을 변수 (임시로 했음)
	fx.t.i= 5;	
	fx.num_flag = 0;

	while(token != eos){
		token = get_token(&str[i]);
		switch(token){
		case blank :
			break;
		case fun_x :	
			if (i != 0 && get_token(&str[i-1]) == num){
				num_to_stack(temp,&j);
				push_op(&topOp,times);
				push(&topStk,fx);
			}else{
				temp[j++] = '5'; 
				num_to_stack(temp,&j);
			}
			break;
		case comma :	// 소숫점 '.'
			temp[j++] = str[i]; 
			float_flag = 1;		// 소수점 연산 
			break;
		case sqrt:
			num_to_stack(temp,&j);
			push_op(&topOp,sqrt);
			operation2(flag);
			break;
		case fac :
			num_to_stack(temp,&j);
			push_op(&topOp,fac);
			operation2(flag);
			break;
		case num :
			temp[j++] = str[i]; break;
		case eos :
			num_to_stack(temp,&j);
			while(topOp != -1 ){	
				operation(flag);
			}
			break;
		case lparen :
			flag = 1;
			push_op(&topOp,get_token(&str[i])); 
			p_top++;
			break;
		case rparen :
			num_to_stack(temp,&j);
			while(oper[topOp] != lparen){  
				operation(flag);
			}
			pop_op(&topOp); // '(' 제거 
			flag = 0; p_top=-1;
			break;
		default :	// 연산자인 경우
			if( get_token(&str[i-1]) == minus || get_token(&str[i-1]) == lparen || i == 0){
				temp[j++] = str[i];
				i++; 
				continue;
			}
			num_to_stack(temp,&j);
			if(topOp == -1){	// 스택에 연산자가 아무것도 없는 경우 
				push_op(&topOp,get_token(&str[i])); 
			}else if ( p_top==0 ){	// 스택에 '(' 있고, 그 후로 연산자가 들어오지 않은 경우 
				push_op(&topOp,get_token(&str[i])); 
				if(flag == 1) p_top++;
			}else if(p_top > 0 ){	// 스택에 '(' 존재하고, 그 후에 연산자가 1개 이상 있는 경우  
				if(oper[topOp] >= get_token(&str[i])){
					operation(flag); i--;
				}else{
					push_op(&topOp,get_token(&str[i]));
					if(flag == 1) p_top++;
				}
			}else if(topOp > -1 ){		// 스택에 '(' 없고 다른연산자가 1개 이상 존재하는 경우  
				if(oper[topOp] >= get_token(&str[i])){
					operation(flag); i--;
				}else{
					push_op(&topOp,get_token(&str[i]));
					if(flag == 1) p_top++;
				}
			}
		
		}
		i++;
	}
	float_flag = 0;
}

int main(void)
{
	char str[MAX];
	
	fflush(stdin);
	printf("input  : ");
	gets(str);
	
	//plot(str,1,5);	// f(x) 함수에서 [1,5]범위의 그래프 그리기 
	postfix(str,0);
	printf("\n\npostfix end\n\n");
	
	result = &stack[topStk];
	if(result->num_flag == 0)
		printf("result is %d \n",result->t.i);
	else 
		printf("result is %f \n",result->t.f);
	return 0;
}