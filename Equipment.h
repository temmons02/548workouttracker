// Equipment.h
// Workout Tracking System - Equipment Class
// Author: Claude, Therin Emmons
// Date: 2026-01-28

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>
#include <ctime>

class Equipment {
private:
    int equipmentId;
    std::string name;
    std::string description;
    std::string category;
    std::string target;  // Target muscle group or body part
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructors
    Equipment();
    Equipment(int id, const std::string& name, const std::string& desc,
              const std::string& category, const std::string& target);
    
    // Destructor
    ~Equipment();
    
    // Getters
    int getEquipmentId() const;
    std::string getName() const;
    std::string getDescription() const;
    std::string getCategory() const;
    std::string getTarget() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;
    
    // Setters
    void setEquipmentId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& desc);
    void setCategory(const std::string& category);
    void setTarget(const std::string& target);
    void setCreatedAt(time_t created);
    void setUpdatedAt(time_t updated);
    
    // Utility methods
    void displayInfo() const;
    std::string toString() const;
    bool isCardioEquipment() const;
};

#endif // EQUIPMENT_H
