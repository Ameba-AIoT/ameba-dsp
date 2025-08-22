#include "ameba_soc.h"
#include "example_idma_dtcm_inference.h"

void app_example(void)
{
	DelayMs(100); // delay for log not mixed
	example_idma_inference();
}
