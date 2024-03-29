/*
 * (c) 2008-2011 Adam Lackorzynski <adam@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#include "uart_imx.h"

namespace L4
{
  enum {
    URXD  = 0x00,   // Receiver
    UTXD  = 0x40,   // Transmitter
    UCR1  = 0x80,   // Control 1
    UCR2  = 0x84,   // Control 2
    UCR3  = 0x88,   // Control 3
    UCR4  = 0x8c,   // Control 4
    UFCR  = 0x90,   // FIFO Control
    USR1  = 0x94,   // Status 1
    USR2  = 0x98,   // Status 2
    UESC  = 0x9c,   // Escape Charater
    UTIM  = 0xa0,   // Escape Timer
    UBIR  = 0xa4,   // BRM Incremental Registers
    UBMR  = 0xa8,   // BRM Modulator Registers
    UBRC  = 0xac,   // Baud Rate Count
    ONEMS = 0xb0,   // One millisecond
    UTS   = 0xb4,   // Test


    UCR1_EN        = 1  <<  0, // Enable UART

    UCR2_SRST      = 1  <<  0, // Software Reset
    UCR2_RXEN      = 1  <<  1, // Receiver Enable
    UCR2_TXEN      = 1  <<  2, // Transmitter Enable
    UCR2_WS        = 1  <<  5, // 8-bit character length
    UCR2_STPB      = 1  <<  6, // 0 = 1 Stop bit, 1 = 2 stop bits
    UCR2_PROE      = 1  <<  7, // 0 = even parity, 1 = odd parity
    UCR2_PREN      = 1  <<  8, // Parity enable
    UCR2_RTEC_MASK = 3  <<  9, // Request to Send Edge Control mask
    UCR2_RTEC_RISI = 0  <<  9, // Trigger IRQ on rising edge
    UCR2_RTEC_FALL = 1  <<  9, // Trigger IRQ on falling edge
    UCR2_RTEC_ANY  = 2  <<  9, // Trigger IRQ on any edge
    UCR2_ESCEN     = 1  << 11, // Escape enable
    UCR2_CTS       = 1  << 12, // Clear to Send: 0 = pin is high (inactive), 1 = pin is low (active)
    UCR2_CTSC      = 1  << 13, // CTS Pin Control: 0 = pin controlled by CTS bit, 1 = pin controlled by the receiver
    UCR2_IRTS      = 1  << 14, // Ignore RTS pin
    UCR2_ESCI      = 1  << 15, // Escape Sequence Interrupt Enable

    UCR3_ACIEN     = 1  <<  0, // Autobaud Counter Interrupt enable
    UCR3_INVT      = 1  <<  1, // Inverted Infrared Transmission
    UCR3_RXDMUXSEL = 1  <<  2, // RXD Muxed Input Selected: 0 = serial ist IPP_UART_RXD, IR is IPP_UART_RXD_IR, 1 = IPP_UART_RXD_MUX for both


    UCR4_DREN      = 1  <<  0, // Receive Data Ready Interrupt Enable
    UCR4_OREN      = 1  <<  1, // Receiver Overrun Interrupt enable
    UCR4_BKEN      = 1  <<  2, // BREAK Condition Dected Interrupt enable
    UCR4_TCEN      = 1  <<  3, // Transmit Complete Interrupt Enable
    UCR4_LPBYP     = 1  <<  4, // Low Power Bypass
    UCR4_IRSC      = 1  <<  5, // IR Special Case
    // Bit 6 is reserve, should be written as 0
    UCR4_WKEN      = 1  <<  7, // WAKE Interrupt Enable
    UCR4_ENIRI     = 1  <<  8, // Serial Infrared Interrupt Enable
    UCR4_INVR      = 1  <<  9, // Inverted Infrared Reception
    UCR4_CTSTL_32  = 32 << 10, // CTS Trigger Level

    UFCR_RXTL_MASK = 63 <<  0, // Receiver Trigger Level Mask
    UFCR_RXTL_1    = 1  <<  0, // Receiver Trigger Level: 1 char
    UFCR_RFDIV_2   = 4  <<  7, // Reference Frequency Divier: by 2
    UFCR_TXTL_MASK = 63 << 10, // Trasmitter Trigger Level: 8 chars
    UFCR_TXTL_8    = 8  << 10, // Trasmitter Trigger Level: 8 chars
    UFCR_TXTL_32   = 32 << 10, // Trasmitter Trigger Level: 32 chars

    USR1_TRDY      = 1  << 13, // Transmitter Ready

    USR2_RDR       = 1  <<  0, // Receive Data Ready
    USR2_ORE       = 1  <<  1, // Overrun Error
    USR2_BRCD      = 1  <<  2, // Break Condition Detected
    USR2_TXDC      = 1  <<  3, // Transmitter Complete
    USR2_TXFE      = 1  << 14, // Transmit Buffer FIFO Empty

  };

  unsigned long Uart_imx::rd(unsigned long reg) const
  { return *(volatile unsigned long *)(_base + reg); }
  void Uart_imx::wr(unsigned long reg, unsigned long val) const
  { *(volatile unsigned long *)(_base + reg) = val; }

  bool Uart_imx::startup(unsigned long base)
  {
    _base = base;

    // 115200Baud, 8n1
    switch (_type)
      {
      case Type_imx21:
        wr(UBIR, 0x0344);
        wr(UBMR, 0x270f);
        break;
      case Type_imx35:
        wr(UBIR, 0xf);
        wr(UBMR, 0x1b2);
        break;
      case Type_imx51:
        wr(UBIR, 0xf);
        wr(UBMR, 0x120);
        break;
      }

    wr(UCR1, UCR1_EN);
    wr(UCR2, UCR2_SRST | UCR2_RXEN | UCR2_TXEN | UCR2_WS | UCR2_IRTS); // note: no IRQs enabled
    wr(UCR3, UCR3_RXDMUXSEL);
    wr(UCR4, UCR4_CTSTL_32);
    wr(UFCR, UFCR_TXTL_8 | UFCR_RFDIV_2 | UFCR_RXTL_1);

    return true;
  }

  void Uart_imx::shutdown()
  {
    wr(UCR1, 0); // Disable UART
  }

  bool Uart_imx::enable_rx_irq(bool enable)
  {
    if (enable)
      {
	wr(UCR2, rd(UCR2) | UCR2_RTEC_ANY);
	wr(UCR4, rd(UCR4) | UCR4_DREN);
      }
    else
      wr(UCR4, rd(UCR4) & ~UCR4_DREN);

    return true;
  }
  bool Uart_imx::enable_tx_irq(bool /*enable*/) { return false; }
  bool Uart_imx::change_mode(Transfer_mode, Baud_rate)
  { return true; }

  int Uart_imx::get_char(bool blocking) const
  {
    while (!char_avail())
      if (!blocking) return -1;

    return rd(URXD) & 0xff;
  }

  int Uart_imx::char_avail() const
  {
    return rd(USR2) & USR2_RDR;
  }

  void Uart_imx::out_char(char c) const
  {
    while (!(rd(USR1) & USR1_TRDY))
     ;
    wr(UTXD, c);
  }

  int Uart_imx::write(char const *s, unsigned long count) const
  {
    unsigned long c = count;
    while (c)
    {
      if (*s == 10)
	out_char(13);
      out_char(*s++);
      --c;
    }
    while (!(rd(USR2) & USR2_TXDC))
      ;

    return count;
  }
};
