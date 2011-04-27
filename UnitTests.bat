echo Voodoo Shader Framework - Unit Tests
@cd Debug
vsperfcmd /start:coverage /output:cover
WinUnit Core_Tests.dll
vsperfcmd /shutdown
@cd ..