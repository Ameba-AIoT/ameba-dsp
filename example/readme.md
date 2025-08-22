[2022.09.25]  
Currently, some example projects is not added to the project_dsp by default.   
If you need to compile these some example projects,   
please refer to AN0900_ RTL8726E_ Application_ Note: Chapter 2.4 Add Virtual Folder and Files  
  
Add to projetc method:   
Suppose our application files "test1.c" and "test2.c" are placed in a folder names "example_xxx", we add these files to our current project.  
(1) We first create a virtual folder named "example_xxx". Right click on "project_dsp", then New > Folder.  
(2) Fill the "Folder name" with "example_xxx",, choose "Folder is not located in the file system", then click "Finish."  
(3) Right click on "example_xxx", and choose "Import".  
(4) Click "General" > "File System" > "Next".  
(5) Browse to the address of your folder and the files you want to include. Click "Advanced" and tick the 3 boxes:  
    - Create links in workspace  
    - Create virtual folders  
    - Create link locations relative to: *PROJECT_LOC*  


