#include <pthread.h>
#define blocksize 64


int threads = 32;
int *A, *B;
int siz;

struct thread_details{
  int threadid;
  int* arr;
};


void* thread_func(void* arg){

 struct thread_details *args = (struct thread_details*) arg;
 int N = siz;
 (args->arr) = (int*)malloc((2*N - 1)*sizeof(int));
 int i = (args->threadid) * blocksize;

 for(int j = 0; j < N; j +=blocksize){
   for(int k = i; k<i+blocksize; k++){
     for(int l = j; l<j+blocksize; l++){
         *((args->arr)+k+l) += A[k * N + l] * B[(l)*N + N-1-k];
     }
   }
 }

 pthread_exit(0);
}
void multiThread(int N, int *matA, int *matB, int *output)
{
   siz = N;
   A = matA;
   B = matB;
   threads = N / blocksize;

   struct thread_details args[threads];
   pthread_t tids[threads];
   for(int i = 0; i < threads; i++){
      args[i].threadid = i;
      pthread_create(&tids[i],NULL,thread_func,&args[i]);
   }
   for(int i = 0; i<threads;i++){
    pthread_join(tids[i],NULL);
     for(int j = 0;j<2*N-1;j++){
        output[j] += args[i].arr[j];
      }
   }
}
