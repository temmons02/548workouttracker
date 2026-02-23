// WorkoutDAO.cpp
// Workout Tracking System - Workout Class Implementation
// Author: Claude, Therin Emmons
// Date: 2026-01-28
#include "WorkoutDAO.h"
#include <iostream>
#include <cstring>

// Constructor
WorkoutDAO::WorkoutDAO(const std::string& h, const std::string& u,
                       const std::string& p, const std::string& db, int pt)
    : host(h), user(u), password(p), database(db), port(pt), connection(nullptr) {
    connection = mysql_init(nullptr);
    if (!connection) {
        std::cerr << "MySQL initialization failed!" << std::endl;
    }
}

// Destructor
WorkoutDAO::~WorkoutDAO() {
    disconnect();
}

// Helper method to connect to database
// Needed to modify the connection, if already connected
bool WorkoutDAO::connect() {
    // Check if already connected - if so, just return true
    if (connection && mysql_ping(connection) == 0) {
        return true;  // Already connected and alive
    }
    
    // Initialize if not already initialized
    if (!connection) {
        connection = mysql_init(nullptr);
        if (!connection) {
            std::cerr << "MySQL initialization failed!" << std::endl;
            return false;
        }
    }
    
    // Only connect if not already connected
    if (!mysql_real_connect(connection, host.c_str(), user.c_str(),
                           password.c_str(), database.c_str(), port, nullptr, 0)) {
        handleError("Connection");
        return false;
    }
    
    return true;
}

// Helper method to disconnect from database
void WorkoutDAO::disconnect() {
    if (connection) {
        mysql_close(connection);
        connection = nullptr;
    }
}

// Helper method to handle errors
void WorkoutDAO::handleError(const std::string& operation) {
    std::cerr << operation << " Error: " << mysql_error(connection) << std::endl;
}

// Test database connection
bool WorkoutDAO::testConnection() {
    if (!connect()) {
        return false;
    }
    std::cout << "Database connection successful!" << std::endl;
    return true;
}

// Get last inserted ID
int WorkoutDAO::getLastInsertId() {
    return static_cast<int>(mysql_insert_id(connection));
}

// ==================== WORKOUT CRUD OPERATIONS ====================

// Create Workout
bool WorkoutDAO::createWorkout(const Workout& workout) {
    if (!connect()) return false;
    
    std::string query = "INSERT INTO Workout (workout_date, workout_time, duration, "
                       "type_description, calories_burned, rate_perceived_exhaustion, muscle_group_id) "
                       "VALUES ('" + workout.getWorkoutDate() + "', '" + workout.getWorkoutTime() + "', "
                       + std::to_string(workout.getDuration()) + ", '" + workout.getTypeDescription() + "', "
                       + std::to_string(workout.getCaloriesBurned()) + ", "
                       + std::to_string(workout.getRatePerceivedExhaustion()) + ", ";
    
    if (workout.getMuscleGroupId() > 0) {
        query += std::to_string(workout.getMuscleGroupId()) + ")";
    } else {
        query += "NULL)";
    }
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Create Workout");
        return false;
    }
    
    std::cout << "Workout created successfully with ID: " << getLastInsertId() << std::endl;
    return true;
}

// Read single Workout by ID
Workout* WorkoutDAO::readWorkout(int workoutId) {
    if (!connect()) return nullptr;
    
    std::string query = "SELECT * FROM Workout WHERE workout_id = " + std::to_string(workoutId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Workout");
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nullptr;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    Workout* workout = nullptr;
    
    if (row) {
        workout = new Workout();
        workout->setWorkoutId(std::stoi(row[0]));
        workout->setWorkoutDate(row[1]);
        workout->setWorkoutTime(row[2]);
        workout->setDuration(std::stoi(row[3]));
        workout->setTypeDescription(row[4] ? row[4] : "");
        workout->setCaloriesBurned(row[5] ? std::stod(row[5]) : 0.0);
        workout->setRatePerceivedExhaustion(row[6] ? std::stoi(row[6]) : 0);
        workout->setMuscleGroupId(row[7] ? std::stoi(row[7]) : 0);
    }
    
    mysql_free_result(result);
    return workout;
}

// Read all Workouts
std::vector<Workout*> WorkoutDAO::readAllWorkouts() {
    std::vector<Workout*> workouts;
    if (!connect()) return workouts;
    
    std::string query = "SELECT * FROM Workout ORDER BY workout_date DESC, workout_time DESC";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read All Workouts");
        return workouts;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return workouts;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Workout* workout = new Workout();
        workout->setWorkoutId(std::stoi(row[0]));
        workout->setWorkoutDate(row[1]);
        workout->setWorkoutTime(row[2]);
        workout->setDuration(std::stoi(row[3]));
        workout->setTypeDescription(row[4] ? row[4] : "");
        workout->setCaloriesBurned(row[5] ? std::stod(row[5]) : 0.0);
        workout->setRatePerceivedExhaustion(row[6] ? std::stoi(row[6]) : 0);
        workout->setMuscleGroupId(row[7] ? std::stoi(row[7]) : 0);
        workouts.push_back(workout);
    }
    
    mysql_free_result(result);
    return workouts;
}

// Read Workouts by date
std::vector<Workout*> WorkoutDAO::readWorkoutsByDate(const std::string& date) {
    std::vector<Workout*> workouts;
    if (!connect()) return workouts;
    
    std::string query = "SELECT * FROM Workout WHERE workout_date = '" + date + "'";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Workouts by Date");
        return workouts;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return workouts;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Workout* workout = new Workout();
        workout->setWorkoutId(std::stoi(row[0]));
        workout->setWorkoutDate(row[1]);
        workout->setWorkoutTime(row[2]);
        workout->setDuration(std::stoi(row[3]));
        workout->setTypeDescription(row[4] ? row[4] : "");
        workout->setCaloriesBurned(row[5] ? std::stod(row[5]) : 0.0);
        workout->setRatePerceivedExhaustion(row[6] ? std::stoi(row[6]) : 0);
        workout->setMuscleGroupId(row[7] ? std::stoi(row[7]) : 0);
        workouts.push_back(workout);
    }
    
    mysql_free_result(result);
    return workouts;
}

// Read Workouts by muscle group
std::vector<Workout*> WorkoutDAO::readWorkoutsByMuscleGroup(int muscleGroupId) {
    std::vector<Workout*> workouts;
    if (!connect()) return workouts;
    
    std::string query = "SELECT * FROM Workout WHERE muscle_group_id = " + std::to_string(muscleGroupId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Workouts by Muscle Group");
        return workouts;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return workouts;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Workout* workout = new Workout();
        workout->setWorkoutId(std::stoi(row[0]));
        workout->setWorkoutDate(row[1]);
        workout->setWorkoutTime(row[2]);
        workout->setDuration(std::stoi(row[3]));
        workout->setTypeDescription(row[4] ? row[4] : "");
        workout->setCaloriesBurned(row[5] ? std::stod(row[5]) : 0.0);
        workout->setRatePerceivedExhaustion(row[6] ? std::stoi(row[6]) : 0);
        workout->setMuscleGroupId(row[7] ? std::stoi(row[7]) : 0);
        workouts.push_back(workout);
    }
    
    mysql_free_result(result);
    return workouts;
}

// Update Workout
bool WorkoutDAO::updateWorkout(const Workout& workout) {
    if (!connect()) return false;
    
    std::string query = "UPDATE Workout SET "
                       "workout_date = '" + workout.getWorkoutDate() + "', "
                       "workout_time = '" + workout.getWorkoutTime() + "', "
                       "duration = " + std::to_string(workout.getDuration()) + ", "
                       "type_description = '" + workout.getTypeDescription() + "', "
                       "calories_burned = " + std::to_string(workout.getCaloriesBurned()) + ", "
                       "rate_perceived_exhaustion = " + std::to_string(workout.getRatePerceivedExhaustion()) + ", "
                       "muscle_group_id = ";
    
    if (workout.getMuscleGroupId() > 0) {
        query += std::to_string(workout.getMuscleGroupId());
    } else {
        query += "NULL";
    }
    
    query += " WHERE workout_id = " + std::to_string(workout.getWorkoutId());
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Update Workout");
        return false;
    }
    
    std::cout << "Workout updated successfully!" << std::endl;
    return true;
}

// Delete Workout
bool WorkoutDAO::deleteWorkout(int workoutId) {
    if (!connect()) return false;
    
    std::string query = "DELETE FROM Workout WHERE workout_id = " + std::to_string(workoutId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Delete Workout");
        return false;
    }
    
    std::cout << "Workout deleted successfully!" << std::endl;
    return true;
}

// ==================== MUSCLEGROUP CRUD OPERATIONS ====================

// Create MuscleGroup
bool WorkoutDAO::createMuscleGroup(const MuscleGroup& muscleGroup) {
    if (!connect()) return false;
    
    std::string query = "INSERT INTO MuscleGroup (name, description, days_per_week, "
                       "sets, reps, weight_amount) VALUES ('"
                       + muscleGroup.getName() + "', '" + muscleGroup.getDescription() + "', "
                       + std::to_string(muscleGroup.getDaysPerWeek()) + ", "
                       + std::to_string(muscleGroup.getSets()) + ", "
                       + std::to_string(muscleGroup.getReps()) + ", "
                       + std::to_string(muscleGroup.getWeightAmount()) + ")";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Create MuscleGroup");
        return false;
    }
    
    std::cout << "MuscleGroup created successfully with ID: " << getLastInsertId() << std::endl;
    return true;
}

// Read single MuscleGroup by ID
MuscleGroup* WorkoutDAO::readMuscleGroup(int muscleGroupId) {
    if (!connect()) return nullptr;
    
    std::string query = "SELECT * FROM MuscleGroup WHERE muscle_group_id = " + std::to_string(muscleGroupId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read MuscleGroup");
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nullptr;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    MuscleGroup* muscleGroup = nullptr;
    
    if (row) {
        muscleGroup = new MuscleGroup();
        muscleGroup->setMuscleGroupId(std::stoi(row[0]));
        muscleGroup->setName(row[1]);
        muscleGroup->setDescription(row[2] ? row[2] : "");
        muscleGroup->setDaysPerWeek(row[3] ? std::stoi(row[3]) : 0);
        muscleGroup->setSets(row[4] ? std::stoi(row[4]) : 0);
        muscleGroup->setReps(row[5] ? std::stoi(row[5]) : 0);
        muscleGroup->setWeightAmount(row[6] ? std::stod(row[6]) : 0.0);
    }
    
    mysql_free_result(result);
    return muscleGroup;
}

// Read all MuscleGroups
std::vector<MuscleGroup*> WorkoutDAO::readAllMuscleGroups() {
    std::vector<MuscleGroup*> muscleGroups;
    if (!connect()) return muscleGroups;
    
    std::string query = "SELECT * FROM MuscleGroup ORDER BY name";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read All MuscleGroups");
        return muscleGroups;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return muscleGroups;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        MuscleGroup* muscleGroup = new MuscleGroup();
        muscleGroup->setMuscleGroupId(std::stoi(row[0]));
        muscleGroup->setName(row[1]);
        muscleGroup->setDescription(row[2] ? row[2] : "");
        muscleGroup->setDaysPerWeek(row[3] ? std::stoi(row[3]) : 0);
        muscleGroup->setSets(row[4] ? std::stoi(row[4]) : 0);
        muscleGroup->setReps(row[5] ? std::stoi(row[5]) : 0);
        muscleGroup->setWeightAmount(row[6] ? std::stod(row[6]) : 0.0);
        muscleGroups.push_back(muscleGroup);
    }
    
    mysql_free_result(result);
    return muscleGroups;
}

// Read MuscleGroup by name
MuscleGroup* WorkoutDAO::readMuscleGroupByName(const std::string& name) {
    if (!connect()) return nullptr;
    
    std::string query = "SELECT * FROM MuscleGroup WHERE name = '" + name + "'";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read MuscleGroup by Name");
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nullptr;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    MuscleGroup* muscleGroup = nullptr;
    
    if (row) {
        muscleGroup = new MuscleGroup();
        muscleGroup->setMuscleGroupId(std::stoi(row[0]));
        muscleGroup->setName(row[1]);
        muscleGroup->setDescription(row[2] ? row[2] : "");
        muscleGroup->setDaysPerWeek(row[3] ? std::stoi(row[3]) : 0);
        muscleGroup->setSets(row[4] ? std::stoi(row[4]) : 0);
        muscleGroup->setReps(row[5] ? std::stoi(row[5]) : 0);
        muscleGroup->setWeightAmount(row[6] ? std::stod(row[6]) : 0.0);
    }
    
    mysql_free_result(result);
    return muscleGroup;
}

// Update MuscleGroup
bool WorkoutDAO::updateMuscleGroup(const MuscleGroup& muscleGroup) {
    if (!connect()) return false;
    
    std::string query = "UPDATE MuscleGroup SET "
                       "name = '" + muscleGroup.getName() + "', "
                       "description = '" + muscleGroup.getDescription() + "', "
                       "days_per_week = " + std::to_string(muscleGroup.getDaysPerWeek()) + ", "
                       "sets = " + std::to_string(muscleGroup.getSets()) + ", "
                       "reps = " + std::to_string(muscleGroup.getReps()) + ", "
                       "weight_amount = " + std::to_string(muscleGroup.getWeightAmount()) +
                       " WHERE muscle_group_id = " + std::to_string(muscleGroup.getMuscleGroupId());
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Update MuscleGroup");
        return false;
    }
    
    std::cout << "MuscleGroup updated successfully!" << std::endl;
    return true;
}

// Delete MuscleGroup
bool WorkoutDAO::deleteMuscleGroup(int muscleGroupId) {
    if (!connect()) return false;
    
    std::string query = "DELETE FROM MuscleGroup WHERE muscle_group_id = " + std::to_string(muscleGroupId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Delete MuscleGroup");
        return false;
    }
    
    std::cout << "MuscleGroup deleted successfully!" << std::endl;
    return true;
}

// ==================== NUTRITION CRUD OPERATIONS ====================

// Create Nutrition
bool WorkoutDAO::createNutrition(const Nutrition& nutrition) {
    if (!connect()) return false;
    
    std::string query = "INSERT INTO Nutrition (family, water, carbs, fat, protein, sugar, meal_date) "
                       "VALUES ('" + nutrition.getFamilyString() + "', "
                       + std::to_string(nutrition.getWater()) + ", "
                       + std::to_string(nutrition.getCarbs()) + ", "
                       + std::to_string(nutrition.getFat()) + ", "
                       + std::to_string(nutrition.getProtein()) + ", "
                       + std::to_string(nutrition.getSugar()) + ", ";
    
    if (!nutrition.getMealDate().empty()) {
        query += "'" + nutrition.getMealDate() + "')";
    } else {
        query += "NULL)";
    }
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Create Nutrition");
        return false;
    }
    
    std::cout << "Nutrition entry created successfully with ID: " << getLastInsertId() << std::endl;
    return true;
}

// Read single Nutrition by ID
Nutrition* WorkoutDAO::readNutrition(int nutritionId) {
    if (!connect()) return nullptr;
    
    std::string query = "SELECT * FROM Nutrition WHERE nutrition_id = " + std::to_string(nutritionId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Nutrition");
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nullptr;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    Nutrition* nutrition = nullptr;
    
    if (row) {
        nutrition = new Nutrition();
        nutrition->setNutritionId(std::stoi(row[0]));
        nutrition->setFamilyFromString(row[1]);
        nutrition->setWater(row[2] ? std::stod(row[2]) : 0.0);
        nutrition->setCarbs(row[3] ? std::stod(row[3]) : 0.0);
        nutrition->setFat(row[4] ? std::stod(row[4]) : 0.0);
        nutrition->setProtein(row[5] ? std::stod(row[5]) : 0.0);
        nutrition->setSugar(row[6] ? std::stod(row[6]) : 0.0);
        nutrition->setMealDate(row[7] ? row[7] : "");
    }
    
    mysql_free_result(result);
    return nutrition;
}

// Read all Nutrition entries
std::vector<Nutrition*> WorkoutDAO::readAllNutrition() {
    std::vector<Nutrition*> nutritionList;
    if (!connect()) return nutritionList;
    
    std::string query = "SELECT * FROM Nutrition ORDER BY meal_date DESC";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read All Nutrition");
        return nutritionList;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nutritionList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Nutrition* nutrition = new Nutrition();
        nutrition->setNutritionId(std::stoi(row[0]));
        nutrition->setFamilyFromString(row[1]);
        nutrition->setWater(row[2] ? std::stod(row[2]) : 0.0);
        nutrition->setCarbs(row[3] ? std::stod(row[3]) : 0.0);
        nutrition->setFat(row[4] ? std::stod(row[4]) : 0.0);
        nutrition->setProtein(row[5] ? std::stod(row[5]) : 0.0);
        nutrition->setSugar(row[6] ? std::stod(row[6]) : 0.0);
        nutrition->setMealDate(row[7] ? row[7] : "");
        nutritionList.push_back(nutrition);
    }
    
    mysql_free_result(result);
    return nutritionList;
}

// Read Nutrition by date
std::vector<Nutrition*> WorkoutDAO::readNutritionByDate(const std::string& date) {
    std::vector<Nutrition*> nutritionList;
    if (!connect()) return nutritionList;
    
    std::string query = "SELECT * FROM Nutrition WHERE meal_date = '" + date + "'";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Nutrition by Date");
        return nutritionList;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nutritionList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Nutrition* nutrition = new Nutrition();
        nutrition->setNutritionId(std::stoi(row[0]));
        nutrition->setFamilyFromString(row[1]);
        nutrition->setWater(row[2] ? std::stod(row[2]) : 0.0);
        nutrition->setCarbs(row[3] ? std::stod(row[3]) : 0.0);
        nutrition->setFat(row[4] ? std::stod(row[4]) : 0.0);
        nutrition->setProtein(row[5] ? std::stod(row[5]) : 0.0);
        nutrition->setSugar(row[6] ? std::stod(row[6]) : 0.0);
        nutrition->setMealDate(row[7] ? row[7] : "");
        nutritionList.push_back(nutrition);
    }
    
    mysql_free_result(result);
    return nutritionList;
}

// Read Nutrition by family
std::vector<Nutrition*> WorkoutDAO::readNutritionByFamily(const std::string& family) {
    std::vector<Nutrition*> nutritionList;
    if (!connect()) return nutritionList;
    
    std::string query = "SELECT * FROM Nutrition WHERE family = '" + family + "'";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Nutrition by Family");
        return nutritionList;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nutritionList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Nutrition* nutrition = new Nutrition();
        nutrition->setNutritionId(std::stoi(row[0]));
        nutrition->setFamilyFromString(row[1]);
        nutrition->setWater(row[2] ? std::stod(row[2]) : 0.0);
        nutrition->setCarbs(row[3] ? std::stod(row[3]) : 0.0);
        nutrition->setFat(row[4] ? std::stod(row[4]) : 0.0);
        nutrition->setProtein(row[5] ? std::stod(row[5]) : 0.0);
        nutrition->setSugar(row[6] ? std::stod(row[6]) : 0.0);
        nutrition->setMealDate(row[7] ? row[7] : "");
        nutritionList.push_back(nutrition);
    }
    
    mysql_free_result(result);
    return nutritionList;
}

// Update Nutrition
bool WorkoutDAO::updateNutrition(const Nutrition& nutrition) {
    if (!connect()) return false;
    
    std::string query = "UPDATE Nutrition SET "
                       "family = '" + nutrition.getFamilyString() + "', "
                       "water = " + std::to_string(nutrition.getWater()) + ", "
                       "carbs = " + std::to_string(nutrition.getCarbs()) + ", "
                       "fat = " + std::to_string(nutrition.getFat()) + ", "
                       "protein = " + std::to_string(nutrition.getProtein()) + ", "
                       "sugar = " + std::to_string(nutrition.getSugar()) + ", "
                       "meal_date = ";
    
    if (!nutrition.getMealDate().empty()) {
        query += "'" + nutrition.getMealDate() + "'";
    } else {
        query += "NULL";
    }
    
    query += " WHERE nutrition_id = " + std::to_string(nutrition.getNutritionId());
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Update Nutrition");
        return false;
    }
    
    std::cout << "Nutrition entry updated successfully!" << std::endl;
    return true;
}

// Delete Nutrition
bool WorkoutDAO::deleteNutrition(int nutritionId) {
    if (!connect()) return false;
    
    std::string query = "DELETE FROM Nutrition WHERE nutrition_id = " + std::to_string(nutritionId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Delete Nutrition");
        return false;
    }
    
    std::cout << "Nutrition entry deleted successfully!" << std::endl;
    return true;
}


// ==================== RECOVERY CRUD OPERATIONS ====================

// Create Recovery
bool WorkoutDAO::createRecovery(const Recovery& recovery) {
    if (!connect()) return false;
    
    std::string query = "INSERT INTO Recovery (recovery_date, duration, type, helpers) "
                       "VALUES ('" + recovery.getRecoveryDate() + "', "
                       + std::to_string(recovery.getDuration()) + ", '"
                       + recovery.getType() + "', '" + recovery.getHelpers() + "')";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Create Recovery");
        return false;
    }
    
    std::cout << "Recovery entry created successfully with ID: " << getLastInsertId() << std::endl;
    return true;
}

// Read single Recovery by ID
Recovery* WorkoutDAO::readRecovery(int recoveryId) {
    if (!connect()) return nullptr;
    
    std::string query = "SELECT * FROM Recovery WHERE recovery_id = " + std::to_string(recoveryId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Recovery");
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nullptr;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    Recovery* recovery = nullptr;
    
    if (row) {
        recovery = new Recovery();
        recovery->setRecoveryId(std::stoi(row[0]));
        recovery->setRecoveryDate(row[1]);
        recovery->setDuration(std::stoi(row[2]));
        recovery->setType(row[3] ? row[3] : "");
        recovery->setHelpers(row[4] ? row[4] : "");
    }
    
    mysql_free_result(result);
    return recovery;
}

// Read all Recovery entries
std::vector<Recovery*> WorkoutDAO::readAllRecovery() {
    std::vector<Recovery*> recoveryList;
    if (!connect()) return recoveryList;
    
    std::string query = "SELECT * FROM Recovery ORDER BY recovery_date DESC";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read All Recovery");
        return recoveryList;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return recoveryList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Recovery* recovery = new Recovery();
        recovery->setRecoveryId(std::stoi(row[0]));
        recovery->setRecoveryDate(row[1]);
        recovery->setDuration(std::stoi(row[2]));
        recovery->setType(row[3] ? row[3] : "");
        recovery->setHelpers(row[4] ? row[4] : "");
        recoveryList.push_back(recovery);
    }
    
    mysql_free_result(result);
    return recoveryList;
}

// Read Recovery by date
std::vector<Recovery*> WorkoutDAO::readRecoveryByDate(const std::string& date) {
    std::vector<Recovery*> recoveryList;
    if (!connect()) return recoveryList;
    
    std::string query = "SELECT * FROM Recovery WHERE recovery_date = '" + date + "'";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Recovery by Date");
        return recoveryList;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return recoveryList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Recovery* recovery = new Recovery();
        recovery->setRecoveryId(std::stoi(row[0]));
        recovery->setRecoveryDate(row[1]);
        recovery->setDuration(std::stoi(row[2]));
        recovery->setType(row[3] ? row[3] : "");
        recovery->setHelpers(row[4] ? row[4] : "");
        recoveryList.push_back(recovery);
    }
    
    mysql_free_result(result);
    return recoveryList;
}

// Read Recovery by type
std::vector<Recovery*> WorkoutDAO::readRecoveryByType(const std::string& type) {
    std::vector<Recovery*> recoveryList;
    if (!connect()) return recoveryList;
    
    std::string query = "SELECT * FROM Recovery WHERE type = '" + type + "'";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Recovery by Type");
        return recoveryList;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return recoveryList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Recovery* recovery = new Recovery();
        recovery->setRecoveryId(std::stoi(row[0]));
        recovery->setRecoveryDate(row[1]);
        recovery->setDuration(std::stoi(row[2]));
        recovery->setType(row[3] ? row[3] : "");
        recovery->setHelpers(row[4] ? row[4] : "");
        recoveryList.push_back(recovery);
    }
    
    mysql_free_result(result);
    return recoveryList;
}

// Update Recovery
bool WorkoutDAO::updateRecovery(const Recovery& recovery) {
    if (!connect()) return false;
    
    std::string query = "UPDATE Recovery SET "
                       "recovery_date = '" + recovery.getRecoveryDate() + "', "
                       "duration = " + std::to_string(recovery.getDuration()) + ", "
                       "type = '" + recovery.getType() + "', "
                       "helpers = '" + recovery.getHelpers() + "' "
                       "WHERE recovery_id = " + std::to_string(recovery.getRecoveryId());
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Update Recovery");
        return false;
    }
    
    std::cout << "Recovery entry updated successfully!" << std::endl;
    return true;
}

// Delete Recovery
bool WorkoutDAO::deleteRecovery(int recoveryId) {
    if (!connect()) return false;
    
    std::string query = "DELETE FROM Recovery WHERE recovery_id = " + std::to_string(recoveryId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Delete Recovery");
        return false;
    }
    
    std::cout << "Recovery entry deleted successfully!" << std::endl;
    return true;
}

// ==================== EQUIPMENT CRUD OPERATIONS ====================

// Create Equipment
bool WorkoutDAO::createEquipment(const Equipment& equipment) {
    if (!connect()) return false;
    
    std::string query = "INSERT INTO Equipment (name, description, category, target) "
                       "VALUES ('" + equipment.getName() + "', '"
                       + equipment.getDescription() + "', '"
                       + equipment.getCategory() + "', '"
                       + equipment.getTarget() + "')";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Create Equipment");
        return false;
    }
    
    std::cout << "Equipment created successfully with ID: " << getLastInsertId() << std::endl;
    return true;
}

// Read single Equipment by ID
Equipment* WorkoutDAO::readEquipment(int equipmentId) {
    if (!connect()) return nullptr;
    
    std::string query = "SELECT * FROM Equipment WHERE equipment_id = " + std::to_string(equipmentId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Equipment");
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nullptr;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    Equipment* equipment = nullptr;
    
    if (row) {
        equipment = new Equipment();
        equipment->setEquipmentId(std::stoi(row[0]));
        equipment->setName(row[1]);
        equipment->setDescription(row[2] ? row[2] : "");
        equipment->setCategory(row[3] ? row[3] : "");
        equipment->setTarget(row[4] ? row[4] : "");
    }
    
    mysql_free_result(result);
    return equipment;
}

// Read all Equipment
std::vector<Equipment*> WorkoutDAO::readAllEquipment() {
    std::vector<Equipment*> equipmentList;
    if (!connect()) return equipmentList;
    
    std::string query = "SELECT * FROM Equipment ORDER BY name";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read All Equipment");
        return equipmentList;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return equipmentList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Equipment* equipment = new Equipment();
        equipment->setEquipmentId(std::stoi(row[0]));
        equipment->setName(row[1]);
        equipment->setDescription(row[2] ? row[2] : "");
        equipment->setCategory(row[3] ? row[3] : "");
        equipment->setTarget(row[4] ? row[4] : "");
        equipmentList.push_back(equipment);
    }
    
    mysql_free_result(result);
    return equipmentList;
}

// Read Equipment by category
std::vector<Equipment*> WorkoutDAO::readEquipmentByCategory(const std::string& category) {
    std::vector<Equipment*> equipmentList;
    if (!connect()) return equipmentList;
    
    std::string query = "SELECT * FROM Equipment WHERE category = '" + category + "'";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Equipment by Category");
        return equipmentList;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return equipmentList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Equipment* equipment = new Equipment();
        equipment->setEquipmentId(std::stoi(row[0]));
        equipment->setName(row[1]);
        equipment->setDescription(row[2] ? row[2] : "");
        equipment->setCategory(row[3] ? row[3] : "");
        equipment->setTarget(row[4] ? row[4] : "");
        equipmentList.push_back(equipment);
    }
    
    mysql_free_result(result);
    return equipmentList;
}

// Read Equipment by name
Equipment* WorkoutDAO::readEquipmentByName(const std::string& name) {
    if (!connect()) return nullptr;
    
    std::string query = "SELECT * FROM Equipment WHERE name = '" + name + "'";
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Read Equipment by Name");
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (!result) return nullptr;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    Equipment* equipment = nullptr;
    
    if (row) {
        equipment = new Equipment();
        equipment->setEquipmentId(std::stoi(row[0]));
        equipment->setName(row[1]);
        equipment->setDescription(row[2] ? row[2] : "");
        equipment->setCategory(row[3] ? row[3] : "");
        equipment->setTarget(row[4] ? row[4] : "");
    }
    
    mysql_free_result(result);
    return equipment;
}

// Update Equipment
bool WorkoutDAO::updateEquipment(const Equipment& equipment) {
    if (!connect()) return false;
    
    std::string query = "UPDATE Equipment SET "
                       "name = '" + equipment.getName() + "', "
                       "description = '" + equipment.getDescription() + "', "
                       "category = '" + equipment.getCategory() + "', "
                       "target = '" + equipment.getTarget() + "' "
                       "WHERE equipment_id = " + std::to_string(equipment.getEquipmentId());
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Update Equipment");
        return false;
    }
    
    std::cout << "Equipment updated successfully!" << std::endl;
    return true;
}

// Delete Equipment
bool WorkoutDAO::deleteEquipment(int equipmentId) {
    if (!connect()) return false;
    
    std::string query = "DELETE FROM Equipment WHERE equipment_id = " + std::to_string(equipmentId);
    
    if (mysql_query(connection, query.c_str())) {
        handleError("Delete Equipment");
        return false;
    }
    
    std::cout << "Equipment deleted successfully!" << std::endl;
    return true;
}
