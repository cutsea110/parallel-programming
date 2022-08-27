#include <stdio.h>

#define NUM 10

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

int cnt = 0; // 共有変数

void some_func() {
  for (;;) {
    semaphore_aqcuire(&cnt);
    printf("some task\n");
    semaphore_release(&cnt);
  }
}

int main(int argc, char *argv[]) {
  some_func();
}
