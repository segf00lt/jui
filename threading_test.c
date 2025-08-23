#include "base.h"

#include <pthread.h>


OS_handle thread_ids[2];

func void* do_a_thing(void *arg) {
  u64 i = 0;
  pthread_t id = pthread_self();

  if(pthread_equal(id, ((OS_linux_entity*)thread_ids[0].hand[0])->e.thread.handle)) {
    printf("hello there I am the first thread\n");
  } else {
    printf("PLING PLING PLONG\n");
  }

  for(i = 0; i < (0xffffffff); i++);

  return 0;
}

int main(void) {
  void *os_state = os_state_alloc();
  os_state_select(os_state);

  printf("hello %p\n", os_state);


  for(int i = 0; i < 2; i++) {
    thread_ids[i]  = os_thread_launch((OS_thread_func*)&do_a_thing, 0, 0);
    printf("thread succesfully created\n");
  }

  os_thread_join(thread_ids[0], MAX_U64);
  os_thread_join(thread_ids[1], MAX_U64);

  return 0;
}
