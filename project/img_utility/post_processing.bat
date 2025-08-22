set XTENSA_CORE=HIFI5_dev_1119_asic

xt-readelf.exe -l %xt_project_loc%\bin\%XTENSA_CORE%\%xt_build_target%\%xt_project% > .\..\img_utility\bin_structure.txt

copy .\bin\%XTENSA_CORE%\%xt_build_target%\%xt_project% .\..\img_utility\image.elf


python .\..\img_utility\elf_process.py

python .\..\img_utility\dsp_combine.py