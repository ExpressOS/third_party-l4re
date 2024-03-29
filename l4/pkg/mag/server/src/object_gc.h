// vi:ft=cpp
/*
 * (c) 2010 Alexander Warg <warg@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */

#pragma once

#include <l4/mag/server/object>

namespace Mag_server {

class Object_gc
{
protected:
  Object *_life;
  Object *_sweep;

public:
  void gc_sweep();
  void gc_step();
  void add_obj(Object *o) { o->enqueue(&_life); }
};

}
