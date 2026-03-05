// WorkoutService.cpp
// Service Layer Implementation
// Location: ServiceLayer/WorkoutService.cpp
// Author: Claude
// Date: 2026-01-28

#include "WorkoutService.h"
#include <iostream>
#include <regex>

// Constructor
WorkoutService::WorkoutService(WorkoutManager* mgr) : manager(mgr) {
    // std::cout << "[WorkoutService] Service Layer initialized" << std::endl;
}

// Destructor
WorkoutService::~WorkoutService() {
    // std::cout << "[WorkoutService] Service Layer destroyed" << std::endl;
}

// ==================== VALIDATION HELPERS ====================

bool WorkoutService::validateDate(const std::string& date) {
    // Simple date validation: YYYY-MM-DD
    std::regex dateRegex("^\\d{4}-\\d{2}-\\d{2}$");
    return std::regex_match(date, dateRegex);
}

bool WorkoutService::validateTime(const std::string& time) {
    // Simple time validation: HH:MM:SS
    std::regex timeRegex("^\\d{2}:\\d{2}:\\d{2}$");
    return std::regex_match(time, timeRegex);
}

bool WorkoutService::validateRPE(int rpe) {
    return rpe >= 1 && rpe <= 10;
}

// ==================== WORKOUT SERVICES ====================

ServiceResponse WorkoutService::insertWorkout(const std::string& date,
                                              const std::string& time,
                                              int duration,
                                              const std::string& type,
                                              double calories,
                                              int rpe,
                                              int muscleGroupId) {
    // Validate inputs
    if (!validateDate(date)) {
        return ServiceResponse(false, "Invalid date format. Use YYYY-MM-DD");
    }
    if (!validateTime(time)) {
        return ServiceResponse(false, "Invalid time format. Use HH:MM:SS");
    }
    if (duration <= 0) {
        return ServiceResponse(false, "Duration must be positive");
    }
    if (!validateRPE(rpe)) {
        return ServiceResponse(false, "RPE must be between 1 and 10");
    }
    if (calories < 0) {
        return ServiceResponse(false, "Calories cannot be negative");
    }
    
    // Create workout object
    Workout workout(0, date, time, duration, type, calories, rpe, muscleGroupId);
    
    // Save through business layer
    bool success = manager->saveWorkout(workout);
    
    if (success) {
        int id = workout.getWorkoutId();
        return ServiceResponse(true, "Workout created successfully", id);
    } else {
        return ServiceResponse(false, "Failed to create workout");
    }
}

ServiceResponse WorkoutService::updateWorkout(int id,
                                              const std::string& date,
                                              const std::string& time,
                                              int duration,
                                              const std::string& type,
                                              double calories,
                                              int rpe,
                                              int muscleGroupId) {
    // Validate inputs
    if (id <= 0) {
        return ServiceResponse(false, "Invalid workout ID");
    }
    if (!validateDate(date)) {
        return ServiceResponse(false, "Invalid date format. Use YYYY-MM-DD");
    }
    if (!validateTime(time)) {
        return ServiceResponse(false, "Invalid time format. Use HH:MM:SS");
    }
    if (!validateRPE(rpe)) {
        return ServiceResponse(false, "RPE must be between 1 and 10");
    }
    
    // Create workout object with existing ID
    Workout workout(id, date, time, duration, type, calories, rpe, muscleGroupId);
    
    // Update through business layer
    bool success = manager->saveWorkout(workout);
    
    if (success) {
        return ServiceResponse(true, "Workout updated successfully", id);
    } else {
        return ServiceResponse(false, "Failed to update workout");
    }
}

ServiceResponse WorkoutService::deleteWorkout(int id) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid workout ID");
    }
    
    bool success = manager->deleteWorkout(id);
    
    if (success) {
        return ServiceResponse(true, "Workout deleted successfully", id);
    } else {
        return ServiceResponse(false, "Failed to delete workout");
    }
}

Workout* WorkoutService::getWorkout(int id) {
    if (id <= 0) {
        std::cerr << "[Service] Invalid workout ID: " << id << std::endl;
        return nullptr;
    }
    return manager->getWorkout(id);
}

std::vector<Workout*> WorkoutService::getAllWorkouts() {
    return manager->getAllWorkouts();
}

// ==================== MUSCLEGROUP SERVICES ====================

ServiceResponse WorkoutService::insertMuscleGroup(const std::string& name,
                                                  const std::string& description,
                                                  int daysPerWeek,
                                                  int sets,
                                                  int reps,
                                                  double weight) {
    // Validate inputs
    if (name.empty()) {
        return ServiceResponse(false, "Name cannot be empty");
    }
    if (daysPerWeek < 0 || daysPerWeek > 7) {
        return ServiceResponse(false, "Days per week must be between 0 and 7");
    }
    if (sets <= 0 || reps <= 0) {
        return ServiceResponse(false, "Sets and reps must be positive");
    }
    if (weight < 0) {
        return ServiceResponse(false, "Weight cannot be negative");
    }
    
    // Create muscle group object
    MuscleGroup mg(0, name, description, daysPerWeek, sets, reps, weight);
    
    // Save through business layer
    bool success = manager->saveMuscleGroup(mg);
    
    if (success) {
        return ServiceResponse(true, "Muscle Group created successfully", mg.getMuscleGroupId());
    } else {
        return ServiceResponse(false, "Failed to create muscle group");
    }
}

ServiceResponse WorkoutService::updateMuscleGroup(int id,
                                                  const std::string& name,
                                                  const std::string& description,
                                                  int daysPerWeek,
                                                  int sets,
                                                  int reps,
                                                  double weight) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid muscle group ID");
    }
    if (name.empty()) {
        return ServiceResponse(false, "Name cannot be empty");
    }
    
    MuscleGroup mg(id, name, description, daysPerWeek, sets, reps, weight);
    bool success = manager->saveMuscleGroup(mg);
    
    if (success) {
        return ServiceResponse(true, "Muscle Group updated successfully", id);
    } else {
        return ServiceResponse(false, "Failed to update muscle group");
    }
}

ServiceResponse WorkoutService::deleteMuscleGroup(int id) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid muscle group ID");
    }
    
    bool success = manager->deleteMuscleGroup(id);
    
    if (success) {
        return ServiceResponse(true, "Muscle Group deleted successfully", id);
    } else {
        return ServiceResponse(false, "Failed to delete muscle group");
    }
}

MuscleGroup* WorkoutService::getMuscleGroup(int id) {
    if (id <= 0) {
        std::cerr << "[Service] Invalid muscle group ID: " << id << std::endl;
        return nullptr;
    }
    return manager->getMuscleGroup(id);
}

std::vector<MuscleGroup*> WorkoutService::getAllMuscleGroups() {
    return manager->getAllMuscleGroups();
}

// ==================== NUTRITION SERVICES ====================

ServiceResponse WorkoutService::insertNutrition(const std::string& family,
                                               double water,
                                               double carbs,
                                               double fat,
                                               double protein,
                                               double sugar,
                                               const std::string& date) {
    // Validate inputs
    if (!validateDate(date)) {
        return ServiceResponse(false, "Invalid date format. Use YYYY-MM-DD");
    }
    if (water < 0 || carbs < 0 || fat < 0 || protein < 0 || sugar < 0) {
        return ServiceResponse(false, "Nutrition values cannot be negative");
    }
    
    // Convert family string to enum
    FoodFamily foodFamily;
    if (family == "Mixed") foodFamily = FoodFamily::MIXED;
    else if (family == "Fruit") foodFamily = FoodFamily::FRUIT;
    else if (family == "Meat") foodFamily = FoodFamily::MEAT;
    else if (family == "Vegetable") foodFamily = FoodFamily::VEGETABLE;
    else if (family == "Dairy") foodFamily = FoodFamily::DAIRY;
    else {
        return ServiceResponse(false, "Invalid food family. Use: Mixed, Fruit, Meat, Vegetable, Dairy");
    }
    
    // Create nutrition object
    Nutrition nutrition(0, foodFamily, water, carbs, fat, protein, sugar, date);
    
    // Save through business layer
    bool success = manager->saveNutrition(nutrition);
    
    if (success) {
        return ServiceResponse(true, "Nutrition entry created successfully", nutrition.getNutritionId());
    } else {
        return ServiceResponse(false, "Failed to create nutrition entry");
    }
}

ServiceResponse WorkoutService::updateNutrition(int id,
                                               const std::string& family,
                                               double water,
                                               double carbs,
                                               double fat,
                                               double protein,
                                               double sugar,
                                               const std::string& date) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid nutrition ID");
    }
    if (!validateDate(date)) {
        return ServiceResponse(false, "Invalid date format");
    }
    
    // Convert family string to enum
    FoodFamily foodFamily = FoodFamily::MIXED;  // Default
    if (family == "Fruit") foodFamily = FoodFamily::FRUIT;
    else if (family == "Meat") foodFamily = FoodFamily::MEAT;
    else if (family == "Vegetable") foodFamily = FoodFamily::VEGETABLE;
    else if (family == "Dairy") foodFamily = FoodFamily::DAIRY;
    
    Nutrition nutrition(id, foodFamily, water, carbs, fat, protein, sugar, date);
    bool success = manager->saveNutrition(nutrition);
    
    if (success) {
        return ServiceResponse(true, "Nutrition entry updated successfully", id);
    } else {
        return ServiceResponse(false, "Failed to update nutrition entry");
    }
}

ServiceResponse WorkoutService::deleteNutrition(int id) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid nutrition ID");
    }
    
    bool success = manager->deleteNutrition(id);
    
    if (success) {
        return ServiceResponse(true, "Nutrition entry deleted successfully", id);
    } else {
        return ServiceResponse(false, "Failed to delete nutrition entry");
    }
}

Nutrition* WorkoutService::getNutrition(int id) {
    if (id <= 0) {
        std::cerr << "[Service] Invalid nutrition ID: " << id << std::endl;
        return nullptr;
    }
    return manager->getNutrition(id);
}

std::vector<Nutrition*> WorkoutService::getAllNutrition() {
    return manager->getAllNutrition();
}

// ==================== RECOVERY SERVICES ====================

ServiceResponse WorkoutService::insertRecovery(const std::string& date,
                                              int duration,
                                              const std::string& type,
                                              const std::string& helpers) {
    if (!validateDate(date)) {
        return ServiceResponse(false, "Invalid date format. Use YYYY-MM-DD");
    }
    if (duration <= 0) {
        return ServiceResponse(false, "Duration must be positive");
    }
    if (type.empty()) {
        return ServiceResponse(false, "Type cannot be empty");
    }
    
    Recovery recovery(0, date, duration, type, helpers);
    bool success = manager->saveRecovery(recovery);
    
    if (success) {
        return ServiceResponse(true, "Recovery session created successfully", recovery.getRecoveryId());
    } else {
        return ServiceResponse(false, "Failed to create recovery session");
    }
}

ServiceResponse WorkoutService::updateRecovery(int id,
                                              const std::string& date,
                                              int duration,
                                              const std::string& type,
                                              const std::string& helpers) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid recovery ID");
    }
    if (!validateDate(date)) {
        return ServiceResponse(false, "Invalid date format");
    }
    if (duration <= 0) {
        return ServiceResponse(false, "Duration must be positive");
    }
    
    Recovery recovery(id, date, duration, type, helpers);
    bool success = manager->saveRecovery(recovery);
    
    if (success) {
        return ServiceResponse(true, "Recovery session updated successfully", id);
    } else {
        return ServiceResponse(false, "Failed to update recovery session");
    }
}

ServiceResponse WorkoutService::deleteRecovery(int id) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid recovery ID");
    }
    
    bool success = manager->deleteRecovery(id);
    
    if (success) {
        return ServiceResponse(true, "Recovery session deleted successfully", id);
    } else {
        return ServiceResponse(false, "Failed to delete recovery session");
    }
}

Recovery* WorkoutService::getRecovery(int id) {
    if (id <= 0) {
        std::cerr << "[Service] Invalid recovery ID: " << id << std::endl;
        return nullptr;
    }
    return manager->getRecovery(id);
}

std::vector<Recovery*> WorkoutService::getAllRecovery() {
    return manager->getAllRecovery();
}

// ==================== EQUIPMENT SERVICES ====================

ServiceResponse WorkoutService::insertEquipment(const std::string& name,
                                               const std::string& description,
                                               const std::string& category,
                                               const std::string& target) {
    if (name.empty()) {
        return ServiceResponse(false, "Name cannot be empty");
    }
    if (category.empty()) {
        return ServiceResponse(false, "Category cannot be empty");
    }
    
    Equipment equipment(0, name, description, category, target);
    bool success = manager->saveEquipment(equipment);
    
    if (success) {
        return ServiceResponse(true, "Equipment created successfully", equipment.getEquipmentId());
    } else {
        return ServiceResponse(false, "Failed to create equipment");
    }
}

ServiceResponse WorkoutService::updateEquipment(int id,
                                               const std::string& name,
                                               const std::string& description,
                                               const std::string& category,
                                               const std::string& target) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid equipment ID");
    }
    if (name.empty()) {
        return ServiceResponse(false, "Name cannot be empty");
    }
    
    Equipment equipment(id, name, description, category, target);
    bool success = manager->saveEquipment(equipment);
    
    if (success) {
        return ServiceResponse(true, "Equipment updated successfully", id);
    } else {
        return ServiceResponse(false, "Failed to update equipment");
    }
}

ServiceResponse WorkoutService::deleteEquipment(int id) {
    if (id <= 0) {
        return ServiceResponse(false, "Invalid equipment ID");
    }
    
    bool success = manager->deleteEquipment(id);
    
    if (success) {
        return ServiceResponse(true, "Equipment deleted successfully", id);
    } else {
        return ServiceResponse(false, "Failed to delete equipment");
    }
}

Equipment* WorkoutService::getEquipment(int id) {
    if (id <= 0) {
        std::cerr << "[Service] Invalid equipment ID: " << id << std::endl;
        return nullptr;
    }
    return manager->getEquipment(id);
}

std::vector<Equipment*> WorkoutService::getAllEquipment() {
    return manager->getAllEquipment();
}

// ==================== UTILITY METHODS ====================

bool WorkoutService::testConnection() {
    return manager->testConnection();
}

int WorkoutService::getTotalWorkouts() {
    std::vector<Workout*> workouts = manager->getAllWorkouts();
    int count = workouts.size();
    manager->cleanupVector(workouts);
    return count;
}

int WorkoutService::getTotalMuscleGroups() {
    std::vector<MuscleGroup*> groups = manager->getAllMuscleGroups();
    int count = groups.size();
    manager->cleanupVector(groups);
    return count;
}

int WorkoutService::getTotalNutrition() {
    std::vector<Nutrition*> nutrition = manager->getAllNutrition();
    int count = nutrition.size();
    manager->cleanupVector(nutrition);
    return count;
}

int WorkoutService::getTotalRecovery() {
    std::vector<Recovery*> recovery = manager->getAllRecovery();
    int count = recovery.size();
    manager->cleanupVector(recovery);
    return count;
}

int WorkoutService::getTotalEquipment() {
    std::vector<Equipment*> equipment = manager->getAllEquipment();
    int count = equipment.size();
    manager->cleanupVector(equipment);
    return count;
}
