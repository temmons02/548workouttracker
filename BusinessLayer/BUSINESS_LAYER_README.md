# WorkoutManager Business Layer & CRUD Test Application

This document describes the business layer (WorkoutManager) and the CRUD test application for the Workout Tracking System.

## Overview

The **WorkoutManager** class provides a business logic layer between your application and the database. It handles all CRUD operations for the 5 main objects:

1. **Workout**
2. **MuscleGroup**
3. **Nutrition**
4. **Recovery**
5. **Equipment**

## Architecture

```
┌─────────────────────┐
│  Application Layer  │  (test_crud_app.cpp / main.cpp)
└──────────┬──────────┘
           │
┌──────────▼──────────┐
│  Business Layer     │  (WorkoutManager)
│  - Save methods     │  - Logic & validation
│  - CRUD operations  │  - Business rules
└──────────┬──────────┘
           │
┌──────────▼──────────┐
│  Data Access Layer  │  (WorkoutDAO)
│  - Database calls   │  - SQL queries
└──────────┬──────────┘
           │
┌──────────▼──────────┐
│  Database (MySQL)   │
└─────────────────────┘
```

## Key Features

### Smart Save Methods

Each object has a `save()` method that automatically determines whether to create or update:

```cpp
bool saveWorkout(Workout& workout);
bool saveMuscleGroup(MuscleGroup& muscleGroup);
bool saveNutrition(Nutrition& nutrition);
bool saveRecovery(Recovery& recovery);
bool saveEquipment(Equipment& equipment);
```

**Logic:**
- If `ID == 0` → **CREATE** (insert new record, assign new ID)
- If `ID != 0` → **UPDATE** (modify existing record)

### Example Usage

```cpp
// Create new workout (ID = 0)
Workout workout(0, "2026-02-28", "07:00:00", 60, "Deadlifts", 550.0, 9, 2);
manager.saveWorkout(workout);  // Creates new record
// workout.getWorkoutId() now contains the assigned ID

// Update the workout
workout.setCaloriesBurned(600.0);
manager.saveWorkout(workout);  // Updates existing record
```

## Files

### Business Layer
- **WorkoutManager.h** - Business layer interface
- **WorkoutManager.cpp** - Business layer implementation

### Applications
- **test_crud_app.cpp** - CRUD test application (demonstrates all operations)
- **main.cpp** - Original demonstration program

### Build System
- **Makefile** - Updated to build both applications

## Building the Applications

### Build Both Applications
```bash
make
# or
make all
```

This creates two executables:
- `workout_tracker` - Original demo program
- `test_crud_app` - CRUD test application

### Build Individual Applications
```bash
# Build only the test application
make test-only

# Build only the main application
make main-only
```

## Running the Applications

### CRUD Test Application (Recommended First)

This application demonstrates Create, Read, Update, Delete operations for **all 5 objects**:

```bash
./test_crud_app
```

**What it does:**
1. Tests database connection
2. For each object type (Workout, MuscleGroup, Nutrition, Recovery, Equipment):
   - **CREATE**: Creates a new record (ID = 0)
   - **GET**: Retrieves the record to verify creation
   - **UPDATE**: Modifies the record
   - **GET**: Retrieves again to verify update
   - **DELETE**: Removes the record
   - **GET**: Attempts retrieval to verify deletion

**Expected Output:**
```
╔══════════════════════════════════════════════════════════════════╗
║     WORKOUT TRACKING SYSTEM - CRUD TEST APPLICATION              ║
║     Testing ALL 5 Objects: Create, Update, Delete, Get          ║
╚══════════════════════════════════════════════════════════════════╝

==================================================================
DATABASE CONNECTION TEST
==================================================================

Testing database connection...
Database connection successful!
✓ Database connection successful!

==================================================================
TESTING WORKOUT CRUD OPERATIONS
==================================================================

[1] CREATE NEW WORKOUT
----------------------------------------------------------------------
Creating workout with ID = 0...
[SUCCESS] Created Workout ID: 61
Assigned ID: 61

[2] GET WORKOUT (Verify Creation)
----------------------------------------------------------------------
[SUCCESS] Retrieved Workout ID: 61
✓ Workout retrieved successfully:
=== Workout Information ===
ID: 61
Date: 2026-02-28
Time: 07:00:00
Duration: 60 minutes
Type: Test Heavy Deadlifts
Calories Burned: 550
RPE: 9/10
Muscle Group ID: 2

[3] UPDATE WORKOUT
----------------------------------------------------------------------
Updating calories from 550 to 600.0...
Updating RPE from 9 to 10...
[SUCCESS] Updated Workout ID: 61

[4] GET WORKOUT (Verify Update)
----------------------------------------------------------------------
[SUCCESS] Retrieved Workout ID: 61
✓ Updated workout retrieved successfully:
=== Workout Information ===
ID: 61
Date: 2026-02-28
Time: 07:00:00
Duration: 60 minutes
Type: Test Heavy Deadlifts
Calories Burned: 600
RPE: 10/10
Muscle Group ID: 2

[5] DELETE WORKOUT
----------------------------------------------------------------------
[SUCCESS] Deleted Workout ID: 61

[6] GET WORKOUT (Verify Deletion)
----------------------------------------------------------------------
[FAILED] Retrieve Workout ID: 61
✓ Workout successfully deleted (not found)

✅ WORKOUT CRUD TEST COMPLETED

[... continues for all 5 object types ...]

==================================================================
ALL TESTS COMPLETED SUCCESSFULLY
==================================================================

✅ All 5 object types tested successfully!
✅ Create, Update, Delete, and Get operations verified!

Summary:
  • Workout CRUD      : ✓ PASSED
  • MuscleGroup CRUD  : ✓ PASSED
  • Nutrition CRUD    : ✓ PASSED
  • Recovery CRUD     : ✓ PASSED
  • Equipment CRUD    : ✓ PASSED
```

### Main Demo Application

```bash
./workout_tracker
```

This runs the original demonstration program showing various operations.

## WorkoutManager API Reference

### Workout Methods

```cpp
// Save (create if ID=0, update otherwise)
bool saveWorkout(Workout& workout);

// Retrieve
Workout* getWorkout(int workoutId);
std::vector<Workout*> getAllWorkouts();
std::vector<Workout*> getWorkoutsByDate(const std::string& date);
std::vector<Workout*> getWorkoutsByMuscleGroup(int muscleGroupId);

// Delete
bool deleteWorkout(int workoutId);

// Business logic methods
std::vector<Workout*> getHighIntensityWorkouts();
double getTotalCaloriesBurned(const std::string& startDate, const std::string& endDate);
```

### MuscleGroup Methods

```cpp
bool saveMuscleGroup(MuscleGroup& muscleGroup);
MuscleGroup* getMuscleGroup(int muscleGroupId);
std::vector<MuscleGroup*> getAllMuscleGroups();
MuscleGroup* getMuscleGroupByName(const std::string& name);
bool deleteMuscleGroup(int muscleGroupId);
```

### Nutrition Methods

```cpp
bool saveNutrition(Nutrition& nutrition);
Nutrition* getNutrition(int nutritionId);
std::vector<Nutrition*> getAllNutrition();
std::vector<Nutrition*> getNutritionByDate(const std::string& date);
std::vector<Nutrition*> getNutritionByFamily(const std::string& family);
bool deleteNutrition(int nutritionId);

// Business logic
double getTotalCaloriesForDate(const std::string& date);
double getTotalProteinForDate(const std::string& date);
```

### Recovery Methods

```cpp
bool saveRecovery(Recovery& recovery);
Recovery* getRecovery(int recoveryId);
std::vector<Recovery*> getAllRecovery();
std::vector<Recovery*> getRecoveryByDate(const std::string& date);
std::vector<Recovery*> getRecoveryByType(const std::string& type);
bool deleteRecovery(int recoveryId);

// Business logic
int getTotalRecoveryTime(const std::string& startDate, const std::string& endDate);
```

### Equipment Methods

```cpp
bool saveEquipment(Equipment& equipment);
Equipment* getEquipment(int equipmentId);
std::vector<Equipment*> getAllEquipment();
std::vector<Equipment*> getEquipmentByCategory(const std::string& category);
Equipment* getEquipmentByName(const std::string& name);
bool deleteEquipment(int equipmentId);

// Business logic
std::vector<Equipment*> getCardioEquipment();
```

## Quick Start

1. **Ensure database is set up**
   ```bash
   mysql -u workout_user -pworkout_pass workout_tracker < create_tables.sql
   mysql -u workout_user -pworkout_pass workout_tracker < insert_expanded_test_data.sql
   ```

2. **Update database credentials** in `test_crud_app.cpp` (lines 290-294)

3. **Build the test application**
   ```bash
   make test-only
   ```

4. **Run the tests**
   ```bash
   ./test_crud_app
   ```

## Makefile Targets

```bash
make              # Build both applications
make all          # Same as make
make main-only    # Build only workout_tracker
make test-only    # Build only test_crud_app
make run          # Build and run workout_tracker
make test         # Build and run test_crud_app
make clean        # Remove all build artifacts
make rebuild      # Clean and rebuild everything
make help         # Show help message
```

## Memory Management

**Important:** The `get` methods return dynamically allocated objects. You must delete them when done:

```cpp
// Single object
Workout* workout = manager.getWorkout(1);
// ... use workout ...
delete workout;  // Don't forget!

// Vector of objects
std::vector<Workout*> workouts = manager.getAllWorkouts();
// ... use workouts ...
for (auto* w : workouts) {
    delete w;  // Clean up each one
}
```

Or use the helper method:
```cpp
std::vector<Workout*> workouts = manager.getAllWorkouts();
// ... use workouts ...
manager.cleanupVector(workouts);  // Deletes all and clears vector
```

## Logging

WorkoutManager automatically logs all operations:
- `[SUCCESS]` - Operation completed successfully
- `[FAILED]` - Operation failed
- `[INFO]` - Informational messages (counts, etc.)

## Testing Your Own Code

Use `test_crud_app.cpp` as a template for your own applications:

```cpp
#include "WorkoutManager.h"

int main() {
    // Setup
    WorkoutDAO dao("localhost", "user", "pass", "workout_tracker");
    WorkoutManager manager(&dao);
    
    // Test connection
    if (!manager.testConnection()) {
        return 1;
    }
    
    // Create a new workout
    Workout workout(0, "2026-03-01", "08:00:00", 45, "Bench Press", 400.0, 8, 1);
    manager.saveWorkout(workout);
    
    // Update it
    workout.setCaloriesBurned(450.0);
    manager.saveWorkout(workout);
    
    // Get it
    Workout* retrieved = manager.getWorkout(workout.getWorkoutId());
    retrieved->displayInfo();
    
    // Delete it
    manager.deleteWorkout(workout.getWorkoutId());
    
    // Cleanup
    delete retrieved;
    
    return 0;
}
```

## Troubleshooting

### "undefined reference to WorkoutManager"
```bash
# Make sure WorkoutManager.cpp is being compiled
make clean
make
```

### "cannot find -lmysqlclient"
```bash
# Install MySQL development libraries
sudo apt install libmysqlclient-dev
```

### Database connection fails
- Check credentials in test_crud_app.cpp
- Verify MySQL is running: `sudo systemctl status mysql`
- Test connection: `mysql -u workout_user -pworkout_pass workout_tracker`

## Next Steps

- Extend WorkoutManager with custom business logic
- Add validation rules in save methods
- Implement transaction support
- Add logging to files
- Create reports and analytics methods

---

**Ready to test?** Run `make test` and watch all 5 object types being created, updated, and deleted! 🚀
