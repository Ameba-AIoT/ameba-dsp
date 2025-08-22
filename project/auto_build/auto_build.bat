Echo off

:: DO compile here
SET COMPILE=C:\usr\xtensa\Xplorer-9.0.18\xplorer.exe
SET DIR=%cd%
SET DSP_BATCH=dsp_batch.xml

%COMPILE% --headless %DIR%\%DSP_BATCH% --import-projects all --import-dir  %DIR%\..  --cleanbuild -- -data  %DIR%\..

pause
