//선형큐 

#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 5

typedef int element;

typedef struct{
	int front;
	int rear;
	element data[MAX_SIZE];
}queue;

void init(queue *q){  // 큐를 초기화 
	q->rear=-1;
	q->front=-1;
}

void enqueue(queue *q,int item){
	q->data[++(q->rear)]=item;
}

int dequeue(queue *q){
	int item=q->data[++(q->front)];
	return item;
	
}
