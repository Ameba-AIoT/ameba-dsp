# Example Description

This example describes how to use GDMA's single-block and multi-block transfer mode.

# HW configuration

None.

# SW configuration

Edit app_example.c to select single-block or multi-block transfer. 

**For multi-block example**:
In order to deal with different situation, modify GdmaType variable, and it can be set:
   - **ReloadDst**, transfer multiple blocks from the source address to the same block at the destination address.
   - **ReloadSrc**, transfer one block from the source address to multi blocks at the destination address.
   - **ReloadSrcDst**,  transfer multiple blocks from the source address to multi blocks at the destination address.
   - **SingleBlock**, unavailable.

# Expect result

**For single-block example**:

GDMA will display "DMA Copy Memory Compare OK!! " in serial port tool. Otherwise£¬output info
is "DMA Copy Memory Compare Err, xxx-th value1 value2";

**For multi-block example**:
GDMA will display "DMA data complete MaxMuliBlock:x " in serial port tool. Example:

```
gdma_multiblock_transfer_task: 
Src=0x6032b480 ==> Dst=0x6032b500, Len=128
ReloadDst Test
DMA Block 0 Transfer Complete

DMA Block 1 Transfer Complete

DMA Block 2 Transfer Complete

DMA Block 3 Transfer Complete

DMA Last Block Complete
DMA Data MaxMuliBlock = 4 
```

# Note

In interrupt handler, you can not use printf (printf destroys CPU interrupt state). In stead, you should use DiagPrintf.

For more GDMA information, please refer to:  
- *AN0900_RTL8726E_Application_Note*: Chapter-20 Direct Memory Access Controller (DMAC)  
- *UM0900_RTL8726E_User_Manual*: Chapter-20 Direct Memory Access Controller (DMAC)

# Supported IC

- AmebaLite








