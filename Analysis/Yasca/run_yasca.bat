@echo off
echo Starting Yasca analysis...
set SA_HOME=M:\yasca-2.1-cppcheck
cd M:\yasca-2.1
yasca -l 5 --fixes m:\voodooShader\analysis\yasca -o m:\voodooshader\analysis\yasca m:\voodooshader
cd M:\VoodooShader\Analysis\Yasca