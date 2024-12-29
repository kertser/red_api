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

REM Build using docker-compose
docker-compose up --build

REM Copy the built files
docker cp red_api-builder-1:/app/build/lib/libred_api.so ./resources/
docker cp red_api-builder-1:/app/build/lib/libjson-c.so.5 ./resources/

REM Clean up
docker-compose down

echo Build complete. Libraries are in ./resources/