#!/bin/bash
# build_rest_api.sh
# Build script for REST API Server and Client
# Author: Claude
# Date: 2026-01-28

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║     WORKOUT TRACKING SYSTEM - REST API BUILD SCRIPT            ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if cpp-httplib is installed
echo "[1/5] Checking dependencies..."
if [ ! -f "/usr/local/include/httplib.h" ]; then
    echo -e "${RED}✗ httplib.h not found!${NC}"
    echo ""
    echo "Installing cpp-httplib..."
    cd /tmp
    if [ -d "cpp-httplib" ]; then
        rm -rf cpp-httplib
    fi
    git clone https://github.com/yhirose/cpp-httplib.git
    sudo cp cpp-httplib/httplib.h /usr/local/include/
    cd - >/dev/null
    echo -e "${GREEN}✓ cpp-httplib installed${NC}"
else
    echo -e "${GREEN}✓ cpp-httplib found${NC}"
fi

# Check MySQL headers
if [ ! -f "/usr/include/mysql/mysql.h" ]; then
    echo -e "${RED}✗ MySQL headers not found!${NC}"
    echo "  Install with: sudo apt-get install libmysqlclient-dev"
    exit 1
else
    echo -e "${GREEN}✓ MySQL headers found${NC}"
fi

echo ""
echo "[2/5] Compiling REST API Server..."
g++ -std=c++17 \
    -I/usr/include/mysql \
    -I/usr/local/include \
    ../ServiceLayer/RestApiServer.cpp \
    ../Workout.cpp \
    ../MuscleGroup.cpp \
    ../Nutrition.cpp \
    ../Recovery.cpp \
    ../Equipment.cpp \
    ../WorkoutDAO.cpp \
    ../BusinessLayer/WorkoutManager.cpp \
    -L/usr/lib/x86_64-linux-gnu \
    -lmysqlclient \
    -lpthread \
    -o rest_api_server

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ REST API Server compiled successfully${NC}"
else
    echo -e "${RED}✗ Failed to compile REST API Server${NC}"
    exit 1
fi

echo ""
echo "[3/5] Compiling API Client..."
g++ -std=c++17 \
    -I/usr/local/include \
    ../ServiceLayer/ApiClient.cpp \
    -lpthread \
    -o api_client

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ API Client compiled successfully${NC}"
else
    echo -e "${RED}✗ Failed to compile API Client${NC}"
    exit 1
fi

echo ""
echo "[4/5] Making executables..."
chmod +x rest_api_server
chmod +x api_client
echo -e "${GREEN}✓ Executables ready${NC}"

echo ""
echo "[5/5] Verifying build..."
ls -lh rest_api_server api_client

echo ""
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                    BUILD SUCCESSFUL!                            ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""
echo "Built executables:"
echo "  • rest_api_server  - REST API service ($(du -h rest_api_server | cut -f1))"
echo "  • api_client       - API test client ($(du -h api_client | cut -f1))"
echo ""
echo "To run:"
echo "  1. Start server:  ./rest_api_server"
echo "  2. Test client:   ./api_client  (in a new terminal)"
echo ""
echo "Test with curl:"
echo "  curl http://localhost:8080/api/workouts"
echo ""
echo "Test in browser:"
echo "  http://localhost:8080"
echo ""
