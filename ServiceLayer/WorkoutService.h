// WorkoutService.h
// Service Layer - Provides API interface for console applications
// Location: ServiceLayer/WorkoutService.h
// Author: Claude
// Date: 2026-01-28

#ifndef WORKOUTSERVICE_H
#define WORKOUTSERVICE_H

#include "../BusinessLayer/WorkoutManager.h"
#include <string>
#include <vector>

/*
 * SERVICE LAYER
 * 
 * This layer provides a clean API interface for console applications.
 * It wraps the business layer and provides:
 * - Input validation
 * - Response formatting
 * - Error handling
 * - Consistent return types
 * 
 * Architecture:
 * Console App → Service Layer → Business Layer → Data Layer → Database
 */

// Response structure for service operations
struct ServiceResponse {
    bool success;
    std::string message;
    int id;  // For create operations
    
    ServiceResponse(bool s = false, const std::string& m = "", int i = 0)
        : success(s), message(m), id(i) {}
};

class WorkoutService {
private:
    WorkoutManager* manager;
    
    // Helper methods for validation
    bool validateDate(const std::string& date);
    bool validateTime(const std::string& time);
    bool validateRPE(int rpe);

public:
    // Constructor
    WorkoutService(WorkoutManager* mgr);
    ~WorkoutService();
    
    // ==================== WORKOUT SERVICES ====================
    
    // Insert (Create) a new workout
    ServiceResponse insertWorkout(const std::string& date, 
                                  const std::string& time,
                                  int duration,
                                  const std::string& type,
                                  double calories,
                                  int rpe,
                                  int muscleGroupId);
    
    // Update an existing workout
    ServiceResponse updateWorkout(int id,
                                  const std::string& date,
                                  const std::string& time,
                                  int duration,
                                  const std::string& type,
                                  double calories,
                                  int rpe,
                                  int muscleGroupId);
    
    // Delete a workout
    ServiceResponse deleteWorkout(int id);
    
    // Get a workout by ID
    Workout* getWorkout(int id);
    
    // Get all workouts
    std::vector<Workout*> getAllWorkouts();
    
    // ==================== MUSCLEGROUP SERVICES ====================
    
    // Insert (Create) a new muscle group
    ServiceResponse insertMuscleGroup(const std::string& name,
                                     const std::string& description,
                                     int daysPerWeek,
                                     int sets,
                                     int reps,
                                     double weight);
    
    // Update an existing muscle group
    ServiceResponse updateMuscleGroup(int id,
                                     const std::string& name,
                                     const std::string& description,
                                     int daysPerWeek,
                                     int sets,
                                     int reps,
                                     double weight);
    
    // Delete a muscle group
    ServiceResponse deleteMuscleGroup(int id);
    
    // Get a muscle group by ID
    MuscleGroup* getMuscleGroup(int id);
    
    // Get all muscle groups
    std::vector<MuscleGroup*> getAllMuscleGroups();
    
    // ==================== NUTRITION SERVICES ====================
    
    // Insert (Create) a new nutrition entry
    ServiceResponse insertNutrition(const std::string& family,
                                   double water,
                                   double carbs,
                                   double fat,
                                   double protein,
                                   double sugar,
                                   const std::string& date);
    
    // Update an existing nutrition entry
    ServiceResponse updateNutrition(int id,
                                   const std::string& family,
                                   double water,
                                   double carbs,
                                   double fat,
                                   double protein,
                                   double sugar,
                                   const std::string& date);
    
    // Delete a nutrition entry
    ServiceResponse deleteNutrition(int id);
    
    // Get a nutrition entry by ID
    Nutrition* getNutrition(int id);
    
    // Get all nutrition entries
    std::vector<Nutrition*> getAllNutrition();
    
    // ==================== RECOVERY SERVICES ====================
    
    // Insert (Create) a new recovery session
    ServiceResponse insertRecovery(const std::string& date,
                                  int duration,
                                  const std::string& type,
                                  const std::string& helpers);
    
    // Update an existing recovery session
    ServiceResponse updateRecovery(int id,
                                  const std::string& date,
                                  int duration,
                                  const std::string& type,
                                  const std::string& helpers);
    
    // Delete a recovery session
    ServiceResponse deleteRecovery(int id);
    
    // Get a recovery session by ID
    Recovery* getRecovery(int id);
    
    // Get all recovery sessions
    std::vector<Recovery*> getAllRecovery();
    
    // ==================== EQUIPMENT SERVICES ====================
    
    // Insert (Create) a new equipment
    ServiceResponse insertEquipment(const std::string& name,
                                   const std::string& description,
                                   const std::string& category,
                                   const std::string& target);
    
    // Update an existing equipment
    ServiceResponse updateEquipment(int id,
                                   const std::string& name,
                                   const std::string& description,
                                   const std::string& category,
                                   const std::string& target);
    
    // Delete an equipment
    ServiceResponse deleteEquipment(int id);
    
    // Get an equipment by ID
    Equipment* getEquipment(int id);
    
    // Get all equipment
    std::vector<Equipment*> getAllEquipment();
    
    // ==================== UTILITY METHODS ====================
    
    // Test connection
    bool testConnection();
    
    // Get statistics
    int getTotalWorkouts();
    int getTotalMuscleGroups();
    int getTotalNutrition();
    int getTotalRecovery();
    int getTotalEquipment();
};

#endif // WORKOUTSERVICE_H
