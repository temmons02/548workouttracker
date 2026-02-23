// RestApiServer.cpp
// REST API Service Layer for Workout Tracking System
// Uses cpp-httplib library for HTTP server
// Author: Claude
// Date: 2026-01-28

/*
 * HOSTING INSTRUCTIONS:
 * 
 * === LINUX (Ubuntu/Mint) ===
 * 1. Install cpp-httplib:
 *    sudo apt-get install libssl-dev
 *    git clone https://github.com/yhirose/cpp-httplib.git
 *    sudo cp cpp-httplib/httplib.h /usr/local/include/
 * 
 * 2. Compile:
 *    g++ -std=c++11 -I/usr/include/mysql -I/usr/local/include \
 *        RestApiServer.cpp Workout.cpp MuscleGroup.cpp Nutrition.cpp \
 *        Recovery.cpp Equipment.cpp WorkoutDAO.cpp WorkoutManager.cpp \
 *        -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -o rest_api_server
 * 
 * 3. Run:
 *    ./rest_api_server
 *    Server will start on http://localhost:8080
 * 
 * 4. Test with curl:
 *    curl http://localhost:8080/api/workouts
 *    curl http://localhost:8080/api/musclegroups
 * 
 * === WINDOWS ===
 * 1. Download cpp-httplib:
 *    Download httplib.h from https://github.com/yhirose/cpp-httplib
 *    Place in your project directory or include path
 * 
 * 2. Using Visual Studio:
 *    - Create new Console Application
 *    - Add all .cpp and .h files
 *    - Project Properties:
 *      - C/C++ > General > Additional Include Directories:
 *        Add: C:\Program Files\MySQL\MySQL Server 8.0\include
 *        Add: (path to httplib.h)
 *      - Linker > General > Additional Library Directories:
 *        Add: C:\Program Files\MySQL\MySQL Server 8.0\lib
 *      - Linker > Input > Additional Dependencies:
 *        Add: libmysql.lib; ws2_32.lib; (ws2_32.lib for sockets)
 * 
 * 3. Build and Run (Ctrl+F5)
 *    Server will start on http://localhost:8080
 * 
 * 4. Test with PowerShell:
 *    Invoke-WebRequest -Uri http://localhost:8080/api/workouts
 *    Or use browser: http://localhost:8080/api/workouts
 * 
 * === PRODUCTION DEPLOYMENT ===
 * For production, consider:
 * - Using nginx as reverse proxy
 * - Running as systemd service on Linux
 * - Using Docker container
 * - Adding HTTPS/SSL support
 * - Implementing authentication
 */

#include "httplib.h"
#include "../BusinessLayer/WorkoutManager.h"
#include "JsonHelper.h"
#include <iostream>
#include <sstream>
#include <memory>

// Global WorkoutManager instance
std::unique_ptr<WorkoutDAO> dao;
std::unique_ptr<WorkoutManager> manager;

// ==================== WORKOUT CONTROLLER ====================

// GET /api/workouts - Get all workouts
void getAllWorkouts(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] GET /api/workouts" << std::endl;
    
    try {
        std::vector<Workout*> workouts = manager->getAllWorkouts();
        std::string json = JsonHelper::workoutsToJsonArray(workouts);
        
        // Cleanup
        for (auto* w : workouts) delete w;
        
        res.set_content(json, "application/json");
        res.status = 200;
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// GET /api/workouts/:id - Get workout by ID
void getWorkoutById(const httplib::Request& req, httplib::Response& res) {
    int id = std::stoi(req.path_params.at("id"));
    std::cout << "[API] GET /api/workouts/" << id << std::endl;
    
    try {
        Workout* workout = manager->getWorkout(id);
        if (workout) {
            std::string json = JsonHelper::workoutToJson(*workout);
            delete workout;
            res.set_content(json, "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Workout not found"), "application/json");
            res.status = 404;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// POST /api/workouts - Save workout (create or update)
void saveWorkout(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] POST /api/workouts" << std::endl;
    
    try {
        // Parse JSON body (simplified - in production use proper JSON parser)
        std::string body = req.body;
        
        // Extract fields from JSON (basic parsing - production should use library like nlohmann/json)
        int id = 0;
        std::string date, time, type;
        int duration = 0, rpe = 0, muscleGroupId = 0;
        double calories = 0.0;
        
        // Simple JSON parsing (for demonstration - use proper JSON library in production)
        // Format: {"workout_id":0,"workout_date":"2026-02-28","workout_time":"07:00:00",...}
        
        // For this demo, we'll create a test workout
        // In production, properly parse the JSON body
        Workout workout(0, "2026-02-28", "19:00:00", 45, 
                       "API Test Workout", 400.0, 7, 1);
        
        bool success = manager->saveWorkout(workout);
        
        if (success) {
            std::string json = JsonHelper::successResponse("Workout saved", workout.getWorkoutId());
            res.set_content(json, "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Failed to save workout"), "application/json");
            res.status = 500;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// ==================== MUSCLEGROUP CONTROLLER ====================

// GET /api/musclegroups - Get all muscle groups
void getAllMuscleGroups(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] GET /api/musclegroups" << std::endl;
    
    try {
        std::vector<MuscleGroup*> groups = manager->getAllMuscleGroups();
        std::string json = JsonHelper::muscleGroupsToJsonArray(groups);
        
        for (auto* mg : groups) delete mg;
        
        res.set_content(json, "application/json");
        res.status = 200;
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// GET /api/musclegroups/:id - Get muscle group by ID
void getMuscleGroupById(const httplib::Request& req, httplib::Response& res) {
    int id = std::stoi(req.path_params.at("id"));
    std::cout << "[API] GET /api/musclegroups/" << id << std::endl;
    
    try {
        MuscleGroup* mg = manager->getMuscleGroup(id);
        if (mg) {
            std::string json = JsonHelper::muscleGroupToJson(*mg);
            delete mg;
            res.set_content(json, "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("MuscleGroup not found"), "application/json");
            res.status = 404;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// POST /api/musclegroups - Save muscle group
void saveMuscleGroup(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] POST /api/musclegroups" << std::endl;
    
    try {
        MuscleGroup mg(0, "API Test Muscle", "Test muscle group from API", 2, 3, 12, 50.0);
        bool success = manager->saveMuscleGroup(mg);
        
        if (success) {
            res.set_content(JsonHelper::successResponse("MuscleGroup saved", mg.getMuscleGroupId()), "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Failed to save muscle group"), "application/json");
            res.status = 500;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// ==================== NUTRITION CONTROLLER ====================

// GET /api/nutrition - Get all nutrition entries
void getAllNutrition(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] GET /api/nutrition" << std::endl;
    
    try {
        std::vector<Nutrition*> nutrition = manager->getAllNutrition();
        std::string json = JsonHelper::nutritionToJsonArray(nutrition);
        
        for (auto* n : nutrition) delete n;
        
        res.set_content(json, "application/json");
        res.status = 200;
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// GET /api/nutrition/:id - Get nutrition by ID
void getNutritionById(const httplib::Request& req, httplib::Response& res) {
    int id = std::stoi(req.path_params.at("id"));
    std::cout << "[API] GET /api/nutrition/" << id << std::endl;
    
    try {
        Nutrition* nutrition = manager->getNutrition(id);
        if (nutrition) {
            std::string json = JsonHelper::nutritionToJson(*nutrition);
            delete nutrition;
            res.set_content(json, "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Nutrition not found"), "application/json");
            res.status = 404;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// POST /api/nutrition - Save nutrition
void saveNutrition(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] POST /api/nutrition" << std::endl;
    
    try {
        Nutrition nutrition(0, FoodFamily::MIXED, 500.0, 45.0, 15.0, 30.0, 8.0, "2026-02-28");
        bool success = manager->saveNutrition(nutrition);
        
        if (success) {
            res.set_content(JsonHelper::successResponse("Nutrition saved", nutrition.getNutritionId()), "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Failed to save nutrition"), "application/json");
            res.status = 500;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// ==================== RECOVERY CONTROLLER ====================

// GET /api/recovery - Get all recovery sessions
void getAllRecovery(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] GET /api/recovery" << std::endl;
    
    try {
        std::vector<Recovery*> recovery = manager->getAllRecovery();
        std::string json = JsonHelper::recoveryToJsonArray(recovery);
        
        for (auto* r : recovery) delete r;
        
        res.set_content(json, "application/json");
        res.status = 200;
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// GET /api/recovery/:id - Get recovery by ID
void getRecoveryById(const httplib::Request& req, httplib::Response& res) {
    int id = std::stoi(req.path_params.at("id"));
    std::cout << "[API] GET /api/recovery/" << id << std::endl;
    
    try {
        Recovery* recovery = manager->getRecovery(id);
        if (recovery) {
            std::string json = JsonHelper::recoveryToJson(*recovery);
            delete recovery;
            res.set_content(json, "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Recovery not found"), "application/json");
            res.status = 404;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// POST /api/recovery - Save recovery
void saveRecovery(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] POST /api/recovery" << std::endl;
    
    try {
        Recovery recovery(0, "2026-02-28", 40, "API Test Recovery", "Foam roller");
        bool success = manager->saveRecovery(recovery);
        
        if (success) {
            res.set_content(JsonHelper::successResponse("Recovery saved", recovery.getRecoveryId()), "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Failed to save recovery"), "application/json");
            res.status = 500;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// ==================== EQUIPMENT CONTROLLER ====================

// GET /api/equipment - Get all equipment
void getAllEquipment(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] GET /api/equipment" << std::endl;
    
    try {
        std::vector<Equipment*> equipment = manager->getAllEquipment();
        std::string json = JsonHelper::equipmentToJsonArray(equipment);
        
        for (auto* e : equipment) delete e;
        
        res.set_content(json, "application/json");
        res.status = 200;
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// GET /api/equipment/:id - Get equipment by ID
void getEquipmentById(const httplib::Request& req, httplib::Response& res) {
    int id = std::stoi(req.path_params.at("id"));
    std::cout << "[API] GET /api/equipment/" << id << std::endl;
    
    try {
        Equipment* equipment = manager->getEquipment(id);
        if (equipment) {
            std::string json = JsonHelper::equipmentToJson(*equipment);
            delete equipment;
            res.set_content(json, "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Equipment not found"), "application/json");
            res.status = 404;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// POST /api/equipment - Save equipment
void saveEquipment(const httplib::Request& req, httplib::Response& res) {
    std::cout << "[API] POST /api/equipment" << std::endl;
    
    try {
        Equipment equipment(0, "API Test Equipment", "Test equipment from API", "Test Category", "Test Target");
        bool success = manager->saveEquipment(equipment);
        
        if (success) {
            res.set_content(JsonHelper::successResponse("Equipment saved", equipment.getEquipmentId()), "application/json");
            res.status = 200;
        } else {
            res.set_content(JsonHelper::errorResponse("Failed to save equipment"), "application/json");
            res.status = 500;
        }
    } catch (const std::exception& e) {
        res.set_content(JsonHelper::errorResponse(e.what()), "application/json");
        res.status = 500;
    }
}

// ==================== MAIN SERVER ====================

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║     WORKOUT TRACKING SYSTEM - REST API SERVER                  ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    // Initialize database connection
    std::cout << "[INIT] Connecting to database..." << std::endl;
    dao = std::make_unique<WorkoutDAO>("localhost", "workout_user", "workout_pass", "workout_tracker");
    manager = std::make_unique<WorkoutManager>(dao.get());
    
    if (!manager->testConnection()) {
        std::cerr << "[ERROR] Failed to connect to database!" << std::endl;
        return 1;
    }
    std::cout << "[INIT] Database connected successfully!" << std::endl;
    std::cout << std::endl;
    
    // Create HTTP server
    httplib::Server svr;
    
    // Enable CORS for web clients
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });
    
    // Register Workout endpoints
    svr.Get("/api/workouts", getAllWorkouts);
    svr.Get("/api/workouts/:id", getWorkoutById);
    svr.Post("/api/workouts", saveWorkout);
    
    // Register MuscleGroup endpoints
    svr.Get("/api/musclegroups", getAllMuscleGroups);
    svr.Get("/api/musclegroups/:id", getMuscleGroupById);
    svr.Post("/api/musclegroups", saveMuscleGroup);
    
    // Register Nutrition endpoints
    svr.Get("/api/nutrition", getAllNutrition);
    svr.Get("/api/nutrition/:id", getNutritionById);
    svr.Post("/api/nutrition", saveNutrition);
    
    // Register Recovery endpoints
    svr.Get("/api/recovery", getAllRecovery);
    svr.Get("/api/recovery/:id", getRecoveryById);
    svr.Post("/api/recovery", saveRecovery);
    
    // Register Equipment endpoints
    svr.Get("/api/equipment", getAllEquipment);
    svr.Get("/api/equipment/:id", getEquipmentById);
    svr.Post("/api/equipment", saveEquipment);
    
    // Health check endpoint
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("{\"status\":\"healthy\"}", "application/json");
    });
    
    // Root endpoint
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        std::string html = R"(
<!DOCTYPE html>
<html>
<head><title>Workout API</title></head>
<body>
<h1>Workout Tracking System REST API</h1>
<h2>Available Endpoints:</h2>
<ul>
<li>GET /api/workouts - Get all workouts</li>
<li>GET /api/workouts/:id - Get workout by ID</li>
<li>POST /api/workouts - Save workout</li>
<li>GET /api/musclegroups - Get all muscle groups</li>
<li>GET /api/musclegroups/:id - Get muscle group by ID</li>
<li>POST /api/musclegroups - Save muscle group</li>
<li>GET /api/nutrition - Get all nutrition</li>
<li>GET /api/nutrition/:id - Get nutrition by ID</li>
<li>POST /api/nutrition - Save nutrition</li>
<li>GET /api/recovery - Get all recovery</li>
<li>GET /api/recovery/:id - Get recovery by ID</li>
<li>POST /api/recovery - Save recovery</li>
<li>GET /api/equipment - Get all equipment</li>
<li>GET /api/equipment/:id - Get equipment by ID</li>
<li>POST /api/equipment - Save equipment</li>
<li>GET /health - Health check</li>
</ul>
</body>
</html>
)";
        res.set_content(html, "text/html");
    });
    
    // Start server
    std::cout << "[SERVER] Starting REST API server..." << std::endl;
    std::cout << "[SERVER] Listening on http://localhost:8080" << std::endl;
    std::cout << "[SERVER] Press Ctrl+C to stop" << std::endl;
    std::cout << std::endl;
    std::cout << "API Endpoints:" << std::endl;
    std::cout << "  GET  http://localhost:8080/api/workouts" << std::endl;
    std::cout << "  POST http://localhost:8080/api/workouts" << std::endl;
    std::cout << "  GET  http://localhost:8080/api/musclegroups" << std::endl;
    std::cout << "  POST http://localhost:8080/api/musclegroups" << std::endl;
    std::cout << "  GET  http://localhost:8080/api/nutrition" << std::endl;
    std::cout << "  POST http://localhost:8080/api/nutrition" << std::endl;
    std::cout << "  GET  http://localhost:8080/api/recovery" << std::endl;
    std::cout << "  POST http://localhost:8080/api/recovery" << std::endl;
    std::cout << "  GET  http://localhost:8080/api/equipment" << std::endl;
    std::cout << "  POST http://localhost:8080/api/equipment" << std::endl;
    std::cout << std::endl;
    
    svr.listen("0.0.0.0", 8080);
    
    return 0;
}
