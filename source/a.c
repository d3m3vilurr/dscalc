#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
//#include "common.h"

#define MAX 100
#define PI 3.1415926535897931
#define E 2.718281828

typedef enum {
	blank,tap,rparen,comma,num,eos,plus,minus,times,divide,mod,_pow,e,pi,fun_x,
		_abs,_sqrt,_fac,_sin,_cos,_tan,_sec,_csc,_cot,_log,_ln,_rand,rad,deg,lparen
} priority;

typedef struct number{
	int num_flag;	// 0 �̸� ����, 1�̸� �Ǽ� 
	union type{
		int i;
		float f;
	}t;
}number;

static number stack[MAX];		// ���� ����
int oper[MAX];					// ������ ����

static int topStk = -1, topOp = -1; // ���� ������
static int EVAL_ERR = 0;			// 1�̸� ���� ����   

void eval(char* str,number n);
void operation1();
void operation2();
priority get_token(char* symbol);

// ����
void push(int *top, number item)
{
	if(*top >= MAX-1) {
		printf("Stack is full!!\n");
		return;
	}
	
	if(item.num_flag == 0){
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
		case 'p': return pi;
		case 'e': return e;
		case '(': return lparen;
		case ')': return rparen;
		case '+': return plus;
		case '-': return minus;
		case '*': return times;
		case '/': return divide;
		case '%': return mod;
		case '^': return _pow;
		case 'R': return _sqrt;
		case '!': return _fac;
		case 's': return _sin;
		case 'c': return _cos;
		case 't': return _tan;
		case 'S': return _sec;
		case 'C': return _csc;
		case 'T': return _cot;
		case 'l': return _log;
		case 'L': return _ln;
		case 'a': return _abs;
		case 'm': return _rand;
		case 'r': return rad;
		case 'd': return deg;
		case '\0': return eos;
		case '\t': return tap;
		case ' ' : return blank;
		case '.' : return comma;
		case 'x' : return fun_x;
		default : return num;
	}
}

number rad2deg_(number num1) 
{
	number n;

	if(num1.num_flag == 0){
		n.t.f = num1.t.i * 180. / PI;
	}else{
		n.t.f = num1.t.f * 180. / PI;
	}
	n.num_flag = 1;
    return n;
}

number deg2rad_(number num1) 
{
	number n;

	if(num1.num_flag == 0){
		n.t.f = num1.t.i * PI / 180.;
	}else{
		n.t.f = num1.t.f * PI / 180.;
	}
	n.num_flag = 1;
    return n;
}


number sin_(number degree)
{
	number n, res;
	
	n = deg2rad_(degree);
	res.num_flag = 1;
	res.t.f = sin(n.t.f);
	return res;
}

number cos_(number degree)
{
	number n, res;

	n = deg2rad_(degree);
	res.num_flag = 1;
	res.t.f = cos(n.t.f);
	return res;
}

number tan_(number degree)
{
	number n, res;

	n = deg2rad_(degree);
	res.num_flag = 1;
	if(cos(n.t.f) == 0){		// 0 ���� ���� ��� 
		EVAL_ERR = 1; 
		res.t.f = 0.0;
	}else{
		res.t.f = sin(n.t.f) / cos(n.t.f);
	}
	return res;
}

number sec_(number degree)
{
	number n, res;

	n = deg2rad_(degree);
	res.num_flag = 1;
	if(sin(n.t.f) == 0){		// 0 ���� ���� ��� 
		EVAL_ERR = 1; 
		res.t.f = 0.0;
	}else{
		res.t.f = 1 / sin(n.t.f);
	}
	return res;
}

number csc_(number degree)
{
	number n, res;

	n = deg2rad_(degree);
	res.num_flag = 1;
	if(cos(n.t.f) == 0){		// 0 ���� ���� ��� 
		EVAL_ERR = 1; 
		res.t.f = 0.0;
	}else{
		res.t.f = 1 / cos(n.t.f);
	}
	return res;
}

number cot_(number degree)
{
	number n, res;

	n = deg2rad_(degree);
	res.num_flag = 1;
	if( (cos(n.t.f) == 0 ) || (sin(n.t.f) / cos(n.t.f)) ){			// 0 ���� ���� ��� 
		EVAL_ERR = 1;		
		res.t.f = 0.0;
	}else{
		res.t.f = 1 / (sin(n.t.f) / cos(n.t.f));
	}
	
	return res;
}

number log_(number num1)
{
	number res;

	if(num1.num_flag==0){
		res.t.f = log(num1.t.i) / log(10);
	}else{
		res.t.f = log(num1.t.f) / log(10);
	}
	res.num_flag = 1;
	
	return res;
}

number ln_(number num1)
{
	number res;
	if(num1.num_flag==0){
		res.t.f = log(num1.t.i);
	}else{
		res.t.f = log(num1.t.f);
	}
	res.num_flag = 1;

	return res;
}


number fac_(number num1)
{ 
	int i;
	number res;

	if (num1.num_flag == 1){		// num1�� �Ǽ� �϶� 
		res.num_flag = 0;
		res.t.i = 0;
		printf("���� �Ǽ� fac������ �̱���\n");		// �Ǽ� fac_()�̱���
	}else{							// num1�� ���� �϶� 
		res.num_flag = 0;
		if(num1.t.i < 0 ){			// num1�� ���� �϶� 
			res.t.i = 0;
			EVAL_ERR = 0;
			//printf("�� �Լ��� �ùٸ� �Է��� �ƴմϴ�(FAC).\n");		
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

	if (num1.num_flag == 1){		// num�� �Ǽ� �϶� 
		res.num_flag = 0;
		res.t.i = 0;
		printf("���� �Ǽ� sqrt������ �̱���\n");		// �Ǽ� sqrt_()�̱���
	}else{
		res.num_flag = 0;

		if(num1.t.i < 0 ){
			res.t.i = 0;
			EVAL_ERR = 0;
			//printf("�� �Լ��� �ùٸ� �Է��� �ƴմϴ�.(SQRT)\n");
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
			res.t.f = -1 * num1.t.f;		
		}
	}else{
		res.num_flag = 0;
		if(num1.t.i > 0 ){
			res.t.i = num1.t.i;
		}else{
			res.t.i = -1 * num1.t.i;		 
		}
	}

	return res;
}


number rand_(number num1)
{
	number n = num1;
	if(num1.num_flag == 0){
		n.t.i -= 1; 
	}else{
		n.t.f -= 1; 
	}
	return n;
}

number pow_(number num1, number num2)
{
    int i, op, n, state;
	number res;

	state = (num1.num_flag * 2 ) + (num2.num_flag);
	if (state > 0 ){								// num1 �Ǵ� num2�� �� �Ǽ� �϶� 
		res.num_flag = 0;
		res.t.i = 0;
		printf("���� �Ǽ� pow������ �̱���\n");		// �Ǽ� pow_()�̱���
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
	if (state >0 ){
		res.num_flag = 1;
	}else{
		res.num_flag = 0;
	}
	if (num2.t.f == 0 && num2.t.i == 0){
		res.num_flag = 0;
		res.t.i = 0;
		EVAL_ERR = 0;
		//printf("0���� ������ �����ϴ�\n");	
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


void operation1()	// ���ÿ��� pop�� �� �����ϰ� ����� push (�ǿ����ڰ� 1���� ����) 
{
	priority token;
	number res,num1;

	token = pop_op(&topOp);
	num1 = stack[topStk--];
	
	switch(token) {
		case _abs:
			res = abs_(num1); break;
		case _rand:
			res = rand_(num1); break;
		case _fac:	
			res = fac_(num1); break;
		case _sqrt:
			res = sqrt_(num1); break;
		case rad:
			res = deg2rad_(num1); break;
		case deg:
			res = rad2deg_(num1); break;
		case _sin:
			res = sin_(num1); break;
		case _cos:
			res = cos_(num1); break;
		case _tan:
			res = tan_(num1); break;
		case _sec:
			res = sec_(num1); break;
		case _csc:
			res = csc_(num1); break;
		case _cot:
			res = cot_(num1); break;
		case _log:
			res = log_(num1); break;
		case _ln:
			res = ln_(num1); break;
		default : 
			break;
	}
	push(&topStk,res);
}


void operation2()	// ���ÿ��� pop�� �� �����ϰ� ����� push (�ǿ����ڰ� 2���� ����) 
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
		case _pow:		
			res = pow_(num1,num2); break;
		case mod:
			res = modul_(num1,num2); break;
		default : 
			break;
	}
	push(&topStk,res);
}

void num_to_stack(char* temp,int *j)	// temp�� ���� ��Ʈ���� ���ÿ� Ǫ��
{
		number n;
		
		temp[*j]='\0';
		if(strchr(temp,'.') !=NULL){
			n.num_flag = 1;
		}else{
			n.num_flag = 0;
		}
		if(temp[0]!='\0'){
			switch(n.num_flag){
				case 0:
					n.t.i = atoi(temp);
					break;
				case 1:
					n.t.f =(float)atof(temp);
					break;
				default:
					break;
			}
			push(&topStk,n);
			temp[0]='\0'; *j=0; 
		}
}

void plot(char* str,int start, int end)		// f(x) �Լ����� [start,end]������ �׷��� �׸��� 
{
	int i; 
	number *result;
	number n;

	printf("%d %d \n",start,end);
	for(i=start; i <= end ; i++){
		n.num_flag = 0;
		n.t.i = i;
		eval(str,n);
		result = &stack[topStk];
		if(result->num_flag == 0)
			printf("i=%d,  result=%d\n",i,result->t.i);
		else 
			printf("i=%d,  result=%f\n",i,result->t.f);

		// TODO F(x)�� (x,y)��ǥ�� ���� �׷��� ��� 
	}
}

void eval(char* str,number n)
{
	char temp[MAX] = "\0";	// �ӽ� ���� ����
	int i = 0, j = 0;		// string ������
	int token;				// ���� ��ū�� ���� 
	int flag = 0;			//  ���ÿ� �����ϴ� '('�� ����  
	number fx = n;			//  f(x)�Լ��� x ���� ���� 
	number const_num;

	while(token != eos){
		token = get_token(&str[i]);
		switch(token){
		case blank :			// ' '
			break;
		case tap :				// '\t'
			break;
		case fun_x :			// ������ x 
			if (i != 0 && get_token(&str[i-1]) == num){
				num_to_stack(temp,&j);
				push_op(&topOp,times);
				push(&topStk,fx);
				operation2();
			}else{
				if( temp[j-1] != '-'){
					push(&topStk,fx);
				}else{
					if(n.num_flag == 0){
						fx.t.i *= -1;
					}else{
						fx.t.f *= -1;
					}
					push(&topStk,fx);
				}
			}
			break;
		case e :				// ��� E
			const_num.num_flag = 1;
			const_num.t.f = (float)E;

			if (i != 0 && get_token(&str[i-1]) == num){
				num_to_stack(temp,&j);
				push_op(&topOp,times);
				push(&topStk,const_num);
				operation2();
			}else{
				if( temp[j-1] != '-'){
					push(&topStk,const_num);
				}else{
					const_num.t.f *= -1;
					push(&topStk,const_num);
				}
			}
			break;
		case pi :				// ��� PI
			const_num.num_flag = 1;
			const_num.t.f = (float)PI;

			if (i != 0 && get_token(&str[i-1]) == num){
				num_to_stack(temp,&j);
				push_op(&topOp,times);
				push(&topStk,const_num);
				operation2();
			}else{
				if( temp[j-1] != '-'){
					push(&topStk,const_num);
				}else{
					const_num.t.f *= -1;
					push(&topStk,const_num);
				}
			}
			break;
		case comma :			// �Ҽ��� '.'
			temp[j++] = str[i]; 
			break;
		case num :				// ���� 
			temp[j++] = str[i]; break;
		case eos :				// �Է��� �� '\0'
			num_to_stack(temp,&j);
			while(topOp != -1 ){
				if(oper[topOp] > 11 && oper[topOp] < 27 ){
					operation1();
				}else{
					operation2();
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
				if(oper[topOp] > 11 && oper[topOp] < 27 ){
					operation1();
				}else{
					operation2();
				}
			}
			pop_op(&topOp);		// '(' ���� 
			flag--; 
			break;
		default :				// �������� ���
			if( get_token(&str[i]) == minus ){ 				
				if( get_token(&str[i-1]) == minus || get_token(&str[i-1]) == lparen || i==0 || get_token(&str[i-1]) == mod 
							|| get_token(&str[i-1]) == times || get_token(&str[i-1]) == divide){
					temp[j++] = str[i];
					temp[j++] = '1';
					num_to_stack(temp,&j);
					push_op(&topOp,times);
					i++; 
					continue;
				}
			}
			
			num_to_stack(temp,&j);
			if(topOp == -1){							// ���ÿ� �����ڰ� �ƹ��͵� ���� ��� 
				push_op(&topOp,get_token(&str[i])); 
			}else if ( oper[topOp] == lparen ){			// ���ÿ� ���� �ֱٿ� ���� �����ڰ� '('���  
				push_op(&topOp,get_token(&str[i])); 
			}else if(topOp > -1 ){		 
				if(oper[topOp] >= get_token(&str[i])){
					if(oper[topOp] > 11 && oper[topOp] < 27 ){
						operation1(); i--;
					}else{
						operation2(); i--;
					}
				}else{
					push_op(&topOp,get_token(&str[i]));
				}
			}
			break;
		}
		i++;
	}
}

int main(void)
{
	char str[MAX];
	char x[MAX];
	number result;
	number n;

	fflush(stdin);
	printf("input  : ");
	gets(str);
	
	//plot(str,1,5);	// f(x) �Լ����� [1,5]������ �׷��� �׸��� 
	
	
	if(strchr(str,'x') != NULL){		// �Է½Ŀ� x�� ���� �� �� 
		printf("x���� �Է� �Ͻÿ� => ");
		gets(x);						// x���� �Է��� �޴´� 
		if (strchr(x,'.') !=NULL){		// x���� �Ǽ� 
			n.num_flag = 1;
			n.t.f = (float)atof(x);
		}else{							// x���� ����  
			n.num_flag = 0;
			n.t.i = atoi(x);
		}
	}else{								// �Է½Ŀ� x�� �������� ���� �� 
		n.num_flag = 0;
		n.t.i = 0;
	}

	eval(str,n);
	result = stack[topStk];

	if(result.num_flag == 0)
		printf("result is %d \n",result.t.i);
	else 
		printf("result is %f \n",result.t.f);

	return 0;
}