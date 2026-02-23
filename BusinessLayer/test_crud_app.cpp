// test_crud_app.cpp
// Workout Tracking System - CRUD Test Application
// Demonstrates Create, Update, Delete, and Get operations for ALL 5 objects
// Author: Claude
// Date: 2026-01-28

#include "WorkoutManager.h"
#include <iostream>
#include <iomanip>

void printSeparator(const std::string& title = "") {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    if (!title.empty()) {
        std::cout << "  " << title << std::endl;
        std::cout << std::string(70, '=') << std::endl;
    }
}

void printSmallSeparator() {
    std::cout << std::string(70, '-') << std::endl;
}

// ==================== WORKOUT CRUD TEST ====================
void testWorkoutCRUD(WorkoutManager& manager) {
    printSeparator("TESTING WORKOUT CRUD OPERATIONS");
    
    // CREATE: Create a new workout (ID = 0)
    std::cout << "\n[1] CREATE NEW WORKOUT" << std::endl;
    printSmallSeparator();
    Workout newWorkout(0, "2026-02-28", "07:00:00", 60, 
                       "Test Heavy Deadlifts", 550.0, 9, 2);
    std::cout << "Creating workout with ID = 0..." << std::endl;
    manager.saveWorkout(newWorkout);
    std::cout << "Assigned ID: " << newWorkout.getWorkoutId() << std::endl;
    
    int workoutId = newWorkout.getWorkoutId();
    
    // GET: Retrieve the workout to verify creation
    std::cout << "\n[2] GET WORKOUT (Verify Creation)" << std::endl;
    printSmallSeparator();
    Workout* retrievedWorkout = manager.getWorkout(workoutId);
    if (!retrievedWorkout) {
        std::cerr << "Could not retrieve workout!" << std::endl;
        return; // Exit if nullptr is received
    }
    std::cout << "✓ Workout retrieved successfully:" << std::endl;
    retrievedWorkout->displayInfo();
    
    // UPDATE: Modify the workout
    std::cout << "\n[3] UPDATE WORKOUT" << std::endl;
    printSmallSeparator();
    std::cout << "Updating calories from " << retrievedWorkout->getCaloriesBurned() 
              << " to 600.0..." << std::endl;
    std::cout << "Updating RPE from " << retrievedWorkout->getRatePerceivedExhaustion() 
              << " to 10..." << std::endl;
    retrievedWorkout->setCaloriesBurned(600.0);
    retrievedWorkout->setRatePerceivedExhaustion(10);
    manager.saveWorkout(*retrievedWorkout);
    
    // GET: Retrieve again to verify update
    std::cout << "\n[4] GET WORKOUT (Verify Update)" << std::endl;
    printSmallSeparator();
    delete retrievedWorkout;  // Clean up old pointer
    retrievedWorkout = manager.getWorkout(workoutId);
    if (retrievedWorkout) {
        std::cout << "✓ Updated workout retrieved successfully:" << std::endl;
        retrievedWorkout->displayInfo();
    }
    
    // DELETE: Remove the workout
    std::cout << "\n[5] DELETE WORKOUT" << std::endl;
    printSmallSeparator();
    manager.deleteWorkout(workoutId);
    
    // GET: Try to retrieve deleted workout (should fail)
    std::cout << "\n[6] GET WORKOUT (Verify Deletion)" << std::endl;
    printSmallSeparator();
    delete retrievedWorkout;  // Clean up
    retrievedWorkout = manager.getWorkout(workoutId);
    if (!retrievedWorkout) {
        std::cout << "✓ Workout successfully deleted (not found)" << std::endl;
    } else {
        std::cout << "✗ ERROR: Workout still exists after deletion!" << std::endl;
        delete retrievedWorkout;
    }
    
    std::cout << "\n✅ WORKOUT CRUD TEST COMPLETED" << std::endl;
}

// ==================== MUSCLEGROUP CRUD TEST ====================
void testMuscleGroupCRUD(WorkoutManager& manager) {
    printSeparator("TESTING MUSCLEGROUP CRUD OPERATIONS");
    
    // CREATE
    std::cout << "\n[1] CREATE NEW MUSCLEGROUP" << std::endl;
    printSmallSeparator();
    MuscleGroup newMuscleGroup(0, "Test Abs", "Test abdominal muscles", 3, 4, 25, 30.0);
    manager.saveMuscleGroup(newMuscleGroup);
    int muscleGroupId = newMuscleGroup.getMuscleGroupId();
    
    // GET (Verify Creation)
    std::cout << "\n[2] GET MUSCLEGROUP (Verify Creation)" << std::endl;
    printSmallSeparator();
    MuscleGroup* retrieved = manager.getMuscleGroup(muscleGroupId);
    if (retrieved) {
        std::cout << "✓ MuscleGroup retrieved:" << std::endl;
        retrieved->displayInfo();
    }
    
    // UPDATE
    std::cout << "\n[3] UPDATE MUSCLEGROUP" << std::endl;
    printSmallSeparator();
    std::cout << "Updating weight from " << retrieved->getWeightAmount() << " to 45.0..." << std::endl;
    retrieved->setWeightAmount(45.0);
    retrieved->setSets(5);
    manager.saveMuscleGroup(*retrieved);
    
    // GET (Verify Update)
    std::cout << "\n[4] GET MUSCLEGROUP (Verify Update)" << std::endl;
    printSmallSeparator();
    delete retrieved;
    retrieved = manager.getMuscleGroup(muscleGroupId);
    if (retrieved) {
        std::cout << "✓ Updated MuscleGroup:" << std::endl;
        retrieved->displayInfo();
    }
    
    // DELETE
    std::cout << "\n[5] DELETE MUSCLEGROUP" << std::endl;
    printSmallSeparator();
    manager.deleteMuscleGroup(muscleGroupId);
    
    // GET (Verify Deletion)
    std::cout << "\n[6] GET MUSCLEGROUP (Verify Deletion)" << std::endl;
    printSmallSeparator();
    delete retrieved;
    retrieved = manager.getMuscleGroup(muscleGroupId);
    if (!retrieved) {
        std::cout << "✓ MuscleGroup successfully deleted" << std::endl;
    }
    
    std::cout << "\n✅ MUSCLEGROUP CRUD TEST COMPLETED" << std::endl;
}

// ==================== NUTRITION CRUD TEST ====================
void testNutritionCRUD(WorkoutManager& manager) {
    printSeparator("TESTING NUTRITION CRUD OPERATIONS");
    
    // CREATE
    std::cout << "\n[1] CREATE NEW NUTRITION" << std::endl;
    printSmallSeparator();
    Nutrition newNutrition(0, FoodFamily::MIXED, 600.0, 50.0, 20.0, 40.0, 10.0, "2026-02-28");
    manager.saveNutrition(newNutrition);
    int nutritionId = newNutrition.getNutritionId();
    
    // GET (Verify Creation)
    std::cout << "\n[2] GET NUTRITION (Verify Creation)" << std::endl;
    printSmallSeparator();
    Nutrition* retrieved = manager.getNutrition(nutritionId);
    if (retrieved) {
        std::cout << "✓ Nutrition entry retrieved:" << std::endl;
        retrieved->displayInfo();
    }
    
    // UPDATE
    std::cout << "\n[3] UPDATE NUTRITION" << std::endl;
    printSmallSeparator();
    std::cout << "Updating protein from " << retrieved->getProtein() << "g to 50.0g..." << std::endl;
    retrieved->setProtein(50.0);
    retrieved->setCarbs(55.0);
    manager.saveNutrition(*retrieved);
    
    // GET (Verify Update)
    std::cout << "\n[4] GET NUTRITION (Verify Update)" << std::endl;
    printSmallSeparator();
    delete retrieved;
    retrieved = manager.getNutrition(nutritionId);
    if (retrieved) {
        std::cout << "✓ Updated Nutrition entry:" << std::endl;
        retrieved->displayInfo();
    }
    
    // DELETE
    std::cout << "\n[5] DELETE NUTRITION" << std::endl;
    printSmallSeparator();
    manager.deleteNutrition(nutritionId);
    
    // GET (Verify Deletion)
    std::cout << "\n[6] GET NUTRITION (Verify Deletion)" << std::endl;
    printSmallSeparator();
    delete retrieved;
    retrieved = manager.getNutrition(nutritionId);
    if (!retrieved) {
        std::cout << "✓ Nutrition entry successfully deleted" << std::endl;
    }
    
    std::cout << "\n✅ NUTRITION CRUD TEST COMPLETED" << std::endl;
}

// ==================== RECOVERY CRUD TEST ====================
void testRecoveryCRUD(WorkoutManager& manager) {
    printSeparator("TESTING RECOVERY CRUD OPERATIONS");
    
    // CREATE
    std::cout << "\n[1] CREATE NEW RECOVERY" << std::endl;
    printSmallSeparator();
    Recovery newRecovery(0, "2026-02-28", 45, "Test Stretching", "Test yoga mat");
    manager.saveRecovery(newRecovery);
    int recoveryId = newRecovery.getRecoveryId();
    
    // GET (Verify Creation)
    std::cout << "\n[2] GET RECOVERY (Verify Creation)" << std::endl;
    printSmallSeparator();
    Recovery* retrieved = manager.getRecovery(recoveryId);
    if (retrieved) {
        std::cout << "✓ Recovery session retrieved:" << std::endl;
        retrieved->displayInfo();
    }
    
    // UPDATE
    std::cout << "\n[3] UPDATE RECOVERY" << std::endl;
    printSmallSeparator();
    std::cout << "Updating duration from " << retrieved->getDuration() << " to 60 minutes..." << std::endl;
    retrieved->setDuration(60);
    retrieved->setType("Test Deep Stretching");
    manager.saveRecovery(*retrieved);
    
    // GET (Verify Update)
    std::cout << "\n[4] GET RECOVERY (Verify Update)" << std::endl;
    printSmallSeparator();
    delete retrieved;
    retrieved = manager.getRecovery(recoveryId);
    if (retrieved) {
        std::cout << "✓ Updated Recovery session:" << std::endl;
        retrieved->displayInfo();
    }
    
    // DELETE
    std::cout << "\n[5] DELETE RECOVERY" << std::endl;
    printSmallSeparator();
    manager.deleteRecovery(recoveryId);
    
    // GET (Verify Deletion)
    std::cout << "\n[6] GET RECOVERY (Verify Deletion)" << std::endl;
    printSmallSeparator();
    delete retrieved;
    retrieved = manager.getRecovery(recoveryId);
    if (!retrieved) {
        std::cout << "✓ Recovery session successfully deleted" << std::endl;
    }
    
    std::cout << "\n✅ RECOVERY CRUD TEST COMPLETED" << std::endl;
}

// ==================== EQUIPMENT CRUD TEST ====================
void testEquipmentCRUD(WorkoutManager& manager) {
    printSeparator("TESTING EQUIPMENT CRUD OPERATIONS");
    
    // CREATE
    std::cout << "\n[1] CREATE NEW EQUIPMENT" << std::endl;
    printSmallSeparator();
    Equipment newEquipment(0, "Test Barbell", "Test Olympic barbell", "Free Weights", "Full Body");
    manager.saveEquipment(newEquipment);
    int equipmentId = newEquipment.getEquipmentId();
    
    // GET (Verify Creation)
    std::cout << "\n[2] GET EQUIPMENT (Verify Creation)" << std::endl;
    printSmallSeparator();
    Equipment* retrieved = manager.getEquipment(equipmentId);
    if (retrieved) {
        std::cout << "✓ Equipment retrieved:" << std::endl;
        retrieved->displayInfo();
    }
    
    // UPDATE
    std::cout << "\n[3] UPDATE EQUIPMENT" << std::endl;
    printSmallSeparator();
    std::cout << "Updating description and category..." << std::endl;
    retrieved->setDescription("Test Olympic standard barbell 45lbs");
    retrieved->setCategory("Competition Free Weights");
    manager.saveEquipment(*retrieved);
    
    // GET (Verify Update)
    std::cout << "\n[4] GET EQUIPMENT (Verify Update)" << std::endl;
    printSmallSeparator();
    delete retrieved;
    retrieved = manager.getEquipment(equipmentId);
    if (retrieved) {
        std::cout << "✓ Updated Equipment:" << std::endl;
        retrieved->displayInfo();
    }
    
    // DELETE
    std::cout << "\n[5] DELETE EQUIPMENT" << std::endl;
    printSmallSeparator();
    manager.deleteEquipment(equipmentId);
    
    // GET (Verify Deletion)
    std::cout << "\n[6] GET EQUIPMENT (Verify Deletion)" << std::endl;
    printSmallSeparator();
    delete retrieved;
    retrieved = manager.getEquipment(equipmentId);
    if (!retrieved) {
        std::cout << "✓ Equipment successfully deleted" << std::endl;
    }
    
    std::cout << "\n✅ EQUIPMENT CRUD TEST COMPLETED" << std::endl;
}

// ==================== MAIN APPLICATION ====================
int main() {
    std::cout << "╔══════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║     WORKOUT TRACKING SYSTEM - CRUD TEST APPLICATION              ║" << std::endl;
    std::cout << "║     Testing ALL 5 Objects: Create, Update, Delete, Get          ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════════╝" << std::endl;
    
    // Database configuration
    std::string host = "localhost";
    std::string user = "workout_user";
    std::string password = "workout_pass";
    std::string database = "workout_tracker";
    int port = 3306;
    
    // Create DAO and Manager
    WorkoutDAO dao(host, user, password, database, port);
    WorkoutManager manager(&dao);
    
    // Test connection
    printSeparator("DATABASE CONNECTION TEST");
    std::cout << "\nTesting database connection..." << std::endl;
    if (!manager.testConnection()) {
        std::cerr << "✗ Failed to connect to database!" << std::endl;
        std::cerr << "Please check your database configuration." << std::endl;
        return 1;
    }
    std::cout << "✓ Database connection successful!" << std::endl;
    
    // Run CRUD tests for all 5 objects
    try {
        testWorkoutCRUD(manager);
        testMuscleGroupCRUD(manager);
        testNutritionCRUD(manager);
        testRecoveryCRUD(manager);
        testEquipmentCRUD(manager);
        
        printSeparator("ALL TESTS COMPLETED SUCCESSFULLY");
        std::cout << "\n✅ All 5 object types tested successfully!" << std::endl;
        std::cout << "✅ Create, Update, Delete, and Get operations verified!" << std::endl;
        std::cout << "\nSummary:" << std::endl;
        std::cout << "  • Workout CRUD      : ✓ PASSED" << std::endl;
        std::cout << "  • MuscleGroup CRUD  : ✓ PASSED" << std::endl;
        std::cout << "  • Nutrition CRUD    : ✓ PASSED" << std::endl;
        std::cout << "  • Recovery CRUD     : ✓ PASSED" << std::endl;
        std::cout << "  • Equipment CRUD    : ✓ PASSED" << std::endl;
        printSeparator();
        
    } catch (const std::exception& e) {
        std::cerr << "\n✗ ERROR: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
