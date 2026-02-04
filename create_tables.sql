-- Workout Tracking System - Database Creation Script
-- Author: Claude
-- Date: 2026-01-28

-- Create MuscleGroup table
CREATE TABLE MuscleGroup (
    muscle_group_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    days_per_week INT,
    sets INT,
    reps INT,
    weight_amount DECIMAL(10, 2),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Create Workout table
CREATE TABLE Workout (
    workout_id INT PRIMARY KEY AUTO_INCREMENT,
    workout_date DATE NOT NULL,
    workout_time TIME NOT NULL,
    duration INT NOT NULL COMMENT 'Duration in minutes',
    type_description VARCHAR(255),
    calories_burned DECIMAL(10, 2),
    rate_perceived_exhaustion INT COMMENT 'Scale 1-10',
    muscle_group_id INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (muscle_group_id) REFERENCES MuscleGroup(muscle_group_id) ON DELETE SET NULL
);

-- Create Nutrition table
CREATE TABLE Nutrition (
    nutrition_id INT PRIMARY KEY AUTO_INCREMENT,
    family ENUM('Mixed', 'Fruit', 'Meat', 'Vegetable', 'Dairy') NOT NULL,
    water DECIMAL(10, 2) COMMENT 'Water in ml',
    carbs DECIMAL(10, 2) COMMENT 'Carbohydrates in grams',
    fat DECIMAL(10, 2) COMMENT 'Fat in grams',
    protein DECIMAL(10, 2) COMMENT 'Protein in grams',
    sugar DECIMAL(10, 2) COMMENT 'Sugar in grams',
    meal_date DATE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Create Recovery table
CREATE TABLE Recovery (
    recovery_id INT PRIMARY KEY AUTO_INCREMENT,
    recovery_date DATE NOT NULL,
    duration INT NOT NULL COMMENT 'Duration in minutes',
    type VARCHAR(100),
    helpers TEXT COMMENT 'Recovery aids or helpers used',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Create Equipment table
CREATE TABLE Equipment (
    equipment_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    category VARCHAR(100),
    target VARCHAR(100) COMMENT 'Target muscle group or body part',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Create indexes for better performance
CREATE INDEX idx_workout_date ON Workout(workout_date);
CREATE INDEX idx_workout_muscle_group ON Workout(muscle_group_id);
CREATE INDEX idx_nutrition_date ON Nutrition(meal_date);
CREATE INDEX idx_recovery_date ON Recovery(recovery_date);
CREATE INDEX idx_equipment_category ON Equipment(category);
