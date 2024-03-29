#pragma once

#ifdef L4_PTHREAD_USE_USEM
#include <l4/sys/semaphore>

typedef L4::K_semaphore Th_sem_cap;

inline int __alloc_thread_sem(pthread_descr th, L4::Cap<Th_sem_cap> const &c)
{
  int err = l4_error(L4Re::Env::env()->factory()->create_semaphore(c));
  if (err < 0)
    return err;

  l4_usem_init(0, &th->p_thsem);
  return err;
}

#else
#include <l4/sys/irq>

typedef L4::Irq Th_sem_cap;

inline int __alloc_thread_sem(pthread_descr th, L4::Cap<Th_sem_cap> const &c)
{
  int err = l4_error(L4Re::Env::env()->factory()->create_irq(c));
  if (err < 0)
    return err;

  err = l4_error(c->attach(16, L4::Cap<L4::Thread>(th->p_th_cap)));
  return err;
}

#endif



