// RUN: %clangxx_tsan -O1 %s -o %t && %deflake %run %t | FileCheck %s
#include "test.h"

int x;

void *Thread(void *a) {
  barrier_wait(&barrier);
  x = 1;
  return 0;
}

int main() {
  barrier_init(&barrier, 2);
  // On FreeBSD, the %p conversion specifier works as 0x%x and thus does not
  // match to the format used in the diagnotic message.
  fprintf(stderr, "addr2=0x%012lx\n", (unsigned long) &x);
  pthread_t t;
  pthread_create(&t, 0, Thread, 0);
  x = 0;
  barrier_wait(&barrier);
  pthread_join(t, 0);
}

// CHECK: addr2=[[ADDR2:0x[0-9,a-f]+]]
// CHECK: WARNING: ThreadSanitizer: data race
// CHECK: Location is global 'x' of size 4 at [[ADDR2]] ({{.*}}+0x{{[0-9,a-f]+}})

