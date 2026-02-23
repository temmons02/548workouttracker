#!/bin/bash
# view_database.sh
# Quick Database Viewer for Workout Tracking System
# Usage: ./view_database.sh

DB_USER="workout_user"
DB_PASS="workout_pass"
DB_NAME="workout_tracker"

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║       WORKOUT TRACKING SYSTEM - DATABASE VIEWER                ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Check if MySQL is accessible
if ! mysql -u $DB_USER -p$DB_PASS -e "SELECT 1" > /dev/null 2>&1; then
    echo "❌ Error: Cannot connect to MySQL"
    echo "   Check your credentials or if MySQL is running"
    echo ""
    echo "   Try: sudo systemctl status mysql"
    exit 1
fi

echo "✓ Connected to MySQL successfully"
echo ""

echo "═══════════════════════════════════════════════════════════════"
echo "TABLE STATISTICS"
echo "═══════════════════════════════════════════════════════════════"
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT 
    'Workouts' as 'Table Name', COUNT(*) as 'Row Count' FROM Workout
UNION ALL SELECT 'MuscleGroups', COUNT(*) FROM MuscleGroup
UNION ALL SELECT 'Nutrition', COUNT(*) FROM Nutrition
UNION ALL SELECT 'Recovery', COUNT(*) FROM Recovery
UNION ALL SELECT 'Equipment', COUNT(*) FROM Equipment;"

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo "LATEST 5 WORKOUTS"
echo "═══════════════════════════════════════════════════════════════"
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT 
    workout_id as ID, 
    workout_date as Date, 
    workout_time as Time, 
    duration as Min,
    LEFT(type_description, 30) as Type, 
    calories_burned as Cal, 
    rate_perceived_exhaustion as RPE
FROM Workout 
ORDER BY workout_date DESC, workout_time DESC 
LIMIT 5;"

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo "ALL MUSCLE GROUPS"
echo "═══════════════════════════════════════════════════════════════"
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT 
    muscle_group_id as ID, 
    name as Name, 
    days_per_week as 'Days/Week', 
    sets as Sets, 
    reps as Reps, 
    weight_amount as Weight 
FROM MuscleGroup 
ORDER BY name;"

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo "NUTRITION SUMMARY (Last 3 Days)"
echo "═══════════════════════════════════════════════════════════════"
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT 
    meal_date as Date,
    COUNT(*) as Meals,
    ROUND(SUM(carbs), 1) as 'Carbs_g',
    ROUND(SUM(protein), 1) as 'Protein_g',
    ROUND(SUM(fat), 1) as 'Fat_g',
    ROUND(SUM(carbs * 4 + protein * 4 + fat * 9), 0) as 'Total_Cal'
FROM Nutrition
GROUP BY meal_date
ORDER BY meal_date DESC
LIMIT 3;"

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo "RECENT RECOVERY SESSIONS (Last 5)"
echo "═══════════════════════════════════════════════════════════════"
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT 
    recovery_id as ID,
    recovery_date as Date,
    type as Type,
    duration as Minutes,
    LEFT(helpers, 30) as Helpers
FROM Recovery
ORDER BY recovery_date DESC
LIMIT 5;"

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo "EQUIPMENT BY CATEGORY"
echo "═══════════════════════════════════════════════════════════════"
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT 
    category as Category,
    COUNT(*) as Count
FROM Equipment
GROUP BY category
ORDER BY Count DESC;"

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo ""
echo "📋 Quick Commands:"
echo "   Login to MySQL:"
echo "     mysql -u $DB_USER -p$DB_PASS $DB_NAME"
echo ""
echo "   View specific table:"
echo "     mysql -u $DB_USER -p$DB_PASS $DB_NAME -e \"SELECT * FROM Workout;\""
echo ""
echo "   Launch MySQL Workbench (GUI):"
echo "     mysql-workbench"
echo ""
echo "   View all data (detailed):"
echo "     mysql -u $DB_USER -p$DB_PASS $DB_NAME < view_all_data.sql"
echo ""
echo "═══════════════════════════════════════════════════════════════"
