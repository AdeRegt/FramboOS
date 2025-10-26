#include "geheugen.h"

void define_linear_memory_block(void *address){
  map_memory(master_page_table,address,address);
}