/*
 * (c) 2008-2009 Alexander Warg <warg@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/cxx/ipc_server>
#include <l4/libkproxy/scheduler_svr>

#include "global.h"

class Sched_proxy :
  public L4::Server_object,
  public L4kproxy::Scheduler_svr,
  public L4kproxy::Scheduler_interface
{
public:
  Sched_proxy();

  int info(l4_umword_t *cpu_max, l4_sched_cpu_set_t *cpus);

  int run_thread(L4::Cap<L4::Thread> thread, l4_sched_param_t const &sp);

  int idle_time(l4_sched_cpu_set_t const &cpus);

  int dispatch(l4_umword_t o, L4::Ipc::Iostream &ios)
  { return scheduler_dispatch(o, ios); }

  void set_prio(unsigned offs, unsigned limit)
  { _prio_offset = offs; _prio_limit = limit; }

  L4::Cap<L4::Thread> received_thread(L4::Ipc::Snd_fpage const &fp);

  void restrict_cpus(l4_umword_t cpus);

private:
  l4_sched_cpu_set_t _cpus;
  unsigned _max_cpus;

  unsigned _prio_offset, _prio_limit;


};

