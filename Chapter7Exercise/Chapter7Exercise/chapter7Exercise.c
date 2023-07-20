/*
	Create a thread-safe implementation of a stack data structure. If a pop operation cannot
succeed, block the thread until data is available (use a condition variable).
*/

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

typedef struct list {
	int data;
	struct list* next;
} list;

typedef struct {
	list* top;
	CRITICAL_SECTION lock;
	CONDITION_VARIABLE cond;

} stack;

//Made it global due to ease of accessing for threads and for other functions;
stack myStack;



list* create_list(int d) {
	list* head = malloc(sizeof(list));
	head->data = d;
	head->next = NULL;
	return head;
}

list* add_to_front(int d, list* h) {
	list* head = create_list(d);
	head->next = h;
	return head;
}

list* populate_list() {
	list* head = create_list(0);
	for (int i = 1; i < 10; i++) {
		head = add_to_front(i,head);
	}
	return head;

}

//TODO
void deleteList() {
	
}

//ToDo: Make Thread safe
void pop() {
	list* tempStack = myStack.top;
	myStack.top = myStack.top->next;
	free(tempStack);
}


void printStack(stack iterateStack) {
	while (iterateStack.top != NULL) {
		printf("myStack.top = %d\n", iterateStack.top->data);
		iterateStack.top = iterateStack.top->next;
	}
}

int main(void) {
	myStack.top = populate_list();
	printStack(myStack);
	pop();
	printf("\n\nAfter pop\n");
	printStack(myStack);



	
	return EXIT_SUCCESS;
}