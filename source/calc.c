/*#include "common.h"
//#define pi() 3.1415926535897931
//#define e() 2.7182818284590451

Num eval1(const char *expr) {
    //return 0; //stub
    Num n;
    n.type = 1;
    n.v.f = 10.101010101;
    return n;
}
Num eval2(const char *expr, Num x) {
    Num n;
    n.type = 1;
    n.v.f = sin(deg2rad(x.v.i));
    return n;
    //return 0;
}


float add(float a, float b) {
    return a + b;
}

float sub(float a, float b) {
    return a - b;
}

float mul(float a, float b) {
    return a * b;
}

float div(float a, float b) {
    return a / b;
}

float rad2deg(float a) {
    return a * 180. / PI;
}

float deg2rad(float a) {
    return a * PI / 180.;
}

int randInt(int max) {
    return PA_RandMax(max);
}*/

#include "common.h"

#define MAX 100
#define PI 3.1415926535897931
#define E 2.718281828

typedef enum {
        blank,tap,rparen,comma,num,eos,plus,minus,times,divide,mod,_pow,e,pi,fun_x,
                _abs,_sqrt,_fac,_sin,_cos,_tan,_sec,_csc,_cot,_log,_ln,_rand,rad,deg,lparen
} priority;

static number stack[MAX];               // ¼ýÀÚ ½ºÅÃ
int oper[MAX];                                  // ¿¬»êÀÚ ½ºÅÃ

static int topStk = -1, topOp = -1; // ½ºÅÃ Æ÷ÀÎÅÍ
static int EVAL_ERR = 0;                        // 1ÀÌ¸é ¿¬»ê ¿¡·¯  

void eval(char* str,number n);
void operation1();
void operation2();
priority get_token(char* symbol);

// ½ºÅÃ
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
        if(cos(n.t.f) == 0){            // 0 À¸·Î ³ª´« °æ¿ì
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
        if(sin(n.t.f) == 0){            // 0 À¸·Î ³ª´« °æ¿ì
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
        if(cos(n.t.f) == 0){            // 0 À¸·Î ³ª´« °æ¿ì
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
        if( (cos(n.t.f) == 0 ) || (sin(n.t.f) / cos(n.t.f)) ){                  // 0 À¸·Î ³ª´« °æ¿ì
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

        if (num1.num_flag == 1){                // num1ÀÌ ½Ç¼ö ÀÏ¶§
                res.num_flag = 0;
                res.t.i = 0;
                printf("¾ÆÁ÷ ½Ç¼ö fac¿¬»êÀº ¹Ì±¸Çö\n");         // ½Ç¼ö fac_()¹Ì±¸Çö
        }else{                                                  // num1ÀÌ Á¤¼ö ÀÏ¶§
                res.num_flag = 0;
                if(num1.t.i < 0 ){                      // num1ÀÌ À½¼ö ÀÏ¶§
                        res.t.i = 0;
                        EVAL_ERR = 0;
                        //printf("ÀÌ ÇÔ¼öÀÇ ¿Ã¹Ù¸¥ ÀÔ·ÂÀÌ ¾Æ´Õ´Ï´Ù(FAC).\n");          
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

        if (num1.num_flag == 1){                // numÀÌ ½Ç¼ö ÀÏ¶§
                res.num_flag = 1;
                res.t.f = (float)sqrt(num1.t.f);
        }else{
                res.num_flag = 0;

                if(num1.t.i < 0 ){
                        res.t.i = 0;
                        EVAL_ERR = 0;
                        //printf("ÀÌ ÇÔ¼öÀÇ ¿Ã¹Ù¸¥ ÀÔ·ÂÀÌ ¾Æ´Õ´Ï´Ù.(SQRT)\n");
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
        number res;
        int state;//i, op, n;

        state = (num1.num_flag * 2 ) + (num2.num_flag);
       
        switch (state){
                case 0:
                        if(num2.t.i < 0) {
                                res.num_flag = 1;
                                res.t.f = (float)pow(num1.t.i,num2.t.i);
                        } else {
                                res.num_flag = 0;
                                res.t.i = (int)pow(num1.t.i, num2.t.i);
                        }
                        break;
                case 1:
                        res.t.f = (float)pow(num1.t.i ,num2.t.f);
                        res.num_flag = 1;
                        break;
                case 2:
                        res.t.f = (float)pow(num1.t.f,num2.t.i);
                        res.num_flag = 1;
                        break;
                case 3:
                        res.t.f = (float)pow(num1.t.f , num2.t.f);
                        res.num_flag = 1;
                        break;
                default:
                        break;
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
                //printf("0À¸·Î ³ª´­¼ö ¾ø½À´Ï´Ù\n");    
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


void operation1()       // ½ºÅÃ¿¡¼­ popÇÑ ÈÄ ¿¬»êÇÏ°í °á°ú¸¦ push (ÇÇ¿¬»êÀÚ°¡ 1°³ÀÎ ¿¬»ê)
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


void operation2()       // ½ºÅÃ¿¡¼­ popÇÑ ÈÄ ¿¬»êÇÏ°í °á°ú¸¦ push (ÇÇ¿¬»êÀÚ°¡ 2°³ÀÎ ¿¬»ê)
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


void num_to_stack(char* temp,int *j)    // tempÀÇ ¼ýÀÚ ½ºÆ®¸µÀ» ½ºÅÃ¿¡ Çª½¬
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

void plot(char* str,int start, int end)         // f(x) ÇÔ¼ö¿¡¼­ [start,end]¹üÀ§ÀÇ ±×·¡ÇÁ ±×¸®±â
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

                // TODO F(x)ÀÇ (x,y)ÁÂÇ¥¿¡ µû¶ó ±×·¡ÇÁ Ãâ·Â
        }
}

void eval(char* str,number n)
{
        char temp[MAX] = "\0";  // ÀÓ½Ã ÀúÀå °ø°£
        int i = 0, j = 0;               // string Æ÷ÀÎÅÍ
        int token;                              // ÇöÀç ÅäÅ«ÀÇ Á¾·ù
        int flag = 0;                   //  ½ºÅÃ¿¡ Á¸ÀçÇÏ´Â '('ÀÇ °¹¼ö  
        number fx = n;                  //  f(x)ÇÔ¼öÀÇ x °ªÀ» ÀúÀå
        number const_num;

        while(token != eos){
                token = get_token(&str[i]);
                switch(token){
                case blank :                    // ' '
                        break;
                case tap :                              // '\t'
                        break;
                case fun_x :                    // ¹ÌÁö¼ö x
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
                case e :                                // »ó¼ö E
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
                case pi :                               // »ó¼ö PI
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
                case comma :                    // ¼Ò¼ýÁ¡ '.'
                        temp[j++] = str[i];
                        break;
                case num :                              // ¼ýÀÚ
                        temp[j++] = str[i]; break;
                case eos :                              // ÀÔ·ÂÀÇ ³¡ '\0'
                        num_to_stack(temp,&j);
                        while(topOp != -1 ){
                                if(oper[topOp] > 11 && oper[topOp] < 27 ){
                                        operation1();
                                }else{
                                        operation2();
                                }
                        }
                        break;
                case lparen :                   // '('
                        flag++;
                        push_op(&topOp,get_token(&str[i]));
                        break;
                case rparen :                   // ')'
                        num_to_stack(temp,&j);
                        while(oper[topOp] != lparen){  
                                if(oper[topOp] > 11 && oper[topOp] < 27 ){
                                        operation1();
                                }else{
                                        operation2();
                                }
                        }
                        pop_op(&topOp);         // '(' Á¦°Å
                        flag--;
                        break;
                default :                               // ¿¬»êÀÚÀÎ °æ¿ì
                        if( get_token(&str[i]) == minus ){                              
                                if( get_token(&str[i-1]) == minus || get_token(&str[i-1]) == lparen || i==0 || get_token(&str[i-1]) == mod
                                                        || get_token(&str[i-1]) == times || get_token(&str[i-1]) == divide || get_token(&str[i-1]) == _pow ){
                                        temp[j++] = str[i];
                                        temp[j++] = '1';
                                        num_to_stack(temp,&j);
                                        push_op(&topOp,times);
                                        i++;
                                        continue;
                                }
                        }
                       
                        num_to_stack(temp,&j);
                        if(topOp == -1){                                                        // ½ºÅÃ¿¡ ¿¬»êÀÚ°¡ ¾Æ¹«°Íµµ ¾ø´Â °æ¿ì
                                push_op(&topOp,get_token(&str[i]));
                        }else if ( oper[topOp] == lparen ){                     // ½ºÅÃ¿¡ °¡Àå ÃÖ±Ù¿¡ µé¾î¿Â ¿¬»êÀÚ°¡ '('°æ¿ì  
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

number eval1(char *expr) {
    number n;
    n.num_flag = 0;
    n.t.i = 0;
    eval(expr, n);
    return stack[topStk];
}
number eval2(char *expr, number n) {
    eval(expr, n);
    return stack[topStk];
}

