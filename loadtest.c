#include <threads.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

static int increment(void *val) {
  atomic_uintmax_t *valptr = val;
  while (true)
    // Increment (add one to) *val
    atomic_fetch_add_explicit(valptr, 1, memory_order_relaxed);
}

int main(void) {
  atomic_uintmax_t val;
  atomic_init(&val, 0);
  thrd_t incrementer;
  thrd_create(&incrementer, increment, &val);
  while (true) {
    uintmax_t a = 0;
    // Only the lr.d is important here
    atomic_compare_exchange_strong_explicit(&val, &a, 0, memory_order_relaxed, memory_order_relaxed);
    uintmax_t b = atomic_load_explicit(&val, memory_order_relaxed);
    if (a > b) {
      fprintf(stderr, "loads not monotonic, a: 0x%" PRIxMAX ", b: 0x%" PRIxMAX "\n", old, b);
      abort();
    }
  }
}
