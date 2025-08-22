#!/usr/bin/env python3

# Realtek 2023.2.17
# This script is used to modify the start and end positions of DSP Link support package and MPU tables.
# Para: first parameter is start address, second parameter is end address.
# Example cmd: python lsp_modify.py 0x60300000 0x61000000
# Please add xtensa tool bin folder into PATH 
# (PATH Example: /opt/xtensa/XtDevTools/install/tools/RI-2021.8-linux/XtensaTools/bin/, 
#            or: C:\usr\xtensa\XtDevTools\install\tools\RI-2021.8-win32\XtensaTools\bin)

import re
import subprocess
import sys
from pathlib import Path
from shutil import copyfile

start_addr = 0
end_addr = 0
lsp_folder = Path("../../project/RTK_LSP/RI-2021.8/HIFI5_PROD_1123_asic_UPG/RTK_LSP")
mpu_table_lsp_folder = Path("../../project/RTK_LSP/RI-2021.8/HIFI5_PROD_1123_asic_UPG/RTK_LSP")
mpu_table_project_folder = Path("../../project/project_dsp")


def print_old_lsp():
    xmm_old_file = open(lsp_folder / "memmap.xmm")
    memmap_xld_lines = xmm_old_file.readlines()
    for line in memmap_xld_lines:
        if "sysram : extra_reset_mem" in line:
            res = re.findall(r'(0x.*?):', line.replace(" ", ""))
            break
    psram_start_old = int(res[0], 16) - 0x1000
    size_old = int(res[1], 16) + 0x1000
    print("Current LSP psram: Start Address: " + hex(psram_start_old) + ", End Address: " + hex(psram_start_old + size_old) + ", Size: " + hex(size_old))
        

def input_check():
    global start_addr, end_addr
    if(len(sys.argv) != 3):
        print("Invalid input. Please enter the start and end addresses in hex. Example: \"python lsp_modify.py 0x60300000 0x61000000\" ")
        return 1
    start_addr_str = sys.argv[1]
    end_addr_str = sys.argv[2]
    start_addr = int(start_addr_str, 16)
    end_addr = int(end_addr_str, 16)
    if (start_addr % 0x1000) != 0 or (end_addr % 0x1000) != 0:
        print("Invalid start or end address. Start and end addresses need 4KByte alignment.")
        return 1
    if start_addr <= 0x60000000 or end_addr > 0x61000000 or end_addr <= start_addr:
        print("Invalid start or end address.")
        return 1
    print("New LSP psram: Start Address: " + hex(start_addr) + ", End Address: " + hex(end_addr) + ", Size: " + hex(end_addr - start_addr))
    return 0
    

def modify_files():
    # save old files
    try:
        copyfile(lsp_folder / "memmap.xld", lsp_folder / "memmap.xld.old")
        copyfile(lsp_folder / "memmap.xmm", lsp_folder / "memmap.xmm.old")
    except IOError as e:
        print("Unable to copy old file. %s" % e)
        exit(1)
    except:
        print("Unexpected error:", sys.exc_info())
        exit(1)

    # memmap.xld
    memmap_xld_file = open(lsp_folder / "memmap.xld")
    memmap_xld_lines = memmap_xld_file.readlines()
    for i in range(len(memmap_xld_lines)):
        if memmap_xld_lines[i].startswith("- { construct: memory"):
            memmap_xld_lines[i] = "- { construct: memory, name: extra_reset_mem, description: extra_reset_mem, space: physical, startaddr: " \
                                  + hex(start_addr + 0x1000) + ", size: " + hex(end_addr - start_addr - 0x1000) + ", attrs: { device: 0, writable: 1 }, }"
    with open((lsp_folder / "memmap.xld"), mode = 'w') as memmap_xld_file_new: 
         memmap_xld_file_new.writelines(memmap_xld_lines)
    
    # memmap.xmm
    memmap_xmm_file = open(lsp_folder / "memmap.xmm")
    memmap_xmm_lines = memmap_xmm_file.readlines()
    for i in range(len(memmap_xmm_lines)):
        if memmap_xmm_lines[i].startswith("VECBASE="):
            memmap_xmm_lines[i] = "VECBASE=" + hex(start_addr + 0x1400) + "\n"
        elif memmap_xmm_lines[i].startswith("VECRESET="):
            memmap_xmm_lines[i] = "VECRESET=" + hex(start_addr + 0x1000) + "\n"
            
        elif "sysram : extra_reset_mem" in memmap_xmm_lines[i]:
            memmap_xmm_lines[i] = hex(start_addr + 0x1000) + ": sysram : extra_reset_mem : " + hex(end_addr - start_addr - 0x1000) + " : executable, writable ;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem0 :"):
            memmap_xmm_lines[i] = " extra_reset_mem0 : F : " + hex(start_addr + 0x1000) + " - " + hex(start_addr + 0x12df) + " : .ResetVector.text .ResetHandler.literal .ResetHandler.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem1 :"):
            memmap_xmm_lines[i] = " extra_reset_mem1 : C : " + hex(start_addr + 0x12e0) + " - " + hex(start_addr + 0x13ff) + " : ;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem2 :"):
            memmap_xmm_lines[i] = " extra_reset_mem2 : F : " + hex(start_addr + 0x1400) + " - " + hex(start_addr + 0x1577) + " : .WindowVectors.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem3 :"):
            memmap_xmm_lines[i] = " extra_reset_mem3 : C : " + hex(start_addr + 0x1578) + " - " + hex(start_addr + 0x157b) + " : .Level2InterruptVector.literal;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem4 :"):
            memmap_xmm_lines[i] = " extra_reset_mem4 : F : " + hex(start_addr + 0x157c) + " - " + hex(start_addr + 0x1597) + " : .Level2InterruptVector.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem5 :"):
            memmap_xmm_lines[i] = " extra_reset_mem5 : C : " + hex(start_addr + 0x1598) + " - " + hex(start_addr + 0x159f) + " : .Level3InterruptVector.literal;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem6 :"):
            memmap_xmm_lines[i] = " extra_reset_mem6 : F : " + hex(start_addr + 0x15a0) + " - " + hex(start_addr + 0x15d7) + " : .Level3InterruptVector.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem7 :"):
            memmap_xmm_lines[i] = " extra_reset_mem7 : C : " + hex(start_addr + 0x15d8) + " - " + hex(start_addr + 0x15df) + " : .Level4InterruptVector.literal;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem8 :"):
            memmap_xmm_lines[i] = " extra_reset_mem8 : F : " + hex(start_addr + 0x15e0) + " - " + hex(start_addr + 0x1617) + " : .Level4InterruptVector.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem9 :"):
            memmap_xmm_lines[i] = " extra_reset_mem9 : C : " + hex(start_addr + 0x1618) + " - " + hex(start_addr + 0x161f) + " : .DebugExceptionVector.literal;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem10 :"):
            memmap_xmm_lines[i] = " extra_reset_mem10 : F : " + hex(start_addr + 0x1620) + " - " + hex(start_addr + 0x1657) + " : .DebugExceptionVector.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem11 :"):
            memmap_xmm_lines[i] = " extra_reset_mem11 : C : " + hex(start_addr + 0x1658) + " - " + hex(start_addr + 0x165f) + " : .NMIExceptionVector.literal;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem12 :"):
            memmap_xmm_lines[i] = " extra_reset_mem12 : F : " + hex(start_addr + 0x1660) + " - " + hex(start_addr + 0x1697) + " : .NMIExceptionVector.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem13 :"):
            memmap_xmm_lines[i] = " extra_reset_mem13 : C : " + hex(start_addr + 0x1698) + " - " + hex(start_addr + 0x169b) + " : .KernelExceptionVector.literal;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem14 :"):
            memmap_xmm_lines[i] = " extra_reset_mem14 : F : " + hex(start_addr + 0x169c) + " - " + hex(start_addr + 0x16b7) + " : .KernelExceptionVector.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem15 :"):
            memmap_xmm_lines[i] = " extra_reset_mem15 : C : " + hex(start_addr + 0x16b8) + " - " + hex(start_addr + 0x16bb) + " : .UserExceptionVector.literal;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem16 :"):
            memmap_xmm_lines[i] = " extra_reset_mem16 : F : " + hex(start_addr + 0x16bc) + " - " + hex(start_addr + 0x16d7) + " : .UserExceptionVector.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem17 :"):
            memmap_xmm_lines[i] = " extra_reset_mem17 : C : " + hex(start_addr + 0x16d8) + " - " + hex(start_addr + 0x16db) + " : .DoubleExceptionVector.literal;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem18 :"):
            memmap_xmm_lines[i] = " extra_reset_mem18 : F : " + hex(start_addr + 0x16dc) + " - " + hex(start_addr + 0x16f7) + " : .DoubleExceptionVector.text;\n"
        elif memmap_xmm_lines[i].startswith(" extra_reset_mem19 :"):
            memmap_xmm_lines[i] = " extra_reset_mem19 : C : " + hex(start_addr + 0x16f8) + " - " + hex(start_addr + 0x1af7) + \
                                " : .extra_reset_mem.rodata .extra_reset_mem.literal .extra_reset_mem.text .extra_reset_mem.data .extra_reset_mem.bss;\n"
        elif memmap_xmm_lines[i].startswith(" psram0 : C : "):
            memmap_xmm_lines[i] = " psram0 : C : " + hex(start_addr + 0xbaf8) + " - " + hex(end_addr - 0x1) + \
                " : .ipc_table .command .sram.rodata .clib.rodata .rtos.rodata .rodata .sram.literal .literal .rtos.literal .clib.literal .sram.text .text .clib.text " \
                ".rtos.text .clib.data .clib.percpu.data .rtos.percpu.data .rtos.data .sram.data .data __llvm_prf_names .clib.bss .clib.percpu.bss .rtos.percpu.bss .rtos.bss .sram.bss .bss;\n"
        elif memmap_xmm_lines[i].startswith(" reset_stack : C : "):
            memmap_xmm_lines[i] = " reset_stack : C : " + hex(start_addr + 0x1af8) + " - " + hex(start_addr + 0xbaf7) + " :  STACK :  HEAP : ;\n"

        elif " sysram : entry_table : " in memmap_xmm_lines[i]:
            memmap_xmm_lines[i] = hex(start_addr + 0x20) + ": sysram : entry_table : 0x20 : writable, executable ;\n"
        elif " entry_table_0 : F : " in memmap_xmm_lines[i]:
            memmap_xmm_lines[i] = " entry_table_0 : F : " + hex(start_addr + 0x20) + " - " + hex(start_addr + 0x3f) + " : .entry_table .entry_table.data .entry_table.bss;\n"
    with open((lsp_folder / "memmap.xmm"), mode = 'w') as memmap_xmm_file_new: 
         memmap_xmm_file_new.writelines(memmap_xmm_lines)
    return 0


def generate_link_script():
        ret = subprocess.run("xt-genldscripts " + "--xtensa-core=HIFI5_PROD_1123_asic_UPG " + " -b " + str(lsp_folder), shell = True, stderr = subprocess.STDOUT, check=False)
        if ret.returncode != 0:
            print("xt-genldscripts error. Please check if xtensa tool bin folder is in the PATH. \nPATH Example: \n" \
                  "Linux: /opt/xtensa/XtDevTools/install/tools/RI-2021.8-linux/XtensaTools/bin/, \n" \
                  "Windows: C:\\usr\\xtensa\\XtDevTools\\install\\tools\\RI-2021.8-win32\\XtensaTools\\bin")
            return 1


def edit_single_mpu_table(file_name):
    mpu_file = open(file_name)
    mpu_lines = mpu_file.readlines()
    for i in range(len(mpu_lines)):
        if mpu_lines[i].startswith("  XTHAL_MPU_ENTRY(0x6"):
            mpu_lines[i] = "  XTHAL_MPU_ENTRY(" + hex(start_addr + 0x1000) + ", 1, XTHAL_AR_RWXrwx, XTHAL_MEM_WRITEBACK), // extra_reset_mem\n"
            mpu_lines[i + 1] = "  XTHAL_MPU_ENTRY(" + hex(end_addr) + ", 1, XTHAL_AR_NONE, XTHAL_MEM_DEVICE), // unused\n"
            break
    with open((file_name), mode = 'w') as mpu_file_new: 
         mpu_file_new.writelines(mpu_lines)


def modify_mpu_table():
    # save old files
    try:
        copyfile(mpu_table_lsp_folder / "mpu_table.c", mpu_table_lsp_folder / "mpu_table.c.old")
        copyfile(mpu_table_project_folder / "mpu_table.c", mpu_table_project_folder / "mpu_table.c.old")
    except IOError as e:
        print("Unable to copy old file. %s" % e)
        exit(1)
    except:
        print("Unexpected error:", sys.exc_info())
        exit(1)
    # memmap.xld
    edit_single_mpu_table(mpu_table_lsp_folder / "mpu_table.c")
    edit_single_mpu_table(mpu_table_project_folder / "mpu_table.c")


if __name__ == "__main__":
    ret = print_old_lsp()
    ret = ret or input_check()
    ret = ret or modify_files()
    ret = ret or generate_link_script()
    ret = ret or modify_mpu_table()
    if ret:
        print("DSP link script change FAIL.")
    else:
        print("Please change MCU layout (amebalite_layout.ld) PSRAM_DSP_START to " + hex(start_addr))

