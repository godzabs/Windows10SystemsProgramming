/*
	Create a thread-safe implementation of a stack data structure. If a pop operation cannot
succeed, block the thread until data is available (use a condition variable).
*/

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

//defining structs for the stack nodes

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


//funciton to create the first head of the list
list* create_list(int d) {
	list* head = malloc(sizeof(list));
	head->data = d;
	head->next = NULL;
	return head;
}

//adds a number to the front and returns a list struct
list* add_to_front(int d, list* h) {
	list* head = create_list(d);
	head->next = h;
	return head;
}

//Function call to populate the list
list* populate_list() {
	list* head = create_list(0);
	for (int i = 1; i < 10; i++) {
		head = add_to_front(i, head);
	}
	WakeConditionVariable(&myStack.cond);
	return head;

}


//Pop funciton that is thread safe
void pop() {
	EnterCriticalSection(&myStack.lock);
	if (myStack.top->next == NULL) {
		SleepConditionVariableCS(&myStack.cond, &myStack.lock, INFINITE);
	}
	list* tempStack = myStack.top;
	myStack.top = myStack.top->next;
	free(tempStack);

	LeaveCriticalSection(&myStack.lock);
}


void printStack(stack iterateStack) {
	while (iterateStack.top != NULL) {
		printf("myStack.top = %d\n", iterateStack.top->data);
		iterateStack.top = iterateStack.top->next;
	}
}

//basic function to pop a list off the stack, using DWORD WINAPI to make it so threads can use it
DWORD WINAPI doPop(PVOID p) {
	pop();
	return 0;
}

//TODO
void deleteList() {
	while (myStack.top->next != NULL) {
		printf("the number %d in the stack has been removed!\n", myStack.top->data);
		pop();
	}
}


int main(void) {
	InitializeCriticalSection(&myStack.lock);
	InitializeConditionVariable(&myStack.cond);

	myStack.top = populate_list();
	printStack(myStack);


	//Create 3 threads to pop the stack 3 times
	HANDLE hThread[3];
	DWORD tid;
	for (int i = 0; i < 3; i++) {
		hThread[i] = CreateThread(NULL, 0, doPop, NULL, 0, NULL);
	}


	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
	printf("\n\nAfter pop\n");
	printStack(myStack);

	deleteList();
	DeleteCriticalSection(&myStack.lock);

	for (int i = 0; i < 3; i++) {
		CloseHandle(hThread[i]);
	}

	return EXIT_SUCCESS;
}