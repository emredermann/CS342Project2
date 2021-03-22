#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#define CommandSize 1500


    struct timeval tv;


struct QNode { 
    time_t curtime;
    int pid;    //thread index
    int bid;    // burst index
    int length; 
    int wallClock;  
    struct QNode* next; 
}; 
  

struct Queue { 
    struct QNode *front, *rear; 
}; 

struct QNode* newNode(int pid,int bid,int length,int wallClock) 
{ 
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->pid = pid;
    temp->bid = bid;
    temp->length = length;
    temp->wallClock = wallClock;
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
void enQueue(struct Queue* q, int pid,int bid,int length,int wallClock) 
{ 
    // Create a new LL node 
    struct QNode* temp = newNode(pid,bid, length, wallClock); 
  
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

void processInputFile(char command[],int *N,int *minB,int *avgB,int *minA,int *avgA,char  *ALG){

    N = command[0];
    ALG = command[1];
    FILE* filepointer =fopen(command[3],"r");
    char buffer[*N];
    while (fgets(buffer, N, filepointer)) {
        buffer[strcspn(buffer, "\n\r")] = '\0'; // remove the newline at the end
        char command[strlen(buffer)];
        strcpy(command, buffer);
        parseFile(command,*minB, *avgB,*minA, *avgA);
    }
    fclose(filepointer);
}

void parseFile(char command[],int *minB,int *avgB,int *minA,int *avgA){
    for (int i = 0; i < 4; i++) {
        command[i] = strsep(&command, " ");

        if (command[i] == NULL) {
            break;
        }
    }
    minB = command[0];
    avgB = command[1];
    minA = command[2];
    avgA = command[3];
}

void processInputManual(char command [],int *N,int *minB,int *avgB,int *minA,int *avgA,char ** ALG){
    
        *N = atoi(strsep(&command, " "));       // NUmber of W threads   1 -- 10
     //   Bcount= atoi(strsep(&command, " "));  //number of bursts that each W thread will generate.
        *minB = atoi(strsep(&command, " "));  
        *avgB = atoi(strsep(&command, " "));  
        *minA = atoi(strsep(&command, " "));  
        *avgA = atoi(strsep(&command, " "));  
       *ALG = command;
}

void PthreadScheduler(int N,int minB,int avgB,int minA,int avgA,char * ALG){
    struct Queue * runquque;
    runquque = createQueue();
    pthread_t tid[N];
    int bid,length,wallClock;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    for (int i = 0; i < N; i++)
    {
        // Empty threads created.
        pthread_create(&tid[i],&attr,NULL,NULL);
        enQueue(runquque,tid[i],bid,length,wallClock);
    }
    

}

int main(int argc, char const *argv[])
{
    char commands[CommandSize];
    while(1){

        int N;
        int Bcount;
        int minB;
        int avgB;
        int minA ;
        int avgA;
        char * ALG;
        printf("\nschedule: ");
        fgets(commands, CommandSize, stdin);
     
        // Replace line endings with string terminator.
        commands[strcspn(commands, "\n\r")] = '\0';      
        if (1)
        {
         processInputManual(commands,&N,&minB,&avgB,&minA,&avgA,&ALG);
        }
 
       printf("N is : %d  minB is : %d  avgB is : %d  minA is : %d  avgA is : %d  ALG is : %s",N,minB,avgB,minA,avgA,ALG);
    
    PthreadScheduler(N,minB,avgB,minA,avgA,ALG);
    
    
    }
 
    return 0;
}


