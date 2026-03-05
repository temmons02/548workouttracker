// cgi-bin/workout_cgi.cpp
// CGI Web Application for Workout Tracking System
// Handles all CRUD operations via web interface
// Author: Claude
// Date: 2026-01-28

/*
 * CGI APPLICATION - Main Handler
 * 
 * This CGI program provides a web-based front-end for the Workout Tracking System.
 * It handles all HTTP requests and routes them to the appropriate service methods.
 * 
 * FEATURES:
 * - INSERT: Create new records
 * - UPDATE: Modify existing records
 * - DELETE: Remove records
 * - GET: Retrieve single record
 * - GET ALL: Retrieve all records
 * - GET SUBSET: Filter records by criteria
 * 
 * SUPPORTED TABLES:
 * - Workout
 * - MuscleGroup
 * - Nutrition
 * - Recovery
 * - Equipment
 * 
 * COMPILATION:
 * g++ -std=c++17 -Isrc -I/usr/include/mysql \
 *     cgi-bin/workout_cgi.cpp \
 *     ServiceLayer/WorkoutService.cpp \
 *     src/models/*.cpp src/data/*.cpp src/business/*.cpp \
 *     -L/usr/lib/x86_64-linux-gnu -lmysqlclient \
 *     -o /usr/lib/cgi-bin/workout.cgi
 * 
 * APACHE SETUP:
 * 1. Enable CGI: sudo a2enmod cgi
 * 2. Copy to: /usr/lib/cgi-bin/
 * 3. Set permissions: chmod +x /usr/lib/cgi-bin/workout.cgi
 * 4. Restart Apache: sudo systemctl restart apache2
 * 
 * ACCESS:
 * http://localhost/cgi-bin/workout.cgi
 */

#include "../ServiceLayer/WorkoutService.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <sstream>
#include <cstdlib>
#include <algorithm>

// ==================== CGI UTILITY FUNCTIONS ====================

// Parse query string into map
std::map<std::string, std::string> parseQueryString(const std::string& query) {
    std::map<std::string, std::string> params;
    std::istringstream iss(query);
    std::string pair;
    
    while (std::getline(iss, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            
            // URL decode
            std::string decoded;
            for (size_t i = 0; i < value.length(); ++i) {
                if (value[i] == '+') {
                    decoded += ' ';
                } else if (value[i] == '%' && i + 2 < value.length()) {
                    int hex;
                    std::istringstream hexStream(value.substr(i + 1, 2));
                    hexStream >> std::hex >> hex;
                    decoded += static_cast<char>(hex);
                    i += 2;
                } else {
                    decoded += value[i];
                }
            }
            params[key] = decoded;
        }
    }
    
    return params;
}

// Get environment variable
std::string getEnv(const char* name) {
    const char* value = std::getenv(name);
    return value ? std::string(value) : "";
}

// URL encode
std::string urlEncode(const std::string& str) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;
    
    for (char c : str) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int((unsigned char)c);
        }
    }
    
    return escaped.str();
}

// HTML escape
std::string htmlEscape(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        switch (c) {
            case '&':  escaped += "&amp;"; break;
            case '<':  escaped += "&lt;"; break;
            case '>':  escaped += "&gt;"; break;
            case '"':  escaped += "&quot;"; break;
            case '\'': escaped += "&#39;"; break;
            default:   escaped += c; break;
        }
    }
    return escaped;
}

// ==================== HTML GENERATION ====================

void printHTMLHeader(const std::string& title) {
    std::cout << "Content-Type: text/html\r\n\r\n";
    std::cout << R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>)" << title << R"(</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: white;
            border-radius: 10px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.3);
            padding: 30px;
        }
        h1 {
            color: #333;
            margin-bottom: 10px;
            border-bottom: 3px solid #667eea;
            padding-bottom: 10px;
        }
        h2 {
            color: #555;
            margin-top: 30px;
            margin-bottom: 15px;
        }
        .nav {
            background: #f8f9fa;
            padding: 15px;
            border-radius: 5px;
            margin-bottom: 30px;
        }
        .nav a {
            display: inline-block;
            padding: 10px 20px;
            margin: 5px;
            background: #667eea;
            color: white;
            text-decoration: none;
            border-radius: 5px;
            transition: background 0.3s;
        }
        .nav a:hover { background: #764ba2; }
        .form-group {
            margin-bottom: 15px;
        }
        label {
            display: block;
            margin-bottom: 5px;
            font-weight: 600;
            color: #555;
        }
        input, select, textarea {
            width: 100%;
            padding: 10px;
            border: 2px solid #ddd;
            border-radius: 5px;
            font-size: 14px;
        }
        input:focus, select:focus, textarea:focus {
            outline: none;
            border-color: #667eea;
        }
        button, .btn {
            padding: 12px 30px;
            background: #667eea;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
            font-weight: 600;
            transition: background 0.3s;
        }
        button:hover, .btn:hover { background: #764ba2; }
        .btn-delete {
            background: #dc3545;
            padding: 8px 15px;
            font-size: 14px;
        }
        .btn-delete:hover { background: #c82333; }
        .btn-edit {
            background: #28a745;
            padding: 8px 15px;
            font-size: 14px;
        }
        .btn-edit:hover { background: #218838; }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
            background: white;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        th {
            background: #667eea;
            color: white;
            padding: 12px;
            text-align: left;
        }
        td {
            padding: 10px 12px;
            border-bottom: 1px solid #eee;
        }
        tr:hover { background: #f8f9fa; }
        .success {
            background: #d4edda;
            color: #155724;
            padding: 15px;
            border-radius: 5px;
            margin-bottom: 20px;
            border: 1px solid #c3e6cb;
        }
        .error {
            background: #f8d7da;
            color: #721c24;
            padding: 15px;
            border-radius: 5px;
            margin-bottom: 20px;
            border: 1px solid #f5c6cb;
        }
        .stats {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        .stat-card {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 20px;
            border-radius: 10px;
            text-align: center;
        }
        .stat-card h3 { font-size: 36px; margin-bottom: 5px; }
        .stat-card p { font-size: 14px; opacity: 0.9; }
        .filter-form {
            background: #f8f9fa;
            padding: 20px;
            border-radius: 5px;
            margin-bottom: 20px;
        }
        .filter-form input, .filter-form select {
            display: inline-block;
            width: auto;
            margin-right: 10px;
        }
    </style>
</head>
<body>
<div class="container">
)";
}

void printHTMLFooter() {
    std::cout << R"(
</div>
</body>
</html>
)";
}

void printNavigation() {
    std::cout << R"(<div class="nav">
    <a href="workout.cgi">Home</a>
    <a href="workout.cgi?action=list&table=workout">Workouts</a>
    <a href="workout.cgi?action=list&table=musclegroup">Muscle Groups</a>
    <a href="workout.cgi?action=list&table=nutrition">Nutrition</a>
    <a href="workout.cgi?action=list&table=recovery">Recovery</a>
    <a href="workout.cgi?action=list&table=equipment">Equipment</a>
</div>
)";
}

// ==================== PAGE HANDLERS ====================

void showHomePage(WorkoutService& service) {
    printHTMLHeader("Workout Tracking System");
    
    std::cout << "<h1>🏋️ Workout Tracking System</h1>\n";
    printNavigation();
    
    std::cout << "<h2>Database Statistics</h2>\n";
    std::cout << "<div class=\"stats\">\n";
    std::cout << "  <div class=\"stat-card\">\n";
    std::cout << "    <h3>" << service.getTotalWorkouts() << "</h3>\n";
    std::cout << "    <p>Workouts</p>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"stat-card\">\n";
    std::cout << "    <h3>" << service.getTotalMuscleGroups() << "</h3>\n";
    std::cout << "    <p>Muscle Groups</p>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"stat-card\">\n";
    std::cout << "    <h3>" << service.getTotalNutrition() << "</h3>\n";
    std::cout << "    <p>Nutrition Entries</p>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"stat-card\">\n";
    std::cout << "    <h3>" << service.getTotalRecovery() << "</h3>\n";
    std::cout << "    <p>Recovery Sessions</p>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"stat-card\">\n";
    std::cout << "    <h3>" << service.getTotalEquipment() << "</h3>\n";
    std::cout << "    <p>Equipment Items</p>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << R"(
<h2>Quick Actions</h2>
<div class="nav">
    <a href="workout.cgi?action=insert_form&table=workout">➕ Add Workout</a>
    <a href="workout.cgi?action=insert_form&table=musclegroup">➕ Add Muscle Group</a>
    <a href="workout.cgi?action=insert_form&table=nutrition">➕ Add Nutrition</a>
    <a href="workout.cgi?action=insert_form&table=recovery">➕ Add Recovery</a>
    <a href="workout.cgi?action=insert_form&table=equipment">➕ Add Equipment</a>
</div>

<h2>About This System</h2>
<p>This web-based interface allows you to manage your workout tracking data through a browser. 
You can create, view, update, and delete records across all five tables in the database.</p>

<h3>Features:</h3>
<ul style="margin-left: 30px; line-height: 2;">
    <li><strong>INSERT:</strong> Create new records with full validation</li>
    <li><strong>UPDATE:</strong> Modify existing records</li>
    <li><strong>DELETE:</strong> Remove records with confirmation</li>
    <li><strong>GET:</strong> View individual records</li>
    <li><strong>GET ALL:</strong> List all records in a table</li>
    <li><strong>FILTER:</strong> Search and filter records by criteria</li>
</ul>
)";
    
    printHTMLFooter();
}

// Forward declarations for list/form handlers
void showWorkoutList(WorkoutService& service, const std::map<std::string, std::string>& params);
void showWorkoutInsertForm();
void showWorkoutUpdateForm(WorkoutService& service, int id);
void handleWorkoutInsert(WorkoutService& service, const std::map<std::string, std::string>& params);
void handleWorkoutUpdate(WorkoutService& service, const std::map<std::string, std::string>& params);
void handleWorkoutDelete(WorkoutService& service, int id);

void viewWorkout(WorkoutService& service, int id) {
    printHTMLHeader("View Workout");
    
    Workout* w = service.getWorkout(id);
    
    if (!w) {
        std::cout << "<div class=\"alert alert-danger\">Workout not found!</div>\n";
        std::cout << "<a href=\"workout.cgi?action=list&table=workout\" class=\"btn\">Back to List</a>\n";
        printHTMLFooter();
        return;
    }
    
    std::cout << "<h2>💪 Workout Details</h2>\n";
    
    std::cout << "<table style=\"max-width: 600px;\">\n";
    std::cout << "<tr><th>Field</th><th>Value</th></tr>\n";
    std::cout << "<tr><td><strong>ID</strong></td><td>" << w->getWorkoutId() << "</td></tr>\n";
    std::cout << "<tr><td><strong>Date</strong></td><td>" << htmlEscape(w->getWorkoutDate()) << "</td></tr>\n";
    std::cout << "<tr><td><strong>Time</strong></td><td>" << htmlEscape(w->getWorkoutTime()) << "</td></tr>\n";
    std::cout << "<tr><td><strong>Duration</strong></td><td>" << w->getDuration() << " minutes</td></tr>\n";
    std::cout << "<tr><td><strong>Type</strong></td><td>" << htmlEscape(w->getTypeDescription()) << "</td></tr>\n";
    std::cout << "<tr><td><strong>Calories Burned</strong></td><td>" << w->getCaloriesBurned() << "</td></tr>\n";
    std::cout << "<tr><td><strong>RPE</strong></td><td>" << w->getRatePerceivedExhaustion() << "/10</td></tr>\n";
    std::cout << "<tr><td><strong>Muscle Group ID</strong></td><td>" << w->getMuscleGroupId() << "</td></tr>\n";
    std::cout << "</table>\n";
    
    std::cout << "<div style=\"margin-top: 20px;\">\n";
    std::cout << "<a href=\"workout.cgi?action=update_form&table=workout&id=" << id << "\" class=\"btn\">✏️ Edit</a>\n";
    std::cout << "<a href=\"workout.cgi?action=delete&table=workout&id=" << id << "\" class=\"btn btn-danger\" "
              << "onclick=\"return confirm('Delete this workout?')\">🗑️ Delete</a>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=workout\" class=\"btn btn-warning\">📋 Back to List</a>\n";
    std::cout << "</div>\n";
    
    delete w;
    printHTMLFooter();
}




// (Similar handlers for MuscleGroup, Nutrition, Recovery, Equipment...)


void showWorkoutUpdateForm(WorkoutService& service, int id) {
    printHTMLHeader("Edit Workout");
    
    Workout* w = service.getWorkout(id);
    if (!w) {
        std::cout << "<div class=\"alert alert-danger\">Workout not found!</div>\n";
        printHTMLFooter();
        return;
    }
    
    std::cout << "<h2>Edit Workout #" << id << "</h2>\n";
    std::cout << "<form method=\"POST\" action=\"workout.cgi\">\n";
    std::cout << "<input type=\"hidden\" name=\"action\" value=\"update\">\n";
    std::cout << "<input type=\"hidden\" name=\"table\" value=\"workout\">\n";
    std::cout << "<input type=\"hidden\" name=\"id\" value=\"" << id << "\">\n";
    
    std::cout << "<div class=\"form-group\"><label>Date:</label>\n";
    std::cout << "<input type=\"date\" name=\"date\" value=\"" << w->getWorkoutDate() << "\"></div>\n";
    
    std::cout << "<div class=\"form-group\"><label>Time:</label>\n";
    std::cout << "<input type=\"time\" name=\"time\" value=\"" << w->getWorkoutTime().substr(0,5) << "\"></div>\n";
    
    std::cout << "<div class=\"form-group\"><label>Duration:</label>\n";
    std::cout << "<input type=\"number\" name=\"duration\" value=\"" << w->getDuration() << "\"></div>\n";
    
    std::cout << "<div class=\"form-group\"><label>Type:</label>\n";
    std::cout << "<textarea name=\"type\">" << w->getTypeDescription() << "</textarea></div>\n";
    
    std::cout << "<div class=\"form-group\"><label>Calories:</label>\n";
    std::cout << "<input type=\"number\" name=\"calories\" value=\"" << w->getCaloriesBurned() << "\"></div>\n";
    
    std::cout << "<div class=\"form-group\"><label>RPE:</label>\n";
    std::cout << "<input type=\"number\" name=\"rpe\" value=\"" << w->getRatePerceivedExhaustion() << "\"></div>\n";
    
    std::cout << "<div class=\"form-group\"><label>Muscle Group ID:</label>\n";
    std::cout << "<input type=\"number\" name=\"muscle_group_id\" value=\"" << w->getMuscleGroupId() << "\"></div>\n";
    
    std::cout << "<button type=\"submit\">Save</button>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=workout\">Cancel</a>\n";
    std::cout << "</form>\n";
    
    delete w;
    printHTMLFooter();
}

void handleWorkoutUpdate(WorkoutService& service, const std::map<std::string, std::string>& params) {
    int id = std::stoi(params.at("id"));
    
    ServiceResponse response = service.updateWorkout(
        id,
        params.at("date"),
        params.at("time") + ":00",
        std::stoi(params.at("duration")),
        params.at("type"),
        std::stod(params.at("calories")),
        std::stoi(params.at("rpe")),
        std::stoi(params.at("muscle_group_id"))
    );
    
    printHTMLHeader("Workout Updated");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✓ " << response.message << "</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">✗ " << response.message << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=workout\">Back to List</a>\n";
    printHTMLFooter();
}

void handleWorkoutDelete(WorkoutService& service, int id) {
    ServiceResponse response = service.deleteWorkout(id);
    
    printHTMLHeader("Workout Deleted");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✓ Workout deleted</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">✗ Failed to delete</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=workout\">Back to List</a>\n";
    printHTMLFooter();
}

// ==================== MUSCLEGROUP HANDLERS ====================

void listMuscleGroups(WorkoutService& service, const std::map<std::string, std::string>& params) {
    printHTMLHeader("Muscle Groups - List All");
    
    std::cout << "<div class=\"action-bar\">\n";
    std::cout << "  <h2>🎯 Muscle Groups</h2>\n";
    std::cout << "  <a href=\"workout.cgi?action=insert_form&table=musclegroup\" class=\"btn btn-success\">➕ Add New Muscle Group</a>\n";
    std::cout << "</div>\n";
    
    std::vector<MuscleGroup*> groups = service.getAllMuscleGroups();
    
    std::cout << "<p>Total muscle groups: <strong>" << groups.size() << "</strong></p>\n";
    
    if (groups.empty()) {
        std::cout << "<p>No muscle groups found.</p>\n";
    } else {
        std::cout << "<table>\n";
        std::cout << "<tr>\n";
        std::cout << "  <th>ID</th><th>Name</th><th>Description</th><th>Days/Week</th>\n";
        std::cout << "  <th>Sets</th><th>Reps</th><th>Weight (lbs)</th><th>Actions</th>\n";
        std::cout << "</tr>\n";
        
        for (MuscleGroup* mg : groups) {
            std::cout << "<tr>\n";
            std::cout << "  <td>" << mg->getMuscleGroupId() << "</td>\n";
            std::cout << "  <td>" << htmlEscape(mg->getName()) << "</td>\n";
            std::cout << "  <td>" << htmlEscape(mg->getDescription().substr(0, 50)) << "</td>\n";
            std::cout << "  <td>" << mg->getDaysPerWeek() << "</td>\n";
            std::cout << "  <td>" << mg->getSets() << "</td>\n";
            std::cout << "  <td>" << mg->getReps() << "</td>\n";
            std::cout << "  <td>" << mg->getWeightAmount() << "</td>\n";
            std::cout << "  <td>\n";
            std::cout << "    <a href=\"workout.cgi?action=view&table=musclegroup&id=" << mg->getMuscleGroupId() << "\" class=\"btn btn-info btn-sm\">👁️ View</a>\n";
            std::cout << "    <a href=\"workout.cgi?action=update_form&table=musclegroup&id=" << mg->getMuscleGroupId() << "\" class=\"btn btn-sm\">✏️ Edit</a>\n";
            std::cout << "    <a href=\"workout.cgi?action=delete&table=musclegroup&id=" << mg->getMuscleGroupId() << "\" class=\"btn btn-danger btn-sm\" onclick=\"return confirm('Delete this muscle group?')\">🗑️ Delete</a>\n";
            std::cout << "  </td>\n";
            std::cout << "</tr>\n";
            delete mg;
        }
        
        std::cout << "</table>\n";
    }
    
    printHTMLFooter();
}

void viewMuscleGroup(WorkoutService& service, int id) {
    printHTMLHeader("View Muscle Group");
    
    MuscleGroup* mg = service.getMuscleGroup(id);
    
    if (!mg) {
        std::cout << "<div class=\"alert alert-danger\">Muscle Group not found!</div>\n";
        std::cout << "<a href=\"workout.cgi?action=list&table=musclegroup\" class=\"btn\">Back to List</a>\n";
        printHTMLFooter();
        return;
    }
    
    std::cout << "<h2>🎯 Muscle Group Details</h2>\n";
    
    std::cout << "<table style=\"max-width: 600px;\">\n";
    std::cout << "<tr><th>Field</th><th>Value</th></tr>\n";
    std::cout << "<tr><td><strong>ID</strong></td><td>" << mg->getMuscleGroupId() << "</td></tr>\n";
    std::cout << "<tr><td><strong>Name</strong></td><td>" << htmlEscape(mg->getName()) << "</td></tr>\n";
    std::cout << "<tr><td><strong>Description</strong></td><td>" << htmlEscape(mg->getDescription()) << "</td></tr>\n";
    std::cout << "<tr><td><strong>Days per Week</strong></td><td>" << mg->getDaysPerWeek() << "</td></tr>\n";
    std::cout << "<tr><td><strong>Sets</strong></td><td>" << mg->getSets() << "</td></tr>\n";
    std::cout << "<tr><td><strong>Reps</strong></td><td>" << mg->getReps() << "</td></tr>\n";
    std::cout << "<tr><td><strong>Weight</strong></td><td>" << mg->getWeightAmount() << " lbs</td></tr>\n";
    std::cout << "</table>\n";
    
    std::cout << "<div style=\"margin-top: 20px;\">\n";
    std::cout << "<a href=\"workout.cgi?action=update_form&table=musclegroup&id=" << id << "\" class=\"btn\">✏️ Edit</a>\n";
    std::cout << "<a href=\"workout.cgi?action=delete&table=musclegroup&id=" << id << "\" class=\"btn btn-danger\" onclick=\"return confirm('Delete?')\">🗑️ Delete</a>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=musclegroup\" class=\"btn btn-warning\">📋 Back to List</a>\n";
    std::cout << "</div>\n";
    
    delete mg;
    printHTMLFooter();
}

void showMuscleGroupInsertForm() {
    printHTMLHeader("Add New Muscle Group");
    
    std::cout << "<h2>➕ Add New Muscle Group</h2>\n";
    
    std::cout << "<form method=\"POST\" action=\"workout.cgi\">\n";
    std::cout << "<input type=\"hidden\" name=\"action\" value=\"insert\">\n";
    std::cout << "<input type=\"hidden\" name=\"table\" value=\"musclegroup\">\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Name: *</label>\n";
    std::cout << "    <input type=\"text\" name=\"name\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Days per Week: *</label>\n";
    std::cout << "    <input type=\"number\" name=\"days_per_week\" min=\"0\" max=\"7\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Description:</label>\n";
    std::cout << "  <textarea name=\"description\" rows=\"3\"></textarea>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Sets: *</label>\n";
    std::cout << "    <input type=\"number\" name=\"sets\" min=\"1\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Reps: *</label>\n";
    std::cout << "    <input type=\"number\" name=\"reps\" min=\"1\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Weight (lbs): *</label>\n";
    std::cout << "  <input type=\"number\" name=\"weight\" min=\"0\" step=\"0.1\" required>\n";
    std::cout << "</div>\n";
    
    std::cout << "<button type=\"submit\" class=\"btn btn-success\">✔️ Create Muscle Group</button>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=musclegroup\" class=\"btn btn-warning\">❌ Cancel</a>\n";
    std::cout << "</form>\n";
    
    printHTMLFooter();
}

void handleMuscleGroupInsert(WorkoutService& service, const std::map<std::string, std::string>& params) {
    ServiceResponse response = service.insertMuscleGroup(
        params.at("name"),
        params.count("description") ? params.at("description") : "",
        std::stoi(params.at("days_per_week")),
        std::stoi(params.at("sets")),
        std::stoi(params.at("reps")),
        std::stod(params.at("weight"))
    );
    
    printHTMLHeader("Muscle Group Created");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">\n";
        std::cout << "  <strong>✔️ Success!</strong> " << htmlEscape(response.message) << "\n";
        std::cout << "  <br>New Muscle Group ID: <strong>" << response.id << "</strong>\n";
        std::cout << "</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">\n";
        std::cout << "  <strong>❌ Error!</strong> " << htmlEscape(response.message) << "\n";
        std::cout << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=view&table=musclegroup&id=" << response.id << "\" class=\"btn\">👁️ View</a>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=musclegroup\" class=\"btn btn-warning\">📋 Back to List</a>\n";
    std::cout << "<a href=\"workout.cgi?action=insert_form&table=musclegroup\" class=\"btn btn-success\">➕ Add Another</a>\n";
    
    printHTMLFooter();
}

void showMuscleGroupUpdateForm(WorkoutService& service, int id) {
    printHTMLHeader("Edit Muscle Group");
    
    MuscleGroup* mg = service.getMuscleGroup(id);
    
    if (!mg) {
        std::cout << "<div class=\"alert alert-danger\">Muscle Group not found!</div>\n";
        printHTMLFooter();
        return;
    }
    
    std::cout << "<h2>✏️ Edit Muscle Group #" << id << "</h2>\n";
    
    std::cout << "<form method=\"POST\" action=\"workout.cgi\">\n";
    std::cout << "<input type=\"hidden\" name=\"action\" value=\"update\">\n";
    std::cout << "<input type=\"hidden\" name=\"table\" value=\"musclegroup\">\n";
    std::cout << "<input type=\"hidden\" name=\"id\" value=\"" << id << "\">\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Name: *</label>\n";
    std::cout << "    <input type=\"text\" name=\"name\" value=\"" << htmlEscape(mg->getName()) << "\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Days per Week: *</label>\n";
    std::cout << "    <input type=\"number\" name=\"days_per_week\" value=\"" << mg->getDaysPerWeek() << "\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Description:</label>\n";
    std::cout << "  <textarea name=\"description\" rows=\"3\">" << htmlEscape(mg->getDescription()) << "</textarea>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Sets: *</label>\n";
    std::cout << "    <input type=\"number\" name=\"sets\" value=\"" << mg->getSets() << "\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Reps: *</label>\n";
    std::cout << "    <input type=\"number\" name=\"reps\" value=\"" << mg->getReps() << "\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Weight (lbs): *</label>\n";
    std::cout << "  <input type=\"number\" name=\"weight\" value=\"" << mg->getWeightAmount() << "\" step=\"0.1\" required>\n";
    std::cout << "</div>\n";
    
    std::cout << "<button type=\"submit\" class=\"btn btn-success\">💾 Save Changes</button>\n";
    std::cout << "<a href=\"workout.cgi?action=view&table=musclegroup&id=" << id << "\" class=\"btn btn-warning\">❌ Cancel</a>\n";
    std::cout << "</form>\n";
    
    delete mg;
    printHTMLFooter();
}

void handleMuscleGroupUpdate(WorkoutService& service, const std::map<std::string, std::string>& params) {
    int id = std::stoi(params.at("id"));
    
    ServiceResponse response = service.updateMuscleGroup(
        id,
        params.at("name"),
        params.count("description") ? params.at("description") : "",
        std::stoi(params.at("days_per_week")),
        std::stoi(params.at("sets")),
        std::stoi(params.at("reps")),
        std::stod(params.at("weight"))
    );
    
    printHTMLHeader("Muscle Group Updated");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✔️ " << htmlEscape(response.message) << "</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">❌ " << htmlEscape(response.message) << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=view&table=musclegroup&id=" << id << "\" class=\"btn\">👁️ View</a>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=musclegroup\" class=\"btn btn-warning\">📋 Back to List</a>\n";
    
    printHTMLFooter();
}

void handleMuscleGroupDelete(WorkoutService& service, int id) {
    ServiceResponse response = service.deleteMuscleGroup(id);
    
    printHTMLHeader("Muscle Group Deleted");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✔️ Muscle Group #" << id << " deleted.</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">❌ " << htmlEscape(response.message) << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=musclegroup\" class=\"btn\">📋 Back to List</a>\n";
    
    printHTMLFooter();
}

// ==================== NUTRITION HANDLERS ====================

void listNutrition(WorkoutService& service, const std::map<std::string, std::string>& params) {
    printHTMLHeader("Nutrition - List All");
    
    std::cout << "<div class=\"action-bar\">\n";
    std::cout << "  <h2>🥗 Nutrition Entries</h2>\n";
    std::cout << "  <a href=\"workout.cgi?action=insert_form&table=nutrition\" class=\"btn btn-success\">➕ Add New Entry</a>\n";
    std::cout << "</div>\n";
    
    std::vector<Nutrition*> entries = service.getAllNutrition();
    
    std::cout << "<p>Total entries: <strong>" << entries.size() << "</strong></p>\n";
    
    if (entries.empty()) {
        std::cout << "<p>No nutrition entries found.</p>\n";
    } else {
        std::cout << "<table>\n";
        std::cout << "<tr>\n";
        std::cout << "  <th>ID</th><th>Date</th><th>Family</th><th>Water (ml)</th>\n";
        std::cout << "  <th>Carbs (g)</th><th>Fat (g)</th><th>Protein (g)</th><th>Sugar (g)</th><th>Actions</th>\n";
        std::cout << "</tr>\n";
        
        for (Nutrition* n : entries) {
            std::cout << "<tr>\n";
            std::cout << "  <td>" << n->getNutritionId() << "</td>\n";
            std::cout << "  <td>" << htmlEscape(n->getMealDate()) << "</td>\n";
            std::cout << "  <td>" << htmlEscape(n->getFamilyString()) << "</td>\n";
            std::cout << "  <td>" << n->getWater() << "</td>\n";
            std::cout << "  <td>" << n->getCarbs() << "</td>\n";
            std::cout << "  <td>" << n->getFat() << "</td>\n";
            std::cout << "  <td>" << n->getProtein() << "</td>\n";
            std::cout << "  <td>" << n->getSugar() << "</td>\n";
            std::cout << "  <td>\n";
            std::cout << "    <a href=\"workout.cgi?action=view&table=nutrition&id=" << n->getNutritionId() << "\" class=\"btn btn-info btn-sm\">👁️ View</a>\n";
            std::cout << "    <a href=\"workout.cgi?action=update_form&table=nutrition&id=" << n->getNutritionId() << "\" class=\"btn btn-sm\">✏️ Edit</a>\n";
            std::cout << "    <a href=\"workout.cgi?action=delete&table=nutrition&id=" << n->getNutritionId() << "\" class=\"btn btn-danger btn-sm\" onclick=\"return confirm('Delete?')\">🗑️ Delete</a>\n";
            std::cout << "  </td>\n";
            std::cout << "</tr>\n";
            delete n;
        }
        
        std::cout << "</table>\n";
    }
    
    printHTMLFooter();
}

void showNutritionInsertForm() {
    printHTMLHeader("Add New Nutrition Entry");
    
    std::cout << "<h2>➕ Add New Nutrition Entry</h2>\n";
    
    std::cout << "<form method=\"POST\" action=\"workout.cgi\">\n";
    std::cout << "<input type=\"hidden\" name=\"action\" value=\"insert\">\n";
    std::cout << "<input type=\"hidden\" name=\"table\" value=\"nutrition\">\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Food Family: *</label>\n";
    std::cout << "    <select name=\"family\" required>\n";
    std::cout << "      <option value=\"Mixed\">Mixed</option>\n";
    std::cout << "      <option value=\"Fruit\">Fruit</option>\n";
    std::cout << "      <option value=\"Meat\">Meat</option>\n";
    std::cout << "      <option value=\"Vegetable\">Vegetable</option>\n";
    std::cout << "      <option value=\"Dairy\">Dairy</option>\n";
    std::cout << "    </select>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Date: *</label>\n";
    std::cout << "    <input type=\"date\" name=\"date\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Water (ml): *</label>\n";
    std::cout << "    <input type=\"number\" name=\"water\" min=\"0\" step=\"0.1\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Carbs (g): *</label>\n";
    std::cout << "    <input type=\"number\" name=\"carbs\" min=\"0\" step=\"0.1\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Fat (g): *</label>\n";
    std::cout << "    <input type=\"number\" name=\"fat\" min=\"0\" step=\"0.1\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Protein (g): *</label>\n";
    std::cout << "    <input type=\"number\" name=\"protein\" min=\"0\" step=\"0.1\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Sugar (g): *</label>\n";
    std::cout << "  <input type=\"number\" name=\"sugar\" min=\"0\" step=\"0.1\" required>\n";
    std::cout << "</div>\n";
    
    std::cout << "<button type=\"submit\" class=\"btn btn-success\">✔️ Create Entry</button>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=nutrition\" class=\"btn btn-warning\">❌ Cancel</a>\n";
    std::cout << "</form>\n";
    
    printHTMLFooter();
}

void handleNutritionInsert(WorkoutService& service, const std::map<std::string, std::string>& params) {
    ServiceResponse response = service.insertNutrition(
        params.at("family"),
        std::stod(params.at("water")),
        std::stod(params.at("carbs")),
        std::stod(params.at("fat")),
        std::stod(params.at("protein")),
        std::stod(params.at("sugar")),
        params.at("date")
    );
    
    printHTMLHeader("Nutrition Entry Created");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✔️ " << htmlEscape(response.message) << " (ID: " << response.id << ")</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">❌ " << htmlEscape(response.message) << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=nutrition\" class=\"btn btn-warning\">📋 Back to List</a>\n";
    std::cout << "<a href=\"workout.cgi?action=insert_form&table=nutrition\" class=\"btn btn-success\">➕ Add Another</a>\n";
    
    printHTMLFooter();
}

void handleNutritionDelete(WorkoutService& service, int id) {
    ServiceResponse response = service.deleteNutrition(id);
    
    printHTMLHeader("Nutrition Entry Deleted");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✔️ Entry deleted.</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">❌ " << htmlEscape(response.message) << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=nutrition\" class=\"btn\">📋 Back to List</a>\n";
    
    printHTMLFooter();
}

// ==================== RECOVERY HANDLERS ====================

void listRecovery(WorkoutService& service, const std::map<std::string, std::string>& params) {
    printHTMLHeader("Recovery - List All");
    
    std::cout << "<div class=\"action-bar\">\n";
    std::cout << "  <h2>😌 Recovery Sessions</h2>\n";
    std::cout << "  <a href=\"workout.cgi?action=insert_form&table=recovery\" class=\"btn btn-success\">➕ Add New Session</a>\n";
    std::cout << "</div>\n";
    
    std::vector<Recovery*> sessions = service.getAllRecovery();
    
    std::cout << "<p>Total sessions: <strong>" << sessions.size() << "</strong></p>\n";
    
    if (sessions.empty()) {
        std::cout << "<p>No recovery sessions found.</p>\n";
    } else {
        std::cout << "<table>\n";
        std::cout << "<tr><th>ID</th><th>Date</th><th>Duration (min)</th><th>Type</th><th>Helpers</th><th>Actions</th></tr>\n";
        
        for (Recovery* r : sessions) {
            std::cout << "<tr>\n";
            std::cout << "  <td>" << r->getRecoveryId() << "</td>\n";
            std::cout << "  <td>" << htmlEscape(r->getRecoveryDate()) << "</td>\n";
            std::cout << "  <td>" << r->getDuration() << "</td>\n";
            std::cout << "  <td>" << htmlEscape(r->getType()) << "</td>\n";
            std::cout << "  <td>" << htmlEscape(r->getHelpers().substr(0, 30)) << "</td>\n";
            std::cout << "  <td>\n";
            std::cout << "    <a href=\"workout.cgi?action=view&table=recovery&id=" << r->getRecoveryId() << "\" class=\"btn btn-info btn-sm\">👁️ View</a>\n";
            std::cout << "    <a href=\"workout.cgi?action=delete&table=recovery&id=" << r->getRecoveryId() << "\" class=\"btn btn-danger btn-sm\" onclick=\"return confirm('Delete?')\">🗑️ Delete</a>\n";
            std::cout << "  </td>\n";
            std::cout << "</tr>\n";
            delete r;
        }
        
        std::cout << "</table>\n";
    }
    
    printHTMLFooter();
}

void showRecoveryInsertForm() {
    printHTMLHeader("Add New Recovery Session");
    
    std::cout << "<h2>➕ Add New Recovery Session</h2>\n";
    
    std::cout << "<form method=\"POST\" action=\"workout.cgi\">\n";
    std::cout << "<input type=\"hidden\" name=\"action\" value=\"insert\">\n";
    std::cout << "<input type=\"hidden\" name=\"table\" value=\"recovery\">\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Date: *</label>\n";
    std::cout << "    <input type=\"date\" name=\"date\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Duration (minutes): *</label>\n";
    std::cout << "    <input type=\"number\" name=\"duration\" min=\"1\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Type: *</label>\n";
    std::cout << "  <input type=\"text\" name=\"type\" placeholder=\"e.g., Yoga, Massage, Stretching\" required>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Helpers/Notes:</label>\n";
    std::cout << "  <textarea name=\"helpers\" rows=\"3\" placeholder=\"e.g., Foam roller, stretching band\"></textarea>\n";
    std::cout << "</div>\n";
    
    std::cout << "<button type=\"submit\" class=\"btn btn-success\">✔️ Create Session</button>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=recovery\" class=\"btn btn-warning\">❌ Cancel</a>\n";
    std::cout << "</form>\n";
    
    printHTMLFooter();
}

void handleRecoveryInsert(WorkoutService& service, const std::map<std::string, std::string>& params) {
    ServiceResponse response = service.insertRecovery(
        params.at("date"),
        std::stoi(params.at("duration")),
        params.at("type"),
        params.count("helpers") ? params.at("helpers") : ""
    );
    
    printHTMLHeader("Recovery Session Created");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✔️ " << htmlEscape(response.message) << " (ID: " << response.id << ")</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">❌ " << htmlEscape(response.message) << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=recovery\" class=\"btn btn-warning\">📋 Back to List</a>\n";
    std::cout << "<a href=\"workout.cgi?action=insert_form&table=recovery\" class=\"btn btn-success\">➕ Add Another</a>\n";
    
    printHTMLFooter();
}

void handleRecoveryDelete(WorkoutService& service, int id) {
    ServiceResponse response = service.deleteRecovery(id);
    
    printHTMLHeader("Recovery Session Deleted");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✔️ Session deleted.</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">❌ " << htmlEscape(response.message) << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=recovery\" class=\"btn\">📋 Back to List</a>\n";
    
    printHTMLFooter();
}

// ==================== EQUIPMENT HANDLERS ====================

void listEquipment(WorkoutService& service, const std::map<std::string, std::string>& params) {
    printHTMLHeader("Equipment - List All");
    
    std::cout << "<div class=\"action-bar\">\n";
    std::cout << "  <h2>🏋️ Equipment</h2>\n";
    std::cout << "  <a href=\"workout.cgi?action=insert_form&table=equipment\" class=\"btn btn-success\">➕ Add New Equipment</a>\n";
    std::cout << "</div>\n";
    
    std::vector<Equipment*> items = service.getAllEquipment();
    
    std::cout << "<p>Total equipment: <strong>" << items.size() << "</strong></p>\n";
    
    if (items.empty()) {
        std::cout << "<p>No equipment found.</p>\n";
    } else {
        std::cout << "<table>\n";
        std::cout << "<tr><th>ID</th><th>Name</th><th>Description</th><th>Category</th><th>Target</th><th>Actions</th></tr>\n";
        
        for (Equipment* e : items) {
            std::cout << "<tr>\n";
            std::cout << "  <td>" << e->getEquipmentId() << "</td>\n";
            std::cout << "  <td>" << htmlEscape(e->getName()) << "</td>\n";
            std::cout << "  <td>" << htmlEscape(e->getDescription().substr(0, 50)) << "</td>\n";
            std::cout << "  <td>" << htmlEscape(e->getCategory()) << "</td>\n";
            std::cout << "  <td>" << htmlEscape(e->getTarget()) << "</td>\n";
            std::cout << "  <td>\n";
            std::cout << "    <a href=\"workout.cgi?action=view&table=equipment&id=" << e->getEquipmentId() << "\" class=\"btn btn-info btn-sm\">👁️ View</a>\n";
            std::cout << "    <a href=\"workout.cgi?action=delete&table=equipment&id=" << e->getEquipmentId() << "\" class=\"btn btn-danger btn-sm\" onclick=\"return confirm('Delete?')\">🗑️ Delete</a>\n";
            std::cout << "  </td>\n";
            std::cout << "</tr>\n";
            delete e;
        }
        
        std::cout << "</table>\n";
    }
    
    printHTMLFooter();
}

void showEquipmentInsertForm() {
    printHTMLHeader("Add New Equipment");
    
    std::cout << "<h2>➕ Add New Equipment</h2>\n";
    
    std::cout << "<form method=\"POST\" action=\"workout.cgi\">\n";
    std::cout << "<input type=\"hidden\" name=\"action\" value=\"insert\">\n";
    std::cout << "<input type=\"hidden\" name=\"table\" value=\"equipment\">\n";
    
    std::cout << "<div class=\"grid-2\">\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Name: *</label>\n";
    std::cout << "    <input type=\"text\" name=\"name\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "  <div class=\"form-group\">\n";
    std::cout << "    <label>Category: *</label>\n";
    std::cout << "    <input type=\"text\" name=\"category\" placeholder=\"e.g., Cardio, Strength\" required>\n";
    std::cout << "  </div>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Description:</label>\n";
    std::cout << "  <textarea name=\"description\" rows=\"3\"></textarea>\n";
    std::cout << "</div>\n";
    
    std::cout << "<div class=\"form-group\">\n";
    std::cout << "  <label>Target Area:</label>\n";
    std::cout << "  <input type=\"text\" name=\"target\" placeholder=\"e.g., Upper Body, Legs\">\n";
    std::cout << "</div>\n";
    
    std::cout << "<button type=\"submit\" class=\"btn btn-success\">✔️ Create Equipment</button>\n";
    std::cout << "<a href=\"workout.cgi?action=list&table=equipment\" class=\"btn btn-warning\">❌ Cancel</a>\n";
    std::cout << "</form>\n";
    
    printHTMLFooter();
}

void handleEquipmentInsert(WorkoutService& service, const std::map<std::string, std::string>& params) {
    ServiceResponse response = service.insertEquipment(
        params.at("name"),
        params.count("description") ? params.at("description") : "",
        params.at("category"),
        params.count("target") ? params.at("target") : ""
    );
    
    printHTMLHeader("Equipment Created");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✔️ " << htmlEscape(response.message) << " (ID: " << response.id << ")</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">❌ " << htmlEscape(response.message) << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=equipment\" class=\"btn btn-warning\">📋 Back to List</a>\n";
    std::cout << "<a href=\"workout.cgi?action=insert_form&table=equipment\" class=\"btn btn-success\">➕ Add Another</a>\n";
    
    printHTMLFooter();
}

void handleEquipmentDelete(WorkoutService& service, int id) {
    ServiceResponse response = service.deleteEquipment(id);
    
    printHTMLHeader("Equipment Deleted");
    
    if (response.success) {
        std::cout << "<div class=\"alert alert-success\">✔️ Equipment deleted.</div>\n";
    } else {
        std::cout << "<div class=\"alert alert-danger\">❌ " << htmlEscape(response.message) << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=equipment\" class=\"btn\">📋 Back to List</a>\n";
    
    printHTMLFooter();
}


// ==================== MAIN CGI HANDLER ====================

int main() {
    std::cout << "Content-Type: text/html\r\n\r\n";
    std::cout.flush();
    try {
        // Get request method and query string
        std::string requestMethod = getEnv("REQUEST_METHOD");
        std::string queryString = getEnv("QUERY_STRING");
        
        // Parse parameters
        std::map<std::string, std::string> params;
        
        if (requestMethod == "GET") {
            params = parseQueryString(queryString);
        } else if (requestMethod == "POST") {
            std::string contentLength = getEnv("CONTENT_LENGTH");
            if (!contentLength.empty()) {
                int length = std::stoi(contentLength);
                std::string postData;
                postData.resize(length);
                std::cin.read(&postData[0], length);
                params = parseQueryString(postData);
            }
        }
        
        // Initialize service layer
        WorkoutDAO dao("localhost", "workout_user", "workout_pass", "workout_tracker");
        WorkoutManager manager(&dao);
        WorkoutService service(&manager);
        
        // Test connection
        if (!service.testConnection()) {
            printHTMLHeader("Database Error");
            std::cout << "<h1>Database Connection Error</h1>\n";
            std::cout << "<p>Could not connect to the database.</p>\n";
            printHTMLFooter();
            return 1;
        }
        
        // Route based on action and table parameters
        std::string action = params["action"];
        std::string table = params["table"];
        
        // ==================== HOME PAGE ====================
        if (action.empty()) {
            showHomePage(service);
        }
        
        // ==================== WORKOUT ROUTING ====================
        else if (table == "workout") {
            if (action == "list") {
                showWorkoutList(service, params);
            } else if (action == "view") {
                viewWorkout(service, std::stoi(params["id"]));
            } else if (action == "insert_form") {
                showWorkoutInsertForm();
            } else if (action == "insert") {
                handleWorkoutInsert(service, params);
            } else if (action == "update_form") {
                showWorkoutUpdateForm(service, std::stoi(params["id"]));
            } else if (action == "update") {
                handleWorkoutUpdate(service, params);
            } else if (action == "delete") {
                handleWorkoutDelete(service, std::stoi(params["id"]));
            } else {
                printHTMLHeader("Unknown Action");
                std::cout << "<h1>Unknown Action</h1>\n";
                std::cout << "<p>Action '" << htmlEscape(action) << "' not recognized for Workout.</p>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=workout\" class=\"btn\">Back to Workouts</a>\n";
                printHTMLFooter();
            }
        }
        
        // ==================== MUSCLEGROUP ROUTING ====================
        else if (table == "musclegroup") {
            if (action == "list") {
                listMuscleGroups(service, params);
            } else if (action == "view") {
                viewMuscleGroup(service, std::stoi(params["id"]));
            } else if (action == "insert_form") {
                showMuscleGroupInsertForm();
            } else if (action == "insert") {
                handleMuscleGroupInsert(service, params);
            } else if (action == "update_form") {
                showMuscleGroupUpdateForm(service, std::stoi(params["id"]));
            } else if (action == "update") {
                handleMuscleGroupUpdate(service, params);
            } else if (action == "delete") {
                handleMuscleGroupDelete(service, std::stoi(params["id"]));
            } else {
                printHTMLHeader("Unknown Action");
                std::cout << "<h1>Unknown Action</h1>\n";
                std::cout << "<p>Action '" << htmlEscape(action) << "' not recognized for MuscleGroup.</p>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=musclegroup\" class=\"btn\">Back to Muscle Groups</a>\n";
                printHTMLFooter();
            }
        }
        
        // ==================== NUTRITION ROUTING ====================
        else if (table == "nutrition") {
            if (action == "list") {
                listNutrition(service, params);
            } else if (action == "view") {
                // View not implemented yet, redirect to list
                printHTMLHeader("View Nutrition");
                std::cout << "<h1>View not yet implemented for Nutrition</h1>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=nutrition\" class=\"btn\">Back to List</a>\n";
                printHTMLFooter();
            } else if (action == "insert_form") {
                showNutritionInsertForm();
            } else if (action == "insert") {
                handleNutritionInsert(service, params);
            } else if (action == "update_form") {
                printHTMLHeader("Update Nutrition");
                std::cout << "<h1>Update not yet implemented for Nutrition</h1>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=nutrition\" class=\"btn\">Back to List</a>\n";
                printHTMLFooter();
            } else if (action == "delete") {
                handleNutritionDelete(service, std::stoi(params["id"]));
            } else {
                printHTMLHeader("Unknown Action");
                std::cout << "<h1>Unknown Action</h1>\n";
                std::cout << "<p>Action '" << htmlEscape(action) << "' not recognized for Nutrition.</p>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=nutrition\" class=\"btn\">Back to Nutrition</a>\n";
                printHTMLFooter();
            }
        }
        
        // ==================== RECOVERY ROUTING ====================
        else if (table == "recovery") {
            if (action == "list") {
                listRecovery(service, params);
            } else if (action == "view") {
                printHTMLHeader("View Recovery");
                std::cout << "<h1>View not yet implemented for Recovery</h1>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=recovery\" class=\"btn\">Back to List</a>\n";
                printHTMLFooter();
            } else if (action == "insert_form") {
                showRecoveryInsertForm();
            } else if (action == "insert") {
                handleRecoveryInsert(service, params);
            } else if (action == "update_form") {
                printHTMLHeader("Update Recovery");
                std::cout << "<h1>Update not yet implemented for Recovery</h1>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=recovery\" class=\"btn\">Back to List</a>\n";
                printHTMLFooter();
            } else if (action == "delete") {
                handleRecoveryDelete(service, std::stoi(params["id"]));
            } else {
                printHTMLHeader("Unknown Action");
                std::cout << "<h1>Unknown Action</h1>\n";
                std::cout << "<p>Action '" << htmlEscape(action) << "' not recognized for Recovery.</p>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=recovery\" class=\"btn\">Back to Recovery</a>\n";
                printHTMLFooter();
            }
        }
        
        // ==================== EQUIPMENT ROUTING ====================
        else if (table == "equipment") {
            if (action == "list") {
                listEquipment(service, params);
            } else if (action == "view") {
                printHTMLHeader("View Equipment");
                std::cout << "<h1>View not yet implemented for Equipment</h1>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=equipment\" class=\"btn\">Back to List</a>\n";
                printHTMLFooter();
            } else if (action == "insert_form") {
                showEquipmentInsertForm();
            } else if (action == "insert") {
                handleEquipmentInsert(service, params);
            } else if (action == "update_form") {
                printHTMLHeader("Update Equipment");
                std::cout << "<h1>Update not yet implemented for Equipment</h1>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=equipment\" class=\"btn\">Back to List</a>\n";
                printHTMLFooter();
            } else if (action == "delete") {
                handleEquipmentDelete(service, std::stoi(params["id"]));
            } else {
                printHTMLHeader("Unknown Action");
                std::cout << "<h1>Unknown Action</h1>\n";
                std::cout << "<p>Action '" << htmlEscape(action) << "' not recognized for Equipment.</p>\n";
                std::cout << "<a href=\"workout.cgi?action=list&table=equipment\" class=\"btn\">Back to Equipment</a>\n";
                printHTMLFooter();
            }
        }
        
        // ==================== UNKNOWN TABLE ====================
        else {
            printHTMLHeader("Error");
            std::cout << "<h1>Unknown Table</h1>\n";
            std::cout << "<p>Table '" << htmlEscape(table) << "' is not recognized.</p>\n";
            std::cout << "<a href=\"workout.cgi\" class=\"btn\">Go Home</a>\n";
            printHTMLFooter();
        }
        
    } catch (const std::exception& e) {
        printHTMLHeader("Error");
        std::cout << "<h1>Error</h1>\n";
        std::cout << "<p>Exception: " << htmlEscape(e.what()) << "</p>\n";
        std::cout << "<a href=\"workout.cgi\" class=\"btn\">Go Home</a>\n";
        printHTMLFooter();
    }
    
    return 0;
}

// ==================== WORKOUT LIST HANDLER ====================

void showWorkoutList(WorkoutService& service, const std::map<std::string, std::string>& params) {
    printHTMLHeader("Workouts List");
    
    std::cout << "<h1>🏋️ Workouts</h1>\n";
    printNavigation();
    
    std::cout << "<a href=\"workout.cgi?action=insert_form&table=workout\" class=\"btn\">➕ Add New Workout</a>\n";
    
    // Get all workouts
    std::vector<Workout*> workouts = service.getAllWorkouts();
    
    std::cout << "<h2>All Workouts (" << workouts.size() << ")</h2>\n";
    
    if (workouts.empty()) {
        std::cout << "<p>No workouts found.</p>\n";
    } else {
        std::cout << "<table>\n";
        std::cout << "<tr>\n";
        std::cout << "  <th>ID</th><th>Date</th><th>Time</th><th>Duration</th>\n";
        std::cout << "  <th>Type</th><th>Calories</th><th>RPE</th><th>Actions</th>\n";
        std::cout << "</tr>\n";
        
        for (Workout* w : workouts) {
            std::cout << "<tr>\n";
            std::cout << "  <td>" << w->getWorkoutId() << "</td>\n";
            std::cout << "  <td>" << htmlEscape(w->getWorkoutDate()) << "</td>\n";
            std::cout << "  <td>" << htmlEscape(w->getWorkoutTime()) << "</td>\n";
            std::cout << "  <td>" << w->getDuration() << " min</td>\n";
            std::cout << "  <td>" << htmlEscape(w->getTypeDescription()) << "</td>\n";
            std::cout << "  <td>" << w->getCaloriesBurned() << "</td>\n";
            std::cout << "  <td>" << w->getRatePerceivedExhaustion() << "/10</td>\n";
            std::cout << "  <td>\n";
            std::cout << "    <a href=\"workout.cgi?action=update_form&table=workout&id=" 
                      << w->getWorkoutId() << "\" class=\"btn btn-edit\">Edit</a>\n";
            std::cout << "    <a href=\"workout.cgi?action=delete&table=workout&id=" 
                      << w->getWorkoutId() << "\" class=\"btn btn-delete\" "
                      << "onclick=\"return confirm('Are you sure?')\">Delete</a>\n";
            std::cout << "  </td>\n";
            std::cout << "</tr>\n";
            
            delete w;
        }
        
        std::cout << "</table>\n";
    }
    
    printHTMLFooter();
}



// ==================== WORKOUT INSERT FORM ====================

void showWorkoutInsertForm() {
    printHTMLHeader("Add New Workout");
    
    std::cout << "<h1>➕ Add New Workout</h1>\n";
    printNavigation();
    
    std::cout << R"(
<form method="POST" action="workout.cgi">
    <input type="hidden" name="action" value="insert">
    <input type="hidden" name="table" value="workout">
    
    <div class="form-group">
        <label>Date:</label>
        <input type="date" name="date" required>
    </div>
    
    <div class="form-group">
        <label>Time:</label>
        <input type="time" name="time" required>
    </div>
    
    <div class="form-group">
        <label>Duration (minutes):</label>
        <input type="number" name="duration" min="1" required>
    </div>
    
    <div class="form-group">
        <label>Type/Description:</label>
        <textarea name="type" rows="3" required></textarea>
    </div>
    
    <div class="form-group">
        <label>Calories Burned:</label>
        <input type="number" name="calories" min="0" step="0.1" required>
    </div>
    
    <div class="form-group">
        <label>RPE (1-10):</label>
        <input type="number" name="rpe" min="1" max="10" required>
    </div>
    
    <div class="form-group">
        <label>Muscle Group ID (0 for none):</label>
        <input type="number" name="muscle_group_id" min="0" value="0" required>
    </div>
    
    <button type="submit">Create Workout</button>
    <a href="workout.cgi?action=list&table=workout" class="btn" style="background: #6c757d;">Cancel</a>
</form>
)";
    
    printHTMLFooter();
}

// ==================== WORKOUT INSERT HANDLER ====================

void handleWorkoutInsert(WorkoutService& service, const std::map<std::string, std::string>& params) {
    ServiceResponse response = service.insertWorkout(
        params.at("date"),
        params.at("time") + ":00",  // Add seconds
        std::stoi(params.at("duration")),
        params.at("type"),
        std::stod(params.at("calories")),
        std::stoi(params.at("rpe")),
        std::stoi(params.at("muscle_group_id"))
    );
    
    printHTMLHeader("Workout Created");
    std::cout << "<h1>Workout Created</h1>\n";
    printNavigation();
    
    if (response.success) {
        std::cout << "<div class=\"success\">\n";
        std::cout << "  <strong>Success!</strong> " << htmlEscape(response.message) << "\n";
        std::cout << "  <br>New Workout ID: " << response.id << "\n";
        std::cout << "</div>\n";
    } else {
        std::cout << "<div class=\"error\">\n";
        std::cout << "  <strong>Error!</strong> " << htmlEscape(response.message) << "\n";
        std::cout << "</div>\n";
    }
    
    std::cout << "<a href=\"workout.cgi?action=list&table=workout\" class=\"btn\">Back to Workouts</a>\n";
    printHTMLFooter();
}

// (Continue with UPDATE and DELETE handlers...)
// Due to length, these are shown in separate files
