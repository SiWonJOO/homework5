#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{ // Operator 들의 우선순위를 정하기 위해서 정의 함.
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];
char postfixExp[MAX_EXPRESSION_SIZE];
char postfixStack[MAX_STACK_SIZE];
int evalStack[MAX_STACK_SIZE];
//초기화
int postfixStackTop = -1;
int evalStackTop = -1;

int evalResult = 0;
// 함수 선언
void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

   printf("[----- [SIWON JOO] [2018038045] -----]");
	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); //q나 Q를 입력받았을때 반복문 탈출함.

	return 1;


}

void postfixPush(char x) // 스택에 쌓는 push 함수
{
    postfixStack[++postfixStackTop] = x; // 스택 탑을 증가시키고 x의 값을 넣는다.
}

char postfixPop() // Pop 스택에서 꺼내는 함수.
{
	char x;
    if(postfixStackTop == -1) // 스택에 아무것도 없다면?
        return '\0'; // null값 리턴
    else { // 스택에 값이 하나이상 쌓여있다면
    	x = postfixStack[postfixStackTop--]; // 현재 스택에 값을 x에 넣어준후 스택을 감소 시킨다.
    }
    return x; // Pop을 해서 스택에 있는 값 return 해준다.
}

void evalPush(int x) // 스택에 쌓는 push 함수
{
    evalStack[++evalStackTop] = x; // 스택 탑을 증가시키고 x의 값을 넣는다.
}

int evalPop() // Pop 스택에서 꺼내는 함수.
{
    if(evalStackTop == -1) // 스택에 아무것도 없다면? -1 값 리턴
        return -1;
    else
        return evalStack[evalStackTop--]; // 스텍이 하나라도 있다면 현재 스텍에 값을 return 해주고 스택을 감소 시킨다.
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */

void getInfix()// 입력 받은 연산식을 저장해 주는 함수
{
    printf("Type the expression >>> "); 
    scanf("%s",infixExp); // 입력받음
}

precedence getToken(char symbol) // 특정 문자를 쉽게 보기 위해 니모닉 사용
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x) // 우선순위 함수 getToken 함수와 같음 가독성을 위해 만든 함수.
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1); // 문자 c 를 postfixExp 에 복사한다.
	else
		strncat(postfixExp, c, 1); // 문자 c를 postfixExp 에 이어 붙힌다.
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') // Null일때까지 무한 반복한다.
	{
		if(getPriority(*exp) == operand) // 피연산자라면
		{
			x = *exp; // 피연산자의 값을 x에 넣어주고
        	charCat(&x); // postfixExp 이어붙히는 함수를 호출한다.
		}
        else if(getPriority(*exp) == lparen) { // 왼쪽 가로(가로 열기) 라면

        	postfixPush(*exp); // postfixPush 함수를 호출하여 후위연산스택에 쌓아둔다.
        }
        else if(getPriority(*exp) == rparen)  // 오른쪽 가로(가로 닫기) 라면
        {
        	while((x = postfixPop()) != '(') { // 왼쪽가로를 찾을때까지 스택을 계속 pop 해준다.
        		charCat(&x); // postfix스택에 있는 값을 꺼내서 postfixExp에 붙여준다.
        	}
        }
        else //연산자(operator)라면
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) // 연산자 우선순위를 계산함, 만약 연산자를 비교했을때 이전에 들어온 연산자가 더 크거나 같다면 반복
            {
            	x = postfixPop(); // postfix 스택에 들어간 연산자를 pop해서 x에 대입해준다.
            	charCat(&x); // postfixExp 에 이어붙힌다.
            }
            postfixPush(*exp); // 만약 우선순위가 더 크다면 연산자를 postfix 스택에 push 한다.
        }
        exp++;
	}

    while(postfixStackTop != -1) // 후위연산자를 저장하는 스택이 빌때까지 반복 - 연산자들만 모두 출력하기 위함
    {
    	x = postfixPop(); // pop 해서 감소한 값을 x에 저장해두고
    	charCat(&x); // postfixExp에 이어붙힌다.
    }

}

void debug() // 디버그 , 값들을 출력하는 함수
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++) //스택에 MAX_STACK_SIZE 만큼 반복
		printf("%c  ", postfixStack[i]); // 스택에있는 연산자를 하나씩 출력한다.

	printf("\n");

}

void reset() // 초기화 해주는 함수
{
	infixExp[0] = '\0'; // infix 밑 postfix 를 null 값으로 초기화
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++) // MAX STACK_SIZE 만큼 반복한다.
		postfixStack[i] = '\0'; // postfix스택을 모두 null 값으로 초기화해준다.
         
	postfixStackTop = -1; //초기상태로 초기화
	evalStackTop = -1;  // 초기상태로 초기화
	evalResult = 0;  //초기상태로 초기화
}
void evaluation() // operator의 우선순위를 평가 하기 위한 함수
{
	int opr1, opr2, i;

	int length = strlen(postfixExp); //문자열의 길이를 저장하는 함수를 사용해 postfixExp배열의 길이를 length 에 저장
	char symbol;
	evalStackTop = -1; // 초기화

	for(i = 0; i < length; i++) // 배열의 최대크기만큼 반복
	{
		symbol = postfixExp[i]; // postfixExp의 i번째 값을 symbol에 대입해준다.
		if(getToken(symbol) == operand) { // 만약 symbol이 피연산자라면
			evalPush(symbol - '0'); // 그 값을 그대로 eavlstack 에 push 해준다
		}   // 왜 symbol 에서 0빼주냐면 symbol 은 ASCII 값이므로 아스키코드에서 0 값이 48이므로 아스키코드 0을 빼주면 인트형 처럼 값을 읽을 수 있다.
		else { // 만약 연산자라면
			opr2 = evalPop(); // eval_Stack에 top에 있는 피연산자를 pop해서 opr2 에 대입해준다.
			opr1 = evalPop(); // 현재 eval_Stack top에 있는 피연산자를 pop해서 opr1 에 대입해준다.
			switch(getToken(symbol)) {
			case plus: evalPush(opr1 + opr2); break; // + 연산자를 발견하면 더해주고 더해준 값을 eval 스택에 push 한다.
			case minus: evalPush(opr1 - opr2); break; // - 연산자를 발견하면 빼주고 그 값을 eval 스택에 push 한다.
			case times: evalPush(opr1 * opr2); break; // * 연산자를 발견하면 곱해주고 그 값을 eval 스택에 push 한다.
			case divide: evalPush(opr1 / opr2); break; // / 연산자를 발견하면 나눠주고 그 값을 eval 스택에 push 한다.
			default: break;
			}
		}
	}
	evalResult = evalPop(); // 반복문이 끝나면 스택 top 에는 연산이 끝난 값이 들어가 있으므로 그 값을 pop해서 result 에 대입해준다.
}

