// Nutrition.cpp
// Workout Tracking System - Nutrition Class Implementation
// Author: Claude
// Date: 2026-01-28

#include "Nutrition.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Default Constructor
Nutrition::Nutrition()
    : nutritionId(0), family(FoodFamily::MIXED), water(0.0), carbs(0.0),
      fat(0.0), protein(0.0), sugar(0.0), mealDate(""), createdAt(0), updatedAt(0) {
}

// Parameterized Constructor
Nutrition::Nutrition(int id, FoodFamily fam, double w, double c,
                     double f, double p, double s, const std::string& date)
    : nutritionId(id), family(fam), water(w), carbs(c),
      fat(f), protein(p), sugar(s), mealDate(date) {
    createdAt = std::time(nullptr);
    updatedAt = std::time(nullptr);
}

// Destructor
Nutrition::~Nutrition() {
}

// Getters
int Nutrition::getNutritionId() const { return nutritionId; }
FoodFamily Nutrition::getFamily() const { return family; }

std::string Nutrition::getFamilyString() const {
    switch(family) {
        case FoodFamily::MIXED: return "Mixed";
        case FoodFamily::FRUIT: return "Fruit";
        case FoodFamily::MEAT: return "Meat";
        case FoodFamily::VEGETABLE: return "Vegetable";
        case FoodFamily::DAIRY: return "Dairy";
        default: return "Unknown";
    }
}

double Nutrition::getWater() const { return water; }
double Nutrition::getCarbs() const { return carbs; }
double Nutrition::getFat() const { return fat; }
double Nutrition::getProtein() const { return protein; }
double Nutrition::getSugar() const { return sugar; }
std::string Nutrition::getMealDate() const { return mealDate; }
time_t Nutrition::getCreatedAt() const { return createdAt; }
time_t Nutrition::getUpdatedAt() const { return updatedAt; }

// Setters
void Nutrition::setNutritionId(int id) { nutritionId = id; }
void Nutrition::setFamily(FoodFamily fam) { family = fam; }

void Nutrition::setFamilyFromString(const std::string& familyStr) {
    std::string lower = familyStr;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "mixed") family = FoodFamily::MIXED;
    else if (lower == "fruit") family = FoodFamily::FRUIT;
    else if (lower == "meat") family = FoodFamily::MEAT;
    else if (lower == "vegetable") family = FoodFamily::VEGETABLE;
    else if (lower == "dairy") family = FoodFamily::DAIRY;
    else family = FoodFamily::MIXED;
}

void Nutrition::setWater(double w) { water = w; }
void Nutrition::setCarbs(double c) { carbs = c; }
void Nutrition::setFat(double f) { fat = f; }
void Nutrition::setProtein(double p) { protein = p; }
void Nutrition::setSugar(double s) { sugar = s; }
void Nutrition::setMealDate(const std::string& date) { mealDate = date; }
void Nutrition::setCreatedAt(time_t created) { createdAt = created; }
void Nutrition::setUpdatedAt(time_t updated) { updatedAt = updated; }

// Utility methods
void Nutrition::displayInfo() const {
    std::cout << "=== Nutrition Information ===" << std::endl;
    std::cout << "ID: " << nutritionId << std::endl;
    std::cout << "Family: " << getFamilyString() << std::endl;
    std::cout << "Date: " << mealDate << std::endl;
    std::cout << "Water: " << water << " ml" << std::endl;
    std::cout << "Carbs: " << carbs << " g" << std::endl;
    std::cout << "Fat: " << fat << " g" << std::endl;
    std::cout << "Protein: " << protein << " g" << std::endl;
    std::cout << "Sugar: " << sugar << " g" << std::endl;
    std::cout << "Total Calories: " << calculateTotalCalories() << std::endl;
}

std::string Nutrition::toString() const {
    std::ostringstream oss;
    oss << "Nutrition[ID=" << nutritionId 
        << ", Family=" << getFamilyString()
        << ", Date=" << mealDate
        << ", Calories=" << calculateTotalCalories() << "]";
    return oss.str();
}

double Nutrition::calculateTotalCalories() const {
    // Carbs: 4 cal/g, Protein: 4 cal/g, Fat: 9 cal/g
    return (carbs * 4.0) + (protein * 4.0) + (fat * 9.0);
}

double Nutrition::getMacroRatio(const std::string& macro) const {
    double totalCalories = calculateTotalCalories();
    if (totalCalories == 0.0) return 0.0;
    
    std::string lower = macro;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "carbs") {
        return (carbs * 4.0) / totalCalories * 100.0;
    } else if (lower == "protein") {
        return (protein * 4.0) / totalCalories * 100.0;
    } else if (lower == "fat") {
        return (fat * 9.0) / totalCalories * 100.0;
    }
    return 0.0;
}
