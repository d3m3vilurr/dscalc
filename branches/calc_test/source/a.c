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
	blank,tap,rparen,comma,num,eos,plus,minus,times,divide,mod,pow,abs1,sqrt,fac,
	sin,cos,tan,sec,csc,cot,log,ln,fun_x,lparen
} priority;

typedef struct number{
	int num_flag;	// 0 이면 정수, 1이면 실수 
	union type{
		int i;
		float f;
	}t;
}number;
static number stack[MAX];
//static number *num1, *num2, *result;

// 숫자 스택

// 연산자 스택
int oper[MAX];

// 수식 스택
//char exp[MAX];

static int p_top = -1, topStk = -1, topOp = -1; // 스택 포인터
static int float_flag=0;	// 0이면 정수 1이면 실수 

void postfix(char* str,int x);
void operation1();
void operation2();
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
	case 's': return sin;
	case 'c': return cos;
	case 't': return tan;
	case 'S': return sec;
	case 'C': return csc;
	case 'T': return cot;
	case 'l': return log;
	case 'L': return ln;
	case 'a': return abs1;
	case '\0': return eos;
	case '\t': return tap;
	case ' ' : return blank;
	case '.' : return comma;
	case 'x' : return fun_x;
	default: return num;
	}
}

number sin_(number degree)
{
	//TODO :
	return;
}

number cos_(number degree)
{
	//TODO :
	return;
}

number tan_(number degree)
{
	//TODO :
	return;
}

number sec_(number degree)
{
	//TODO :
	return;
}

number csc_(number degree)
{
	//TODO :
	return;
}

number cot_(number degree)
{
	//TODO :
	return;
}

number log_(number degree)
{
	//TODO :
	return;
}

number ln_(number degree)
{
	//TODO :
	return;
}


number fac_(number num1)
{ 
	int i;
	number res;

	if (num1.num_flag == 1){		// num1이 실수 일때 
		res.num_flag = 0;
		res.t.i = 0;
		printf("아직 실수 fac연산은 미구현\n");		// 실수 fac_()미구현
	}else{							// num1이 정수 일때 
		res.num_flag = 0;
		if(num1.t.i < 0 ){			// num1이 음수 일때 
			res.t.i = 0;
			printf("이 함수의 올바른 입력이 아닙니다(FAC).\n");		
			return res;
		}
		res.t.i = 1;
		for(i = num1.t.i; i > 0 ; i--) 
			res.t.i *= i;
	}
	return res;
}

number sqrt_(number num1)
{
    int op, one, n;
	number res;

	if (num1.num_flag == 1){		// num이 실수 일때 
		res.num_flag = 0;
		res.t.i = 0;
		printf("아직 실수 sqrt연산은 미구현\n");		// 실수 sqrt_()미구현
	}else{
		res.num_flag = 0;

		if(num1.t.i < 0 ){
			res.t.i = 0;
			printf("이 함수의 올바른 입력이 아닙니다.(SQRT)\n");
			return res;
		}

		op = num1.t.i;
		n = 0;
		
		one = 1UL << (32 - 2);
		while (one > op)
			one >>= 2;

		while (one != 0) {
			if (op >= n + one) {
				op = op - (n + one);
				n = n +  2 * one;
			}
			n /= 2;
			one /= 4;
		}
		res.t.i = n;
	}
    return res;
}

number abs_(number num1)
{
	number res;

	if (num1.num_flag == 1){
		res.num_flag = 1;
		if(num1.t.f > 0 ){
			res.t.f = num1.t.f;
		}else{
			//res.t.f = abs(num1.t.f);		// abc함수 호출
		}
	}else{
		res.num_flag = 0;
		if(num1.t.i > 0 ){
			res.t.i = num1.t.i;
		}else{
			//res.t.i = abs(num1.t.i);		// abc함수 호출 
		}
	}

	return res;
}

number pow_(number num1, number num2)
{
    int i, op, n, state;
	number res;

	state = (num1.num_flag * 2 ) + (num2.num_flag);
	if (state > 0 ){								// num1 또는 num2가 이 실수 일때 
		res.num_flag = 0;
		res.t.i = 0;
		printf("아직 실수 pow연산은 미구현\n");		// 실수 pow_()미구현
	}else{
		res.num_flag = 0;
		op = n = num1.t.i;
		for (i = 0; i < num2.t.i - 1; i++) {
			n = n * op;
		}
		res.t.i = n;
	}
    return res;
}

number add_(number num1,number num2) 
{	
	int state;
	number res;

	state = (num1.num_flag * 2 ) + (num2.num_flag);
	if (state >0 )
		res.num_flag = 1;
	else
		res.num_flag = 0;

	switch (state){
		case 0:
			res.t.i = num1.t.i + num2.t.i; 
			break;
		case 1:
			res.t.f = (float)num1.t.i + num2.t.f; 
			break;
		case 2:
			res.t.f = num1.t.f + (float)num2.t.i; 
			break;
		case 3: 
			res.t.f = num1.t.f + num2.t.f; 
			break;
		default:
			break;
	}			
	return res; 
}

number sub_(number num1,number num2) 
{	
	int state;
	number res;
	
	state = (num1.num_flag * 2 ) + (num2.num_flag);
	if (state >0 )
		res.num_flag = 1;
	else
		res.num_flag = 0;
	
	switch (state){
		case 0:
			res.t.i = num1.t.i - num2.t.i; 
			break;
		case 1:
			res.t.f = (float)num1.t.i - num2.t.f; 
			break;
		case 2:
			res.t.f = num1.t.f - (float)num2.t.i; 
			break;
		case 3: 
			res.t.f = num1.t.f - num2.t.f; 
			break;
		default:
			break;
	}			
	return res; 
}

number mul_(number num1,number num2) 
{	
	int state;
	number res;
	
	state = (num1.num_flag * 2 ) + (num2.num_flag);
	if (state >0 )
		res.num_flag = 1;
	else
		res.num_flag = 0;
	
	switch (state){
		case 0:
			res.t.i = num1.t.i * num2.t.i; 
			break;
		case 1:
			res.t.f = (float)num1.t.i * num2.t.f; 
			break;
		case 2:
			res.t.f = num1.t.f * (float)num2.t.i; 
			break;
		case 3: 
			res.t.f = num1.t.f * num2.t.f; 
			break;
		default:
			break;
	}			
	return res; 
}

number divi_(number num1,number num2) 
{	
	int state;
	number res;
	
	state = (num1.num_flag * 2 ) + (num2.num_flag);
	if (state >0 )
		res.num_flag = 1;
	else
		res.num_flag = 0;
	
	if (num2.t.f == 0 || num2.t.i == 0){
		res.num_flag = 0;
		res.t.i = 0;
		printf("0으로 나눌수 없습니다\n");	// 에러 처리가 필요함 
		return res;
	}else{
		switch (state){
		case 0:
			res.t.i = num1.t.i / num2.t.i; 
			break;
		case 1:
			res.t.f = (float)num1.t.i / num2.t.f; 
			break;
		case 2:
			res.t.f = num1.t.f / (float)num2.t.i; 
			break;
		case 3: 
			res.t.f = num1.t.f / num2.t.f; 
			break;
		default:
			break;
		}			
		return res;
	}
}


number modul_(number num1,number num2) 
{	
	int state;
	number res, n;
	
	state = (num1.num_flag * 2 ) + (num2.num_flag);
	if (state >0 )
		res.num_flag = 1;
	else
		res.num_flag = 0;


	switch (state){
		case 0:
			res.t.i = num1.t.i % num2.t.i;
			break;
		case 1:
			n = divi_(num1,num2);
			n.t.f = (int)n.t.f * num2.t.f;
			n = abs_(n);
			res.t.f = num1.t.i - n.t.f;
			break;
		case 2:
			n = divi_(num1,num2);
			n.t.f = (int)n.t.f * num2.t.i;
			n = abs_(n);
			res.t.f = num1.t.f - n.t.f;
			break;
		case 3: 
			n = divi_(num1,num2);
			n.t.f = (int)n.t.f * num2.t.f;
			n = abs_(n);
			res.t.f = num1.t.f - n.t.f;
			break;
		default:
			break;
	}			
	return res; 
}


void operation1()	// 스택에서 pop한 후 연산하고 결과를 push (피연산자가 1개인 연산) 
{
	priority token;
	number res,num1;

	token = pop_op(&topOp);
	num1 = stack[topStk--];
	
	switch(token) {
		case sqrt:
			res = sqrt_(num1); break;
		case fac:	
			res = fac_(num1); break;
		case abs1:
			res = abs_(num1); break;
		default : 
			break;
	}
	push(&topStk,res);
}


void operation2()	// 스택에서 pop한 후 연산하고 결과를 push (피연산자가 2개인 연산) 
{
	priority token;
	number res,num1,num2;

	token = pop_op(&topOp);
	num2 = stack[topStk--];
	num1 = stack[topStk--];
	
	switch(token) {
		case plus:
			res = add_(num1,num2); break;
		case minus:
			res = sub_(num1,num2); break;
		case times:
			res = mul_(num1,num2); break;	
		case divide:
			res = divi_(num1,num2); break;			
		case pow:		
			res = pow_(num1,num2); break;
		case mod:
			res = modul_(num1,num2); break;
		default : 
			break;
	}
	push(&topStk,res);
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
	number *result;
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
	int flag = 0;	//  스택에 존재하는 '('의 갯수  
	
	number fx;	// f(x)에서 x의 값을 받을 변수 (임시로 했음)
	fx.t.i= 5;	
	fx.num_flag = 0;

	while(token != eos){
		token = get_token(&str[i]);
		switch(token){
		case blank :	// ' '
			break;
		case tap :		// '\t'
			break;
		case sin :
			//TODO:	
			break;
		case cos :
			//TODO
			break;
		case tan :
			//TODO
			break;
		case sec :
			//TODO
			break;
		case csc :
			//TODO
			break;
		case cot :
			//TODO
			break;
		case log :
			//TODO
			break;
		case ln :
			//TODO
			break;
		case fun_x :	
			if (i != 0 && get_token(&str[i-1]) == num){
				num_to_stack(temp,&j);
				push_op(&topOp,times);
				push(&topStk,fx);
				operation2();
			}else{
				temp[j++] = '5';	// 임시로 x값 5를 받음... 
				num_to_stack(temp,&j);
			}
			break;
		case comma :			// 소숫점 '.'
			temp[j++] = str[i]; 
			float_flag = 1;		
			break;
		case num :				// 숫자 
			temp[j++] = str[i]; break;
		case eos :				// 입력의 끝 '\0'
			num_to_stack(temp,&j);
			while(topOp != -1 ){
				if ((oper[topOp]) != fac && (oper[topOp] != sqrt)){
					operation2();
				}else{
					operation1();
				}
			}
			break;
		case lparen :			// '('
			flag++;
			push_op(&topOp,get_token(&str[i])); 
			break;
		case rparen :			// ')'
			num_to_stack(temp,&j);
			while(oper[topOp] != lparen){  
				if(oper[topOp]==sqrt || oper[topOp]==fac ||oper[topOp]==sqrt||oper[topOp]==abs1){
					operation1();
				}else{
					operation2();
				}
			}
			pop_op(&topOp);		// '(' 제거 
			flag--; 
			break;
		default :				// 연산자인 경우
			if( get_token(&str[i]) == minus ){ 				
				if( get_token(&str[i-1]) == minus || get_token(&str[i-1]) == lparen || i==0 || get_token(&str[i-1]) == mod){
					temp[j++] = str[i];
					i++; 
					continue;
				}
			}
			num_to_stack(temp,&j);
			if(topOp == -1){	// 스택에 연산자가 아무것도 없는 경우 
				push_op(&topOp,get_token(&str[i])); 
			}else if ( oper[topOp] == lparen ){	// 스택에 가장 최근에 들어온 연산자가 '('경우  
				push_op(&topOp,get_token(&str[i])); 
			}else if(topOp > -1 ){		 
				if(oper[topOp] >= get_token(&str[i])){
					if ((oper[topOp]) != fac && (oper[topOp] != sqrt)){
						operation2(); i--;
					}else{
						operation1();
					}
				}else{
					push_op(&topOp,get_token(&str[i]));
				}
			}
			break;
		}
		i++;
	}
	float_flag = 0;
}

int main(void)
{
	char str[MAX];
	number result;
	fflush(stdin);
	printf("input  : ");
	gets(str);
	
	//plot(str,1,5);	// f(x) 함수에서 [1,5]범위의 그래프 그리기 
	postfix(str,0);
	printf("\n\npostfix end\n\n");
	
	result = stack[topStk];
	if(result.num_flag == 0)
		printf("result is %d \n",result.t.i);
	else 
		printf("result is %f \n",result.t.f);
	return 0;
}