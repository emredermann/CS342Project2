#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define CommandSize 1500


struct timeval tv;
pthread_mutex_t mutexBuffer;
pthread_cond_t condition;

int bid;
struct Queue * runquque;
int Bcount;
int avgB,avgA;
int N;
int minB;
int minA ;
char * ALG;
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
  
bool isEmpty(struct Queue* q){
    return q->front == NULL;
} 
// Function to remove a key from given queue q 
struct QNode * deQueue_FCFS(struct Queue* q) 
{   
    // Store previous front and move front one node ahead 
    struct QNode* temp = q->front; 
  
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
    return temp;
} 

struct QNode * deQueue_SJF(struct Queue* q){
    struct QNode* temp = q->front; 
  
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
    return temp;
}

struct QNode * deQueue_PRIO(struct Queue* q){
    struct QNode* temp = q->front; 
    struct QNode* before_target;

    // Finds the less prio and sets the before_target to the appropriote index.
  while(temp != NULL){
      if(temp->pid > temp->next->pid){
          before_target->next = temp->next;
      }
      temp=temp->next;
  }
  if(before_target == NULL){

    free(before_target);
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 

    }else{
        temp = before_target->next;
        before_target->next=before_target->next;        
    }
        if (q->front == NULL) 
            q->rear = NULL; 
        return temp;
    }

struct QNode * deQueue_VRUNTIME(struct Queue* q){struct QNode* temp = q->front; 
  //while(temp.)
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
    return temp;}


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

void processInputManual(char command [],int *N,int *Bcount,int *minB,int *avgB,int *minA,int *avgA,char ** ALG){
    
        *N = atoi(strsep(&command, " "));       // NUmber of W threads   1 -- 10
        *Bcount= atoi(strsep(&command, " "));  //number of bursts that each W thread will generate.
        *minB = atoi(strsep(&command, " "));  
        *avgB = atoi(strsep(&command, " "));  
        *minA = atoi(strsep(&command, " "));  
        *avgA = atoi(strsep(&command, " "));  
        *ALG = command;
}


float exponential_dist(int a){
    int u ;
     u = rand()% 500;
    return u;
   // return  -log(1-u) / (1/a);
}

void * producer(void * pid){
while(1){
    //Bcount the amount of bursts each thread generated.
    for (int i = 0; i < Bcount; i++)
    {
        int length,wallClock;
        printf("\nBcount is : %d \n",Bcount);
        printf("\nBurst counter %d \n",i);

        //Created burst
        int tmp = random() % 100;

        //Length each burst is random and exponentially distributed with mean avgB.
        //)
        do{
          length = (int)exponential_dist(avgB); 
        }while(length < minB );
        
        struct timeval current_time;
        gettimeofday(&current_time,NULL);
        wallClock = (int) current_time.tv_usec;

        pthread_mutex_lock(&mutexBuffer);
        // Enqueue cpu burst.
        enQueue(runquque, pid,i,length,wallClock);
        printf("\n(producer thread) thread index is :%d burst index is :%d length is(ms):%d wallClock is :%d \n",pid,bid,length,wallClock);
        pthread_mutex_unlock(&mutexBuffer);
        int avgA_checker = exponential_dist(avgA);
        while(avgA_checker < minA){avgA_checker = exponential_dist(avgA);}
        sleep(avgA_checker);
    }

    pthread_cond_signal(&condition);

    pthread_exit(0);
}
}


// for s_thread which is going to consume the W threads productions.
void * consumer(void * param){

  while(1){
      
    // If the queue is empty then waits
    
    pthread_mutex_lock(&mutexBuffer);   
    while(isEmpty(runquque)){
        printf("\n Queue is empty. \n");
        pthread_cond_wait(&condition, &mutexBuffer);  
    }    

            struct QNode * Tmp ;   
    if(strcmp(ALG , "FCFS") == 0){Tmp =  deQueue_FCFS(runquque);}
    if(strcmp(ALG , "SJF") == 0){Tmp =  deQueue_SJF(runquque);}
    if(strcmp(ALG , "PRIO") == 0){Tmp =  deQueue_PRIO(runquque);}
    if(strcmp(ALG , "VRUNTIME") == 0){Tmp =  deQueue_VRUNTIME(runquque);}

    // Enqueue cpu burst.
    sleep(Tmp->length/100);     
    pthread_mutex_unlock(&mutexBuffer);
    printf("\n(consumer thread) thread index is :%d burst index is :%d length is(ms):%d wallClock is :%d \n",Tmp->pid,Tmp->bid,Tmp->length,Tmp->wallClock);
        
       
    }
    pthread_exit(0);
}


void PthreadScheduler(int N,int minB,int avgB,int minA,int avgA,char * ALG){
    
    pthread_mutex_init(&mutexBuffer,NULL);
    pthread_cond_init(&condition,NULL);
    bid = 0;
    runquque = createQueue();
    pthread_t tid[N+1];
 
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //N threads created

    // Each thread will generate a sequence of cpu burst one-byone and add them to rq.
    // Length f each burst id random and exponentially distributed with mean avgB
    // The inter-arrival time between two consecutive bursts is also random and exponential distributed with mean avgA
    // W thread will sleep between two burst generations
    // Burst created must be added to queue.
    
    for (int i = 0; i < N; i++)
    {
        // Work threads created.
        if (pthread_create(&tid[i],&attr,producer,i+1)){
            perror("Failed to create W thread");
        }
    }
    // S thread created
    if(pthread_create(&tid[N],&attr,consumer,NULL))
    {
        perror("Failed to create S thread");
    }
    for (int i = 0; i < N+1; i++)
    {
        if(pthread_join(tid[i],NULL)){
            perror("Failed to create join");
        }
    }
    
    pthread_mutex_destroy(&mutexBuffer);
}

int main(int argc, char const *argv[])
{
    char commands[CommandSize];
    while(1){


        printf("\nschedule: ");
        fgets(commands, CommandSize, stdin);
     
        // Replace line endings with string terminator.
        commands[strcspn(commands, "\n\r")] = '\0';      
        if (1)
        {
         processInputManual(commands,&N,&Bcount,&minB,&avgB,&minA,&avgA,&ALG);
        }
 
       //printf("N is : %d Bcount is :%d minB is : %d  avgB is : %d  minA is : %d  avgA is : %d  ALG is : %s",N,Bcount,minB,avgB,minA,avgA,ALG);
    
    PthreadScheduler(N,minB,avgB,minA,avgA,ALG);
    
    }
 
    return 0;
}


