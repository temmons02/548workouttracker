#!/bin/bash
# Quick Setup Script for Workout Tracking System
# Author: Claude
# Date: 2026-01-28

echo "========================================="
echo "Workout Tracking System - Quick Setup"
echo "========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Database configuration
DB_NAME="workout_tracker"
DB_USER="workout_user"
DB_PASS="workout_pass"
DB_HOST="localhost"

# Function to print colored messages
print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ $1${NC}"
}

# Check if MySQL is installed
echo "Step 1: Checking MySQL installation..."
if command -v mysql &> /dev/null; then
    print_success "MySQL is installed"
else
    print_error "MySQL is not installed"
    print_info "Please install MySQL first:"
    echo "  Ubuntu/Debian: sudo apt-get install mysql-server libmysqlclient-dev"
    echo "  macOS: brew install mysql"
    exit 1
fi

# Check if MySQL is running
echo ""
echo "Step 2: Checking MySQL service..."
if pgrep -x mysqld > /dev/null || pgrep -x mysql > /dev/null; then
    print_success "MySQL service is running"
else
    print_error "MySQL service is not running"
    print_info "Starting MySQL service..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo systemctl start mysql
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        brew services start mysql
    fi
fi

# Check for MySQL development libraries
echo ""
echo "Step 3: Checking MySQL development libraries..."
if [ -f "/usr/include/mysql/mysql.h" ] || [ -f "/usr/local/include/mysql/mysql.h" ]; then
    print_success "MySQL development libraries found"
else
    print_error "MySQL development libraries not found"
    print_info "Installing development libraries..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo apt-get install -y libmysqlclient-dev
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        brew install mysql-client
    fi
fi

# Get MySQL root password
echo ""
echo "Step 4: Setting up database..."
read -sp "Enter MySQL root password (press Enter if no password): " MYSQL_ROOT_PASS
echo ""

# Create database and user
echo ""
print_info "Creating database and user..."

if [ -z "$MYSQL_ROOT_PASS" ]; then
    # No password
    mysql -u root <<EOF
CREATE DATABASE IF NOT EXISTS ${DB_NAME};
CREATE USER IF NOT EXISTS '${DB_USER}'@'${DB_HOST}' IDENTIFIED BY '${DB_PASS}';
GRANT ALL PRIVILEGES ON ${DB_NAME}.* TO '${DB_USER}'@'${DB_HOST}';
FLUSH PRIVILEGES;
EOF
else
    # With password
    mysql -u root -p${MYSQL_ROOT_PASS} <<EOF
CREATE DATABASE IF NOT EXISTS ${DB_NAME};
CREATE USER IF NOT EXISTS '${DB_USER}'@'${DB_HOST}' IDENTIFIED BY '${DB_PASS}';
GRANT ALL PRIVILEGES ON ${DB_NAME}.* TO '${DB_USER}'@'${DB_HOST}';
FLUSH PRIVILEGES;
EOF
fi

if [ $? -eq 0 ]; then
    print_success "Database and user created successfully"
else
    print_error "Failed to create database and user"
    exit 1
fi

# Run SQL scripts
echo ""
echo "Step 5: Creating tables and inserting test data..."

if [ -f "create_tables.sql" ]; then
    mysql -u ${DB_USER} -p${DB_PASS} ${DB_NAME} < create_tables.sql
    if [ $? -eq 0 ]; then
        print_success "Tables created successfully"
    else
        print_error "Failed to create tables"
        exit 1
    fi
else
    print_error "create_tables.sql not found in current directory"
    exit 1
fi

if [ -f "insert_test_data.sql" ]; then
    mysql -u ${DB_USER} -p${DB_PASS} ${DB_NAME} < insert_test_data.sql
    if [ $? -eq 0 ]; then
        print_success "Test data inserted successfully"
    else
        print_error "Failed to insert test data"
        exit 1
    fi
else
    print_error "insert_test_data.sql not found in current directory"
    exit 1
fi

# Verify tables
echo ""
echo "Step 6: Verifying database setup..."
TABLE_COUNT=$(mysql -u ${DB_USER} -p${DB_PASS} ${DB_NAME} -sN -e "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = '${DB_NAME}';")

if [ "$TABLE_COUNT" -eq 5 ]; then
    print_success "All 5 tables created successfully"
else
    print_error "Expected 5 tables, found ${TABLE_COUNT}"
fi

# Show table counts
echo ""
print_info "Data verification:"
mysql -u ${DB_USER} -p${DB_PASS} ${DB_NAME} <<EOF
SELECT 'Workouts' as Table_Name, COUNT(*) as Count FROM Workout
UNION ALL
SELECT 'MuscleGroups', COUNT(*) FROM MuscleGroup
UNION ALL
SELECT 'Nutrition', COUNT(*) FROM Nutrition
UNION ALL
SELECT 'Recovery', COUNT(*) FROM Recovery
UNION ALL
SELECT 'Equipment', COUNT(*) FROM Equipment;
EOF

# Update main.cpp if it exists
echo ""
echo "Step 7: Updating main.cpp connection settings..."
if [ -f "main.cpp" ]; then
    # Create backup
    cp main.cpp main.cpp.backup
    print_info "Created backup: main.cpp.backup"
    
    # Note: This is a simple replacement, might need manual verification
    print_info "Database credentials in main.cpp:"
    echo "  Host: ${DB_HOST}"
    echo "  User: ${DB_USER}"
    echo "  Password: ${DB_PASS}"
    echo "  Database: ${DB_NAME}"
    print_info "Please verify these settings in main.cpp manually"
else
    print_error "main.cpp not found"
fi

# Build the project
echo ""
echo "Step 8: Building the project..."
if [ -f "Makefile" ]; then
    make clean
    make
    if [ $? -eq 0 ]; then
        print_success "Build successful!"
    else
        print_error "Build failed. Check compiler output above."
        exit 1
    fi
else
    print_error "Makefile not found. Please build manually."
    print_info "Manual build command:"
    echo "  g++ -std=c++11 -I/usr/include/mysql main.cpp Workout.cpp MuscleGroup.cpp \\"
    echo "      Nutrition.cpp Recovery.cpp Equipment.cpp WorkoutDAO.cpp \\"
    echo "      -L/usr/lib/x86_64-linux-gnu -lmysqlclient -o workout_tracker"
fi

# Test connection
echo ""
echo "Step 9: Testing database connection..."
mysql -u ${DB_USER} -p${DB_PASS} -h ${DB_HOST} ${DB_NAME} -e "SELECT 'Connection successful!' AS Status;" 2>/dev/null

if [ $? -eq 0 ]; then
    print_success "Database connection test passed!"
else
    print_error "Database connection test failed"
    exit 1
fi

# Summary
echo ""
echo "========================================="
echo "Setup Complete!"
echo "========================================="
print_success "Database: ${DB_NAME}"
print_success "User: ${DB_USER}"
print_success "Password: ${DB_PASS}"
print_success "Tables: 5 (Workout, MuscleGroup, Nutrition, Recovery, Equipment)"
echo ""
print_info "Next steps:"
echo "  1. Verify connection settings in main.cpp"
echo "  2. Run the program: ./workout_tracker"
echo ""
print_info "To reset the database, run:"
echo "  mysql -u ${DB_USER} -p${DB_PASS} ${DB_NAME} -e 'DROP DATABASE ${DB_NAME};'"
echo "  Then run this setup script again"
echo ""
echo "========================================="
