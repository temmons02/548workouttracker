# CGI Web Application - Complete Setup Guide

## Overview

A full-featured **Common Gateway Interface (CGI)** web application that provides a browser-based interface for the Workout Tracking System.

## Features

### CRUD Operations (All Tables)
- ✅ **INSERT** - Create new records
- ✅ **UPDATE** - Modify existing records
- ✅ **DELETE** - Remove records
- ✅ **GET** - View single record
- ✅ **GET ALL** - List all records
- ✅ **GET SUBSET** - Filter/search records

### Supported Tables
1. Workout
2. MuscleGroup
3. Nutrition
4. Recovery
5. Equipment

## Architecture

```
Web Browser
    ↓ HTTP Request
Apache Web Server
    ↓ Executes CGI
workout.cgi (C++ CGI Program)
    ↓ Calls
ServiceLayer/WorkoutService
    ↓
BusinessLayer/WorkoutManager
    ↓
DataLayer/WorkoutDAO
    ↓
MySQL Database
```

## Installation

### Step 1: Install Apache and Enable CGI

```bash
# Install Apache
sudo apt-get update
sudo apt-get install apache2

# Enable CGI module
sudo a2enmod cgi

# Restart Apache
sudo systemctl restart apache2
```

### Step 2: Compile CGI Program

```bash
# Compile the CGI application
g++ -std=c++17 -Isrc -I/usr/include/mysql \
    cgi-bin/workout_cgi.cpp \
    ServiceLayer/WorkoutService.cpp \
    src/models/*.cpp \
    src/data/*.cpp \
    src/business/*.cpp \
    -L/usr/lib/x86_64-linux-gnu -lmysqlclient \
    -o workout.cgi

# Make it executable
chmod +x workout.cgi
```

### Step 3: Deploy to Apache

```bash
# Copy to CGI directory
sudo cp workout.cgi /usr/lib/cgi-bin/

# Set permissions
sudo chmod +x /usr/lib/cgi-bin/workout.cgi
sudo chown www-data:www-data /usr/lib/cgi-bin/workout.cgi
```

### Step 4: Configure Apache (if needed)

Edit Apache configuration:
```bash
sudo nano /etc/apache2/sites-enabled/000-default.conf
```

Ensure it has:
```apache
ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/
<Directory "/usr/lib/cgi-bin">
    AllowOverride None
    Options +ExecCGI
    Require all granted
</Directory>
```

Restart Apache:
```bash
sudo systemctl restart apache2
```

### Step 5: Test the Application

Open browser and navigate to:
```
http://localhost/cgi-bin/workout.cgi
```

## URL Structure

### Main Pages
- `http://localhost/cgi-bin/workout.cgi` - Home page with statistics
- `http://localhost/cgi-bin/workout.cgi?action=list&table=workout` - List workouts
- `http://localhost/cgi-bin/workout.cgi?action=list&table=musclegroup` - List muscle groups

### CRUD Operations

#### INSERT
```
GET  /cgi-bin/workout.cgi?action=insert_form&table=workout
POST /cgi-bin/workout.cgi?action=insert&table=workout&date=2026-03-01&...
```

#### UPDATE
```
GET  /cgi-bin/workout.cgi?action=update_form&table=workout&id=5
POST /cgi-bin/workout.cgi?action=update&table=workout&id=5&date=2026-03-01&...
```

#### DELETE
```
GET  /cgi-bin/workout.cgi?action=delete&table=workout&id=5
```

#### GET (Single Record)
```
GET  /cgi-bin/workout.cgi?action=view&table=workout&id=5
```

#### GET ALL
```
GET  /cgi-bin/workout.cgi?action=list&table=workout
```

#### GET SUBSET (Filtered)
```
GET  /cgi-bin/workout.cgi?action=list&table=workout&filter_date=2026-03-01
GET  /cgi-bin/workout.cgi?action=list&table=nutrition&filter_family=Protein
```

## Screenshots (What You'll See)

### Home Page
```
╔══════════════════════════════════════════════════════════╗
║  🏋️ Workout Tracking System                              ║
╠══════════════════════════════════════════════════════════╣
║  [Home] [Workouts] [Muscle Groups] [Nutrition]          ║
║  [Recovery] [Equipment]                                  ║
╠══════════════════════════════════════════════════════════╣
║  Database Statistics                                     ║
║  ┌─────────┐ ┌─────────┐ ┌─────────┐                   ║
║  │   60    │ │   15    │ │   60    │                   ║
║  │Workouts │ │ Groups  │ │Nutrition│                   ║
║  └─────────┘ └─────────┘ └─────────┘                   ║
║                                                          ║
║  Quick Actions:                                          ║
║  [➕ Add Workout] [➕ Add Muscle Group] [➕ Add Nutrition]║
╚══════════════════════════════════════════════════════════╝
```

### Workout List Page
```
╔══════════════════════════════════════════════════════════╗
║  🏋️ Workouts                         [➕ Add New Workout] ║
╠══════════════════════════════════════════════════════════╣
║  ID │ Date       │ Time  │ Type      │ Cal │ RPE │Actions║
║  ───┼────────────┼───────┼───────────┼─────┼─────┼───────║
║  65 │ 2026-03-01 │ 08:00 │ Cardio    │ 400 │  7  │[Edit] ║
║     │            │       │           │     │     │[Delete║
║  66 │ 2026-03-02 │ 09:00 │ Strength  │ 350 │  8  │[Edit] ║
║     │            │       │           │     │     │[Delete║
╚══════════════════════════════════════════════════════════╝
```

### Insert Form
```
╔══════════════════════════════════════════════════════════╗
║  ➕ Add New Workout                                      ║
╠══════════════════════════════════════════════════════════╣
║  Date:           [2026-03-01        ▼]                  ║
║  Time:           [08:00            ▼]                   ║
║  Duration:       [45           minutes]                 ║
║  Type:           [________________________]              ║
║  Calories:       [400                  ]                ║
║  RPE (1-10):     [7                    ]                ║
║  Muscle Group:   [0 (none)             ]                ║
║                                                          ║
║  [Create Workout] [Cancel]                              ║
╚══════════════════════════════════════════════════════════╝
```

## Features Implementation

### 1. INSERT - All Tables

**Workout:**
- Form validates: date, time, duration, calories, RPE (1-10)
- Calls: `service.insertWorkout(...)`
- Shows success message with new ID

**MuscleGroup:**
- Form validates: name (required), days/week (0-7), sets, reps, weight
- Calls: `service.insertMuscleGroup(...)`

**Nutrition:**
- Form validates: food family, macros (≥0), date
- Dropdown for: Mixed, Fruit, Meat, Vegetable, Dairy
- Calls: `service.insertNutrition(...)`

**Recovery:**
- Form validates: date, duration (>0), type
- Calls: `service.insertRecovery(...)`

**Equipment:**
- Form validates: name, category (required)
- Calls: `service.insertEquipment(...)`

### 2. UPDATE - All Tables

**Process:**
1. Click "Edit" button on any record
2. Form pre-fills with current values
3. User modifies fields
4. Submits → `service.updateWorkout(id, ...)`
5. Shows success/error message

### 3. DELETE - All Tables

**Process:**
1. Click "Delete" button
2. JavaScript confirmation: "Are you sure?"
3. If yes → `service.deleteWorkout(id)`
4. Shows success message
5. Redirects to list

### 4. GET (Single) - All Tables

**URL Pattern:**
```
?action=view&table=workout&id=5
```

**Display:**
- Shows all fields in readable format
- Buttons: [Edit] [Delete] [Back to List]

### 5. GET ALL - All Tables

**Features:**
- Table view with all records
- Sortable columns (click header)
- Pagination (if >50 records)
- Action buttons per row

### 6. GET SUBSET (Filter) - All Tables

**Workout Filters:**
- By date: `?filter_date=2026-03-01`
- By date range: `?filter_start=2026-03-01&filter_end=2026-03-31`
- By muscle group: `?filter_muscle_group=1`
- By RPE: `?filter_rpe_min=8`

**MuscleGroup Filters:**
- By name: `?filter_name=Chest`

**Nutrition Filters:**
- By family: `?filter_family=Protein`
- By date: `?filter_date=2026-03-01`
- By date range: `?filter_start=...&filter_end=...`

**Recovery Filters:**
- By type: `?filter_type=Yoga`
- By date: `?filter_date=2026-03-01`

**Equipment Filters:**
- By category: `?filter_category=Cardio`
- By name: `?filter_name=Treadmill`

## Database Configuration

The CGI program reads database credentials from:
```cpp
WorkoutDAO dao("localhost", "workout_user", "workout_pass", "workout_tracker");
```

To change credentials, edit the CGI source code before compiling.

## Security Considerations

### Input Validation
- ✅ All inputs validated before database operations
- ✅ SQL injection prevented (using prepared statements in DAO)
- ✅ HTML escaping for all output
- ✅ URL encoding for parameters

### Recommended Additional Security
1. **HTTPS** - Use SSL certificate
2. **Authentication** - Add login system
3. **Session Management** - Track user sessions
4. **CSRF Protection** - Add tokens to forms
5. **Rate Limiting** - Prevent abuse

## Troubleshooting

### "500 Internal Server Error"

**Check Apache error log:**
```bash
sudo tail -f /var/log/apache2/error.log
```

**Common causes:**
1. CGI not executable: `chmod +x workout.cgi`
2. Missing libraries: Check compilation
3. Database connection failed: Check credentials
4. Wrong permissions: `chown www-data:www-data workout.cgi`

### "CGI Not Found"

```bash
# Check if CGI module is enabled
sudo a2enmod cgi
sudo systemctl restart apache2

# Check if file exists
ls -l /usr/lib/cgi-bin/workout.cgi
```

### Database Connection Errors

```bash
# Test MySQL connection
mysql -u workout_user -pworkout_pass workout_tracker

# Check if MySQL is running
sudo systemctl status mysql
```

### Permissions Issues

```bash
# Set correct permissions
sudo chown www-data:www-data /usr/lib/cgi-bin/workout.cgi
sudo chmod 755 /usr/lib/cgi-bin/workout.cgi
```

## Performance Optimization

### For Production
1. **Connection Pooling** - Reuse database connections
2. **Caching** - Cache frequently accessed data
3. **FastCGI** - Use FastCGI instead of CGI
4. **Compression** - Enable gzip compression
5. **CDN** - Use CDN for static assets

## Testing

### Test All Operations

```bash
# Test home page
curl http://localhost/cgi-bin/workout.cgi

# Test list
curl "http://localhost/cgi-bin/workout.cgi?action=list&table=workout"

# Test insert (POST)
curl -X POST "http://localhost/cgi-bin/workout.cgi" \
  -d "action=insert&table=workout&date=2026-03-01&time=08:00&duration=45&type=Test&calories=400&rpe=7&muscle_group_id=0"

# Test delete
curl "http://localhost/cgi-bin/workout.cgi?action=delete&table=workout&id=100"
```

## File Structure

```
workout-tracker/
├── cgi-bin/
│   └── workout_cgi.cpp          ← Main CGI program
├── ServiceLayer/
│   ├── WorkoutService.h
│   └── WorkoutService.cpp
├── src/
│   ├── models/
│   ├── data/
│   └── business/
└── /usr/lib/cgi-bin/
    └── workout.cgi              ← Deployed CGI executable
```

## Next Steps

1. ✅ Compile CGI program
2. ✅ Deploy to Apache
3. ✅ Test in browser
4. ✅ Add authentication (optional)
5. ✅ Add HTTPS (optional)
6. ✅ Deploy to production server

## Summary

This CGI web application provides:
- ✅ Full CRUD operations for all 5 tables
- ✅ Web-based interface (no REST API needed)
- ✅ Direct database access via Service Layer
- ✅ Form validation
- ✅ Responsive design
- ✅ Error handling
- ✅ User-friendly interface

**Access your application at:**
```
http://localhost/cgi-bin/workout.cgi
```

🎉 **Your workout tracking system is now web-enabled!**
