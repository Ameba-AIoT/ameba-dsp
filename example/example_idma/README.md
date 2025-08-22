# Example Description

This example move data from psram to DTCM. Fixed-Buffer Mode, One-dimensional (1D)

# HW Configuration

None

# SW Configuration

Currently, this example projects is not added to the project_dsp by default. Please refer to Application Note: Chapter 2.4 "Add Virtual Folder and Files".  

# Expected Result

Success log example:

```
idma fetch finished 
Data copy finished. 
idma transfer time: 88637 cycles, DATA_SIZE:65536 
dsp memcpy transfer time: 81018 cycles, DATA_SIZE:65536 
Firmware Disable
```

- If copy right, it will print "Data copy finished.", then print the iDMA copy time (CPU cycles) compare with memcopy time.
- If the destination data is different from the original data, it will print "Data copy error" and block task.
- If an error occurs during iDMA transmission, print "ERROR CALLBACK: iDMA in Error".

# Note
before using DMA, the data in cache should be flushed into RAM.
please refer to:
- Xplorer Doc: sys_sw_rm.pdf (Xtensa® System Software): 8. The Integrated DMA Library API
 
In interrupt handler, you can not use printf (printf destroys CPU interrupt state). In stead, you should use DiagPrintf.

# Supported IC

* AMEBALITE (with DSP core)


