@echo Voodoo Shader Framework - Unit Tests
@cd bin\Release

@echo Instrumenting modules...
@vsinstr /coverage Voodoo_Core.dll

@echo Running tests....
vsperfcmd /start:coverage /output:../results
WinUnit -o ../results.log VoodooTest_Core.dll
vsperfcmd /shutdown

@cd ..\..
@echo Unit tests complete. Test results in results.log, code coverage in results.coverage. Modules will remain instrumented.
