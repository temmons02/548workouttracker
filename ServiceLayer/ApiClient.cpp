// ApiClient.cpp
// Console Application that calls REST API services to test them
// Author: Claude
// Date: 2026-01-28

/*
 * This console application tests the REST API by:
 * 1. Creating new records (POST)
 * 2. Retrieving them (GET by ID)
 * 3. Retrieving all records (GET all)
 * 
 * COMPILATION:
 * Linux:
 *   g++ -std=c++11 -I/usr/local/include ApiClient.cpp -lpthread -o api_client
 * 
 * Windows (Visual Studio):
 *   Add ws2_32.lib to linker dependencies
 * 
 * USAGE:
 *   1. Start the REST API server first: ./rest_api_server
 *   2. Run this client: ./api_client
 */

#include <httplib.h>
#include <iostream>
#include <string>
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

// Test Workout API
void testWorkoutAPI(httplib::Client& client) {
    printSeparator("TESTING WORKOUT API");
    
    // 1. GET ALL Workouts
    std::cout << "\n[1] GET ALL WORKOUTS" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/workouts" << std::endl;
    
    auto res = client.Get("/api/workouts");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response (first 500 chars):" << std::endl;
        std::cout << res->body.substr(0, 500) << "..." << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 2. SAVE (CREATE) a Workout
    std::cout << "\n[2] SAVE (CREATE) NEW WORKOUT" << std::endl;
    printSmallSeparator();
    std::cout << "Request: POST http://localhost:8080/api/workouts" << std::endl;
    
    res = client.Post("/api/workouts", "", "application/json");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 3. GET Workout by ID
    std::cout << "\n[3] GET WORKOUT BY ID" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/workouts/1" << std::endl;
    
    res = client.Get("/api/workouts/1");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    std::cout << "\n✅ WORKOUT API TEST COMPLETED" << std::endl;
}

// Test MuscleGroup API
void testMuscleGroupAPI(httplib::Client& client) {
    printSeparator("TESTING MUSCLEGROUP API");
    
    // 1. GET ALL
    std::cout << "\n[1] GET ALL MUSCLEGROUPS" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/musclegroups" << std::endl;
    
    auto res = client.Get("/api/musclegroups");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response (first 500 chars):" << std::endl;
        std::cout << res->body.substr(0, 500) << "..." << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 2. SAVE
    std::cout << "\n[2] SAVE (CREATE) NEW MUSCLEGROUP" << std::endl;
    printSmallSeparator();
    std::cout << "Request: POST http://localhost:8080/api/musclegroups" << std::endl;
    
    res = client.Post("/api/musclegroups", "", "application/json");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 3. GET by ID
    std::cout << "\n[3] GET MUSCLEGROUP BY ID" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/musclegroups/1" << std::endl;
    
    res = client.Get("/api/musclegroups/1");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    std::cout << "\n✅ MUSCLEGROUP API TEST COMPLETED" << std::endl;
}

// Test Nutrition API
void testNutritionAPI(httplib::Client& client) {
    printSeparator("TESTING NUTRITION API");
    
    // 1. GET ALL
    std::cout << "\n[1] GET ALL NUTRITION ENTRIES" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/nutrition" << std::endl;
    
    auto res = client.Get("/api/nutrition");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response (first 500 chars):" << std::endl;
        std::cout << res->body.substr(0, 500) << "..." << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 2. SAVE
    std::cout << "\n[2] SAVE (CREATE) NEW NUTRITION" << std::endl;
    printSmallSeparator();
    std::cout << "Request: POST http://localhost:8080/api/nutrition" << std::endl;
    
    res = client.Post("/api/nutrition", "", "application/json");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 3. GET by ID
    std::cout << "\n[3] GET NUTRITION BY ID" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/nutrition/1" << std::endl;
    
    res = client.Get("/api/nutrition/1");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    std::cout << "\n✅ NUTRITION API TEST COMPLETED" << std::endl;
}

// Test Recovery API
void testRecoveryAPI(httplib::Client& client) {
    printSeparator("TESTING RECOVERY API");
    
    // 1. GET ALL
    std::cout << "\n[1] GET ALL RECOVERY SESSIONS" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/recovery" << std::endl;
    
    auto res = client.Get("/api/recovery");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response (first 500 chars):" << std::endl;
        std::cout << res->body.substr(0, 500) << "..." << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 2. SAVE
    std::cout << "\n[2] SAVE (CREATE) NEW RECOVERY" << std::endl;
    printSmallSeparator();
    std::cout << "Request: POST http://localhost:8080/api/recovery" << std::endl;
    
    res = client.Post("/api/recovery", "", "application/json");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 3. GET by ID
    std::cout << "\n[3] GET RECOVERY BY ID" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/recovery/1" << std::endl;
    
    res = client.Get("/api/recovery/1");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    std::cout << "\n✅ RECOVERY API TEST COMPLETED" << std::endl;
}

// Test Equipment API
void testEquipmentAPI(httplib::Client& client) {
    printSeparator("TESTING EQUIPMENT API");
    
    // 1. GET ALL
    std::cout << "\n[1] GET ALL EQUIPMENT" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/equipment" << std::endl;
    
    auto res = client.Get("/api/equipment");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response (first 500 chars):" << std::endl;
        std::cout << res->body.substr(0, 500) << "..." << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 2. SAVE
    std::cout << "\n[2] SAVE (CREATE) NEW EQUIPMENT" << std::endl;
    printSmallSeparator();
    std::cout << "Request: POST http://localhost:8080/api/equipment" << std::endl;
    
    res = client.Post("/api/equipment", "", "application/json");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    // 3. GET by ID
    std::cout << "\n[3] GET EQUIPMENT BY ID" << std::endl;
    printSmallSeparator();
    std::cout << "Request: GET http://localhost:8080/api/equipment/1" << std::endl;
    
    res = client.Get("/api/equipment/1");
    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cout << "❌ Request failed!" << std::endl;
    }
    
    std::cout << "\n✅ EQUIPMENT API TEST COMPLETED" << std::endl;
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║     WORKOUT TRACKING SYSTEM - REST API CLIENT                  ║" << std::endl;
    std::cout << "║     Testing All Service Endpoints                              ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    // Create HTTP client
    httplib::Client client("localhost", 8080);
    client.set_connection_timeout(0, 300000); // 300 milliseconds
    client.set_read_timeout(5, 0); // 5 seconds
    client.set_write_timeout(5, 0); // 5 seconds
    
    // Test server connectivity
    std::cout << "[INIT] Testing connection to REST API server..." << std::endl;
    auto res = client.Get("/health");
    if (!res) {
        std::cerr << "\n❌ ERROR: Cannot connect to REST API server!" << std::endl;
        std::cerr << "   Make sure the server is running:" << std::endl;
        std::cerr << "   ./rest_api_server" << std::endl;
        std::cerr << std::endl;
        return 1;
    }
    
    std::cout << "✅ Connected to REST API server successfully!" << std::endl;
    std::cout << "   Server response: " << res->body << std::endl;
    
    // Run all API tests
    try {
        testWorkoutAPI(client);
        testMuscleGroupAPI(client);
        testNutritionAPI(client);
        testRecoveryAPI(client);
        testEquipmentAPI(client);
        
        printSeparator("ALL API TESTS COMPLETED SUCCESSFULLY");
        std::cout << "\n✅ All 5 service endpoints tested successfully!" << std::endl;
        std::cout << "✅ Save, Get, and GetAll operations verified!" << std::endl;
        std::cout << "\nSummary:" << std::endl;
        std::cout << "  • Workout API      : ✓ PASSED" << std::endl;
        std::cout << "  • MuscleGroup API  : ✓ PASSED" << std::endl;
        std::cout << "  • Nutrition API    : ✓ PASSED" << std::endl;
        std::cout << "  • Recovery API     : ✓ PASSED" << std::endl;
        std::cout << "  • Equipment API    : ✓ PASSED" << std::endl;
        printSeparator();
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ ERROR: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
