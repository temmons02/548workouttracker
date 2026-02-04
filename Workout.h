// Workout.h
// Workout Tracking System - Workout Class
// Author: Claude
// Date: 2026-01-28

#ifndef WORKOUT_H
#define WORKOUT_H

#include <string>
#include <ctime>

class Workout {
private:
    int workoutId;
    std::string workoutDate;  // Format: YYYY-MM-DD
    std::string workoutTime;  // Format: HH:MM:SS
    int duration;             // Duration in minutes
    std::string typeDescription;
    double caloriesBurned;
    int ratePerceivedExhaustion;  // Scale 1-10
    int muscleGroupId;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructors
    Workout();
    Workout(int id, const std::string& date, const std::string& time,
            int duration, const std::string& type, double calories,
            int rpe, int muscleGroupId);
    
    // Destructor
    ~Workout();
    
    // Getters
    int getWorkoutId() const;
    std::string getWorkoutDate() const;
    std::string getWorkoutTime() const;
    int getDuration() const;
    std::string getTypeDescription() const;
    double getCaloriesBurned() const;
    int getRatePerceivedExhaustion() const;
    int getMuscleGroupId() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;
    
    // Setters
    void setWorkoutId(int id);
    void setWorkoutDate(const std::string& date);
    void setWorkoutTime(const std::string& time);
    void setDuration(int duration);
    void setTypeDescription(const std::string& type);
    void setCaloriesBurned(double calories);
    void setRatePerceivedExhaustion(int rpe);
    void setMuscleGroupId(int muscleGroupId);
    void setCreatedAt(time_t created);
    void setUpdatedAt(time_t updated);
    
    // Utility methods
    void displayInfo() const;
    std::string toString() const;
    bool isHighIntensity() const;  // RPE >= 8
};

#endif // WORKOUT_H
