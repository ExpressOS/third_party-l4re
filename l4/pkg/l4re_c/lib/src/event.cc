/*
 * (c) 2008-2009 Adam Lackorzynski <adam@os.inf.tu-dresden.de>,
 *               Alexander Warg <warg@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 */
#include <l4/re/env>
#include <l4/re/event>
#include <l4/re/c/event.h>

L4_CV long
l4re_event_get(const l4_cap_idx_t server,
               const l4_cap_idx_t ds) L4_NOTHROW
{
  L4::Cap<L4Re::Event>     x(server);
  L4::Cap<L4Re::Dataspace> _ds(ds);
  return x->get_buffer(_ds);
}
