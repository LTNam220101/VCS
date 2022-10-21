#include <pthread.h>
#include <stdio.h>

int x;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * thread()
{
    int i;
    for(i=0; i<1000; i++)
        x++;
}

void * thread2()
{
    int i;
    for(i=0; i<1000; i++){
        pthread_mutex_lock(&lock);
        x++;
        pthread_mutex_unlock(&lock);
    }
}

void main(){
    pthread_t t1_id, t2_id, t3_id;
    pthread_create(&t1_id, NULL, thread, NULL);
    pthread_create(&t2_id, NULL, thread, NULL);
    pthread_create(&t3_id, NULL, thread, NULL);

    pthread_join(t1_id, NULL);
    pthread_join(t2_id, NULL);
    pthread_join(t3_id, NULL);

    printf("%d\n", x);
    x = 0;

    pthread_create(&t1_id, NULL, thread2, NULL);
    pthread_create(&t2_id, NULL, thread2, NULL);
    pthread_create(&t3_id, NULL, thread2, NULL);

    pthread_join(t1_id, NULL);
    pthread_join(t2_id, NULL);
    pthread_join(t3_id, NULL);

    printf("%d", x);
}