// MuscleGroup.h
// Workout Tracking System - MuscleGroup Class
// Author: Claude, Therin Emmons
// Date: 2026-01-28

#ifndef MUSCLEGROUP_H
#define MUSCLEGROUP_H

#include <string>
#include <ctime>

class MuscleGroup {
private:
    int muscleGroupId;
    std::string name;
    std::string description;
    int daysPerWeek;
    int sets;
    int reps;
    double weightAmount;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructors
    MuscleGroup();
    MuscleGroup(int id, const std::string& name, const std::string& desc,
                int days, int sets, int reps, double weight);
    
    // Destructor
    ~MuscleGroup();
    
    // Getters
    int getMuscleGroupId() const;
    std::string getName() const;
    std::string getDescription() const;
    int getDaysPerWeek() const;
    int getSets() const;
    int getReps() const;
    double getWeightAmount() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;
    
    // Setters
    void setMuscleGroupId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& desc);
    void setDaysPerWeek(int days);
    void setSets(int sets);
    void setReps(int reps);
    void setWeightAmount(double weight);
    void setCreatedAt(time_t created);
    void setUpdatedAt(time_t updated);
    
    // Utility methods
    void displayInfo() const;
    std::string toString() const;
};

#endif // MUSCLEGROUP_H
