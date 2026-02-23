// JsonHelper.h
// JSON utility functions for REST API responses
// Author: Claude
// Date: 2026-01-28

#ifndef JSONHELPER_H
#define JSONHELPER_H

#include "../Workout.h"
#include "../MuscleGroup.h"
#include "../Nutrition.h"
#include "../Recovery.h"
#include "../Equipment.h"
#include <string>
#include <sstream>
#include <vector>

class JsonHelper {
public:
    // Escape special characters for JSON
    static std::string escapeJson(const std::string& str) {
        std::ostringstream oss;
        for (char c : str) {
            switch (c) {
                case '"':  oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;
                default:   oss << c; break;
            }
        }
        return oss.str();
    }
    
    // Convert Workout to JSON
    static std::string workoutToJson(const Workout& workout) {
        std::ostringstream json;
        json << "{";
        json << "\"workout_id\":" << workout.getWorkoutId() << ",";
        json << "\"workout_date\":\"" << escapeJson(workout.getWorkoutDate()) << "\",";
        json << "\"workout_time\":\"" << escapeJson(workout.getWorkoutTime()) << "\",";
        json << "\"duration\":" << workout.getDuration() << ",";
        json << "\"type_description\":\"" << escapeJson(workout.getTypeDescription()) << "\",";
        json << "\"calories_burned\":" << workout.getCaloriesBurned() << ",";
        json << "\"rate_perceived_exhaustion\":" << workout.getRatePerceivedExhaustion() << ",";
        json << "\"muscle_group_id\":" << workout.getMuscleGroupId();
        json << "}";
        return json.str();
    }
    
    // Convert MuscleGroup to JSON
    static std::string muscleGroupToJson(const MuscleGroup& mg) {
        std::ostringstream json;
        json << "{";
        json << "\"muscle_group_id\":" << mg.getMuscleGroupId() << ",";
        json << "\"name\":\"" << escapeJson(mg.getName()) << "\",";
        json << "\"description\":\"" << escapeJson(mg.getDescription()) << "\",";
        json << "\"days_per_week\":" << mg.getDaysPerWeek() << ",";
        json << "\"sets\":" << mg.getSets() << ",";
        json << "\"reps\":" << mg.getReps() << ",";
        json << "\"weight_amount\":" << mg.getWeightAmount();
        json << "}";
        return json.str();
    }
    
    // Convert Nutrition to JSON
    static std::string nutritionToJson(const Nutrition& nutrition) {
        std::ostringstream json;
        json << "{";
        json << "\"nutrition_id\":" << nutrition.getNutritionId() << ",";
        json << "\"family\":\"" << escapeJson(nutrition.getFamilyString()) << "\",";
        json << "\"water\":" << nutrition.getWater() << ",";
        json << "\"carbs\":" << nutrition.getCarbs() << ",";
        json << "\"fat\":" << nutrition.getFat() << ",";
        json << "\"protein\":" << nutrition.getProtein() << ",";
        json << "\"sugar\":" << nutrition.getSugar() << ",";
        json << "\"meal_date\":\"" << escapeJson(nutrition.getMealDate()) << "\",";
        json << "\"total_calories\":" << nutrition.calculateTotalCalories();
        json << "}";
        return json.str();
    }
    
    // Convert Recovery to JSON
    static std::string recoveryToJson(const Recovery& recovery) {
        std::ostringstream json;
        json << "{";
        json << "\"recovery_id\":" << recovery.getRecoveryId() << ",";
        json << "\"recovery_date\":\"" << escapeJson(recovery.getRecoveryDate()) << "\",";
        json << "\"duration\":" << recovery.getDuration() << ",";
        json << "\"type\":\"" << escapeJson(recovery.getType()) << "\",";
        json << "\"helpers\":\"" << escapeJson(recovery.getHelpers()) << "\"";
        json << "}";
        return json.str();
    }
    
    // Convert Equipment to JSON
    static std::string equipmentToJson(const Equipment& equipment) {
        std::ostringstream json;
        json << "{";
        json << "\"equipment_id\":" << equipment.getEquipmentId() << ",";
        json << "\"name\":\"" << escapeJson(equipment.getName()) << "\",";
        json << "\"description\":\"" << escapeJson(equipment.getDescription()) << "\",";
        json << "\"category\":\"" << escapeJson(equipment.getCategory()) << "\",";
        json << "\"target\":\"" << escapeJson(equipment.getTarget()) << "\"";
        json << "}";
        return json.str();
    }
    
    // Convert vector of Workouts to JSON array
    static std::string workoutsToJsonArray(const std::vector<Workout*>& workouts) {
        std::ostringstream json;
        json << "[";
        for (size_t i = 0; i < workouts.size(); ++i) {
            json << workoutToJson(*workouts[i]);
            if (i < workouts.size() - 1) json << ",";
        }
        json << "]";
        return json.str();
    }
    
    // Convert vector of MuscleGroups to JSON array
    static std::string muscleGroupsToJsonArray(const std::vector<MuscleGroup*>& groups) {
        std::ostringstream json;
        json << "[";
        for (size_t i = 0; i < groups.size(); ++i) {
            json << muscleGroupToJson(*groups[i]);
            if (i < groups.size() - 1) json << ",";
        }
        json << "]";
        return json.str();
    }
    
    // Convert vector of Nutrition to JSON array
    static std::string nutritionToJsonArray(const std::vector<Nutrition*>& nutrition) {
        std::ostringstream json;
        json << "[";
        for (size_t i = 0; i < nutrition.size(); ++i) {
            json << nutritionToJson(*nutrition[i]);
            if (i < nutrition.size() - 1) json << ",";
        }
        json << "]";
        return json.str();
    }
    
    // Convert vector of Recovery to JSON array
    static std::string recoveryToJsonArray(const std::vector<Recovery*>& recovery) {
        std::ostringstream json;
        json << "[";
        for (size_t i = 0; i < recovery.size(); ++i) {
            json << recoveryToJson(*recovery[i]);
            if (i < recovery.size() - 1) json << ",";
        }
        json << "]";
        return json.str();
    }
    
    // Convert vector of Equipment to JSON array
    static std::string equipmentToJsonArray(const std::vector<Equipment*>& equipment) {
        std::ostringstream json;
        json << "[";
        for (size_t i = 0; i < equipment.size(); ++i) {
            json << equipmentToJson(*equipment[i]);
            if (i < equipment.size() - 1) json << ",";
        }
        json << "]";
        return json.str();
    }
    
    // Create error response JSON
    static std::string errorResponse(const std::string& message) {
        std::ostringstream json;
        json << "{\"error\":\"" << escapeJson(message) << "\"}";
        return json.str();
    }
    
    // Create success response JSON
    static std::string successResponse(const std::string& message, int id = -1) {
        std::ostringstream json;
        json << "{\"success\":true,\"message\":\"" << escapeJson(message) << "\"";
        if (id >= 0) {
            json << ",\"id\":" << id;
        }
        json << "}";
        return json.str();
    }
};

#endif // JSONHELPER_H
