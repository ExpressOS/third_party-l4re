/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-sparc", "elf32-sparc",
	      "elf32-sparc")
OUTPUT_ARCH(sparc)
ENTRY(_start)
SEARCH_DIR("=/usr/local/lib"); SEARCH_DIR("=/lib"); SEARCH_DIR("=/usr/lib");

PHDRS
{
  phdr PT_PHDR PHDRS;
  ro PT_LOAD FILEHDR PHDRS;
  rw PT_LOAD;
  L4_DEFINE_L4PHDRS
}

SECTIONS
{
  PROVIDE (__executable_start = 0x010000);
  . = __executable_start + SIZEOF_HEADERS;

  /* Read-only sections, merged into text segment: */
  /* PROVIDE (__executable_start = 0x10000000); . = 0x10000000 + SIZEOF_HEADERS;
   */
  /DISCARD/ :
  { *(.interp)
    *(.note.gnu.build-id)
    *(.hash)
    *(.gnu.hash)
    *(.dynsym)
    *(.dynstr)
    *(.gnu.version)
    *(.gnu.version_d)
    *(.gnu.version_r)
    *(.rel.init)
    *(.rela.init)
    *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*)
    *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
    *(.rel.fini)
    *(.rela.fini)
    *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*)
    *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
    *(.rel.data.rel.ro* .rel.gnu.linkonce.d.rel.ro.*)
    *(.rela.data.rel.ro* .rela.gnu.linkonce.d.rel.ro.*)
    *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*)
    *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
    *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*)
    *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*)
    *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*)
    *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*)
    *(.rel.ctors)
    *(.rela.ctors)
    *(.rel.dtors)
    *(.rela.dtors)
    *(.rel.got)
    *(.rela.got)
    *(.rela.got1)
    *(.rela.got2)
    *(.rel.sdata .rel.sdata.* .rel.gnu.linkonce.s.*)
    *(.rela.sdata .rela.sdata.* .rela.gnu.linkonce.s.*)
    *(.rel.sbss .rel.sbss.* .rel.gnu.linkonce.sb.*)
    *(.rela.sbss .rela.sbss.* .rela.gnu.linkonce.sb.*)
    *(.rel.sdata2 .rel.sdata2.* .rel.gnu.linkonce.s2.*)
    *(.rela.sdata2 .rela.sdata2.* .rela.gnu.linkonce.s2.*)
    *(.rel.sbss2 .rel.sbss2.* .rel.gnu.linkonce.sb2.*)
    *(.rela.sbss2 .rela.sbss2.* .rela.gnu.linkonce.sb2.*)
    *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*)
    *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*)
    *(.rel.plt)
    *(.rela.plt) }
  .text :
  {
    _stext = .;
    _program_img_start = _stext;
    *(.text .stub .text.* .gnu.linkonce.t.*)
    . = ALIGN (16);
    KEEP(*(.text.*personality*))
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.glink)
  } :ro = 0 
  .init            :
  {
    KEEP (*(.init))
  } :ro = 0 
  .fini           :
  {
    KEEP (*(.fini))
  } :ro = 0 
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  .rol4re_elf_aux : {
    KEEP (*(.rol4re_elf_aux))
  } : ro : l4re_aux
  _rodata_start = . ;
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) } : ro 
  .rodata1        : { *(.rodata1) } 
   _erodata = ALIGN( 0x10 ) ;
  .sdata2         :
  {
    PROVIDE (_SDA2_BASE_ = 32768);
    *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
  } 
  .sbss2          : { *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*) } 
  .eh_frame_hdr : { *(.eh_frame_hdr) } :ro 
  .eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) } 
  .gcc_except_table   : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) } 
  . = ALIGN(CONSTANT(MAXPAGESIZE));
  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = ALIGN (CONSTANT (MAXPAGESIZE)) - ((CONSTANT (MAXPAGESIZE) - .) & (CONSTANT (MAXPAGESIZE) - 1)); . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));

  PROVIDE(_sdata = .);
  /* Exception handling  */
  .eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) } :rw 
  .gcc_except_table   : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) } :rw 
  /* Thread Local Storage sections  */
  .tdata	  : { *(.tdata .tdata.* .gnu.linkonce.td.*) } :rw 
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } 
  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } 
  .init_array     :
  {
     PROVIDE_HIDDEN (__init_array_start = .);
     KEEP (*(SORT(.init_array.*)))
     KEEP (*(.init_array))
     PROVIDE_HIDDEN (__init_array_end = .);
  } 
  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } 
  /* . = ALIGN(0x1000); */

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } 
  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
    /* linux style initcalls constructors */
    __leonbare_initcall_start = .;  
        *(.initcall1.init) 
    	*(.initcall2.init) 
    	*(.initcall3.init) 
    	*(.initcall4.init) 
    	*(.initcall5.init) 
    	*(.initcall6.init) 
    	*(.initcall7.init) 
    __leonbare_initcall_end = .;    
    
  } 

  .jcr            : { KEEP (*(.jcr)) } 
  .data.rel.ro : { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro* .gnu.linkonce.d.rel.ro.*) }
  .got1           : { *(.got1) } 
  .got2           : { *(.got2) } 
  .dynamic        : { *(.dynamic) } 
  .got            : SPECIAL { *(.got) } 
  . = DATA_SEGMENT_RELRO_END (0, .);
  .plt            : SPECIAL { *(.plt) } 
  .data           :
  {
    *(.data .data.* .gnu.linkonce.d.*)
    KEEP (*(.gnu.linkonce.d.*personality*))
    SORT(CONSTRUCTORS)
  } 
  .data1          : { *(.data1) } 
  .got            : SPECIAL { *(.got) } 
  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */
  .sdata          :
  {
    PROVIDE (_SDA_BASE_ = 32768);
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  } 
  _edata = .; PROVIDE (edata = .);
  __bss_start = .;
  .sbss           :
  {
    PROVIDE (__sbss_start = .); PROVIDE (___sbss_start = .);
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    PROVIDE (__sbss_end = .); PROVIDE (___sbss_end = .);
  } 
  .plt            : SPECIAL { *(.plt) }
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.
      FIXME: Why do we need it? When there is no .bss section, we don't
      pad the .data section.  */
   . = ALIGN(. != 0 ? 32 / 8 : 1);
  } 
  . = ALIGN(32 / 8);
  . = ALIGN(32 / 8);
  _end = .; PROVIDE (end = .);
  . = DATA_SEGMENT_END (.);
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) } 
  .stabstr       0 : { *(.stabstr) } 
  .stab.excl     0 : { *(.stab.excl) } 
  .stab.exclstr  0 : { *(.stab.exclstr) } 
  .stab.index    0 : { *(.stab.index) } 
  .stab.indexstr 0 : { *(.stab.indexstr) } 
  .comment       0 : { *(.comment) } 
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) } 
  .line           0 : { *(.line) } 
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) } 
  .debug_sfnames  0 : { *(.debug_sfnames) } 
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) } 
  .debug_pubnames 0 : { *(.debug_pubnames) } 
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) } 
  .debug_abbrev   0 : { *(.debug_abbrev) } 
  .debug_line     0 : { *(.debug_line) } 
  .debug_frame    0 : { *(.debug_frame) } 
  .debug_str      0 : { *(.debug_str) } 
  .debug_loc      0 : { *(.debug_loc) } 
  .debug_macinfo  0 : { *(.debug_macinfo) } 
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) } 
  .debug_funcnames 0 : { *(.debug_funcnames) } 
  .debug_typenames 0 : { *(.debug_typenames) } 
  .debug_varnames  0 : { *(.debug_varnames) } 
  /* DWARF 3 */
  .debug_pubtypes 0 : { *(.debug_pubtypes) } 
  .debug_ranges   0 : { *(.debug_ranges) } 
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) } 
  /DISCARD/	: { *(.fixup) } 
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) } 
}
