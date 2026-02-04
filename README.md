# Workout Tracking System

A comprehensive C++ workout tracking system with MySQL database integration. This system allows you to track workouts, muscle groups, nutrition, recovery sessions, and equipment.

## Features

- **Complete CRUD Operations** for all entities:
  - Workouts (date, time, duration, type, calories, RPE, muscle group)
  - Muscle Groups (name, description, training parameters)
  - Nutrition (food family, macros, water intake)
  - Recovery Sessions (date, duration, type, helpers)
  - Equipment (name, description, category, target)

- **Advanced Queries**:
  - Search by date, muscle group, category, family, type
  - Retrieve related data through foreign keys
  - Filter and sort results

- **Data Validation and Utility Methods**:
  - Calculate total calories from macros
  - Determine high-intensity workouts (RPE ≥ 8)
  - Identify long recovery sessions (> 60 minutes)
  - Check cardio equipment

## Database Schema

### Tables

1. **Workout**
   - workout_id (PK)
   - workout_date, workout_time
   - duration, type_description
   - calories_burned, rate_perceived_exhaustion
   - muscle_group_id (FK)

2. **MuscleGroup**
   - muscle_group_id (PK)
   - name, description
   - days_per_week, sets, reps, weight_amount

3. **Nutrition**
   - nutrition_id (PK)
   - family (ENUM: Mixed, Fruit, Meat, Vegetable, Dairy)
   - water, carbs, fat, protein, sugar
   - meal_date

4. **Recovery**
   - recovery_id (PK)
   - recovery_date, duration
   - type, helpers

5. **Equipment**
   - equipment_id (PK)
   - name, description
   - category, target

## Project Structure

```
workout-tracker/
├── SQL Scripts
│   ├── create_tables.sql       # Database schema definition
│   └── insert_test_data.sql    # Sample data for testing
│
├── C++ Classes
│   ├── Workout.h / Workout.cpp
│   ├── MuscleGroup.h / MuscleGroup.cpp
│   ├── Nutrition.h / Nutrition.cpp
│   ├── Recovery.h / Recovery.cpp
│   └── Equipment.h / Equipment.cpp
│
├── Data Access Layer
│   ├── WorkoutDAO.h            # DAO interface
│   ├── WorkoutDAO.cpp    # Workout operations, MuscleGroup & Nutrition ops, Recovery & Equipment ops
│
├── Application
│   ├── main.cpp                # Demonstration program
│   ├── Makefile                # Build configuration
│   └── README.md               # This file
```

## Prerequisites

### System Requirements
- C++ compiler with C++11 support (g++ recommended)
- MySQL Server 5.7 or higher
- MySQL Client Development Libraries

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential libmysqlclient-dev mysql-server
```

**Fedora/RHEL/CentOS:**
```bash
sudo dnf install gcc-c++ mysql-devel mysql-server
```

**macOS (with Homebrew):**
```bash
brew install mysql mysql-connector-c++
```

## Setup Instructions

### 1. Database Setup

```bash
# Login to MySQL
mysql -u root -p

# Create database
CREATE DATABASE workout_tracker;

# Create user (optional but recommended)
CREATE USER 'workout_user'@'localhost' IDENTIFIED BY 'workout_pass';
GRANT ALL PRIVILEGES ON workout_tracker.* TO 'workout_user'@'localhost';
FLUSH PRIVILEGES;

# Use the database
USE workout_tracker;

# Run schema creation
SOURCE /path/to/create_tables.sql;

# Insert test data
SOURCE /path/to/insert_test_data.sql;
```

### 2. Configure Database Credentials

Edit `main.cpp` and update the database connection parameters:

```cpp
std::string host = "localhost";
std::string user = "workout_user";      // Your MySQL username
std::string password = "workout_pass";  // Your MySQL password
std::string database = "workout_tracker";
int port = 3306;
```

### 3. Build the Project

```bash
# Using make
make

# Or manually with g++
g++ -std=c++11 -I/usr/include/mysql main.cpp Workout.cpp MuscleGroup.cpp \
    Nutrition.cpp Recovery.cpp Equipment.cpp WorkoutDAO_Part1.cpp \
    WorkoutDAO_Part2.cpp WorkoutDAO_Part3.cpp \
    -L/usr/lib/x86_64-linux-gnu -lmysqlclient -o workout_tracker
```

### 4. Run the Program

```bash
./workout_tracker
```

## Usage Examples

### Creating a Workout

```cpp
WorkoutDAO dao("localhost", "user", "pass", "workout_tracker");

Workout workout(0, "2026-01-28", "08:00:00", 60, 
                "Heavy Leg Day", 550.0, 9, 3);
dao.createWorkout(workout);
```

### Reading Workouts

```cpp
// Get all workouts
std::vector<Workout*> allWorkouts = dao.readAllWorkouts();

// Get workouts by date
std::vector<Workout*> todayWorkouts = dao.readWorkoutsByDate("2026-01-28");

// Get workouts by muscle group
std::vector<Workout*> legWorkouts = dao.readWorkoutsByMuscleGroup(3);
```

### Creating Nutrition Entry

```cpp
Nutrition meal(0, FoodFamily::MIXED, 500.0, 45.0, 15.0, 30.0, 8.0, "2026-01-28");
dao.createNutrition(meal);

// Calculate calories
double calories = meal.calculateTotalCalories();
double carbRatio = meal.getMacroRatio("carbs");
```

### Creating Recovery Session

```cpp
Recovery recovery(0, "2026-01-28", 60, "Yoga Session", "Yoga mat, blocks");
dao.createRecovery(recovery);

// Check if it's a long recovery session
bool isLong = recovery.isLongRecovery();  // true if > 60 minutes
```

## WorkoutDAO API Reference

### Workout Operations
- `createWorkout(const Workout& workout)` - Create new workout
- `readWorkout(int id)` - Read single workout by ID
- `readAllWorkouts()` - Read all workouts
- `readWorkoutsByDate(const std::string& date)` - Filter by date
- `readWorkoutsByMuscleGroup(int muscleGroupId)` - Filter by muscle group
- `updateWorkout(const Workout& workout)` - Update existing workout
- `deleteWorkout(int id)` - Delete workout

### MuscleGroup Operations
- `createMuscleGroup(const MuscleGroup& mg)` - Create new muscle group
- `readMuscleGroup(int id)` - Read by ID
- `readAllMuscleGroups()` - Read all
- `readMuscleGroupByName(const std::string& name)` - Find by name
- `updateMuscleGroup(const MuscleGroup& mg)` - Update
- `deleteMuscleGroup(int id)` - Delete

### Nutrition Operations
- `createNutrition(const Nutrition& nutrition)` - Create new entry
- `readNutrition(int id)` - Read by ID
- `readAllNutrition()` - Read all entries
- `readNutritionByDate(const std::string& date)` - Filter by date
- `readNutritionByFamily(const std::string& family)` - Filter by food family
- `updateNutrition(const Nutrition& nutrition)` - Update
- `deleteNutrition(int id)` - Delete

### Recovery Operations
- `createRecovery(const Recovery& recovery)` - Create new session
- `readRecovery(int id)` - Read by ID
- `readAllRecovery()` - Read all sessions
- `readRecoveryByDate(const std::string& date)` - Filter by date
- `readRecoveryByType(const std::string& type)` - Filter by type
- `updateRecovery(const Recovery& recovery)` - Update
- `deleteRecovery(int id)` - Delete

### Equipment Operations
- `createEquipment(const Equipment& equipment)` - Create new equipment
- `readEquipment(int id)` - Read by ID
- `readAllEquipment()` - Read all equipment
- `readEquipmentByCategory(const std::string& category)` - Filter by category
- `readEquipmentByName(const std::string& name)` - Find by name
- `updateEquipment(const Equipment& equipment)` - Update
- `deleteEquipment(int id)` - Delete

## Makefile Targets

- `make` or `make all` - Build the project
- `make clean` - Remove build artifacts
- `make rebuild` - Clean and rebuild
- `make run` - Build and run the program
- `make install-deps` - Install MySQL dependencies (Ubuntu/Debian)
- `make help` - Show help message

## Troubleshooting

### MySQL Connection Errors

**Error: "Can't connect to MySQL server"**
- Ensure MySQL server is running: `sudo systemctl start mysql`
- Check connection parameters in `main.cpp`
- Verify user has proper privileges

**Error: "Access denied for user"**
- Verify username and password
- Grant privileges: `GRANT ALL PRIVILEGES ON workout_tracker.* TO 'user'@'localhost';`

### Compilation Errors

**Error: "mysql.h: No such file or directory"**
- Install MySQL development libraries: `sudo apt-get install libmysqlclient-dev`
- Check include path in Makefile

**Error: "undefined reference to mysql_*"**
- Ensure MySQL client library is linked: `-lmysqlclient`
- Check library path: `-L/usr/lib/x86_64-linux-gnu`

## Future Enhancements

Potential improvements for the system:
- Web interface using REST API
- Mobile app integration
- Data visualization and charts
- Workout plan generator
- Progress tracking and analytics
- Export to CSV/PDF
- Multi-user support with authentication
- Workout history comparison
- Goal setting and achievement tracking

## License

This project is provided as-is for educational and personal use.

## Author
Therin Emmons - Senior Student; Project 'Prompt Engineer'
Claude - Anthropic AI Assistant
Date: January 28, 2026

## Support

For issues or questions, please review the troubleshooting section above or consult the MySQL and C++ documentation.
