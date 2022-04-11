#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 크기가 4이고 원형큐 이므로 정수가 3개 들어간다.

typedef char element; // 구조체 선언
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();


int main(void)
{
	QueueType *cQ = createQueue();
	element data;

	char command;
      printf("[----- [SIWON JOO] [2018038045] -----]");
	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ,&data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
          freeQueue(cQ); // 메모리 할당 해제가 안되어있어서 메모리 할당 해제 함수를 호출해준다.
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue() // 원형큐 형태를 만들기 위해 구조체를 포인터변수를 선언후 메모리 할당및 초기화를 해준다.
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType)); // 메모리를 할당해준다.
	cQ->front = 0; // front 비어있음
	cQ->rear = 0; // rear 비어있음
	return cQ;
}

int freeQueue(QueueType *cQ) // 할당된 메모리를 해제하는 함수 - 솔루션에서는 free 함수만 정의되어 있고 메인함수에서 호출 되어 있지 않음.
{
    if(cQ == NULL) return 1; // 할당된 메모리가 없다면 함수를 탈출한다. 
    free(cQ); // cQ 에 할당된 메모리를 해제한다.
    return 1;
}

element getElement() // 사용자로부터 값을 입력받는 함수
{
	element item; // 구조체 변수 item 선언
	printf("Input element = ");
	scanf(" %c", &item); // 값을 char 형으로 입력받는다.
	return item; // 받은 값을 리턴해주고 data에 저장한다.
}


int isEmpty(QueueType *cQ) // 큐가 비어있을때 호출하는 함수
{
	if (cQ->front == cQ->rear){ // front 와 rear 을 메인함수 시작때 0으로 초기화 해주었는데 만약 같다면 값이 비어있는 것이다.
		printf("Circular Queue is empty!");
		return 1;
	}
	else return 0;
}

int isFull(QueueType *cQ) // 큐가 가득 차 있을때
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { // 큐가 비어있는지 꽉 차있는지 구별하기 위해서 rear 값을 1 증가시킨후 나머지값 연산을 하여 비교한다.
		printf(" Circular Queue is full!");
		return 1;
	}
	else return 0;
}

void enQueue(QueueType *cQ, element item) // 큐에 값을 채워주는 함수. 값을 바꾸기 위해서 cQ 를 포인터 변수로 인자값을 받는다.
{
	if(isFull(cQ)) return; // 만약 큐가 가득차있으면 enqueue를 하지않고 오류를 출력한다.
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // 나머지값 연산을 하여 rear을 1 증가 시킨다.
		cQ->queue[cQ->rear] = item; // 큐에 item 을 삽입한다.
	}
}

void deQueue(QueueType *cQ, element *item) // 큐의 앞 원소를 삭제하는 함수. // 값을 비워야하기때문에 cQ와 ITEM 전부 값을 변경시키므로 싱글 포인터로 인자값을 받아온다.
{
	if(isEmpty(cQ)) return; // 큐가 비어있다면 삭제를 하지않고 오류를 출력한다.
	else { 
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; // front 값을 한칸 증가시키기 위해 나머지값 연산을 한다.
		*item = cQ->queue[cQ->front]; // 현재 front 가 가르키는 곳의 값을 item 에 return 해준다.                                                           
		return;
	}
}


void printQ(QueueType *cQ) // 큐를 출력하는 함수이다.
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // front 처음은 이미 디큐 된 값이므로 하나 증가시킨다.
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // last 에 rear 모듈로 계산한 값을 넣는다.

	printf("Circular Queue : [");

	i = first; //first 처음 순서를 i에 대입해서
	while(i != last){ // max size 에 도달할 떄까지 반복
		printf("%3c", cQ->queue[i]); // 3칸 띄워서 값을 출력한다. 
		i = (i+1)%MAX_QUEUE_SIZE; // 다음번째 값을 출력하기위해 i의 값을 증가시킨다.

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) // 인덱스에 어떤 값이 들어있는지 출력하는 함수
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) //max_큐 사이즈만큼 반복한다. 결국 4번 반복한다는 뜻이다.
	{
		if(i == cQ->front) { // front 가 가르키는 값과 i가 일치한다면 출력한다.
			printf("  [%d] = front\n", i); //front를 출력한다.
			continue; // 바로 반복문으로 다시 이동
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // i번째에 있는 큐에 값 출력

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // 현재 front 와 rear 의 위치를 출력한다.
}


