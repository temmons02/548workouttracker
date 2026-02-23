-- view_all_data.sql
-- Quick view of all data in the Workout Tracking System
-- Usage: mysql -u workout_user -pworkout_pass workout_tracker < view_all_data.sql

SELECT '╔════════════════════════════════════════════════════════════════╗' as '';
SELECT '║          WORKOUT TRACKING SYSTEM - DATABASE CONTENTS           ║' as '';
SELECT '╚════════════════════════════════════════════════════════════════╝' as '';
SELECT '' as '';

-- Current Database
SELECT DATABASE() as 'Current Database';
SELECT '' as '';

-- Table Statistics
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 'TABLE STATISTICS' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 
    'Workouts' as 'Table Name', 
    COUNT(*) as 'Row Count' 
FROM Workout
UNION ALL SELECT 'MuscleGroups', COUNT(*) FROM MuscleGroup
UNION ALL SELECT 'Nutrition', COUNT(*) FROM Nutrition
UNION ALL SELECT 'Recovery', COUNT(*) FROM Recovery
UNION ALL SELECT 'Equipment', COUNT(*) FROM Equipment
UNION ALL SELECT '─────────────', '──────────'
UNION ALL SELECT 'TOTAL', 
    (SELECT COUNT(*) FROM Workout) + 
    (SELECT COUNT(*) FROM MuscleGroup) + 
    (SELECT COUNT(*) FROM Nutrition) + 
    (SELECT COUNT(*) FROM Recovery) + 
    (SELECT COUNT(*) FROM Equipment);

SELECT '' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 'LATEST 10 WORKOUTS' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 
    workout_id as 'ID',
    workout_date as 'Date',
    workout_time as 'Time',
    duration as 'Min',
    type_description as 'Type',
    calories_burned as 'Cal',
    rate_perceived_exhaustion as 'RPE'
FROM Workout 
ORDER BY workout_date DESC, workout_time DESC 
LIMIT 10;

SELECT '' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 'ALL MUSCLE GROUPS' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 
    muscle_group_id as 'ID',
    name as 'Name',
    days_per_week as 'Days/Wk',
    sets as 'Sets',
    reps as 'Reps',
    weight_amount as 'Weight'
FROM MuscleGroup
ORDER BY name;

SELECT '' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 'LATEST 10 NUTRITION ENTRIES' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 
    nutrition_id as 'ID',
    family as 'Family',
    meal_date as 'Date',
    ROUND(carbs, 1) as 'Carbs_g',
    ROUND(protein, 1) as 'Prot_g',
    ROUND(fat, 1) as 'Fat_g',
    ROUND(carbs * 4 + protein * 4 + fat * 9, 0) as 'Cal'
FROM Nutrition
ORDER BY meal_date DESC
LIMIT 10;

SELECT '' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 'LATEST 10 RECOVERY SESSIONS' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 
    recovery_id as 'ID',
    recovery_date as 'Date',
    type as 'Type',
    duration as 'Min',
    helpers as 'Helpers'
FROM Recovery
ORDER BY recovery_date DESC
LIMIT 10;

SELECT '' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 'EQUIPMENT BY CATEGORY' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 
    category as 'Category',
    COUNT(*) as 'Count'
FROM Equipment
GROUP BY category
ORDER BY Count DESC;

SELECT '' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 'EQUIPMENT LIST (First 15)' as '';
SELECT '═══════════════════════════════════════════════════════════════' as '';
SELECT 
    equipment_id as 'ID',
    name as 'Name',
    category as 'Category',
    target as 'Target'
FROM Equipment
ORDER BY category, name
LIMIT 15;
