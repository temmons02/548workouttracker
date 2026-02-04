// MuscleGroup.cpp
// Workout Tracking System - MuscleGroup Class Implementation
// Author: Claude
// Date: 2026-01-28

#include "MuscleGroup.h"
#include <iostream>
#include <sstream>

// Default Constructor
MuscleGroup::MuscleGroup() 
    : muscleGroupId(0), name(""), description(""), daysPerWeek(0), 
      sets(0), reps(0), weightAmount(0.0), createdAt(0), updatedAt(0) {
}

// Parameterized Constructor
MuscleGroup::MuscleGroup(int id, const std::string& name, const std::string& desc,
                         int days, int sets, int reps, double weight)
    : muscleGroupId(id), name(name), description(desc), daysPerWeek(days),
      sets(sets), reps(reps), weightAmount(weight) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

// Destructor
MuscleGroup::~MuscleGroup() {
}

// Getters
int MuscleGroup::getMuscleGroupId() const { return muscleGroupId; }
std::string MuscleGroup::getName() const { return name; }
std::string MuscleGroup::getDescription() const { return description; }
int MuscleGroup::getDaysPerWeek() const { return daysPerWeek; }
int MuscleGroup::getSets() const { return sets; }
int MuscleGroup::getReps() const { return reps; }
double MuscleGroup::getWeightAmount() const { return weightAmount; }
time_t MuscleGroup::getCreatedAt() const { return createdAt; }
time_t MuscleGroup::getUpdatedAt() const { return updatedAt; }

// Setters
void MuscleGroup::setMuscleGroupId(int id) { muscleGroupId = id; }
void MuscleGroup::setName(const std::string& n) { name = n; }
void MuscleGroup::setDescription(const std::string& desc) { description = desc; }
void MuscleGroup::setDaysPerWeek(int days) { daysPerWeek = days; }
void MuscleGroup::setSets(int s) { sets = s; }
void MuscleGroup::setReps(int r) { reps = r; }
void MuscleGroup::setWeightAmount(double weight) { weightAmount = weight; }
void MuscleGroup::setCreatedAt(time_t created) { createdAt = created; }
void MuscleGroup::setUpdatedAt(time_t updated) { updatedAt = updated; }

// Utility methods
void MuscleGroup::displayInfo() const {
    std::cout << "=== Muscle Group Information ===" << std::endl;
    std::cout << "ID: " << muscleGroupId << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Days per Week: " << daysPerWeek << std::endl;
    std::cout << "Sets: " << sets << std::endl;
    std::cout << "Reps: " << reps << std::endl;
    std::cout << "Weight: " << weightAmount << " lbs" << std::endl;
}

std::string MuscleGroup::toString() const {
    std::ostringstream oss;
    oss << "MuscleGroup[ID=" << muscleGroupId 
        << ", Name=" << name
        << ", Days/Week=" << daysPerWeek
        << ", Sets=" << sets
        << ", Reps=" << reps
        << ", Weight=" << weightAmount << "]";
    return oss.str();
}
