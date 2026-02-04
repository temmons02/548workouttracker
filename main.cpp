// main.cpp
// Workout Tracking System - Main Demonstration Program
// Author: Claude
// Date: 2026-01-28

#include "WorkoutDAO.h"
#include <iostream>
#include <vector>

void printSeparator() {
    std::cout << "\n========================================\n" << std::endl;
}

void demonstrateWorkoutOperations(WorkoutDAO& dao) {
    printSeparator();
    std::cout << "WORKOUT CRUD OPERATIONS DEMO" << std::endl;
    printSeparator();
    
    // Create a new workout
    Workout newWorkout(0, "2026-01-28", "08:00:00", 45, 
                       "Morning Chest Workout", 400.0, 8, 1);
    std::cout << "\n[CREATE] Creating new workout..." << std::endl;
    dao.createWorkout(newWorkout);
    
    // Read all workouts
    std::cout << "\n[READ ALL] Retrieving all workouts..." << std::endl;
    std::vector<Workout*> workouts = dao.readAllWorkouts();
    std::cout << "Found " << workouts.size() << " workouts:" << std::endl;
    for (int i = 0; i < std::min(3, (int)workouts.size()); i++) {
        std::cout << "  - " << workouts[i]->toString() << std::endl;
    }
    
    // Read workouts by date
    std::cout << "\n[READ BY DATE] Workouts on 2026-01-20:" << std::endl;
    std::vector<Workout*> dateWorkouts = dao.readWorkoutsByDate("2026-01-20");
    for (auto* w : dateWorkouts) {
        w->displayInfo();
        std::cout << std::endl;
    }
    
    // Update a workout
    if (!workouts.empty()) {
        std::cout << "\n[UPDATE] Updating first workout..." << std::endl;
        Workout* toUpdate = workouts[0];
        toUpdate->setCaloriesBurned(500.0);
        toUpdate->setRatePerceivedExhaustion(9);
        dao.updateWorkout(*toUpdate);
    }
    
    // Cleanup
    for (auto* w : workouts) delete w;
    for (auto* w : dateWorkouts) delete w;
}

void demonstrateMuscleGroupOperations(WorkoutDAO& dao) {
    printSeparator();
    std::cout << "MUSCLE GROUP CRUD OPERATIONS DEMO" << std::endl;
    printSeparator();
    
    // Create a new muscle group
    MuscleGroup newGroup(0, "Abs", "Abdominal muscles", 3, 4, 25, 0.0);
    std::cout << "\n[CREATE] Creating new muscle group..." << std::endl;
    dao.createMuscleGroup(newGroup);
    
    // Read all muscle groups
    std::cout << "\n[READ ALL] All muscle groups:" << std::endl;
    std::vector<MuscleGroup*> groups = dao.readAllMuscleGroups();
    for (auto* mg : groups) {
        std::cout << "  - " << mg->toString() << std::endl;
    }
    
    // Read by name
    std::cout << "\n[READ BY NAME] Finding 'Chest' muscle group:" << std::endl;
    MuscleGroup* chest = dao.readMuscleGroupByName("Chest");
    if (chest) {
        chest->displayInfo();
        delete chest;
    }
    
    // Cleanup
    for (auto* mg : groups) delete mg;
}

void demonstrateNutritionOperations(WorkoutDAO& dao) {
    printSeparator();
    std::cout << "NUTRITION CRUD OPERATIONS DEMO" << std::endl;
    printSeparator();
    
    // Create nutrition entry
    Nutrition newNutrition(0, FoodFamily::MIXED, 600.0, 50.0, 20.0, 40.0, 10.0, "2026-01-28");
    std::cout << "\n[CREATE] Creating nutrition entry..." << std::endl;
    dao.createNutrition(newNutrition);
    
    // Read by date
    std::cout << "\n[READ BY DATE] Nutrition for 2026-01-20:" << std::endl;
    std::vector<Nutrition*> nutrition = dao.readNutritionByDate("2026-01-20");
    for (auto* n : nutrition) {
        n->displayInfo();
        std::cout << "Carb Ratio: " << n->getMacroRatio("carbs") << "%" << std::endl;
        std::cout << "Protein Ratio: " << n->getMacroRatio("protein") << "%" << std::endl;
        std::cout << "Fat Ratio: " << n->getMacroRatio("fat") << "%" << std::endl;
        std::cout << std::endl;
    }
    
    // Read by family
    std::cout << "\n[READ BY FAMILY] All 'Fruit' entries:" << std::endl;
    std::vector<Nutrition*> fruits = dao.readNutritionByFamily("Fruit");
    std::cout << "Found " << fruits.size() << " fruit entries." << std::endl;
    
    // Cleanup
    for (auto* n : nutrition) delete n;
    for (auto* n : fruits) delete n;
}

void demonstrateRecoveryOperations(WorkoutDAO& dao) {
    printSeparator();
    std::cout << "RECOVERY CRUD OPERATIONS DEMO" << std::endl;
    printSeparator();
    
    // Create recovery entry
    Recovery newRecovery(0, "2026-01-28", 45, "Stretching", "Yoga mat, foam roller");
    std::cout << "\n[CREATE] Creating recovery entry..." << std::endl;
    dao.createRecovery(newRecovery);
    
    // Read all recovery
    std::cout << "\n[READ ALL] All recovery sessions:" << std::endl;
    std::vector<Recovery*> recoveries = dao.readAllRecovery();
    for (int i = 0; i < std::min(3, (int)recoveries.size()); i++) {
        std::cout << "  - " << recoveries[i]->toString();
        if (recoveries[i]->isLongRecovery()) {
            std::cout << " [LONG SESSION]";
        }
        std::cout << std::endl;
    }
    
    // Read by type
    std::cout << "\n[READ BY TYPE] Yoga sessions:" << std::endl;
    std::vector<Recovery*> yoga = dao.readRecoveryByType("Yoga Session");
    for (auto* r : yoga) {
        r->displayInfo();
        std::cout << std::endl;
    }
    
    // Cleanup
    for (auto* r : recoveries) delete r;
    for (auto* r : yoga) delete r;
}

void demonstrateEquipmentOperations(WorkoutDAO& dao) {
    printSeparator();
    std::cout << "EQUIPMENT CRUD OPERATIONS DEMO" << std::endl;
    printSeparator();
    
    // Create equipment
    Equipment newEquipment(0, "Ab Wheel", "Core strengthening roller", 
                          "Accessories", "Core, Abs");
    std::cout << "\n[CREATE] Creating equipment..." << std::endl;
    dao.createEquipment(newEquipment);
    
    // Read all equipment
    std::cout << "\n[READ ALL] All equipment:" << std::endl;
    std::vector<Equipment*> equipment = dao.readAllEquipment();
    for (int i = 0; i < std::min(5, (int)equipment.size()); i++) {
        std::cout << "  - " << equipment[i]->toString();
        if (equipment[i]->isCardioEquipment()) {
            std::cout << " [CARDIO]";
        }
        std::cout << std::endl;
    }
    
    // Read by category
    std::cout << "\n[READ BY CATEGORY] Free Weights:" << std::endl;
    std::vector<Equipment*> freeWeights = dao.readEquipmentByCategory("Free Weights");
    for (auto* e : freeWeights) {
        e->displayInfo();
        std::cout << std::endl;
    }
    
    // Cleanup
    for (auto* e : equipment) delete e;
    for (auto* e : freeWeights) delete e;
}

int main() {
    std::cout << "===========================================\n"
              << "  WORKOUT TRACKING SYSTEM DEMONSTRATION\n"
              << "===========================================\n" << std::endl;
    
    // Database configuration
    std::string host = "localhost";
    std::string user = "workout_user";
    std::string password = "workout_pass";
    std::string database = "workout_tracker";
    int port = 3306;
    
    // Create DAO instance
    WorkoutDAO dao(host, user, password, database, port);
    
    // Test connection
    std::cout << "Testing database connection..." << std::endl;
    if (!dao.testConnection()) {
        std::cerr << "Failed to connect to database!" << std::endl;
        std::cerr << "Please update the database credentials in main.cpp" << std::endl;
        return 1;
    }
    
    // Demonstrate all operations
    try {
        demonstrateWorkoutOperations(dao);
        demonstrateMuscleGroupOperations(dao);
        demonstrateNutritionOperations(dao);
        demonstrateRecoveryOperations(dao);
        demonstrateEquipmentOperations(dao);
        
        printSeparator();
        std::cout << "ALL DEMONSTRATIONS COMPLETED SUCCESSFULLY!" << std::endl;
        printSeparator();
        
    } catch (const std::exception& e) {
        std::cerr << "Error during demonstration: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
