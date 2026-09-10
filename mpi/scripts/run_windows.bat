@echo off
setlocal

if "%~1"=="" goto usage
if "%~2"=="" goto usage

set "BENCHMARK=%~1"
set "OPERATIONS=%~2"
set "REPETITIONS=5"

if not "%~3"=="" set "REPETITIONS=%~3"

for /L %%N in (1,1,20) do (
    echo Running %%N MPI processes
    mpiexec -n %%N "%BENCHMARK%" %OPERATIONS% %REPETITIONS%
)

for %%N in (60 120 240) do (
    echo Running oversubscription test with %%N MPI processes
    mpiexec -n %%N "%BENCHMARK%" %OPERATIONS% %REPETITIONS%
)

exit /b 0

:usage
echo Usage: run_windows.bat BENCHMARK_PATH OPERATIONS [REPETITIONS]
exit /b 1
