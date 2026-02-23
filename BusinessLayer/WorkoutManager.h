// WorkoutManager.h
// Workout Tracking System - Business Layer
// Author: Claude
// Date: 2026-01-28

#ifndef WORKOUTMANAGER_H
#define WORKOUTMANAGER_H

#include "../WorkoutDAO.h"
#include "../Workout.h"
#include "../MuscleGroup.h"
#include "../Nutrition.h"
#include "../Recovery.h"
#include "../Equipment.h"
#include <vector>
#include <string>

class WorkoutManager {
private:
    WorkoutDAO* dao;
    
    // Helper methods
    void logOperation(const std::string& operation, bool success);

public:
    // Constructor and Destructor
    WorkoutManager(WorkoutDAO* dataAccess);
    ~WorkoutManager();
    
    // ==================== WORKOUT BUSINESS METHODS ====================
    
    // Save method - creates if ID is 0, updates if ID exists
    bool saveWorkout(Workout& workout);
    
    // Get a single workout
    Workout* getWorkout(int workoutId);
    
    // Get all workouts
    std::vector<Workout*> getAllWorkouts();
    
    // Get workouts by date
    std::vector<Workout*> getWorkoutsByDate(const std::string& date);
    
    // Get workouts by muscle group
    std::vector<Workout*> getWorkoutsByMuscleGroup(int muscleGroupId);
    
    // Delete workout
    bool deleteWorkout(int workoutId);
    
    // Get high intensity workouts (RPE >= 8)
    std::vector<Workout*> getHighIntensityWorkouts();
    
    // Get total calories burned for a date range
    double getTotalCaloriesBurned(const std::string& startDate, const std::string& endDate);
    
    // ==================== MUSCLEGROUP BUSINESS METHODS ====================
    
    // Save method - creates if ID is 0, updates if ID exists
    bool saveMuscleGroup(MuscleGroup& muscleGroup);
    
    // Get a single muscle group
    MuscleGroup* getMuscleGroup(int muscleGroupId);
    
    // Get all muscle groups
    std::vector<MuscleGroup*> getAllMuscleGroups();
    
    // Get muscle group by name
    MuscleGroup* getMuscleGroupByName(const std::string& name);
    
    // Delete muscle group
    bool deleteMuscleGroup(int muscleGroupId);
    
    // ==================== NUTRITION BUSINESS METHODS ====================
    
    // Save method - creates if ID is 0, updates if ID exists
    bool saveNutrition(Nutrition& nutrition);
    
    // Get a single nutrition entry
    Nutrition* getNutrition(int nutritionId);
    
    // Get all nutrition entries
    std::vector<Nutrition*> getAllNutrition();
    
    // Get nutrition by date
    std::vector<Nutrition*> getNutritionByDate(const std::string& date);
    
    // Get nutrition by family
    std::vector<Nutrition*> getNutritionByFamily(const std::string& family);
    
    // Delete nutrition entry
    bool deleteNutrition(int nutritionId);
    
    // Get total calories for a date
    double getTotalCaloriesForDate(const std::string& date);
    
    // Get total protein for a date
    double getTotalProteinForDate(const std::string& date);
    
    // ==================== RECOVERY BUSINESS METHODS ====================
    
    // Save method - creates if ID is 0, updates if ID exists
    bool saveRecovery(Recovery& recovery);
    
    // Get a single recovery session
    Recovery* getRecovery(int recoveryId);
    
    // Get all recovery sessions
    std::vector<Recovery*> getAllRecovery();
    
    // Get recovery by date
    std::vector<Recovery*> getRecoveryByDate(const std::string& date);
    
    // Get recovery by type
    std::vector<Recovery*> getRecoveryByType(const std::string& type);
    
    // Delete recovery session
    bool deleteRecovery(int recoveryId);
    
    // Get total recovery time for a date range
    int getTotalRecoveryTime(const std::string& startDate, const std::string& endDate);
    
    // ==================== EQUIPMENT BUSINESS METHODS ====================
    
    // Save method - creates if ID is 0, updates if ID exists
    bool saveEquipment(Equipment& equipment);
    
    // Get a single equipment
    Equipment* getEquipment(int equipmentId);
    
    // Get all equipment
    std::vector<Equipment*> getAllEquipment();
    
    // Get equipment by category
    std::vector<Equipment*> getEquipmentByCategory(const std::string& category);
    
    // Get equipment by name
    Equipment* getEquipmentByName(const std::string& name);
    
    // Delete equipment
    bool deleteEquipment(int equipmentId);
    
    // Get all cardio equipment
    std::vector<Equipment*> getCardioEquipment();
    
    // ==================== UTILITY METHODS ====================
    
    // Test database connection
    bool testConnection();
    
    // Cleanup memory for vectors
    template<typename T>
    void cleanupVector(std::vector<T*>& vec);
};

#endif // WORKOUTMANAGER_H
