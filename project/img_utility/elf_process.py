#!/usr/bin/env python3.8
import os

file_path = os.path.dirname(__file__)
section_file = open(os.path.join(file_path, "bin_structure.txt"),'r')

#skip 7 unrelated lines
for i in range(7):
    section_file.readline()

info_line = section_file.readline()

info_array_flash = []
info_array_sram = []
info_array_psram = []
info_array_dtcm0 = []
info_array_dtcm1 = []

while(info_line != "\n"):
    info_line_array = info_line.split()
    if((info_line_array[0] == 'LOAD') and (int(info_line_array[4], 16) != 0)):
        info_tuple = (int(info_line_array[1], 16), int(info_line_array[2], 16), int(info_line_array[4], 16))
        if(int(info_line_array[2], 16) < 0xFFFFFFF):
            info_array_flash.append(info_tuple)
            
        if((int(info_line_array[2], 16) >= 0x1ffc0000) and int(info_line_array[2], 16) < 0x1ffe0000):
            info_array_dtcm0.append(info_tuple)

        if((int(info_line_array[2], 16) >= 0x1ffe0000) and int(info_line_array[2], 16) < 0x1fffffff):
            info_array_dtcm1.append(info_tuple)
            
        if((int(info_line_array[2], 16) >= 0x1FFFFFFF) and int(info_line_array[2], 16) < 0x28000000):
            info_array_sram.append(info_tuple)
            
        if(int(info_line_array[2], 16) >= 0x60000000):
            info_array_psram.append(info_tuple)

    info_line = section_file.readline()


bin_file = open(os.path.join(file_path, "image.elf"),'rb')

output_flash = open(os.path.join(file_path, "output_flash.bin"), 'wb')
output_sram = open(os.path.join(file_path, "output_sram.bin"), 'wb')
output_psram = open(os.path.join(file_path, "output_psram.bin"), 'wb')
output_dtcm0 = open(os.path.join(file_path, "output_dtcm0.bin"), 'wb')
output_dtcm1 = open(os.path.join(file_path, "output_dtcm1.bin"), 'wb')

elf_bin = bin_file.read()

flash_image_size = 0
sram_image_size = 0
psram_image_size = 0
dtcm0_image_size = 0
dtcm1_image_size = 0

info_array_flash_len = len(info_array_flash)
info_array_sram_len = len(info_array_sram)
info_array_psram_len = len(info_array_psram)
info_array_dtcm0_len = len(info_array_dtcm0)
info_array_dtcm1_len = len(info_array_dtcm1)

#processing image going to flash
for i in range(info_array_flash_len - 1):
    start_addr = info_array_flash[i][0]
    length =  info_array_flash[i][2]
    padding = info_array_flash[i+1][1] - info_array_flash[i][1] - length
    
    flash_image_size += (length + padding)
    
    output_flash.write(elf_bin[start_addr: start_addr + length])
    output_flash.write(bytes([0]*padding))

#last entry needs special care
if(info_array_flash_len >= 1):
    if(info_array_flash[info_array_flash_len - 1][0] != 0):
        start_addr = info_array_flash[info_array_flash_len - 1][0]
        length =  info_array_flash[info_array_flash_len - 1][2]
        output_flash.write(elf_bin[start_addr: start_addr + length])
        flash_image_size += length

#padding to align 16 bytes
align16_padding = (16 - flash_image_size%16)
if(align16_padding != 16):
    output_flash.write(bytes([0]*align16_padding))

output_flash.close()

#processing image going to sram
for i in range(info_array_sram_len - 1):
    start_addr = info_array_sram[i][0]
    length =  info_array_sram[i][2]
    padding = info_array_sram[i+1][1] - info_array_sram[i][1] - length
        
    sram_image_size += (length + padding)

    output_sram.write(elf_bin[start_addr: start_addr + length])
    output_sram.write(bytes([0]*padding))

#last entry needs special care
if(info_array_sram_len >= 1):
    if(info_array_sram[info_array_sram_len - 1][0] != 0):
        start_addr = info_array_sram[info_array_sram_len - 1][0]
        length =  info_array_sram[info_array_sram_len - 1][2]
        output_sram.write(elf_bin[start_addr: start_addr + length])
        sram_image_size += length
        
#padding to align 16 bytes
align16_padding = (16 - sram_image_size%16)
if(align16_padding != 16):
    output_sram.write(bytes([0]*align16_padding))

output_sram.close()

#processing image going to psram
for i in range(info_array_psram_len - 1):
    start_addr = info_array_psram[i][0]
    length =  info_array_psram[i][2]
    padding = info_array_psram[i+1][1] - info_array_psram[i][1] - length
    
    psram_image_size += (length + padding)

    output_psram.write(elf_bin[start_addr: start_addr + length])
    output_psram.write(bytes([0]*padding))

#last entry needs special care
if(info_array_psram_len >= 1):
    if(info_array_psram[info_array_psram_len - 1][0] != 0):
        start_addr = info_array_psram[info_array_psram_len - 1][0]
        length =  info_array_psram[info_array_psram_len - 1][2]
        output_psram.write(elf_bin[start_addr: start_addr + length])
        psram_image_size += length

#padding to align 16 bytes
align16_padding = (16 - psram_image_size%16)
if(align16_padding != 16):
    output_psram.write(bytes([0]*align16_padding))

output_psram.close()

#processing image going to dtcm0
for i in range(info_array_dtcm0_len - 1):
    start_addr = info_array_dtcm0[i][0]
    length =  info_array_dtcm0[i][2]
    padding = info_array_dtcm0[i+1][1] - info_array_dtcm0[i][1] - length
    
    dtcm0_image_size += (length + padding)

    output_dtcm0.write(elf_bin[start_addr: start_addr + length])
    output_dtcm0.write(bytes([0]*padding))

#last entry needs special care
if(info_array_dtcm0_len >= 1):
    if(info_array_dtcm0[info_array_dtcm0_len - 1][0] != 0):
        start_addr = info_array_dtcm0[info_array_dtcm0_len - 1][0]
        length =  info_array_dtcm0[info_array_dtcm0_len - 1][2]
        output_dtcm0.write(elf_bin[start_addr: start_addr + length])
        dtcm0_image_size += length

#padding to align 16 bytes
align16_padding = (16 - dtcm0_image_size%16)
if(align16_padding != 16):
    output_dtcm0.write(bytes([0]*align16_padding))

output_dtcm0.close()


#processing image going to dtcm1
for i in range(info_array_dtcm1_len - 1):
    start_addr = info_array_dtcm1[i][0]
    length =  info_array_dtcm1[i][2]
    padding = info_array_dtcm1[i+1][1] - info_array_dtcm1[i][1] - length
    
    dtcm1_image_size += (length + padding)

    output_dtcm1.write(elf_bin[start_addr: start_addr + length])
    output_dtcm1.write(bytes([0]*padding))

#last entry needs special care
if(info_array_dtcm1_len >= 1):
    if(info_array_dtcm1[info_array_dtcm1_len - 1][0] != 0):
        start_addr = info_array_dtcm1[info_array_dtcm1_len - 1][0]
        length =  info_array_dtcm1[info_array_dtcm1_len - 1][2]
        output_dtcm1.write(elf_bin[start_addr: start_addr + length])
        dtcm1_image_size += length

#padding to align 16 bytes
align16_padding = (16 - dtcm1_image_size%16)
if(align16_padding != 16):
    output_dtcm1.write(bytes([0]*align16_padding))

output_dtcm1.close()
