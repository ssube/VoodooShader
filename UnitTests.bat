@echo Voodoo Shader Framework - Unit Tests
@cd bin/Debug

@echo Instrumenting modules...
@vsinstr /coverage Voodoo_Core.dll
@vsinstr /coverage Voodoo_XmlLogger.dll

@echo Running tests....
vsperfcmd /start:coverage /output:results
WinUnit -o results.log .
vsperfcmd /shutdown

@cd ../..
@echo Unit tests complete. Test results in results.log, code coverage in results.coverage. Modules will remain instrumented.
