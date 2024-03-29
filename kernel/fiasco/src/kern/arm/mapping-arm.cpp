INTERFACE [arm]:

#include "types.h"
class Treemap;
class Space;

class Mapping_entry
{
public:
  enum { Alignment = 4 };
  union 
  {
    struct 
    {
      unsigned long _space:32;	///< Address-space number
/*      unsigned long _pad:1; */
      unsigned long address:20;	///< Virtual address in address space
      unsigned long tag:11;	        ///< Unmap tag
    } data;
    Treemap *_submap;
  };
  Unsigned8 _depth;
  void set_space(Space *s) { data._space = (unsigned long)s; }
  Space *space() const { return (Space *)data._space; }
};

