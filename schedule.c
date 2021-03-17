#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#define CommandSize 1500

struct QNode { 
    int data; 
    QNode* next; 
    QNode(int d) 
    { 
        data = d; 
        next = NULL; 
    } 
}; 
  
struct Queue { 
    QNode *front, *rear; 
    Queue() 
    { 
        front = rear = NULL; 
    } 
void enQueue(int x) 
    { 
  
        // Create a new LL node 
        QNode* temp = new QNode(x); 
  
        // If queue is empty, then 
        // new node is front and rear both 
        if (rear == NULL) { 
            front = rear = temp; 
            return; 
        } 
  
        // Add the new node at 
        // the end of queue and change rear 
        rear->next = temp; 
        rear = temp; 
    } 
  
    // Function to remove 
    // a key from given queue q 
    void deQueue() 
    { 
        // If queue is empty, return NULL. 
        if (front == NULL) 
            return; 
  
        // Store previous front and 
        // move front one node ahead 
        QNode* temp = front; 
        front = front->next; 
  
        // If front becomes NULL, then 
        // change rear also as NULL 
        if (front == NULL) 
            rear = NULL; 
  
        delete (temp); 
    } 
}; 

void processInput(char commands []){
   int N;       // NUmber of W threads   1 -- 10
   int Bcount;  //number of bursts that each W thread will generate.
   int minB;  
   int avgB;  
   int minA;  
   int avgA;  
   char* ALG[10];
    
    int mode;
    int N;
    
    if(commandDecider(commands,argv1,argv2) == 1){      

            getMode(&N,&mode);
            
            if (mode == 1){
             
                runMultipleCommandNormalMode(argv1,argv2);

            } 
            if (mode == 2){
                printf("N is :%d\n",N);
                runMultipleCommandTappedMode(argv1,argv2,N);

            }  

        }else{

            runSingleCommand(argv1);
    }
    
}


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


