@echo off
rmdir /s /q Build
rmdir /s /q Tmp
rmdir /s /q Temp
rmdir /s /q ipch
del /f /q *.sdf
del /f /q /s *.ilk
del /f /q /s *.exp
del /f /q /s *.ncb
del /f /q /s *.aps
del /f /q /s *.pdb
del /f /q /s *.suo