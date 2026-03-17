# 🏋️ Workout Tracking System

A comprehensive multi-layer C++ application with MySQL database for tracking workouts, muscle groups, nutrition, recovery sessions, and equipment.

[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![MySQL](https://img.shields.io/badge/MySQL-5.7+-orange.svg)](https://www.mysql.com/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

---

## 📑 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building](#building)
- [Usage](#usage)
- [API Documentation](#api-documentation)
- [Project Structure](#project-structure)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

---

## Overview

This application provides multiple interfaces for managing fitness data:

- **🌐 Web Interface** (CGI) - Browser-based CRUD operations
- **📡 REST API** - JSON endpoints for external integrations
- **💻 Console Applications** - Command-line tools

Built with a **four-layer architecture** ensuring clean separation of concerns, maintainability, and scalability.

## Features

### 🎨 Web Interface

- Modern gradient purple design
- Responsive layout (mobile & desktop)
- Real-time validation
- Intuitive navigation
- Professional styling

### 🔧 Developer Features

- Makefile-based build system
- Modular architecture
- Comprehensive error handling
- Unit tests
- Easy deployment

### 🛡️ Security & Validation

- SQL injection prevention
- HTML escaping
- Multi-layer validation
- Type-safe C++17

---

## Architecture

```
┌─────────────────────────────────────────────┐
│         Front-End Layer (CGI)               │
│     Generates HTML, handles HTTP            │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│         Service Layer (API)                 │
│   Validation, REST endpoints, responses     │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│       Business Layer (Logic)                │
│   Smart save, orchestration, rules          │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│         Data Layer (DAO)                    │
│     SQL queries, ORM, CRUD operations       │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│          MySQL Database                     │
│       5 tables with constraints             │
└─────────────────────────────────────────────┘
```

---

## Prerequisites

### Required

- **OS:** Linux (Ubuntu 20.04+) or WSL
- **Compiler:** g++ with C++17 (GCC 7.0+)
- **Database:** MySQL 5.7+
- **Web Server:** Apache2 with CGI
- **Libraries:**
  - libmysqlclient-dev
  - libcurl4-openssl-dev
  - cpp-httplib (header-only)

### Installation

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y \
    g++ make mysql-server \
    libmysqlclient-dev apache2 \
    libcurl4-openssl-dev

# Enable CGI
sudo a2enmod cgi
sudo systemctl restart apache2

# Install cpp-httplib
sudo wget -O /usr/local/include/httplib.h \
    https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
```

---

## Database Setup

```bash
# 1. Create database
mysql -u root -p
```

```sql
CREATE DATABASE workout_tracker;
CREATE USER 'workout_user'@'localhost' IDENTIFIED BY 'workout_pass';
GRANT ALL PRIVILEGES ON workout_tracker.* TO 'workout_user'@'localhost';
FLUSH PRIVILEGES;
EXIT;
```

```bash
# 2. Initialize schema
mysql -u workout_user -pworkout_pass workout_tracker < create_tables.sql

# 3. Load test data (optional)
mysql -u workout_user -pworkout_pass workout_tracker < insert_test_data.sql

# 4. Verify
mysql -u workout_user -pworkout_pass workout_tracker -e "SHOW TABLES;"
```

---

## Building

### Quick Build

```bash
# Build everything
make all

# Build and deploy
make deploy
```

### Individual Components

```bash
make core         # Main and test apps
make api          # REST API server
make cgi          # CGI web application
make install-cgi  # Deploy CGI to Apache
```

### Useful Targets

```bash
make clean        # Remove build artifacts
make rebuild      # Clean and rebuild
make test         # Run CRUD tests
make help         # Show all targets
```

---

## Usage

### Web Application (CGI)

```bash
# Ensure Apache is running
sudo systemctl start apache2

# Access web interface
firefox http://localhost/cgi-bin/workout.cgi
```

**Available URLs:**

- Home: `http://localhost/cgi-bin/workout.cgi`
- Workouts: `?action=list&table=workout`
- Muscle Groups: `?action=list&table=musclegroup`
- Nutrition: `?action=list&table=nutrition`
- Recovery: `?action=list&table=recovery`
- Equipment: `?action=list&table=equipment`

### 📡 REST API Server

```bash
# Start server
make run-server
# or
./build/rest_api_server
```

**Server runs on:** `http://localhost:8080`

**Test endpoints:**

```bash
# Health check
curl http://localhost:8080/health

# Get all workouts
curl http://localhost:8080/api/workouts

# Get specific workout
curl http://localhost:8080/api/workouts/1

# Create workout
curl -X POST http://localhost:8080/api/workouts \
  -H "Content-Type: application/json" \
  -d '{
    "workout_date": "2026-03-16",
    "workout_time": "08:00:00",
    "duration": 45,
    "type_description": "Morning Run",
    "calories_burned": 400,
    "rate_perceived_exhaustion": 7
  }'

# Delete workout
curl -X DELETE http://localhost:8080/api/workouts/5
```

### Console Applications

```bash
# Main demo
make run-main

# CRUD tests
make run-test

# Console with Service Layer
make run-console
```

---

## API Documentation

### REST Endpoints

#### Health Check

```
GET /health
→ {"status": "healthy", "database": "connected"}
```

#### Workout Operations

```
GET    /api/workouts        # List all
GET    /api/workouts/:id    # Get one
POST   /api/workouts        # Create/Update
DELETE /api/workouts/:id    # Delete
```

#### Similar endpoints for

- `/api/musclegroups`
- `/api/nutrition`
- `/api/recovery`
- `/api/equipment`

### Service Layer API (C++)

```cpp
#include "ServiceLayer/WorkoutService.h"

// Initialize
WorkoutDAO dao("localhost", "workout_user", "workout_pass", "workout_tracker");
WorkoutManager manager(&dao);
WorkoutService service(&manager);

// Insert
ServiceResponse response = service.insertWorkout(
    "2026-03-16", "08:00:00", 45, "Morning Run", 400.0, 7, 0
);

if (response.success) {
    std::cout << "Created ID: " << response.id << std::endl;
}

// Update
response = service.updateWorkout(id, ...);

// Delete
response = service.deleteWorkout(id);

// Get
Workout* workout = service.getWorkout(id);
if (workout) {
    workout->displayInfo();
    delete workout;
}
```

---

## 📂 Project Structure

```
workout-tracker/
|__ Makefile                    # Build system
|__ create_tables.sql          # Database schema
|__ insert_test_data.sql       # Sample data
|__ Workout.h/cpp
|__ MuscleGroup.h/cpp
|__ Nutrition.h/cpp
|__ Recovery.h/cpp
|__ Equipment.h/cpp
|__ WorkoutDAO.h/cpp
|__ main.cpp
|
|__ BusinessLayer/
|   |__ WorkoutManager.h/cpp
|   |__ test_crud_app.cpp
|   |__ view_all_data.sql
|
|__ ServiceLayer/
|  |__ WorkoutService.h/cpp
|  |__ RestApiServer.cpp
|  |__ JsonHelper.h
|  |__ CrudFrontEnd.cpp
|
|__ Front-EndLayer/
|   |__ workout_cgi.cpp        # CGI web application
|
|__ build/
|    |__ workout_tracker        # Main executable
|    |__ test_crud_app         # Test suite
|    |__ rest_api_server       # REST API
|    |__ console_app           # Service Layer console
```

---

## Troubleshooting

### Database Connection Failed

```bash
# Check MySQL is running
sudo systemctl status mysql
sudo systemctl start mysql

# Test connection
mysql -u workout_user -pworkout_pass workout_tracker

# Reset password if needed
mysql -u root -p
ALTER USER 'workout_user'@'localhost' IDENTIFIED BY 'workout_pass';
FLUSH PRIVILEGES;
```

### Compilation Errors

**Error:** `undefined reference to 'mysql_init'`

```bash
sudo apt-get install libmysqlclient-dev
make clean && make all
```

**Error:** `multiple definition of 'main'`

- Your Makefile is compiling multiple programs together
- Use the corrected Makefile that compiles each separately

### CGI 500 Error

```bash
# Check Apache error log
sudo tail -50 /var/log/apache2/error.log

# Fix permissions
sudo chmod +x /usr/lib/cgi-bin/workout.cgi
sudo chown www-data:www-data /usr/lib/cgi-bin/workout.cgi

# Check for missing libraries
ldd /usr/lib/cgi-bin/workout.cgi | grep "not found"
```

**Error:** `malformed header: Bad header`

- Remove `std::cout` from constructors/destructors
- Use `std::cerr` for logging instead
- Check `WorkoutManager.cpp` and `WorkoutService.cpp`

### Apache CGI Not Working

```bash
# Enable CGI module
sudo a2enmod cgi
sudo systemctl restart apache2

# Verify file exists
ls -l /usr/lib/cgi-bin/workout.cgi

# Test manually
sudo -u www-data /usr/lib/cgi-bin/workout.cgi
```

### View Logs

```bash
# Apache error log
sudo tail -f /var/log/apache2/error.log

# Apache access log
sudo tail -f /var/log/apache2/access.log

# MySQL error log
sudo tail -f /var/log/mysql/error.log
```

---

## License

MIT License - see [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- **cpp-httplib** - HTTP library
- **MySQL** - Database system
- **Apache** - Web server

---

---

**Made with C++17**

_Last Updated: March 2026_
