@echo off
echo Building Linux shared library (.so)...

REM Ensure Docker is running
docker info > nul 2>&1
if %errorlevel% neq 0 (
    echo Error: Docker is not running!
    exit /b 1
)

REM Create directories
if not exist build mkdir build
if not exist resources mkdir resources

REM Create a temporary directory for new build
if not exist resources\temp mkdir resources\temp

REM Build using docker-compose with temporary directory
set TEMP_RESOURCES=resources\temp
docker-compose up --build

REM Move only the needed files to main resources directory
move /Y resources\temp\libred_api.so.1.0 resources\
move /Y resources\temp\libred_api.so.1 resources\
move /Y resources\temp\libjson-c.so.5.3.0 resources\
move /Y resources\temp\libjson-c.so.5 resources\

REM Clean up
rmdir /S /Q resources\temp
docker-compose down

echo.
echo Build complete. Updated Linux shared libraries in ./resources/:
echo  - libred_api.so.1.0    (Main library)
echo  - libred_api.so.1      (Symbolic link)
echo  - libjson-c.so.5.3.0   (Dependency library)
echo  - libjson-c.so.5       (Symbolic link)
echo.
echo Other files in resources directory have been preserved.