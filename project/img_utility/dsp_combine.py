#!/usr/bin/env python3.8

import os
import sys
import re
import struct
import subprocess
import shutil
import platform

# Constant Variables
PATTERN_1 = [0x99, 0x99, 0x96, 0x96]
PATTERN_2 = [0x3F, 0xCC, 0x66, 0xFC]
RSVD = [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF]
IMG2SIGN = [0x38, 0x31, 0x39, 0x35, 0x38, 0x37, 0x31, 0x31]

#point the initial address to the last possible address
sram_start = 0x20080000
psram_start = 0x61000000
flash_start = 0x0D000020
#no need to find the start address of dtcm0/1, since they are fixed address
dtcm0_start_fixed = 0x1ffc0000
dtcm1_start_fixed = 0x1ffe0000

file_path = os.path.dirname(os.path.abspath(__file__))

image_generate_method = "no_merge"



km4_path = "../../../sdk/project/realtek_amebaLite_va0_example/GCC-RELEASE/project_km4/asdk/image/"
kr4_path = "../../../sdk/project/realtek_amebaLite_va0_example/GCC-RELEASE/project_kr4/vsdk/image/"
manifest_img2_path = "../../../sdk/project/realtek_amebaLite_va0_example/GCC-RELEASE/project_km4/asdk/gnu_utility/image_tool/"

    
def find_entry_address():
    print("find entry address")
    with open(os.path.join(file_path, "bin_structure.txt"), "r") as bin_struct:
        struct = bin_struct.read()
        struct.readline()
        struct.readline()
        entry_line = struct.readline()
        
def find_start_address():
    global sram_start
    global psram_start

    try:
        re_load = r"(\s+LOAD\s+(0x[\da-f]+) (0x[\da-f]{8}) 0x[\da-f]+ (0x[\da-f]+).*)"
        with open(os.path.join(file_path, "bin_structure.txt"), "r") as bin_struct:
            struct = bin_struct.read()
            all_loads = re.findall(re_load, struct, flags=re.IGNORECASE)
            # print(all_loads)
            for load in all_loads:
                virt_addr = int(load[2], 16)
                if 0x20000000 <= virt_addr < 0x20080000 and virt_addr < sram_start:
                    sram_start = virt_addr
                elif 0x60000000 <= virt_addr < 0x61000000 and virt_addr < psram_start:
                    psram_start = virt_addr
            else:
                # not found, set default value
                if sram_start == 0x20080000:
                    sram_start = 0x20000000
                if psram_start == 0x61000000:
                    psram_start = 0x60000000
                print('sram_start: %#x' % sram_start)
                print('psram_start: %#x' % psram_start)
        return True
    except Exception as find_addr_err:
        print(f"Find start address err:{find_addr_err}")

    return False


def string_to_byte_list(string, reorder=True):
    byte_list = []
    try:
        if string.lower().startswith("0x"):
            string = string[2:]
        for i in range(0, len(string) - 1, 2):
            if reorder:
                byte_list.insert(0, int(string[i:i + 2], 16))
            else:
                byte_list.append(int(string[i:i + 2], 16))

        return byte_list
    except Exception as convert_to_bytes_err:
        print(f"Convert {string} to byte error:{convert_to_bytes_err}")
    return []


def prepend_header(bin_file, bin_addr):
    header_bytes = []
    file_name = os.path.split(bin_file)[-1]
    file_size = "{:08x}".format(os.path.getsize(bin_file))

    print("file_size")
    print(file_size)
    
    addr = "{:08x}".format(bin_addr)
    file_size_bytes = string_to_byte_list(file_size)
    bin_addr_bytes = string_to_byte_list(addr)
    if file_size_bytes == [] or bin_addr_bytes == []:
        return False

    if file_name.lower == "xip_boot.bin" or file_name.lower() == "ram_1.bin":
        header_bytes = PATTERN_1 + PATTERN_2 + file_size_bytes + bin_addr_bytes + RSVD * 2
    else:
        header_bytes = IMG2SIGN + file_size_bytes + bin_addr_bytes + RSVD * 2
    with open(bin_file, "rb") as rb, \
            open(os.path.join(file_path, f"pre_{file_name}"), "wb") as wb:
        src = rb.read()
        for header in header_bytes:
            enc = struct.pack("B", header)
            wb.write(enc)
        wb.write(src)

    return True


def run_elf2bin(cmd, work_space=os.getcwd()):
    try:
        p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                             universal_newlines=False, cwd=work_space)
        out, err = p.communicate()
        out_msg = ""
        if p.returncode == 0:
            out_msg += "\n".join(
                [x.strip("\r").strip() for x in out.decode("utf-8", "ignore").split('\n')]) if out is not None else ""
            print(out_msg)
            return True
        else:
            print(err)
    except Exception as run_err:
        print(f"Run {cmd} failed:{run_err}")

    return False


def merge_kr4_km4_dsp():
    global file_path
    
    print(km4_path)

    if(os.path.exists(os.path.join(km4_path, "km4_image2_all.bin"))):
        shutil.copyfile(os.path.join(km4_path, "km4_image2_all.bin"), os.path.join(file_path, "km4_image2_all.bin"))
    else:
        print("no km4_image2_all.bin")
        
    if(os.path.exists(os.path.join(kr4_path, "kr4_image2_all.bin"))):
        shutil.copyfile(os.path.join(kr4_path, "kr4_image2_all.bin"), os.path.join(file_path, "kr4_image2_all.bin"))
        
    if(os.path.exists(os.path.join(manifest_img2_path, "manifest_img2.json"))):
        shutil.copyfile(os.path.join(manifest_img2_path, "manifest_img2.json"), os.path.join(file_path, "manifest_img2.json"))
    else:
        print("no manifest_img2.json")
    print("km4 kr4 path")
    print(km4_path)
    f_dsp = open(os.path.join(file_path, "dsp.bin"), 'rb')
    
    f_km4 = open(os.path.join(file_path, "km4_image2_all.bin"), 'rb')
    f_kr4 = open(os.path.join(file_path, "kr4_image2_all.bin"), 'rb')

    l_dsp = f_dsp.readlines()
    l_km4 = f_km4.readlines()
    l_kr4 = f_kr4.readlines()

    l_kr4_km4_dsp = l_kr4 + l_km4 + l_dsp

    f_kr4_km4_dsp = open(os.path.join(file_path, "image2_all_tmp.bin"), 'wb')
    f_kr4_km4_dsp.writelines(l_kr4_km4_dsp)

    f_dsp.close()
    f_km4.close()
    f_kr4.close()
    f_kr4_km4_dsp.close()

    if platform.system().lower() == 'linux':
        cmd1 = "./elf2bin manifest manifest_img2.json key_img2.json image2_all_tmp.bin manifest.bin"
        cmd2 = "./elf2bin rsip kr4_image2_all.bin kr4_image2_all_en.bin 0x0c000000 manifest_img2.json"
        cmd3 = "./elf2bin rsip km4_image2_all.bin km4_image2_all_en.bin 0x0e000000 manifest_img2.json"
        cmd4 = "./elf2bin rsip dsp.bin dsp_en.bin 0x0D000000 manifest_img2.json"
        cmd5 = "./elf2bin cert cert.json key_cert.json cert.bin 0 key_img2.json 1 key_img3.json"    
    else:
        cmd1 = "elf2bin.exe manifest manifest_img2.json key_img2.json image2_all_tmp.bin manifest.bin"
        cmd2 = "elf2bin.exe rsip kr4_image2_all.bin kr4_image2_all_en.bin 0x0c000000 manifest_img2.json"
        cmd3 = "elf2bin.exe rsip km4_image2_all.bin km4_image2_all_en.bin 0x0e000000 manifest_img2.json"
        cmd4 = "elf2bin.exe rsip dsp.bin dsp_en.bin 0x0D000000 manifest_img2.json"
        cmd5 = "elf2bin.exe cert cert.json key_cert.json cert.bin 0 key_img2.json 1 key_img3.json"

    run_elf2bin(cmd1, work_space=file_path)
    run_elf2bin(cmd2, work_space=file_path)
    run_elf2bin(cmd3, work_space=file_path)
    run_elf2bin(cmd4, work_space=file_path)
    run_elf2bin(cmd5, work_space=file_path)

    kr4_en_file = os.path.join(file_path, "kr4_image2_all_en.bin")
    km4_en_file = os.path.join(file_path, "km4_image2_all_en.bin")
    dsp_en_file = os.path.join(file_path, "dsp_en.bin")

    kr4_file = os.path.join(file_path, "kr4_image2_all.bin")
    km4_file = os.path.join(file_path, "km4_image2_all.bin")
    dsp_file = os.path.join(file_path, "dsp.bin")
    
    if(os.path.exists(kr4_en_file) and os.path.exists(km4_en_file) and os.path.exists(dsp_en_file)):
        os.remove(os.path.join(file_path, "image2_all_tmp.bin"))
        
        f_dsp_en = open(os.path.join(file_path, "dsp_en.bin"), 'rb')
        f_km4_en = open(os.path.join(file_path, "km4_image2_all_en.bin"), 'rb')
        f_kr4_en = open(os.path.join(file_path, "kr4_image2_all_en.bin"), 'rb')
        f_cert_en = open(os.path.join(file_path, "cert.bin"), 'rb')
        f_manifest_en = open(os.path.join(file_path, "manifest.bin"), 'rb')
    
        l_dsp_en = f_dsp_en.readlines()
        l_km4_en = f_km4_en.readlines()
        l_kr4_en = f_kr4_en.readlines()
        l_cert_en = f_cert_en.readlines()
        l_manifest_en = f_manifest_en.readlines()
    
        l_cert_manifest_kr4_km4_dsp_en = l_cert_en + l_manifest_en + l_kr4_en + l_km4_en + l_dsp_en
        
        if platform.system().lower() == 'linux':
            file_path_img = file_path + "/../image" 
        else:
            file_path_img = file_path + "\..\image" 
            
        f_image2_all_en = open(os.path.join(file_path_img, "image2_all.bin"), 'wb')
        f_image2_all_en.writelines(l_cert_manifest_kr4_km4_dsp_en)

        f_dsp_en.close()
        f_km4_en.close()
        f_kr4_en.close()
        f_cert_en.close()
        f_manifest_en.close()
        f_image2_all_en.close()
    else:
        if(os.path.exists(kr4_file) and os.path.exists(km4_file) and os.path.exists(dsp_file)):
        
            f_dsp = open(os.path.join(file_path, "dsp.bin"), 'rb')
            f_km4 = open(os.path.join(file_path, "km4_image2_all.bin"), 'rb')
            f_kr4 = open(os.path.join(file_path, "kr4_image2_all.bin"), 'rb')
            f_cert = open(os.path.join(file_path, "cert.bin"), 'rb')
            f_manifest = open(os.path.join(file_path, "manifest.bin"), 'rb')
    
            l_dsp = f_dsp.readlines()
            l_km4 = f_km4.readlines()
            l_kr4 = f_kr4.readlines()
            l_cert = f_cert.readlines()
            l_manifest = f_manifest.readlines()
    
            l_cert_manifest_kr4_km4_dsp = l_cert + l_manifest + l_kr4 + l_km4 + l_dsp
            
            if platform.system().lower() == 'linux':
                file_path_img = file_path + "/../image" 
            else:
                file_path_img = file_path + "\..\image" 
            
            print(file_path_img)
            f_image2_all = open(os.path.join(file_path_img, "image2_all.bin"), 'wb')
            f_image2_all.writelines(l_cert_manifest_kr4_km4_dsp)

            f_dsp.close()
            f_km4.close()
            f_kr4.close()
            f_cert.close()
            f_manifest.close()
            f_image2_all.close()



if __name__ == "__main__":
    for filename in os.listdir(file_path):
        if(filename.endswith(".bin")):
            if(filename in ["output_flash.bin", "output_sram.bin", "output_psram.bin", "output_dtcm0.bin", "output_dtcm1.bin"]):
                pass
            else:
                os.remove(os.path.join(file_path, filename))
                
    if not find_start_address():
        sys.exit(-1)
    with open(os.path.join(file_path, "dsp.bin"), "wb") as dsp:
        for file_tpl in [("output_flash.bin", flash_start), ("output_sram.bin", sram_start),
                     ("output_psram.bin", psram_start), ("output_dtcm0.bin", dtcm0_start_fixed), ("output_dtcm1.bin", dtcm1_start_fixed)]:
            file = file_tpl[0]
            address = file_tpl[1]
            real_file = os.path.join(file_path, file)
            pre_real_file = os.path.join(file_path, f"pre_{file}")
            if not os.path.exists(real_file):
                print(f"{real_file} does not exists.")
                sys.exit(-1)
            if not prepend_header(real_file, address):
                print(f"{pre_real_file} generate failed!")
                sys.exit(-1)
            with open(f"{pre_real_file}", "rb") as bin_file:
                dsp.write(bin_file.read())
            os.remove(f"{pre_real_file}")  # remove pre-temp bin
    print("dsp.bin generate success.")


    if(image_generate_method != "merge"):
        if platform.system().lower() == 'linux':
            cmd1 = "./elf2bin manifest manifest_dsp.json key_dsp.json dsp.bin manifest.bin"
            cmd2 = "./elf2bin rsip dsp.bin dsp_en.bin 0x0D000000 manifest_dsp.json"
        else:
            cmd1 = "elf2bin.exe manifest manifest_dsp.json key_dsp.json dsp.bin manifest.bin"
            cmd2 = "elf2bin.exe rsip dsp.bin dsp_en.bin 0x0D000000 manifest_dsp.json"
            
        if not run_elf2bin(cmd1, work_space=file_path):
            print("")
            sys.exit(-1)
        if not run_elf2bin(cmd2, work_space=file_path):
            print("")
            sys.exit(-1)
    
        f_dsp = open(os.path.join(file_path, "dsp.bin"), "rb")
        l_dsp = f_dsp.readlines()

        f_manifest = open(os.path.join(file_path, "manifest.bin"), "rb")
        l_manifest = f_manifest.readlines()
    
        l_manifest_dsp = l_manifest + l_dsp
    
        if platform.system().lower() == 'linux':
            file_path_img = file_path + "/../image" 
        else:
            file_path_img = file_path + "\..\image" 
        f_dsp_all =  open(os.path.join(file_path_img, "dsp_all.bin"), "wb")
        f_dsp_all.writelines(l_manifest_dsp)
        f_dsp_all.close()
        f_dsp.close()
        f_manifest.close()
        dsp_path = os.path.join(file_path_img, "dsp.bin")
        if os.path.exists(dsp_path):
            os.remove(dsp_path)
        shutil.move(os.path.join(file_path, "dsp.bin"), file_path_img)
    else:
        merge_kr4_km4_dsp()
        
    for filename in os.listdir(file_path):
        if(filename.endswith(".bin")):
            os.remove(os.path.join(file_path, filename))
            
    sys.exit(0)
