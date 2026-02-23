// WorkoutManager.cpp
// Workout Tracking System - Business Layer Implementation
// Author: Claude
// Date: 2026-01-28

#include "WorkoutManager.h"
#include <iostream>
#include <algorithm>

// Constructor
WorkoutManager::WorkoutManager(WorkoutDAO* dataAccess) : dao(dataAccess) {
    std::cout << "[WorkoutManager] Initialized" << std::endl;
}

// Destructor
WorkoutManager::~WorkoutManager() {
    std::cout << "[WorkoutManager] Destroyed" << std::endl;
}

// Helper method to log operations
void WorkoutManager::logOperation(const std::string& operation, bool success) {
    if (success) {
        std::cout << "[SUCCESS] " << operation << std::endl;
    } else {
        std::cerr << "[FAILED] " << operation << std::endl;
    }
}

// Test database connection
bool WorkoutManager::testConnection() {
    return dao->testConnection();
}

// Cleanup vector helper
template<typename T>
void WorkoutManager::cleanupVector(std::vector<T*>& vec) {
    for (auto* item : vec) {
        delete item;
    }
    vec.clear();
}

// ==================== WORKOUT BUSINESS METHODS ====================

// Save workout - creates if ID is 0, updates otherwise
bool WorkoutManager::saveWorkout(Workout& workout) {
    bool result;
    
    if (workout.getWorkoutId() == 0) {
        // Create new workout
        result = dao->createWorkout(workout);
        if (result) {
            workout.setWorkoutId(dao->getLastInsertId());
            logOperation("Created Workout ID: " + std::to_string(workout.getWorkoutId()), true);
        } else {
            logOperation("Create Workout", false);
        }
    } else {
        // Update existing workout
        result = dao->updateWorkout(workout);
        logOperation("Updated Workout ID: " + std::to_string(workout.getWorkoutId()), result);
    }
    
    return result;
}

// Get a single workout
Workout* WorkoutManager::getWorkout(int workoutId) {
    Workout* workout = dao->readWorkout(workoutId);
    if (workout) {
        logOperation("Retrieved Workout ID: " + std::to_string(workoutId), true);
    } else {
        logOperation("Retrieve Workout ID: " + std::to_string(workoutId), false);
    }
    return workout;
}

// Get all workouts
std::vector<Workout*> WorkoutManager::getAllWorkouts() {
    std::vector<Workout*> workouts = dao->readAllWorkouts();
    std::cout << "[INFO] Retrieved " << workouts.size() << " workouts" << std::endl;
    return workouts;
}

// Get workouts by date
std::vector<Workout*> WorkoutManager::getWorkoutsByDate(const std::string& date) {
    std::vector<Workout*> workouts = dao->readWorkoutsByDate(date);
    std::cout << "[INFO] Found " << workouts.size() << " workouts on " << date << std::endl;
    return workouts;
}

// Get workouts by muscle group
std::vector<Workout*> WorkoutManager::getWorkoutsByMuscleGroup(int muscleGroupId) {
    std::vector<Workout*> workouts = dao->readWorkoutsByMuscleGroup(muscleGroupId);
    std::cout << "[INFO] Found " << workouts.size() << " workouts for muscle group " << muscleGroupId << std::endl;
    return workouts;
}

// Delete workout
bool WorkoutManager::deleteWorkout(int workoutId) {
    bool result = dao->deleteWorkout(workoutId);
    logOperation("Deleted Workout ID: " + std::to_string(workoutId), result);
    return result;
}

// Get high intensity workouts
std::vector<Workout*> WorkoutManager::getHighIntensityWorkouts() {
    std::vector<Workout*> allWorkouts = dao->readAllWorkouts();
    std::vector<Workout*> highIntensity;
    
    for (auto* workout : allWorkouts) {
        if (workout->isHighIntensity()) {
            highIntensity.push_back(workout);
        }
    }
    
    std::cout << "[INFO] Found " << highIntensity.size() << " high intensity workouts" << std::endl;
    return highIntensity;
}

// Get total calories burned for date range
double WorkoutManager::getTotalCaloriesBurned(const std::string& startDate, const std::string& endDate) {
    std::vector<Workout*> allWorkouts = dao->readAllWorkouts();
    double totalCalories = 0.0;
    
    for (auto* workout : allWorkouts) {
        std::string date = workout->getWorkoutDate();
        if (date >= startDate && date <= endDate) {
            totalCalories += workout->getCaloriesBurned();
        }
    }
    
    // Cleanup
    cleanupVector(allWorkouts);
    
    std::cout << "[INFO] Total calories burned from " << startDate << " to " << endDate 
              << ": " << totalCalories << std::endl;
    return totalCalories;
}

// ==================== MUSCLEGROUP BUSINESS METHODS ====================

// Save muscle group - creates if ID is 0, updates otherwise
bool WorkoutManager::saveMuscleGroup(MuscleGroup& muscleGroup) {
    bool result;
    
    if (muscleGroup.getMuscleGroupId() == 0) {
        // Create new muscle group
        result = dao->createMuscleGroup(muscleGroup);
        if (result) {
            muscleGroup.setMuscleGroupId(dao->getLastInsertId());
            logOperation("Created MuscleGroup ID: " + std::to_string(muscleGroup.getMuscleGroupId()), true);
        } else {
            logOperation("Create MuscleGroup", false);
        }
    } else {
        // Update existing muscle group
        result = dao->updateMuscleGroup(muscleGroup);
        logOperation("Updated MuscleGroup ID: " + std::to_string(muscleGroup.getMuscleGroupId()), result);
    }
    
    return result;
}

// Get a single muscle group
MuscleGroup* WorkoutManager::getMuscleGroup(int muscleGroupId) {
    MuscleGroup* muscleGroup = dao->readMuscleGroup(muscleGroupId);
    if (muscleGroup) {
        logOperation("Retrieved MuscleGroup ID: " + std::to_string(muscleGroupId), true);
    } else {
        logOperation("Retrieve MuscleGroup ID: " + std::to_string(muscleGroupId), false);
    }
    return muscleGroup;
}

// Get all muscle groups
std::vector<MuscleGroup*> WorkoutManager::getAllMuscleGroups() {
    std::vector<MuscleGroup*> muscleGroups = dao->readAllMuscleGroups();
    std::cout << "[INFO] Retrieved " << muscleGroups.size() << " muscle groups" << std::endl;
    return muscleGroups;
}

// Get muscle group by name
MuscleGroup* WorkoutManager::getMuscleGroupByName(const std::string& name) {
    MuscleGroup* muscleGroup = dao->readMuscleGroupByName(name);
    if (muscleGroup) {
        logOperation("Retrieved MuscleGroup: " + name, true);
    } else {
        logOperation("Retrieve MuscleGroup: " + name, false);
    }
    return muscleGroup;
}

// Delete muscle group
bool WorkoutManager::deleteMuscleGroup(int muscleGroupId) {
    bool result = dao->deleteMuscleGroup(muscleGroupId);
    logOperation("Deleted MuscleGroup ID: " + std::to_string(muscleGroupId), result);
    return result;
}

// ==================== NUTRITION BUSINESS METHODS ====================

// Save nutrition - creates if ID is 0, updates otherwise
bool WorkoutManager::saveNutrition(Nutrition& nutrition) {
    bool result;
    
    if (nutrition.getNutritionId() == 0) {
        // Create new nutrition entry
        result = dao->createNutrition(nutrition);
        if (result) {
            nutrition.setNutritionId(dao->getLastInsertId());
            logOperation("Created Nutrition ID: " + std::to_string(nutrition.getNutritionId()), true);
        } else {
            logOperation("Create Nutrition", false);
        }
    } else {
        // Update existing nutrition entry
        result = dao->updateNutrition(nutrition);
        logOperation("Updated Nutrition ID: " + std::to_string(nutrition.getNutritionId()), result);
    }
    
    return result;
}

// Get a single nutrition entry
Nutrition* WorkoutManager::getNutrition(int nutritionId) {
    Nutrition* nutrition = dao->readNutrition(nutritionId);
    if (nutrition) {
        logOperation("Retrieved Nutrition ID: " + std::to_string(nutritionId), true);
    } else {
        logOperation("Retrieve Nutrition ID: " + std::to_string(nutritionId), false);
    }
    return nutrition;
}

// Get all nutrition entries
std::vector<Nutrition*> WorkoutManager::getAllNutrition() {
    std::vector<Nutrition*> nutrition = dao->readAllNutrition();
    std::cout << "[INFO] Retrieved " << nutrition.size() << " nutrition entries" << std::endl;
    return nutrition;
}

// Get nutrition by date
std::vector<Nutrition*> WorkoutManager::getNutritionByDate(const std::string& date) {
    std::vector<Nutrition*> nutrition = dao->readNutritionByDate(date);
    std::cout << "[INFO] Found " << nutrition.size() << " nutrition entries on " << date << std::endl;
    return nutrition;
}

// Get nutrition by family
std::vector<Nutrition*> WorkoutManager::getNutritionByFamily(const std::string& family) {
    std::vector<Nutrition*> nutrition = dao->readNutritionByFamily(family);
    std::cout << "[INFO] Found " << nutrition.size() << " " << family << " nutrition entries" << std::endl;
    return nutrition;
}

// Delete nutrition entry
bool WorkoutManager::deleteNutrition(int nutritionId) {
    bool result = dao->deleteNutrition(nutritionId);
    logOperation("Deleted Nutrition ID: " + std::to_string(nutritionId), result);
    return result;
}

// Get total calories for a date
double WorkoutManager::getTotalCaloriesForDate(const std::string& date) {
    std::vector<Nutrition*> nutrition = dao->readNutritionByDate(date);
    double totalCalories = 0.0;
    
    for (auto* n : nutrition) {
        totalCalories += n->calculateTotalCalories();
    }
    
    // Cleanup
    cleanupVector(nutrition);
    
    std::cout << "[INFO] Total calories consumed on " << date << ": " << totalCalories << std::endl;
    return totalCalories;
}

// Get total protein for a date
double WorkoutManager::getTotalProteinForDate(const std::string& date) {
    std::vector<Nutrition*> nutrition = dao->readNutritionByDate(date);
    double totalProtein = 0.0;
    
    for (auto* n : nutrition) {
        totalProtein += n->getProtein();
    }
    
    // Cleanup
    cleanupVector(nutrition);
    
    std::cout << "[INFO] Total protein consumed on " << date << ": " << totalProtein << "g" << std::endl;
    return totalProtein;
}

// ==================== RECOVERY BUSINESS METHODS ====================

// Save recovery - creates if ID is 0, updates otherwise
bool WorkoutManager::saveRecovery(Recovery& recovery) {
    bool result;
    
    if (recovery.getRecoveryId() == 0) {
        // Create new recovery session
        result = dao->createRecovery(recovery);
        if (result) {
            recovery.setRecoveryId(dao->getLastInsertId());
            logOperation("Created Recovery ID: " + std::to_string(recovery.getRecoveryId()), true);
        } else {
            logOperation("Create Recovery", false);
        }
    } else {
        // Update existing recovery session
        result = dao->updateRecovery(recovery);
        logOperation("Updated Recovery ID: " + std::to_string(recovery.getRecoveryId()), result);
    }
    
    return result;
}

// Get a single recovery session
Recovery* WorkoutManager::getRecovery(int recoveryId) {
    Recovery* recovery = dao->readRecovery(recoveryId);
    if (recovery) {
        logOperation("Retrieved Recovery ID: " + std::to_string(recoveryId), true);
    } else {
        logOperation("Retrieve Recovery ID: " + std::to_string(recoveryId), false);
    }
    return recovery;
}

// Get all recovery sessions
std::vector<Recovery*> WorkoutManager::getAllRecovery() {
    std::vector<Recovery*> recovery = dao->readAllRecovery();
    std::cout << "[INFO] Retrieved " << recovery.size() << " recovery sessions" << std::endl;
    return recovery;
}

// Get recovery by date
std::vector<Recovery*> WorkoutManager::getRecoveryByDate(const std::string& date) {
    std::vector<Recovery*> recovery = dao->readRecoveryByDate(date);
    std::cout << "[INFO] Found " << recovery.size() << " recovery sessions on " << date << std::endl;
    return recovery;
}

// Get recovery by type
std::vector<Recovery*> WorkoutManager::getRecoveryByType(const std::string& type) {
    std::vector<Recovery*> recovery = dao->readRecoveryByType(type);
    std::cout << "[INFO] Found " << recovery.size() << " " << type << " sessions" << std::endl;
    return recovery;
}

// Delete recovery session
bool WorkoutManager::deleteRecovery(int recoveryId) {
    bool result = dao->deleteRecovery(recoveryId);
    logOperation("Deleted Recovery ID: " + std::to_string(recoveryId), result);
    return result;
}

// Get total recovery time for date range
int WorkoutManager::getTotalRecoveryTime(const std::string& startDate, const std::string& endDate) {
    std::vector<Recovery*> allRecovery = dao->readAllRecovery();
    int totalMinutes = 0;
    
    for (auto* recovery : allRecovery) {
        std::string date = recovery->getRecoveryDate();
        if (date >= startDate && date <= endDate) {
            totalMinutes += recovery->getDuration();
        }
    }
    
    // Cleanup
    cleanupVector(allRecovery);
    
    std::cout << "[INFO] Total recovery time from " << startDate << " to " << endDate 
              << ": " << totalMinutes << " minutes" << std::endl;
    return totalMinutes;
}

// ==================== EQUIPMENT BUSINESS METHODS ====================

// Save equipment - creates if ID is 0, updates otherwise
bool WorkoutManager::saveEquipment(Equipment& equipment) {
    bool result;
    
    if (equipment.getEquipmentId() == 0) {
        // Create new equipment
        result = dao->createEquipment(equipment);
        if (result) {
            equipment.setEquipmentId(dao->getLastInsertId());
            logOperation("Created Equipment ID: " + std::to_string(equipment.getEquipmentId()), true);
        } else {
            logOperation("Create Equipment", false);
        }
    } else {
        // Update existing equipment
        result = dao->updateEquipment(equipment);
        logOperation("Updated Equipment ID: " + std::to_string(equipment.getEquipmentId()), result);
    }
    
    return result;
}

// Get a single equipment
Equipment* WorkoutManager::getEquipment(int equipmentId) {
    Equipment* equipment = dao->readEquipment(equipmentId);
    if (equipment) {
        logOperation("Retrieved Equipment ID: " + std::to_string(equipmentId), true);
    } else {
        logOperation("Retrieve Equipment ID: " + std::to_string(equipmentId), false);
    }
    return equipment;
}

// Get all equipment
std::vector<Equipment*> WorkoutManager::getAllEquipment() {
    std::vector<Equipment*> equipment = dao->readAllEquipment();
    std::cout << "[INFO] Retrieved " << equipment.size() << " equipment items" << std::endl;
    return equipment;
}

// Get equipment by category
std::vector<Equipment*> WorkoutManager::getEquipmentByCategory(const std::string& category) {
    std::vector<Equipment*> equipment = dao->readEquipmentByCategory(category);
    std::cout << "[INFO] Found " << equipment.size() << " " << category << " equipment items" << std::endl;
    return equipment;
}

// Get equipment by name
Equipment* WorkoutManager::getEquipmentByName(const std::string& name) {
    Equipment* equipment = dao->readEquipmentByName(name);
    if (equipment) {
        logOperation("Retrieved Equipment: " + name, true);
    } else {
        logOperation("Retrieve Equipment: " + name, false);
    }
    return equipment;
}

// Delete equipment
bool WorkoutManager::deleteEquipment(int equipmentId) {
    bool result = dao->deleteEquipment(equipmentId);
    logOperation("Deleted Equipment ID: " + std::to_string(equipmentId), result);
    return result;
}

// Get all cardio equipment
std::vector<Equipment*> WorkoutManager::getCardioEquipment() {
    std::vector<Equipment*> allEquipment = dao->readAllEquipment();
    std::vector<Equipment*> cardio;
    
    for (auto* equipment : allEquipment) {
        if (equipment->isCardioEquipment()) {
            cardio.push_back(equipment);
        }
    }
    
    std::cout << "[INFO] Found " << cardio.size() << " cardio equipment items" << std::endl;
    return cardio;
}
