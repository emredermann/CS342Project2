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
#include <complex.h>
#define CommandSize 1500


struct timeval tv;
pthread_mutex_t mutexBuffer;
pthread_cond_t condition;
char* fileName;
char commands[CommandSize];
bool flag = false;
int bid;
int averageWaitingTime;
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
    int avgA_time;
    clock_t local_clock;
    struct QNode* next; 
}; 
  

struct Queue { 
    struct QNode *front, *rear; 
}; 

struct QNode* newNode(int pid,int bid,int length,int wallClock,int avgA_time) 
{ 
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->pid = pid;
    temp->bid = bid;
    temp->length = length;
    temp->wallClock = wallClock;
    temp->next = NULL; 
    temp->avgA_time = avgA_time;
    temp->local_clock = clock();
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
void enQueue(struct Queue* q, int pid,int bid,int length,int wallClock,int avgA_time) 
{ 
    // Create a new LL node 
    struct QNode* temp = newNode(pid,bid, length, wallClock,avgA_time); 
  
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
    struct QNode * current;
    struct QNode * target;
    
    current = q->front;
    target = q->front;

    int target_tid = q->front->pid;
    while(current->next != NULL){
      if(target-> length > current->next->length && current->pid == target_tid){
          target = current->next;
      }
      current=current->next;
  }
  // if the target initial node of the queue
if(target == q->front){
    q->front = q->front->next; 
 
  }
  // iterate the cur node to find the previous node of the target node to deallocate it.
  else{
      current = q->front;
      while(current->next!=target){
      current = current->next;}
    current->next = target->next;
  }
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
    return target;
}

struct QNode * deQueue_PRIO(struct Queue* q){
    struct QNode* temp = q->front; 
    struct QNode* target = q->front;

    // Finds the less thread id and sets the before_target to the appropriote index.
  while(temp->next!= NULL){
      if(target->pid > temp->next->pid){
          target = temp;
      }
      temp = temp->next;
  }
  if(target == q->front){
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 

    }else{
        temp=q->front;
        while(temp->next!=target){temp = temp->next;} 
        temp->next = target->next;
    }
        if (q->front == NULL) 
            q->rear = NULL; 
        return target;
    }

int virtualRuntime(int threadId,int length){
    return (int)length * (0.7+(0.3*threadId));
}

struct QNode * deQueue_VRUNTIME(struct Queue* q){
    
    struct QNode* temp = q->front; 
    struct QNode* target = q->front;

    // Finds the less thread id and sets the before_target to the appropriote index.
  while(temp->next!= NULL){
      if(virtualRuntime(target->pid,target->length) > virtualRuntime(temp->next->pid,temp->next->length)){
          target = temp;
      }
      temp = temp->next;
  }
  if(target == q->front){
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 

    }else{
        temp=q->front;
        while(temp->next!=target){temp = temp->next;} 
        temp->next = target->next;
    }
        if (q->front == NULL) 
            q->rear = NULL; 
        return target;
}
void processInputManual(char command []){
    
        N = atoi(strsep(&command, " "));       // NUmber of W threads   1 -- 10
        Bcount= atoi(strsep(&command, " "));  //number of bursts that each W thread will generate.
        minB = atoi(strsep(&command, " "));  
        avgB = atoi(strsep(&command, " "));  
        minA = atoi(strsep(&command, " "));  
        avgA = atoi(strsep(&command, " "));  
        ALG = command;
}

float exponential_dist(int a){
     srand((unsigned)time(NULL));
    double u;
    double lambda;
    lambda = (float)1 / a;
    u = rand() / (RAND_MAX +1.0);
    return -log(1-u) / lambda; 
}



void * producer(void * pid){

    // Random waiting time for each W thread.
    // For the sake of the experiments the randomized variable limited.
    sleep(rand() % 5);
   
    if ( flag == true){
        
        // For the implementation of the file reading however segmentation fault could not be solved therefore commented those parts.
        /*
        char* abc[10];
        char * target ;
        char * txtAppend= ".txt";
        char  charpid = (char)pid;
        
        target = malloc(strlen(fileName)+strlen(txtAppend)+2);

        strcat(target ,fileName);
        strcat(target,charpid);
        strcat(target,txtAppend);


        FILE* fp = fopen(fileName, "r");
        char buffer[100];
        printf("\nStarting execution of the commands in the file.\n");
        fgets(buffer, 100, fp) ;
        buffer[strcspn(buffer, "\n\r")] = '\0'; // remove the newline at the end
        char command[strlen(buffer)];
        
        parseCommand(command,abc);
        fclose(fp);
        
        Bcount = abc[0];
        avgB = abc[2];
        minA = abc[3];
        minB = abc[1];
        avgA = abc[4];
*/
    }
while(1){
    //Bcount the amount of bursts each thread generated.
   int bid = 0;
    for (int i = 0; i < Bcount; i++)
    {
        int length,wallClock;
        //Created burst
        int tmp = random() % 100;

        //Length each burst is random and exponentially distributed with mean avgB.
       
        
        length = (int)(exponential_dist(avgB)); 
           
        while(length < minB ){
           
            length = (int)exponential_dist(avgB); 
        
        }
    
        struct timeval current_time;
        gettimeofday(&current_time,NULL);
        wallClock = (int) current_time.tv_usec;

        pthread_mutex_lock(&mutexBuffer);
        // Enqueue cpu burst.
        int avgA_time = (int)exponential_dist(avgA);
        
        while(avgA_time < minA){  avgA_time = avgA_time = (int)exponential_dist(avgA); }

        enQueue(runquque, pid,bid,length,wallClock,avgA_time);
        
        pthread_mutex_unlock(&mutexBuffer);
        printf("\n(producer thread) thread index is :%d burst index is :%d length is(ms):%d wallClock is :%d, avgA time: %d \n",pid,bid,length,wallClock,avgA_time);
        bid++;
        // Waiting time between consective threads.
        
    
        sleep(avgA_time/1000); 
        pthread_cond_signal(&condition);
    }

   
   // pthread_cond_signal(&condition);
    pthread_exit(0);
}
}


// for s_thread which is going to consume the W threads productions.
void * consumer(void * param){
   
  while(1){
      
    // If the queue is empty then waits
    pthread_mutex_lock(&mutexBuffer);   
    if(isEmpty(runquque)){
        printf("Average waiting time of %s algorithm is %d",ALG,(averageWaitingTime / N*Bcount));
        printf("\n Queue is empty. \n");
        
        pthread_cond_wait(&condition, &mutexBuffer);  
    }    

    struct QNode * Tmp ;   
    if(strcmp(ALG , "FCFS") == 0){Tmp =  deQueue_FCFS(runquque);}
    if(strcmp(ALG , "SJF") == 0){Tmp =  deQueue_SJF(runquque);}
    if(strcmp(ALG , "PRIO") == 0){ Tmp =  deQueue_PRIO(runquque);}
    if(strcmp(ALG , "VRUNTIME") == 0){Tmp =  deQueue_VRUNTIME(runquque);}

    // Enqueue cpu burst.
    sleep(Tmp->length/100);     
    /*struct timeval current_time;
    gettimeofday(&current_time,NULL);
    int accurate_time = (int) current_time.tv_usec;
   */
    int accurate_time = ((int)(clock() - Tmp->local_clock) )/ CLOCKS_PER_SEC;
    averageWaitingTime += accurate_time ;
    pthread_mutex_unlock(&mutexBuffer);
    printf("\n**(consumer thread) thread index is :%d burst index is :%d length is(ms):%d wallClock is :%d avgA time : %d waiting time of burst : %d\n",Tmp->pid,Tmp->bid,Tmp->length,Tmp->wallClock,Tmp->avgA_time,accurate_time);     
    }
    pthread_exit(0);
}



void PthreadScheduler(){
    
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
int parseCommand(char command[],char * argv1[]) {
    int command_counter = 0;
    for (int i = 0; i < 7; i++) {
       
        argv1[i] = strsep(&command, " ");
        command_counter++;
        if (argv1[i] == NULL) {
            break;
        }
        
    }
    return command_counter;
}

int main(int argc, char const *argv[])
{
    char tmp[CommandSize];
    averageWaitingTime = 0;
     char* abc[100];
    while(1){

        printf("\nschedule: ");
        fgets(commands, CommandSize, stdin);
     
        // Replace line endings with string terminator.
        commands[strcspn(commands, "\n\r")] = '\0';      
      
        strcpy(tmp,commands);
        int test = parseCommand(tmp,abc);
       
       if( test != 4){
            processInputManual(commands);
        }else{
            N = atoi(abc[0]);
            ALG = abc[1];
            fileName = abc[3];
            flag = true;
        //    printf("N is --> %d ALG is  --> %s filename is  --> %s",N,ALG,fileName);
    }
    
    PthreadScheduler();
    
    }
 
    return 0;
}


