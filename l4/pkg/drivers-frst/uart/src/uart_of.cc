/*
 * (c) 2009 Adam Lackorzynski <adam@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#include "uart_of.h"
#include <stdio.h>

namespace L4 {

  bool Uart_of::startup(unsigned long /*base*/)
  {
    char path[64], type[16];
    for(phandle_t node = 0; prom_next_node(&node); ) {
        prom_getprop(node, "device_type", type, sizeof(type));

        if(strcmp(type, "serial"))
          continue;

        if(prom_call("package-to-path", 3, 1, node, path, sizeof(path)) == Of::PROM_ERROR) 
          return false;

        /* open port */
        if((_serial = (ihandle_t)prom_call("open", 1, 1, path)) <= 0)
          return false;

        break;
    }
    //prom_call("exit", 0, 0);
    return (!_serial)?false:true;
  }

  void Uart_of::shutdown()
  {
    prom_call("close", 1, 0, _serial);
  }

  int Uart_of::get_char(bool blocking) const
  {
    int c, len = 0;

    while(len != 1 && blocking)
      len = prom_call("read", 3, 1, _serial, &c, 1);

    return (len)?c:-1;
  }

  int Uart_of::write(char const *s, unsigned long count) const
  {
    return prom_call("write", 3, 1, _serial, s, count);
  }

  void Uart_of::out_char(char c) const
  {
    prom_call("write", 3, 0, _serial, c, 1);
  }

  /* UNIMPLEMENTED */
  bool Uart_of::enable_rx_irq(bool){return true;}
  bool Uart_of::enable_tx_irq(bool){return true;}
  bool Uart_of::change_mode(Transfer_mode, Baud_rate){return true;}
  int  Uart_of::char_avail() const {return 1;}
};
