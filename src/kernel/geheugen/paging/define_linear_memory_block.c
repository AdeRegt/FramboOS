#include "geheugen.h"

void define_linear_memory_block(void *address,uint8_t with_cache){
  map_memory(master_page_table,address,address,0, with_cache);
}