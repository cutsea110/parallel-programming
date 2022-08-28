#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM 3
#define NUM_THREADS 100

void semaphore_aqcuire(volatile int *cnt) {
  for (;;) {
    while (*cnt >= NUM);
    __sync_fetch_and_add(cnt, 1);
    if (*cnt <= NUM)
      break;
    __sync_fetch_and_sub(cnt, 1);
  }
}

void semaphore_release(int *cnt) {
  __sync_fetch_and_sub(cnt, 1);
}

// 共有変数
// NOTE: 保証されているのは NUM より多くのスレッドがクリティカルセクションには入らないこと
//       cnt が NUM を超えることは semaphore_aqcuire の実装上ありえる
int cnt = 0;

void *some_func(void *arg) {
  int id = (int)arg;
  semaphore_aqcuire(&cnt);
  printf("critical section [id = %d, cnt = %d]\n", id, cnt);
  sleep(1);
  semaphore_release(&cnt);

  return arg;
}

int main(int argc, char *argv[]) {
  pthread_t v[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    if (pthread_create(&v[i], NULL, some_func, (void *)i) != 0) {
      perror("pthread_create");
      return -1;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    char *ptr;
    if (pthread_join(v[i], (void **)&ptr) == 0) {
      int id = (int *)ptr;
      printf("msg = %d\n", id);
    } else {
      perror("pthread_create");
      return -1;
    }
  }

  return 0;
}
