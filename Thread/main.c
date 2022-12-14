#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct agr {
    float time;
    char c;
};

void * thread(void *arg)
{
    struct agr *a = (struct agr *)arg;
    float time = a->time;
    char c = a->c;
    if (time && c) {
    while(1) {
	printf("%c\n", c);
	usleep(time*1000000);
    }
    }
}

int main(int argc, char** agrv){
    pthread_t t1_id, t2_id, t3_id;
    struct agr a1 = {0}, a2 = {0}, a3 = {0};
    a1.time = 0.2;
    a1.c = 'A';
    pthread_create(&t1_id, NULL, &thread, &a1);
    a2.time = 0.5;
    a2.c = 'B';
    pthread_create(&t2_id, NULL, &thread, &a2);
    a3.time = 1;
    a3.c = 'C';
    pthread_create(&t3_id, NULL, &thread, &a3);

    pthread_join(t1_id, NULL);
    pthread_join(t2_id, NULL);
    pthread_join(t3_id, NULL);
    return 0;
}
