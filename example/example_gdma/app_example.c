#include "ameba_soc.h"
#include "example_gdma_multi_block.h"
#include "example_gdma_single_block.h"

void app_example(void)
{
	DelayMs(100);
//	gdma_singleblock_transfer_test();  // single block
	gdma_multiblock_transfer_test();  //multi block
}
