// Equipment.cpp
// Workout Tracking System - Equipment Class Implementation
// Author: Claude
// Date: 2026-01-28

#include "Equipment.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Default Constructor
Equipment::Equipment()
    : equipmentId(0), name(""), description(""), category(""),
      target(""), createdAt(0), updatedAt(0) {
}

// Parameterized Constructor
Equipment::Equipment(int id, const std::string& n, const std::string& desc,
                     const std::string& cat, const std::string& tgt)
    : equipmentId(id), name(n), description(desc), category(cat), target(tgt) {
    createdAt = std::time(nullptr);
    updatedAt = std::time(nullptr);
}

// Destructor
Equipment::~Equipment() {
}

// Getters
int Equipment::getEquipmentId() const { return equipmentId; }
std::string Equipment::getName() const { return name; }
std::string Equipment::getDescription() const { return description; }
std::string Equipment::getCategory() const { return category; }
std::string Equipment::getTarget() const { return target; }
time_t Equipment::getCreatedAt() const { return createdAt; }
time_t Equipment::getUpdatedAt() const { return updatedAt; }

// Setters
void Equipment::setEquipmentId(int id) { equipmentId = id; }
void Equipment::setName(const std::string& n) { name = n; }
void Equipment::setDescription(const std::string& desc) { description = desc; }
void Equipment::setCategory(const std::string& cat) { category = cat; }
void Equipment::setTarget(const std::string& tgt) { target = tgt; }
void Equipment::setCreatedAt(time_t created) { createdAt = created; }
void Equipment::setUpdatedAt(time_t updated) { updatedAt = updated; }

// Utility methods
void Equipment::displayInfo() const {
    std::cout << "=== Equipment Information ===" << std::endl;
    std::cout << "ID: " << equipmentId << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Category: " << category << std::endl;
    std::cout << "Target: " << target << std::endl;
}

std::string Equipment::toString() const {
    std::ostringstream oss;
    oss << "Equipment[ID=" << equipmentId 
        << ", Name=" << name
        << ", Category=" << category
        << ", Target=" << target << "]";
    return oss.str();
}

bool Equipment::isCardioEquipment() const {
    std::string lower = category;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower.find("cardio") != std::string::npos;
}
