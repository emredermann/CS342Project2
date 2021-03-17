#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#define CommandSize 1500

struct QNode { 
    int key; 
    struct QNode* next; 
}; 
  
struct Queue { 
    struct QNode *front, *rear; 
}; 
struct QNode* newNode(int k) 
{ 
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->key = k; 
    temp->next = NULL; 
    return temp; 
} 
  
// A utility function to create an empty queue 
struct Queue* createQueue() 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void enQueue(struct Queue* q, int k) 
{ 
    // Create a new LL node 
    struct QNode* temp = newNode(k); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
} 
  
// Function to remove a key from given queue q 
void deQueue(struct Queue* q) 
{ 
    // If queue is empty, return NULL. 
    if (q->front == NULL) 
        return; 
  
    // Store previous front and move front one node ahead 
    struct QNode* temp = q->front; 
  
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
  
    free(temp); 
} 





void processInput(char command []){
    int N = atoi(strsep(&command, " "));       // NUmber of W threads   1 -- 10
    int Bcount= atoi(strsep(&command, " "));  //number of bursts that each W thread will generate.
    int minB = atoi(strsep(&command, " "));  
    int avgB = atoi(strsep(&command, " "));  
    int minA = atoi(strsep(&command, " "));  
    int avgA = atoi(strsep(&command, " "));  
    char* ALG = command;

    printf("%d - %d -%d -%d -%d- %d - %s ",N,Bcount,minB,avgB,minA,avgA,ALG);
    
}

/*
void parseCommand(char command[], char* argv1[]) {
    int i =0;
    while(i < 11){  
        argv1[i] = strsep(&command, " ");
        if (argv1[i] == NULL) {
            break;
        }
        if (strlen(argv1[i]) == 0){
            i -= 1;
            }
        i += 1;
    }
}
*/

int main(int argc, char const *argv[])
{
    char commands[CommandSize];
    while(1){
        printf("\nschedule$: ");
        fgets(commands, CommandSize, stdin);
        // Replace line endings with string terminator.
        commands[strcspn(commands, "\n\r")] = '\0';      
        processInput(commands);
    }
 
    return 0;
}


