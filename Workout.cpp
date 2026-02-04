// Workout.cpp
// Workout Tracking System - Workout Class Implementation
// Author: Claude
// Date: 2026-01-28

#include "Workout.h"
#include <iostream>
#include <sstream>

// Default Constructor
Workout::Workout()
    : workoutId(0), workoutDate(""), workoutTime(""), duration(0),
      typeDescription(""), caloriesBurned(0.0), ratePerceivedExhaustion(0),
      muscleGroupId(0), createdAt(0), updatedAt(0) {
}

// Parameterized Constructor
Workout::Workout(int id, const std::string& date, const std::string& time,
                 int dur, const std::string& type, double calories,
                 int rpe, int mgId)
    : workoutId(id), workoutDate(date), workoutTime(time), duration(dur),
      typeDescription(type), caloriesBurned(calories), 
      ratePerceivedExhaustion(rpe), muscleGroupId(mgId) {
    createdAt = std::time(nullptr);
    updatedAt = std::time(nullptr);
}

// Destructor
Workout::~Workout() {
}

// Getters
int Workout::getWorkoutId() const { return workoutId; }
std::string Workout::getWorkoutDate() const { return workoutDate; }
std::string Workout::getWorkoutTime() const { return workoutTime; }
int Workout::getDuration() const { return duration; }
std::string Workout::getTypeDescription() const { return typeDescription; }
double Workout::getCaloriesBurned() const { return caloriesBurned; }
int Workout::getRatePerceivedExhaustion() const { return ratePerceivedExhaustion; }
int Workout::getMuscleGroupId() const { return muscleGroupId; }
time_t Workout::getCreatedAt() const { return createdAt; }
time_t Workout::getUpdatedAt() const { return updatedAt; }

// Setters
void Workout::setWorkoutId(int id) { workoutId = id; }
void Workout::setWorkoutDate(const std::string& date) { workoutDate = date; }
void Workout::setWorkoutTime(const std::string& time) { workoutTime = time; }
void Workout::setDuration(int dur) { duration = dur; }
void Workout::setTypeDescription(const std::string& type) { typeDescription = type; }
void Workout::setCaloriesBurned(double calories) { caloriesBurned = calories; }
void Workout::setRatePerceivedExhaustion(int rpe) { ratePerceivedExhaustion = rpe; }
void Workout::setMuscleGroupId(int mgId) { muscleGroupId = mgId; }
void Workout::setCreatedAt(time_t created) { createdAt = created; }
void Workout::setUpdatedAt(time_t updated) { updatedAt = updated; }

// Utility methods
void Workout::displayInfo() const {
    std::cout << "=== Workout Information ===" << std::endl;
    std::cout << "ID: " << workoutId << std::endl;
    std::cout << "Date: " << workoutDate << std::endl;
    std::cout << "Time: " << workoutTime << std::endl;
    std::cout << "Duration: " << duration << " minutes" << std::endl;
    std::cout << "Type: " << typeDescription << std::endl;
    std::cout << "Calories Burned: " << caloriesBurned << std::endl;
    std::cout << "RPE: " << ratePerceivedExhaustion << "/10" << std::endl;
    std::cout << "Muscle Group ID: " << muscleGroupId << std::endl;
}

std::string Workout::toString() const {
    std::ostringstream oss;
    oss << "Workout[ID=" << workoutId 
        << ", Date=" << workoutDate
        << ", Duration=" << duration << "min"
        << ", Type=" << typeDescription
        << ", Calories=" << caloriesBurned
        << ", RPE=" << ratePerceivedExhaustion << "]";
    return oss.str();
}

bool Workout::isHighIntensity() const {
    return ratePerceivedExhaustion >= 8;
}
