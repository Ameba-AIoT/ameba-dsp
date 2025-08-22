auto_build.sh:(linux command line)
1. Before each compilation, the workspace (including all intermediate files) will be emptied
2. If you need to change the compiled project, target or configuration, please change the dsp_batch.xml file, or use Xplorer batch build generate xml file
3. The default compiler is /opt/xtensa/Xplorer-9.0.17/xplorer, set it according to the actual installation location and Xplorer version

auto_build.bat: (windows) 
1. Run Xplorer.exe in command line(without GUI)
2. Run auto_build.bat for dsp compile with cleanbuild
3. If cleanbuild is not needed, remove "--cleanbuild" option in commandline of auto_build.bat 
4. If you need to change the compiled project, target or configuration, please change the dsp_batch.xml file, or use Xplorer batch build generate xml file