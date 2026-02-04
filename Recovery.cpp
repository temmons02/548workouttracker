// Recovery.cpp
// Workout Tracking System - Recovery Class Implementation
// Author: Claude
// Date: 2026-01-28

#include "Recovery.h"
#include <iostream>
#include <sstream>

// Default Constructor
Recovery::Recovery()
    : recoveryId(0), recoveryDate(""), duration(0), type(""),
      helpers(""), createdAt(0), updatedAt(0) {
}

// Parameterized Constructor
Recovery::Recovery(int id, const std::string& date, int dur,
                   const std::string& t, const std::string& h)
    : recoveryId(id), recoveryDate(date), duration(dur), type(t), helpers(h) {
    createdAt = std::time(nullptr);
    updatedAt = std::time(nullptr);
}

// Destructor
Recovery::~Recovery() {
}

// Getters
int Recovery::getRecoveryId() const { return recoveryId; }
std::string Recovery::getRecoveryDate() const { return recoveryDate; }
int Recovery::getDuration() const { return duration; }
std::string Recovery::getType() const { return type; }
std::string Recovery::getHelpers() const { return helpers; }
time_t Recovery::getCreatedAt() const { return createdAt; }
time_t Recovery::getUpdatedAt() const { return updatedAt; }

// Setters
void Recovery::setRecoveryId(int id) { recoveryId = id; }
void Recovery::setRecoveryDate(const std::string& date) { recoveryDate = date; }
void Recovery::setDuration(int dur) { duration = dur; }
void Recovery::setType(const std::string& t) { type = t; }
void Recovery::setHelpers(const std::string& h) { helpers = h; }
void Recovery::setCreatedAt(time_t created) { createdAt = created; }
void Recovery::setUpdatedAt(time_t updated) { updatedAt = updated; }

// Utility methods
void Recovery::displayInfo() const {
    std::cout << "=== Recovery Information ===" << std::endl;
    std::cout << "ID: " << recoveryId << std::endl;
    std::cout << "Date: " << recoveryDate << std::endl;
    std::cout << "Duration: " << duration << " minutes" << std::endl;
    std::cout << "Type: " << type << std::endl;
    std::cout << "Helpers/Aids: " << helpers << std::endl;
}

std::string Recovery::toString() const {
    std::ostringstream oss;
    oss << "Recovery[ID=" << recoveryId 
        << ", Date=" << recoveryDate
        << ", Duration=" << duration << "min"
        << ", Type=" << type << "]";
    return oss.str();
}

bool Recovery::isLongRecovery() const {
    return duration > 60;
}
