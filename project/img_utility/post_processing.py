#!/usr/bin/env python3.8

import os
import subprocess
import shutil
import platform

vvv = os.environ["xt_project_loc"] + '/' + 'bin' + '/' + os.environ["TARGET_CONFIG"] + '/' +  os.environ["xt_build_target"] + '/' + os.environ["xt_project"] 

xtensa_param = '--xtensa-core=' + os.environ["TARGET_CONFIG"]


fout = open('bin_structure.txt', 'w')

if platform.system().lower() == 'linux':
    if((os.environ["TARGET_CONFIG"] == 'HIFI5_PROD_1123_asic_UPG') or (os.environ["TARGET_CONFIG"] == 'HIFI5_PROD_1123_asic_wUPG')):
        subprocess.run(['xt-readelf', xtensa_param, '-l', vvv], stdout=fout)
    else:
        subprocess.run(['xt-readelf', '-l', vvv], stdout=fout)
else:
    if((os.environ["TARGET_CONFIG"] == 'HIFI5_PROD_1123_asic_UPG') or (os.environ["TARGET_CONFIG"] == 'HIFI5_PROD_1123_asic_wUPG')):
        subprocess.run(['xt-readelf.exe', xtensa_param, '-l', vvv], stdout=fout)
    else:
        subprocess.run(['xt-readelf.exe', '-l', vvv], stdout=fout)
    
fout.close()

src_file = os.environ["xt_project_loc"] + '/' + 'bin' + '/' + os.environ["TARGET_CONFIG"] + '/' + os.environ["xt_build_target"] + '/' + os.environ["xt_project"] 

shutil.copyfile(src_file, "image.elf")