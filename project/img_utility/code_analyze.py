#!/usr/bin/env python

from pickle import FALSE
import platform
import sys
import subprocess
import os
import re
import shutil
from typing import List
import time 

# enable Disassembly output
ENABLE_Disassembler = 0

memory_config = 'Memory Configuration'
dram_start = 'dram0_0_seg'
sram_start = 'sram_dsp_seg'
psram_start = 'entry_table_0_seg'

file_path = os.path.dirname(os.path.abspath(__file__))
TARGET_CONFIG = os.environ["TARGET_CONFIG"]
text_filepath = os.environ["xt_project_loc"] + '/' + 'bin' + '/' + os.environ["TARGET_CONFIG"] + '/' +  os.environ["xt_build_target"] + '/' + os.environ["xt_project"] + '.map'
elf_filepath = os.environ["xt_project_loc"] + '/' + 'bin' + '/' + os.environ["TARGET_CONFIG"] + '/' +  os.environ["xt_build_target"] + '/' + os.environ["xt_project"]
# please set the path of the lsp according to the selected lsp and tool chain version
if TARGET_CONFIG == 'HIFI5_PROD_1123_asic':
    ld_filepath = '/psram_lsp/RI-2021.7/' + 'HIFI5_PROD_1123_asic' + '/psram_lsp/ldscripts/elf32xtensa.x'
else:
    ld_filepath = '/RTK_LSP/RI-2021.8/' + 'HIFI5_PROD_1123_asic_UPG' + '/RTK_LSP/ldscripts/elf32xtensa.x'

textpath = ''
ldpath = ''

psram_base_addr = 0x60000000
sram_base_addr = 0x20010020  
dram_base_addr = 0x1ffc0000

sections = []
noload_sections = []
all_modules = []

def get_sections():
    global sections, noload_sections, ldpath

    if os.path.exists(ldpath):
        parse_file = open(ldpath, 'r')
    else:
        return
    all_lines = parse_file.readlines()

    Is_Noload = False
    Is_Start = False
    sections_temp = {} 

    for line in all_lines:
        if line.find('SECTIONS') >= 0:
            Is_Start = True

        if Is_Start:

            if line.find('rom') > 0:
                continue

            if line.find('NOLOAD') > 0 or line.find('COPY') > 0 or line.find('bluetooth_trace') > 0:
                Is_Noload = True
            
            if line.find('}') > 0:
                Is_Noload = False

            spos = line.find('*')
            epos1 = line.find('*)')
            epos2 = line.find(')')
            if spos>0 and epos1>0:
                newline = line[spos:epos1+2]
                if newline.find('SORT'):
                    newline = newline.replace('SORT', 'SORT_BY_NAME')
                if sections_temp.get(newline) == None:
                    sections_temp[newline] = dict(noload=False)
                sections_temp[newline] = Is_Noload
            elif spos>0 and epos2>0:
                newline = line[spos:epos2+1]
                if newline.find('SORT'):
                    newline = newline.replace('SORT', 'SORT_BY_NAME')
                if sections_temp.get(newline) == None:
                    sections_temp[newline] = dict(noload=False)
                sections_temp[newline] = Is_Noload

        
    for key,value in sections_temp.items():
        if value == True:
            noload_sections.append(key)  
        sections.append(key)
    #print(sections)
    #print(noload_sections)

def get_base_addr():
    global psram_base_addr, sram_base_addr, dram_base_addr, textpath

    text_file = open(textpath, 'r')
    text_all_lines = text_file.readlines()

    Is_psram = False
    Is_Sram = False
    Is_dram = False

    for line in text_all_lines:
        if line.find(dram_start) >= 0:
            items = list(filter(None, line.strip().split(' ')))
            dram_base_addr = (int(items[1], 16) >> 24 ) << 24
            Is_dram = True

        if line.find(psram_start) >= 0:
            items = list(filter(None, line.strip().split(' ')))
            psram_base_addr = (int(items[1], 16) >> 24 ) << 24
            Is_psram = True
        
        if line.find(sram_start) >= 0:
            items = list(filter(None, line.strip().split(' ')))
            sram_base_addr = (int(items[1], 16) >> 24 ) << 24
            Is_Sram = True
        
        if Is_psram and Is_Sram and Is_dram:
            break
    
    #print(hex(psram_base_addr), hex(sram_base_addr), hex(dram_base_addr))

def resort(ls):
    # transfer to upper
    str_olds_tem = []
    for i in ls:
        str_olds_tem.append(i.upper())

    # sort
    sort_index = []
    for i in str_olds_tem:
        sort_index.append({
            'key': str_olds_tem.index(i),
            'value': i
        })
    sort_index = sorted(sort_index, key=lambda e: e.__getitem__('value'), reverse=False)
    # 
    ture_index = []
    for i in sort_index:
        ture_index.append(i.get('key'))
    ture_str = []
    for i in ture_index:
        ture_str.append(ls[i])
    return ture_str

def sort_modules(strx):
    srcfile = 'parse_sections_' + strx + '.map'
    dstfile = 'parse_sections_' + strx + '_n.map'

    f = open(srcfile, 'r')
    all_lines = f.readlines()
    lib = []
    opensrc = []
    for line in all_lines:
        if line.find('.a') > 0:
            lib.append(line)
        else:
            opensrc.append(line)
    f.close()
    new_lib = []
    for line in lib:
        s = line.find('(')
        e = line.find(')')
        #libname = line[:s]
        filename = line[s+1:e]
        new_line = filename + ' ' + line
        new_lib.append(new_line)

    newopen = resort(opensrc)
    newlib = resort(new_lib)
    f = open(dstfile, 'w')
    for line in newlib:
        line = (line.strip())
        item = line.split(' ')
        if srcfile == 'parse_sections_image.map':
            print(item[2] + ' ' + item[3] + ' ' + item[4] + ' ' + item[5] + ' ' + item[6] + ' ' + item[7] + ' ' 
                + item[8] + ' ' + item[9] + ' ' + item[10] + ' ' + item[11] + ' ' + item[12] + ' ' + item[13] + ' ' + item[14] + ' '+ item[1], file=f)
        else:
            print(item[2] + ' ' + item[3] + ' ' + item[4] + ' ' + item[5] + ' '+ item[1], file=f)
    for line in newopen:
        line = (line.strip())
        item = line.split(' ')
        if srcfile == 'parse_sections_image.map':
            print(item[1] + ' ' + item[2] + ' ' + item[3] + ' ' + item[4] + ' ' + item[5] + ' ' + item[6] + ' ' 
                + item[7] + ' ' + item[8] + ' ' + item[9] + ' ' + item[10] + ' ' + item[11] + ' ' + item[12] + ' ' + item[13] + ' '+ item[0], file=f)
        else:
            print(item[1] + ' ' + item[2] + ' ' + item[3] + ' ' + item[4] + ' '+ item[0], file=f)
    f.close()


def parse_sections():
    global psram_base_addr, sram_base_addr, dram_base_addr, textpath, all_modules

    if os.path.exists('parse_sections_temp.map'):
        os.remove('parse_sections_temp.map')

    if os.path.exists('parse_sections_psram.map'):
        os.remove('parse_sections_psram.map')

    if os.path.exists('parse_sections_sram.map'):
        os.remove('parse_sections_sram.map')
    
    if os.path.exists('parse_sections_dram.map'):
        os.remove('parse_sections_dram.map')

    if os.path.exists('parse_sections_image.map'):
        os.remove('parse_sections_image.map')

    psramf = open('parse_sections_psram.map', 'w')
    sramf = open('parse_sections_sram.map', 'w')
    dramf = open('parse_sections_dram.map', 'w')
    imagef = open('parse_sections_image.map', 'w')

    temp_file = open('parse_sections_temp.map', 'w')
    text_file = open(textpath, 'r')
    #text_file = open('temp_text.map', 'r')
    
    text_all_lines = text_file.readlines()

    modules_psram = {}
    modules_sram = {}
    modules_dram = {}

    IF_Write = False
    last_line = ''
    find_sections = []
    fill_size = 0
    for section in sections:
        for text_line in text_all_lines:
            if text_line.find(section) > 0:
                find_sections.append(section)
                IF_Write = True

            if IF_Write and (text_line.startswith(' *(') or text_line.startswith(' *'))  and text_line.find(section) < 0 and text_line.find('*fill*') < 0:
                IF_Write = False
                #if(text_line.find('*fill*') > 0):
                #    print(text_line)
                break
                    
            if IF_Write:
                
                items = list(filter(None, text_line.strip().split(' ')))
                num = len(items)
                if num == 3:
                    if items[0] == '*fill*':
                        fill_size += int(items[2],16)
                    if (items[0].find('0x') == 0 and (items[-1].endswith('.o') or items[-1].endswith('.o)'))):
                        #modules[items[2]] = dict(rodata='',data='',text='',bss='')
                        if ((int(items[0],16) & psram_base_addr) == 0) and ((int(items[0],16) & sram_base_addr) == 0) and ((int(items[0],16) & dram_base_addr) == 0):
                            #print(text_line)
                            continue
                        bdir = os.path.basename(os.path.split(items[2])[0])
                        items[2] = os.path.split(items[2])[1]
                        if ((int(items[0],16) & psram_base_addr) == psram_base_addr):
                            if modules_psram.get(items[2]) == None:
                                #modules[items[2]] = dict.fromkeys(['text', 'rodata', 'data', 'bss'])
                                modules_psram[items[2]] = dict(rodata=0,data=0,text=0,bss=0,noload_data=0,noload_bss=0,dir='')
                            modules_psram[items[2]]['dir'] = bdir
                            if section in noload_sections:
                                if section.find('data') > 0:
                                    modules_psram[items[2]]['noload_data'] += int(items[1],16)
                                elif section.find('bss') > 0:
                                    modules_psram[items[2]]['noload_bss'] += int(items[1],16)
                            else:

                                if section.find('rodata') > 0 and section.find('text') > 0:
                                    if(last_line.find('rodata') > 0) :
                                        modules_psram[items[2]]['rodata']  += int(items[1],16)
                                    elif(last_line.find('text') > 0) :
                                        modules_psram[items[2]]['text'] += int(items[1],16)
                                elif section.find('rodata') > 0 or section.find('tors') > 0:
                                    modules_psram[items[2]]['rodata']  += int(items[1],16)
                                elif section.find('data') > 0 or section.find('table') > 0 or section.find('command') > 0 or section.find('__llvm_prf') > 0:
                                    modules_psram[items[2]]['rodata'] += int(items[1],16)
                                elif section.find('text') > 0 or section.find('literal') > 0 or section.find('init') > 0 or section.find('fini') > 0:
                                    modules_psram[items[2]]['text'] += int(items[1],16)
                                elif section.find('bss') > 0:
                                    modules_psram[items[2]]['bss'] += int(items[1],16)
                        elif ((int(items[0],16) & sram_base_addr) == sram_base_addr):
                            if modules_sram.get(items[2]) == None:
                                #modules[items[2]] = dict.fromkeys(['text', 'rodata', 'data', 'bss'])
                                modules_sram[items[2]] = dict(rodata=0,data=0,text=0,bss=0,noload_data=0,noload_bss=0,dir='')
                            modules_sram[items[2]]['dir'] = bdir
                            if section in noload_sections:
                                if section.find('data') > 0:
                                    modules_sram[items[2]]['noload_data'] += int(items[1],16)
                                elif section.find('bss') > 0:
                                    modules_sram[items[2]]['noload_bss'] += int(items[1],16)
                            else:
                                if section.find('rodata') > 0 and section.find('text') > 0:
                                    #print(section+' '+text_line)
                                    if(last_line.find('rodata') > 0) :
                                        modules_sram[items[2]]['rodata']  += int(items[1],16)
                                    elif(last_line.find('text') > 0) :
                                        modules_sram[items[2]]['text'] += int(items[1],16)
                                elif section.find('rodata') > 0:
                                    modules_sram[items[2]]['rodata']  += int(items[1],16)
                                elif section.find('data') > 0 or section.find('table') > 0 or section.find('command') > 0:
                                    modules_sram[items[2]]['data'] += int(items[1],16)
                                elif section.find('text') > 0 or section.find('literal') > 0:
                                    modules_sram[items[2]]['text'] += int(items[1],16)
                                elif section.find('bss') > 0:
                                    modules_sram[items[2]]['bss'] += int(items[1],16)
                        elif ((int(items[0],16) & dram_base_addr) == dram_base_addr):
                            if modules_dram.get(items[2]) == None:
                                #modules[items[2]] = dict.fromkeys(['text', 'rodata', 'data', 'bss'])
                                modules_dram[items[2]] = dict(rodata=0,data=0,text=0,bss=0,noload_data=0,noload_bss=0,dir='')
                            modules_dram[items[2]]['dir'] = bdir
                            if section in noload_sections:
                                if section.find('data') > 0:
                                    modules_dram[items[2]]['noload_data'] += int(items[1],16)
                                elif section.find('bss') > 0:
                                    modules_dram[items[2]]['noload_bss'] += int(items[1],16)
                            else:
                                if section.find('rodata') > 0 and section.find('text') > 0:
                                    #print(section+' '+text_line)
                                    if(last_line.find('rodata') > 0) :
                                        modules_dram[items[2]]['rodata']  += int(items[1],16)
                                    elif(last_line.find('text') > 0) :
                                        modules_dram[items[2]]['text'] += int(items[1],16)
                                elif section.find('rodata') > 0:
                                    modules_dram[items[2]]['rodata']  += int(items[1],16)
                                elif section.find('data') > 0 or section.find('table') > 0 or section.find('command') > 0:
                                    modules_dram[items[2]]['data'] += int(items[1],16)
                                elif section.find('text') > 0 or section.find('literal') > 0:
                                    modules_dram[items[2]]['text'] += int(items[1],16)
                                elif section.find('bss') > 0:
                                    modules_dram[items[2]]['bss'] += int(items[1],16)
                        temp_file.write(text_line)
                if num == 4:
                    if (items[1].find('0x') == 0 and (items[-1].endswith('.o') or items[-1].endswith('.o)'))):
                        #modules[items[2]] = dict(rodata='',data='',text='',bss='')
                        if ((int(items[1],16) & psram_base_addr) == 0) and ((int(items[1],16) & sram_base_addr) == 0) and ((int(items[1],16) & dram_base_addr) == 0):
                            #print(text_line)
                            continue
                        bdir = os.path.basename(os.path.split(items[3])[0])
                        items[3] = os.path.split(items[3])[1]
                        if ((int(items[1],16) & psram_base_addr) == psram_base_addr):
                            if modules_psram.get(items[3]) == None:
                                #modules[items[2]] = dict.fromkeys(['text', 'rodata', 'data', 'bss'])
                                modules_psram[items[3]] = dict(rodata=0,data=0,text=0,bss=0,noload_data=0,noload_bss=0,dir='')
                            modules_psram[items[3]]['dir'] = bdir
                            if section in noload_sections:
                                if section.find('data') > 0:
                                    modules_psram[items[3]]['noload_data'] += int(items[2],16)
                                elif section.find('bss') > 0:
                                    modules_psram[items[3]]['noload_bss'] += int(items[2],16)
                            else:
                                if section.find('rodata') > 0 and section.find('text') > 0:
                                    #print('='+text_line)
                                    if(last_line.find('rodata') > 0) :
                                        modules_psram[items[3]]['rodata']  += int(items[2],16)
                                    elif(last_line.find('text') > 0) :
                                        modules_psram[items[3]]['text'] += int(items[2],16)
                                elif section.find('rodata') > 0 or section.find('tors') > 0:
                                    modules_psram[items[3]]['rodata']  += int(items[2],16)
                                elif section.find('data') > 0 or section.find('table') > 0 or section.find('command') > 0 or section.find('__llvm_prf') > 0:
                                    modules_psram[items[3]]['rodata'] += int(items[2],16)
                                elif section.find('text') > 0 or section.find('literal') > 0 or section.find('init') > 0 or section.find('fini') > 0:
                                    modules_psram[items[3]]['text'] += int(items[2],16)
                                elif section.find('bss') > 0:
                                    modules_psram[items[3]]['bss'] += int(items[2],16)
                        elif ((int(items[1],16) & sram_base_addr) == sram_base_addr):
                            if modules_sram.get(items[3]) == None:
                                #modules[items[2]] = dict.fromkeys(['text', 'rodata', 'data', 'bss'])
                                modules_sram[items[3]] = dict(rodata=0,data=0,text=0,bss=0,noload_data=0,noload_bss=0,dir='')
                            modules_sram[items[3]]['dir'] = bdir
                            if section in noload_sections:
                                if section.find('data') > 0:
                                    modules_sram[items[3]]['noload_data'] += int(items[2],16)
                                elif section.find('bss') > 0:
                                    modules_sram[items[3]]['noload_bss'] += int(items[2],16)
                            else:
                                if section.find('rodata') > 0 and section.find('text') > 0:
                                    if(last_line.find('rodata') > 0) :
                                        modules_sram[items[3]]['rodata']  += int(items[2],16)
                                    elif(last_line.find('text') > 0) :
                                        modules_sram[items[3]]['text'] += int(items[2],16)
                                elif section.find('rodata') > 0:
                                    modules_sram[items[3]]['rodata']  += int(items[2],16)
                                elif section.find('data') > 0 or section.find('table') > 0 or section.find('command') > 0:
                                    modules_sram[items[3]]['data'] += int(items[2],16)
                                elif section.find('text') > 0 or section.find('literal') > 0:
                                    modules_sram[items[3]]['text'] += int(items[2],16)
                                elif section.find('bss') > 0:
                                    modules_sram[items[3]]['bss'] += int(items[2],16)
                        elif ((int(items[1],16) & dram_base_addr) == dram_base_addr):
                            if modules_dram.get(items[3]) == None:
                                #modules[items[2]] = dict.fromkeys(['text', 'rodata', 'data', 'bss'])
                                modules_dram[items[3]] = dict(rodata=0,data=0,text=0,bss=0,noload_data=0,noload_bss=0,dir='')
                            modules_dram[items[3]]['dir'] = bdir
                            if section in noload_sections:
                                if section.find('data') > 0:
                                    modules_dram[items[3]]['noload_data'] += int(items[2],16)
                                elif section.find('bss') > 0:
                                    modules_dram[items[3]]['noload_bss'] += int(items[2],16)
                            else:
                                if section.find('rodata') > 0 and section.find('text') > 0:
                                    #print(section+' '+text_line)
                                    if(last_line.find('rodata') > 0) :
                                        modules_dram[items[3]]['rodata']  += int(items[2],16)
                                    elif(last_line.find('text') > 0) :
                                        modules_dram[items[3]]['text'] += int(items[2],16)
                                elif section.find('rodata') > 0:
                                    modules_dram[items[3]]['rodata']  += int(items[2],16)
                                elif section.find('data') > 0 or section.find('table') > 0 or section.find('command') > 0:
                                    modules_dram[items[3]]['data'] += int(items[2],16)
                                elif section.find('text') > 0 or section.find('literal') > 0:
                                    modules_dram[items[3]]['text'] += int(items[2],16)
                                elif section.find('bss') > 0:
                                    modules_dram[items[3]]['bss'] += int(items[2],16)
                        temp_file.write(text_line)

                last_line = text_line
    # print(fill_size)
    for key,value in modules_psram.items():
        print(key + ' ' +  hex(value['text'])+ ' ' + hex(value['rodata']) + ' ' + hex(value['data']) + ' ' + hex(value['bss']) + ' ' + value['dir'], file=psramf)
        all_modules.append(value['dir'])

    for key,value in modules_sram.items():
        print(key + ' ' +  hex(value['text'])+ ' ' + hex(value['rodata']) + ' ' + hex(value['data']) + ' ' + hex(value['bss']) + ' ' + value['dir'], file=sramf)
        all_modules.append(value['dir'])

    for key,value in modules_dram.items():
        print(key + ' ' +  hex(value['text'])+ ' ' + hex(value['rodata']) + ' ' + hex(value['data']) + ' ' + hex(value['bss']) + ' ' + value['dir'], file=dramf)
        all_modules.append(value['dir'])

    all_modules = list(set(all_modules))
    #print(all_modules)
    # merge psram(psram) and sram
    for key,value in modules_psram.items():
        item = modules_dram.pop(key, None)
        if item == None:
            item = dict(rodata=0,data=0,text=0,bss=0,noload_data=0,noload_bss=0)
        psram_total_size = value['text'] + value['rodata'] + value['noload_data'] + value['noload_bss']
        sram_total_size = item['text'] + item['rodata'] + item['data'] + item['bss']
        noload_total_size = item['noload_data'] + item['noload_bss']
        image_total_size = value['text'] + value['rodata'] + sram_total_size
        ram_total_size = sram_total_size + noload_total_size
        print(key + ' ' +  hex(value['text'])+ ' ' + hex(value['rodata']) + ' ' + hex(item['text'])+ ' ' + hex(item['rodata']) + ' '
                         + hex(item['data']) + ' ' + hex(item['bss']) + ' ' + hex(item['noload_data']) + ' ' + hex(item['noload_bss']) + ' ' 
                         + hex(value['noload_data']) + ' ' + hex(value['noload_bss']) + ' ' 
                         + hex(image_total_size) + ' ' + hex(psram_total_size) + ' ' + hex(ram_total_size) + ' ' + value['dir'], file=imagef)
    # files in sram but not in psram(psram)
    for key,item in modules_dram.items():
        value['text'] = 0
        value['rodata'] = 0
        value['noload_data'] = 0
        value['noload_bss'] = 0
        psram_total_size = value['text'] + value['rodata'] + value['noload_data'] + value['noload_bss']
        sram_total_size = item['text'] + item['rodata'] + item['data'] + item['bss']
        noload_total_size = item['noload_data'] + item['noload_bss']
        image_total_size = value['text'] + value['rodata'] + sram_total_size
        ram_total_size = sram_total_size + noload_total_size
        print(key + ' ' +  hex(value['text'])+ ' ' + hex(value['rodata']) + ' ' + hex(item['text'])+ ' ' + hex(item['rodata']) + ' ' 
                        + hex(item['data']) + ' ' + hex(item['bss']) + ' ' + hex(item['noload_data']) + ' ' + hex(item['noload_bss']) + ' '
                        + hex(value['noload_data']) + ' ' + hex(value['noload_bss']) + ' ' 
                        + hex(image_total_size) + ' ' + hex(psram_total_size) + ' ' + hex(ram_total_size) + ' ' + item['dir'], file=imagef) 
    psramf.close()
    sramf.close()
    dramf.close()
    imagef.close()
    temp_file.close()


def merge_size_and_objs_2(strx):
    des_file = 'code_size_' + strx + '.map'
    src_file = 'parse_sections_' + strx + '_n.map'
    f = open(src_file, 'r')
    sepsize = 25
    if strx == 'image':
        sepsize = 90

    if os.path.exists(des_file):
        os.remove(des_file)
    file_result = open(des_file, 'w')
    if des_file == 'code_size_image.map':
        print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}\n'
                .format('image_total', 'psram_total', 'dram_total', 'psram_text', 'psram_rodata','dram_text', 'dram_rodata', 'dram_data', 'dram_bss', 
                        'data(dram noload)', 'bss(dram noload)','data(psram noload)', 'bss(psram noload)', 'module'), file=file_result)
    else:
        print('{:<13}{:<13}{:<13}{:<13}{:<13}\n'.format('text', 'rodata', 'data', 'bss', 'module'), file=file_result)

    
    all_lines = f.readlines()
    total_size = 0
    total_section_size = dict(image_total=0,psram_total=0,dram_total=0,rodata=0,data=0,text=0,bss=0,data_dram=0,bss_dram=0,noload_data_dram=0,noload_bss_dram=0,noload_data_psram=0,noload_bss_psram=0)
    #
    lable = []
    #
    for line in all_lines:
        #
        line_strip = (line.strip())
        item_split_bank = line_strip.split(' ')
        if des_file == 'code_size_image.map':
            found_a = item_split_bank[13].find('.a')
            filestr = item_split_bank[13]
        else:
            found_a = item_split_bank[4].find('.a')
            filestr = item_split_bank[4]

        if found_a > 0:
            item_split_bank_a = filestr.split('.a')
            if item_split_bank_a[0] not in lable:
                lable.append(item_split_bank_a[0])

    for i in range(len(lable)):
        print('{0} {1} {2} {3}{4}'.format('='*sepsize, lable[i],'start','='*(30-len(lable[i])-len('start')), '='*sepsize), file=file_result)
        section_size = dict(image_total=0,psram_total=0,dram_total=0,rodata=0,data=0,text=0,bss=0,data_dram=0,bss_dram=0,noload_data_dram=0,noload_bss_dram=0,noload_data_psram=0,noload_bss_psram=0)
        for line in all_lines:
            line_strip = (line.strip())
            item_split_bank = line_strip.split(' ')

            found = line.find(lable[i])

            if (found > 0):
                section_size['text'] += int(item_split_bank[0], 16)
                section_size['rodata'] += int(item_split_bank[1], 16)
                section_size['data'] += int(item_split_bank[2], 16)
                section_size['bss'] += int(item_split_bank[3], 16)

                if des_file == 'code_size_image.map':            
                    section_size['data_dram'] += int(item_split_bank[4], 16)
                    section_size['bss_dram'] += int(item_split_bank[5], 16)
                    section_size['noload_data_dram'] += int(item_split_bank[6], 16)
                    section_size['noload_bss_dram'] += int(item_split_bank[7], 16)
                    section_size['noload_data_psram'] += int(item_split_bank[8], 16)
                    section_size['noload_bss_psram'] += int(item_split_bank[9], 16)
                    section_size['image_total'] += int(item_split_bank[10], 16)
                    section_size['psram_total'] += int(item_split_bank[11], 16)
                    section_size['dram_total'] += int(item_split_bank[12], 16)

                    print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}'.format(
                        str(int(item_split_bank[10], 16)),
                        str(int(item_split_bank[11], 16)),
                        str(int(item_split_bank[12], 16)),
                        str(int(item_split_bank[0], 16)), 
                        str(int(item_split_bank[1], 16)), 
                        str(int(item_split_bank[2], 16)),
                        str(int(item_split_bank[3], 16)),
                        str(int(item_split_bank[4], 16)),
                        str(int(item_split_bank[5], 16)),
                        str(int(item_split_bank[6], 16)),
                        str(int(item_split_bank[7], 16)),
                        str(int(item_split_bank[8], 16)),
                        str(int(item_split_bank[9], 16)),
                                item_split_bank[13]), file=file_result)
                else:
                    print('{:<13}{:<13}{:<13}{:<13}{:<13}'.format(
                        str(int(item_split_bank[0], 16)), 
                        str(int(item_split_bank[1], 16)), 
                        str(int(item_split_bank[2], 16)),
                        str(int(item_split_bank[3], 16)),
                        item_split_bank[4]), file=file_result)

        for key,value in total_section_size.items():
            total_section_size[key] += section_size[key]
        print('{0} {1} {2} {3}{4}'.format('-'*sepsize, lable[i],'end','-'*(30-len(lable[i])-len('end')), '-'*sepsize), file=file_result)
        #print('\n' + 'total:', file=file_result)

        if des_file == 'code_size_image.map':
            print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}\n'
                .format('image_total', 'psram_total', 'dram_total', 'psram_text', 'psram_rodata','dram_text', 'dram_rodata', 'dram_data', 'dram_bss', 
                        'data(dram noload)', 'bss(dram noload)','data(psram noload)', 'bss(psram noload)', 'module'), file=file_result)
        else:
            print('{:<13}{:<13}{:<13}{:<13}{:<13}\n'.format('text', 'rodata', 'data', 'bss', 'module'), file=file_result)
        
        if des_file == 'code_size_image.map':
            print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}\n'.format(
                str(section_size['image_total']), 
                str(section_size['psram_total']), 
                str(section_size['dram_total']),
                str(section_size['text']), 
                str(section_size['rodata']), 
                str(section_size['data']),
                str(section_size['bss']),
                str(section_size['data_dram']),
                str(section_size['bss_dram']),
                str(section_size['noload_data_dram']),
                str(section_size['noload_bss_dram']),
                str(section_size['noload_data_psram']),
                str(section_size['noload_bss_psram']),
                '(TOTALS)'), file=file_result)
            total_size = section_size['text'] + section_size['rodata'] + section_size['data'] + section_size['bss'] + section_size['data_dram'] + section_size['bss_dram']
        else:
            print('{:<13}{:<13}{:<13}{:<13}{:<13}\n'.format(
                str(section_size['text']), 
                str(section_size['rodata']), 
                str(section_size['data']),
                str(section_size['bss_dram']),'(TOTALS)'), file=file_result)
            total_size = section_size['text'] + section_size['rodata'] + section_size['data'] + section_size['bss']
        print('Module {} size:'.format(lable[i]), file=file_result)        
        print(str(hex(total_size)) + '\t\t\t' + str(total_size) + '\n', file=file_result)

    if os.path.exists('sort_temp_file.map'):
        os.remove('sort_temp_file.map')
    file_sort_temp = open('sort_temp_file.map', 'w')

    for line in all_lines:
        line_strip = (line.strip())
        item_split_bank = line_strip.split(' ')

        found = line.find('.a')

        if (found < 0):
            if des_file == 'code_size_image.map':
                print(item_split_bank[0] + ' ' + 
                item_split_bank[1] + ' ' + 
                item_split_bank[2] + ' ' + 
                item_split_bank[3] + ' ' + 
                item_split_bank[4] + ' ' + 
                item_split_bank[5] + ' ' + 
                item_split_bank[6] + ' ' + 
                item_split_bank[7] + ' ' + 
                item_split_bank[8] + ' ' + 
                item_split_bank[9] + ' ' + 
                item_split_bank[10] + ' ' + 
                item_split_bank[11] + ' ' + 
                item_split_bank[12] + ' ' + 
                item_split_bank[13], file=file_sort_temp)
            else:
                print(item_split_bank[0] + ' ' + 
                item_split_bank[1] + ' ' + 
                item_split_bank[2] + ' ' + 
                item_split_bank[3] + ' ' + 
                item_split_bank[4], file=file_sort_temp)

    file_sort_temp.close()



    sort_lable_ipc = ['ipc', 'ipc_', 'share_']
    sort_lable_fwlib = ['bsp', 'ameba_']

    sort_lable_per = ['project', 'platform', 'main', 'example', 'mpu']
    sort_lable_shell = ['shell', 'shell', 'log', 'monitor', 'low_level_io']
    sort_lable_os = ['os', 'croutine', 'event_groups', 'list', 'queue', 'tasks', 'timers', 'heap_', 'port', 'freertos_', 'freertos',
                     'xtensa', 'device_lock', 'high_level_int_disptcher', 'hli_vectors']
    sort_lable_audio = ['audio', 'Voice', 'voice', 'RPC', 'buffer']
    sort_lable_test = ['test', 'test']


    sort_labels = [sort_lable_ipc, sort_lable_per, sort_lable_fwlib,
                    sort_lable_shell, sort_lable_os, sort_lable_audio, sort_lable_test]

    file_sort_temp_r = open('sort_temp_file.map', 'r')
    all_lines_sort = file_sort_temp_r.readlines()

    array_flag = [0 for i in range(len(all_lines_sort))]

    
    for label in sort_labels:
        print('{0} {1} {2} {3}{4}'.format('='*sepsize, label[0],'start','='*(30-len(label[0])-len('start')), '='*sepsize), file=file_result)
        section_size = dict(image_total=0,psram_total=0,dram_total=0,rodata=0,data=0,text=0,bss=0,data_dram=0,bss_dram=0,noload_data_dram=0,noload_bss_dram=0,noload_data_psram=0,noload_bss_psram=0)
        temp_flag = 0
        for all_lines_sort_t in all_lines_sort:
            item = list(filter(None, all_lines_sort_t.strip().split(' ')))
            line_strip = (all_lines_sort_t.strip())
            item_split_bank = line_strip.split(' ')
            temp_flag = 0
            for idxn in range(1,len(label)):
                if des_file == 'code_size_image.map':
                    found = item_split_bank[13].find(label[idxn])
                else:
                    found = item_split_bank[4].find(label[idxn])
                if found >= 0:
                    temp_flag = 1
                    break
            if (temp_flag == 1):
                if (array_flag[all_lines_sort.index(all_lines_sort_t)] == 0):
                    section_size['text'] += int(item_split_bank[0], 16)
                    section_size['rodata'] += int(item_split_bank[1], 16)
                    section_size['data'] += int(item_split_bank[2], 16)
                    section_size['bss'] += int(item_split_bank[3], 16)

                    if des_file == 'code_size_image.map':              
                        section_size['data_dram'] += int(item_split_bank[4], 16)
                        section_size['bss_dram'] += int(item_split_bank[5], 16)
                        section_size['noload_data_dram'] += int(item_split_bank[6], 16)
                        section_size['noload_bss_dram'] += int(item_split_bank[7], 16)
                        section_size['noload_data_psram'] += int(item_split_bank[8], 16)
                        section_size['noload_bss_psram'] += int(item_split_bank[9], 16)
                        section_size['image_total'] += int(item_split_bank[10], 16)
                        section_size['psram_total'] += int(item_split_bank[11], 16)
                        section_size['dram_total'] += int(item_split_bank[12], 16)

                        print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}'.format(
                            str(int(item_split_bank[10], 16)),
                            str(int(item_split_bank[11], 16)),
                            str(int(item_split_bank[12], 16)),
                            str(int(item_split_bank[0], 16)), 
                            str(int(item_split_bank[1], 16)), 
                            str(int(item_split_bank[2], 16)),
                            str(int(item_split_bank[3], 16)),
                            str(int(item_split_bank[4], 16)),
                            str(int(item_split_bank[5], 16)),
                            str(int(item_split_bank[6], 16)),
                            str(int(item_split_bank[7], 16)),
                            str(int(item_split_bank[8], 16)),
                            str(int(item_split_bank[9], 16)),
                                    item_split_bank[13]), file=file_result)
                    else:
                        print('{:<13}{:<13}{:<13}{:<13}{:<13}'.format(
                            str(int(item_split_bank[0], 16)), 
                            str(int(item_split_bank[1], 16)), 
                            str(int(item_split_bank[2], 16)),
                            str(int(item_split_bank[3], 16)),
                            item_split_bank[4]), file=file_result)
                    array_flag[all_lines_sort.index(all_lines_sort_t)] = 1
        for key,value in total_section_size.items():
            total_section_size[key] += section_size[key]
        print('{0} {1} {2} {3}{4}'.format('-'*sepsize, label[0],'end','-'*(30-len(label[0])-len('end')), '-'*sepsize), file=file_result)
        #print('\n' + 'total:', file=file_result)

        if des_file == 'code_size_image.map':
            print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}\n'
                .format('image_total', 'psram_total', 'dram_total', 'psram_text', 'psram_rodata','dram_text', 'dram_rodata', 'dram_data', 'dram_bss', 
                        'data(dram noload)', 'bss(dram noload)','data(psram noload)', 'bss(psram noload)', 'module'), file=file_result)
        else:
            print('{:<13}{:<13}{:<13}{:<13}{:<13}\n'.format('text', 'rodata', 'data', 'bss', 'module'), file=file_result)
        if des_file == 'code_size_image.map':
            print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}\n'.format(
                str(section_size['image_total']), 
                str(section_size['psram_total']), 
                str(section_size['dram_total']),
                str(section_size['text']), 
                str(section_size['rodata']), 
                str(section_size['data']),
                str(section_size['bss']),
                str(section_size['data_dram']),
                str(section_size['bss_dram']),
                str(section_size['noload_data_dram']),
                str(section_size['noload_bss_dram']),
                str(section_size['noload_data_psram']),
                str(section_size['noload_bss_psram']),
                '(TOTALS)'), file=file_result)
            total_size = section_size['text'] + section_size['rodata'] + section_size['data'] + section_size['bss'] + section_size['data_dram'] + section_size['bss_dram']
        else:
            print('{:<13}{:<13}{:<13}{:<13}{:<13}\n'.format(
                str(section_size['text']), 
                str(section_size['rodata']), 
                str(section_size['data']),
                str(section_size['bss_dram']),'(TOTALS)'), file=file_result)
            total_size = section_size['text'] + section_size['rodata'] + section_size['data'] + section_size['bss']
        print('Module {} size:'.format(label[0]), file=file_result)
        
        print(str(hex(total_size)) + '\t\t\t' + str(total_size) + '\n', file=file_result)
    file_sort_temp_r.close()

    print('{0} {1} {2} {3}{4}'.format('='*sepsize,'others','start','='*(30-len('others')-len('start')), '='*sepsize), file=file_result)

    file_sort_temp_r = open('sort_temp_file.map', 'r')
    all_lines_sort = file_sort_temp_r.readlines()
    section_size = dict(image_total=0,psram_total=0,dram_total=0,rodata=0,data=0,text=0,bss=0,data_dram=0,bss_dram=0,noload_data_dram=0,noload_bss_dram=0,noload_data_psram=0,noload_bss_psram=0)
    for all_lines_sort_t in all_lines_sort:
        line_strip = (all_lines_sort_t.strip())
        item_split_bank = line_strip.split(' ')

        temp_flag = 0
        for label in sort_labels:
            for idxn in range(1,len(label)):
                if des_file == 'code_size_image.map':
                    found = item_split_bank[13].find(label[idxn])
                else:
                    found = item_split_bank[4].find(label[idxn])
                if found >= 0:
                    temp_flag = 1
                    break
            if(temp_flag == 1):
                break

        if (temp_flag == 0):
            if (array_flag[all_lines_sort.index(all_lines_sort_t)] == 0):
                section_size['text'] += int(item_split_bank[0], 16)
                section_size['rodata'] += int(item_split_bank[1], 16)
                section_size['data'] += int(item_split_bank[2], 16)
                section_size['bss'] += int(item_split_bank[3], 16)

                if des_file == 'code_size_image.map':              
                    section_size['data_dram'] += int(item_split_bank[4], 16)
                    section_size['bss_dram'] += int(item_split_bank[5], 16)
                    section_size['noload_data_dram'] += int(item_split_bank[6], 16)
                    section_size['noload_bss_dram'] += int(item_split_bank[7], 16)
                    section_size['noload_data_psram'] += int(item_split_bank[8], 16)
                    section_size['noload_bss_psram'] += int(item_split_bank[9], 16)
                    section_size['image_total'] += int(item_split_bank[10], 16)
                    section_size['psram_total'] += int(item_split_bank[11], 16)
                    section_size['dram_total'] += int(item_split_bank[12], 16)

                    print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}'.format(
                        str(int(item_split_bank[10], 16)),
                        str(int(item_split_bank[11], 16)),
                        str(int(item_split_bank[12], 16)),
                        str(int(item_split_bank[0], 16)), 
                        str(int(item_split_bank[1], 16)), 
                        str(int(item_split_bank[2], 16)),
                        str(int(item_split_bank[3], 16)),
                        str(int(item_split_bank[4], 16)),
                        str(int(item_split_bank[5], 16)),
                        str(int(item_split_bank[6], 16)),
                        str(int(item_split_bank[7], 16)),
                        str(int(item_split_bank[8], 16)),
                        str(int(item_split_bank[9], 16)),
                                item_split_bank[13]), file=file_result)
                else:
                    print('{:<13}{:<13}{:<13}{:<13}{:<13}'.format(
                        str(int(item_split_bank[0], 16)), 
                        str(int(item_split_bank[1], 16)), 
                        str(int(item_split_bank[2], 16)),
                        str(int(item_split_bank[3], 16)),
                        item_split_bank[4]), file=file_result)
                array_flag[all_lines_sort.index(all_lines_sort_t)] = 1

    for key,value in total_section_size.items():
        total_section_size[key] += section_size[key]
    print('{0} {1} {2} {3}{4}'.format('-'*sepsize, 'others','end','-'*(30-len('others')-len('end')), '-'*sepsize), file=file_result)

    if des_file == 'code_size_image.map':
        print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}\n'
                .format('image_total', 'psram_total', 'dram_total', 'psram_text', 'psram_rodata','dram_text', 'dram_rodata', 'dram_data', 'dram_bss', 
                        'data(dram noload)', 'bss(dram noload)','data(psram noload)', 'bss(psram noload)', 'module'), file=file_result)
    else:
        print('{:<13}{:<13}{:<13}{:<13}{:<13}\n'.format('text', 'rodata', 'data', 'bss', 'module'), file=file_result)

    if des_file == 'code_size_image.map':
        print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}\n'.format(
            str(section_size['image_total']), 
            str(section_size['psram_total']), 
            str(section_size['dram_total']),
            str(section_size['text']), 
            str(section_size['rodata']), 
            str(section_size['data']),
            str(section_size['bss']),
            str(section_size['data_dram']),
            str(section_size['bss_dram']),
            str(section_size['noload_data_dram']),
            str(section_size['noload_bss_dram']),
            str(section_size['noload_data_psram']),
            str(section_size['noload_bss_psram']),
            '(TOTALS)'), file=file_result)
        total_size = section_size['text'] + section_size['rodata'] + section_size['data'] + section_size['bss'] + section_size['data_dram'] + section_size['bss_dram']
    else:
        print('{:<13}{:<13}{:<13}{:<13}{:<13}\n'.format(
            str(section_size['text']), 
            str(section_size['rodata']), 
            str(section_size['data']),
            str(section_size['bss_dram']),'(TOTALS)'), file=file_result)
        total_size = section_size['text'] + section_size['rodata'] + section_size['data'] + section_size['bss']
    print('Module {} size:'.format('others'), file=file_result)
    
    print(str(hex(total_size)) + '\t\t\t' + str(total_size) + '\n', file=file_result)
    print('{0}'.format('='*(30 + sepsize * 2)), file=file_result)

    file_sort_temp_r.close()
    if des_file == 'code_size_image.map':
        print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}\n'
                .format('image_total', 'psram_total', 'dram_total', 'psram_text', 'psram_rodata','dram_text', 'dram_rodata', 'dram_data', 'dram_bss', 
                        'data(dram noload)', 'bss(dram noload)','data(psram noload)', 'bss(psram noload)', 'module'), file=file_result)
        print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<20}{:<20}{:<20}{:<20}{:<13}'.format(
            str(total_section_size['image_total']), 
            str(total_section_size['psram_total']), 
            str(total_section_size['dram_total']),
            str(total_section_size['text']), 
            str(total_section_size['rodata']), 
            str(total_section_size['data']),
            str(total_section_size['bss']),
            str(total_section_size['data_dram']),
            str(total_section_size['bss_dram']),
            str(total_section_size['noload_data_dram']),
            str(total_section_size['noload_bss_dram']),
            str(total_section_size['noload_data_psram']),
            str(total_section_size['noload_bss_psram']),
            '(TOTALS)'), file=file_result)
        total_size = total_section_size['text'] + total_section_size['rodata'] + total_section_size['data'] + total_section_size['bss'] + total_section_size['data_dram'] + total_section_size['bss_dram']
    else:
        print('{:<13}{:<13}{:<13}{:<13}{:<13}'.format('text', 'rodata', 'data', 'bss', 'module'), file=file_result)
        print('{:<13}{:<13}{:<13}{:<13}{:<13}'.format(
                str(total_section_size['text']), 
                str(total_section_size['rodata']), 
                str(total_section_size['data']),
                str(total_section_size['bss']),
                '(TOTALS)'), file=file_result)
        total_size = total_section_size['text'] + total_section_size['rodata'] + total_section_size['data'] + total_section_size['bss']
    print('\n' + 'Image size:', file=file_result)
    print(str(hex(total_size)) + '\t\t\t' + str(total_size) + '\n', file=file_result)

    file_result.close()
    f.close()
    return

#print(os.environ)

if platform.system().lower() == 'linux':
    textpath = text_filepath.replace('\\','/')
    ldpath = os.path.abspath('..') + ld_filepath.replace('\\','/')
else:
    textpath = text_filepath.replace('/','\\')
    ldpath = os.path.abspath('..') + ld_filepath.replace('/','\\')

print('[code_analyze.py] map path:' + textpath)
print('[code_analyze.py] link script path:' + ldpath)

if os.path.exists(textpath) and os.path.exists(ldpath) :
    get_base_addr()
    get_sections()
    parse_sections()
    str_list = ['psram', 'sram', 'dram', 'image']
    for strr in str_list:
        sort_modules(strr)
        merge_size_and_objs_2(strr)
    
    # remove temp files
    #os.remove('obj_list.map')
    dstpath =  os.path.join(os.path.abspath('..'), 'image')

    os.remove('parse_sections_temp.map')
    os.remove('sort_temp_file.map')

    for strr in str_list:
        os.remove('parse_sections_' + strr + '.map')
        os.remove('parse_sections_' + strr + '_n.map')
        if os.path.exists(os.path.join(dstpath, 'code_size_' + strr + '.map')):
            os.remove(os.path.join(dstpath, 'code_size_' + strr + '.map'))
        shutil.move('code_size_' + strr + '.map', dstpath)
else:
    print('[code_analyze.py] fail to open link scripts path, please check the LSP path in this python file')
# generate disassembler
if ENABLE_Disassembler:
    start = time.time()
    print('[code_analyze.py] disassembler auto generate enabled. You can disable it in code_analyze.py')
    print('[code_analyze.py] elf path:' + elf_filepath)
    if platform.system().lower() == 'linux':
        asm_fout = open('../image/' + os.environ["xt_project"] + '.asm', 'w')
        subprocess.run(['xt-objdump', '-D', '-C', '-S', '--xtensa-core=' + TARGET_CONFIG, elf_filepath], stdout=asm_fout)
        print('[code_analyze.py] disassembler generate finished. ASM file path: <sdk_path>/project/image/' + os.environ["xt_project"] + '.asm')
    else:
        asm_fout = open('..\\image\\' + os.environ["xt_project"] + '.asm', 'w')
        subprocess.run(['xt-objdump.exe', '-D', '-C', '-S', '--xtensa-core=' + TARGET_CONFIG, elf_filepath], stdout=asm_fout)
        print('[code_analyze.py] disassembler generate finished. ASM file path: <sdk_path>\\project\\image\\' + os.environ["xt_project"] + '.asm')
    end = time.time()
    print('[code_analyze.py] generate disassembler time taken:', end - start, 's')
else:
    print('[code_analyze.py] disassembler auto generate disabled. You can enable it in code_analyze.py')

