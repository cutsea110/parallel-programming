#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// CAS (atomic)
bool compare_and_swap(uint64_t *p, uint64_t val, uint64_t newval) {
  return __sync_bool_compare_and_swap(p, val, newval);
}

// TAS
bool test_and_set(volatile bool *p) {
  return __sync_lock_test_and_set(p, 1);
}
void tas_release(volatile bool *p) {
  return __sync_lock_release(p);
}


bool lock = false; // 共有変数

void some_func() {
 retry:
  if (!test_and_set(&lock)) { // 検査とロック獲得
    printf("critical section\n");
  } else {
    goto retry;
  }
  tas_release(&lock); // ロック開放
}


int main(int argc, char *argv[]) {
  some_func();
}
