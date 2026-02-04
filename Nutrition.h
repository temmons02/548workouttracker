// Nutrition.h
// Workout Tracking System - Nutrition Class
// Author: Claude, Therin Emmons
// Date: 2026-01-28

#ifndef NUTRITION_H
#define NUTRITION_H

#include <string>
#include <ctime>

enum class FoodFamily {
    MIXED,
    FRUIT,
    MEAT,
    VEGETABLE,
    DAIRY
};

class Nutrition {
private:
    int nutritionId;
    FoodFamily family;
    double water;      // in ml
    double carbs;      // in grams
    double fat;        // in grams
    double protein;    // in grams
    double sugar;      // in grams
    std::string mealDate;  // Format: YYYY-MM-DD
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructors
    Nutrition();
    Nutrition(int id, FoodFamily family, double water, double carbs,
              double fat, double protein, double sugar, const std::string& date);
    
    // Destructor
    ~Nutrition();
    
    // Getters
    int getNutritionId() const;
    FoodFamily getFamily() const;
    std::string getFamilyString() const;
    double getWater() const;
    double getCarbs() const;
    double getFat() const;
    double getProtein() const;
    double getSugar() const;
    std::string getMealDate() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;
    
    // Setters
    void setNutritionId(int id);
    void setFamily(FoodFamily family);
    void setFamilyFromString(const std::string& familyStr);
    void setWater(double water);
    void setCarbs(double carbs);
    void setFat(double fat);
    void setProtein(double protein);
    void setSugar(double sugar);
    void setMealDate(const std::string& date);
    void setCreatedAt(time_t created);
    void setUpdatedAt(time_t updated);
    
    // Utility methods
    void displayInfo() const;
    std::string toString() const;
    double calculateTotalCalories() const;  // 4 cal/g carbs & protein, 9 cal/g fat
    double getMacroRatio(const std::string& macro) const;  // Returns percentage
};

#endif // NUTRITION_H
