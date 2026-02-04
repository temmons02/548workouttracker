// WorkoutDAO.h
// Workout Tracking System - Data Access Object
// Author: Claude, Therin Emmons
// Date: 2026-01-28

#ifndef WORKOUTDAO_H
#define WORKOUTDAO_H

#include "Workout.h"
#include "MuscleGroup.h"
#include "Nutrition.h"
#include "Recovery.h"
#include "Equipment.h"
#include <mysql/mysql.h>
#include <vector>
#include <string>

class WorkoutDAO {
private:
    MYSQL* connection;
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    int port;
    
    // Helper methods
    bool connect();
    void disconnect();
    void handleError(const std::string& operation);

public:
    // Constructor and Destructor
    WorkoutDAO(const std::string& host, const std::string& user,
               const std::string& password, const std::string& database, int port = 3306);
    ~WorkoutDAO();
    
    // Workout CRUD operations
    bool createWorkout(const Workout& workout);
    Workout* readWorkout(int workoutId);
    std::vector<Workout*> readAllWorkouts();
    std::vector<Workout*> readWorkoutsByDate(const std::string& date);
    std::vector<Workout*> readWorkoutsByMuscleGroup(int muscleGroupId);
    bool updateWorkout(const Workout& workout);
    bool deleteWorkout(int workoutId);
    
    // MuscleGroup CRUD operations
    bool createMuscleGroup(const MuscleGroup& muscleGroup);
    MuscleGroup* readMuscleGroup(int muscleGroupId);
    std::vector<MuscleGroup*> readAllMuscleGroups();
    MuscleGroup* readMuscleGroupByName(const std::string& name);
    bool updateMuscleGroup(const MuscleGroup& muscleGroup);
    bool deleteMuscleGroup(int muscleGroupId);
    
    // Nutrition CRUD operations
    bool createNutrition(const Nutrition& nutrition);
    Nutrition* readNutrition(int nutritionId);
    std::vector<Nutrition*> readAllNutrition();
    std::vector<Nutrition*> readNutritionByDate(const std::string& date);
    std::vector<Nutrition*> readNutritionByFamily(const std::string& family);
    bool updateNutrition(const Nutrition& nutrition);
    bool deleteNutrition(int nutritionId);
    
    // Recovery CRUD operations
    bool createRecovery(const Recovery& recovery);
    Recovery* readRecovery(int recoveryId);
    std::vector<Recovery*> readAllRecovery();
    std::vector<Recovery*> readRecoveryByDate(const std::string& date);
    std::vector<Recovery*> readRecoveryByType(const std::string& type);
    bool updateRecovery(const Recovery& recovery);
    bool deleteRecovery(int recoveryId);
    
    // Equipment CRUD operations
    bool createEquipment(const Equipment& equipment);
    Equipment* readEquipment(int equipmentId);
    std::vector<Equipment*> readAllEquipment();
    std::vector<Equipment*> readEquipmentByCategory(const std::string& category);
    Equipment* readEquipmentByName(const std::string& name);
    bool updateEquipment(const Equipment& equipment);
    bool deleteEquipment(int equipmentId);
    
    // Utility methods
    bool testConnection();
    int getLastInsertId();
};

#endif // WORKOUTDAO_H
