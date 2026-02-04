// Recovery.h
// Workout Tracking System - Recovery Class
// Author: Claude
// Date: 2026-01-28

#ifndef RECOVERY_H
#define RECOVERY_H

#include <string>
#include <ctime>

class Recovery {
private:
    int recoveryId;
    std::string recoveryDate;  // Format: YYYY-MM-DD
    int duration;              // Duration in minutes
    std::string type;
    std::string helpers;       // Recovery aids or helpers used
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructors
    Recovery();
    Recovery(int id, const std::string& date, int duration,
             const std::string& type, const std::string& helpers);
    
    // Destructor
    ~Recovery();
    
    // Getters
    int getRecoveryId() const;
    std::string getRecoveryDate() const;
    int getDuration() const;
    std::string getType() const;
    std::string getHelpers() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;
    
    // Setters
    void setRecoveryId(int id);
    void setRecoveryDate(const std::string& date);
    void setDuration(int duration);
    void setType(const std::string& type);
    void setHelpers(const std::string& helpers);
    void setCreatedAt(time_t created);
    void setUpdatedAt(time_t updated);
    
    // Utility methods
    void displayInfo() const;
    std::string toString() const;
    bool isLongRecovery() const;  // Duration > 60 minutes
};

#endif // RECOVERY_H
