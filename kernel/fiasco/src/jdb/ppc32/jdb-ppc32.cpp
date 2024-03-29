IMPLEMENTATION [ppc32]:

#include "globals.h"
#include "kmem_alloc.h"
#include "space.h"
#include "mem_layout.h"
#include "mem_unit.h"
#include "static_init.h"


STATIC_INITIALIZE_P(Jdb, JDB_INIT_PRIO);

static Per_cpu<Proc::Status> DEFINE_PER_CPU jdb_irq_state;

// disable interrupts before entering the kernel debugger
IMPLEMENT
void
Jdb::save_disable_irqs(unsigned cpu)
{
  jdb_irq_state.cpu(cpu) = Proc::cli_save();
}

// restore interrupts after leaving the kernel debugger
IMPLEMENT
void
Jdb::restore_irqs(unsigned cpu)
{
  Proc::sti_restore(jdb_irq_state.cpu(cpu));
}

IMPLEMENT inline
void
Jdb::enter_trap_handler(unsigned /*cpu*/)
{}

IMPLEMENT inline
void
Jdb::leave_trap_handler(unsigned /*cpu*/)
{}

IMPLEMENT inline
bool
Jdb::handle_conditional_breakpoint(unsigned /*cpu*/)
{ return false; }

IMPLEMENT
void
Jdb::handle_nested_trap(Jdb_entry_frame *e)
{
  printf("Trap in JDB: IP:%08lx SRR1=%08lx\n",
         e->ip(), e->srr1);
}

IMPLEMENT
bool
Jdb::handle_debug_traps(unsigned cpu)
{
  Jdb_entry_frame *ef = entry_frame.cpu(cpu);
  snprintf(error_buffer.cpu(cpu), sizeof(error_buffer.cpu(0)), "%s",
           (char const *)ef->r[2]);

  return true;
}

IMPLEMENT inline
bool
Jdb::handle_user_request(unsigned cpu)
{
  Jdb_entry_frame *ef = Jdb::entry_frame.cpu(cpu);
  const char *str = (char const *)ef->r[2];
  Space * task = get_task(cpu);
  char tmp;

  if (!peek(str, task, tmp) || tmp != '*')
    return false;
  if (!peek(str+1, task, tmp) || tmp != '#')
    return false;

  return execute_command_ni(task, str+2);
}

IMPLEMENT inline
bool
Jdb::test_checksums()
{ return true; }

static
bool
Jdb::handle_special_cmds(int)
{ return 1; }

PUBLIC static
FIASCO_INIT FIASCO_NOINLINE void
Jdb::init()
{
  static Jdb_handler enter(at_jdb_enter);
  static Jdb_handler leave(at_jdb_leave);

  Jdb::jdb_enter.add(&enter);
  Jdb::jdb_leave.add(&leave);

  Thread::nested_trap_handler = (Trap_state::Handler)enter_jdb;
  Kconsole::console()->register_console(push_cons());
}


PRIVATE static
void *
Jdb::access_mem_task(Address virt, Space * task)
{
  // align

  virt &= ~0x03;

  Address phys;

  if (!task)
    {
      if (Mem_layout::in_kernel(virt))
	{
	  phys = Kmem::virt_to_phys((void *)virt);
	  if (phys == (Address)-1)
	    return 0;

	}
      else
	phys = virt;
    }
  else
    {
      phys = Address(task->mem_space()->virt_to_phys(virt));

      if(phys = (Address)-1)
	phys = task->mem_space()->virt_to_phys_s0((void *)virt);

      if (phys == (Address)-1)
	return 0;
    }

   return (Mword*)phys;
}

PUBLIC static
Space *
Jdb::translate_task(Address addr, Space * task)
{
//  return (Kmem::is_kmem_page_fault(addr, 0)) ? 0 : task;

  return task;
}

PUBLIC static
int
Jdb::peek_task(Address virt, Space * task, void *value, int width)
{

  void const *mem = access_mem_task(virt, task);
  if (!mem)
    return -1;

  switch (width)
    {
    case 1:
        {
          Mword dealign = (virt & 0x3) * 8;
          *(Mword*)value = (*(Mword*)mem & (0xff << dealign)) >> dealign;
          break;
        }
    case 2:
        {
          Mword dealign = ((virt & 0x2) >> 1) * 16;
          *(Mword*)value = (*(Mword*)mem & (0xffff << dealign)) >> dealign;
          break;
        }
    case 4:
      memcpy(value, mem, width);
    }

  return 0;
}

PUBLIC static
int
Jdb::is_adapter_memory(Address, Space *)
{
  return 0;
}

PUBLIC static
int
Jdb::poke_task(Address virt, Space * task, void const *val, int width)
{
  /*
  void *mem = access_mem_task(virt, task);
  if (!mem)
    return -1;

  memcpy(mem, val, width);*/
  return 0;
}


PRIVATE static
void
Jdb::at_jdb_enter()
{
//  Mem_unit::clean_vdcache();
}

PRIVATE static
void
Jdb::at_jdb_leave()
{
//  Mem_unit::flush_vcache();
}

PUBLIC static inline
void
Jdb::enter_getchar()
{}

PUBLIC static inline
void
Jdb::leave_getchar()
{}

PUBLIC static
void
Jdb::write_tsc_s(Signed64 /*tsc*/, char * /*buf*/, int /*maxlen*/, bool /*sign*/)
{}

PUBLIC static
void
Jdb::write_tsc(Signed64 /*tsc*/, char * /*buf*/, int /*maxlen*/, bool /*sign*/)
{}

PROTECTED static inline
template< typename T >
void
Jdb::set_monitored_address(T *dest, T val)
{
  *dest = val;
}
