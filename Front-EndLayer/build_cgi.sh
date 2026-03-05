#!/bin/bash
# build_cgi.sh
# Build and deploy CGI web application
# Author: Claude
# Date: 2026-01-28

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║     WORKOUT TRACKING SYSTEM - CGI BUILD & DEPLOY              ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if Apache is installed
if ! command -v apache2 &>/dev/null; then
    echo -e "${RED}✗ Apache not installed!${NC}"
    echo ""
    echo "Install Apache:"
    echo "  sudo apt-get install apache2"
    exit 1
fi

echo -e "${GREEN}✓ Apache found${NC}"

# Check if CGI module is enabled
if ! apache2ctl -M 2>/dev/null | grep -q "cgi_module"; then
    echo -e "${YELLOW}⚠ CGI module not enabled${NC}"
    echo "Enabling CGI module..."
    sudo a2enmod cgi
    sudo systemctl restart apache2
    echo -e "${GREEN}✓ CGI module enabled${NC}"
else
    echo -e "${GREEN}✓ CGI module enabled${NC}"
fi

# Compile CGI program
echo ""
echo "[1/4] Compiling CGI application..."

g++ -std=c++17 -Isrc -I/usr/include/mysql \
    workout_cgi.cpp \
    ../ServiceLayer/WorkoutService.cpp \
    ../BusinessLayer/*.cpp \
    ../*.cpp \
    -L/usr/lib/x86_64-linux-gnu -lmysqlclient \
    -o workout.cgi

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Compilation successful${NC}"
else
    echo -e "${RED}✗ Compilation failed${NC}"
    exit 1
fi

# Make executable
echo ""
echo "[2/4] Setting permissions..."
chmod +x workout.cgi
echo -e "${GREEN}✓ Permissions set${NC}"

# Deploy to Apache CGI directory
echo ""
echo "[3/4] Deploying to Apache..."
sudo cp workout.cgi /usr/lib/cgi-bin/
sudo chmod +x /usr/lib/cgi-bin/workout.cgi
sudo chown www-data:www-data /usr/lib/cgi-bin/workout.cgi

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Deployed to /usr/lib/cgi-bin/${NC}"
else
    echo -e "${RED}✗ Deployment failed${NC}"
    exit 1
fi

# Restart Apache
echo ""
echo "[4/4] Restarting Apache..."
sudo systemctl restart apache2

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Apache restarted${NC}"
else
    echo -e "${YELLOW}⚠ Apache restart failed${NC}"
fi

# Test the CGI
echo ""
echo "Testing CGI application..."
sleep 1

# Try to access the CGI
if curl -s -o /dev/null -w "%{http_code}" http://localhost/cgi-bin/workout.cgi | grep -q "200"; then
    echo -e "${GREEN}✓ CGI is responding${NC}"
else
    echo -e "${YELLOW}⚠ CGI may not be responding (check Apache logs)${NC}"
fi

# Final summary
echo ""
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                    DEPLOYMENT COMPLETE!                         ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""
echo "Your web application is now available at:"
echo -e "${GREEN}  http://localhost/cgi-bin/workout.cgi${NC}"
echo ""
echo "Test in your browser:"
echo "  firefox http://localhost/cgi-bin/workout.cgi"
echo "  Or open: http://localhost/cgi-bin/workout.cgi"
echo ""
echo "Troubleshooting:"
echo "  View Apache logs: sudo tail -f /var/log/apache2/error.log"
echo "  Test CGI: curl http://localhost/cgi-bin/workout.cgi"
echo "  Restart Apache: sudo systemctl restart apache2"
echo ""
