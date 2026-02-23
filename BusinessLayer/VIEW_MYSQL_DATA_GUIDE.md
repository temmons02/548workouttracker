# How to View Your Workout Tracking System in MySQL

This guide shows you multiple ways to view and interact with your project data in MySQL.

## Method 1: MySQL Command Line (Quick & Easy)

### Basic Login and View

```bash
# Login to MySQL
mysql -u workout_user -pworkout_pass workout_tracker

# Or if you prefer to be prompted for password (more secure):
mysql -u workout_user -p workout_tracker
```

### Essential Commands Once Logged In

```sql
-- Show which database you're using
SELECT DATABASE();

-- List all tables
SHOW TABLES;

-- Show table structure
DESCRIBE Workout;
DESCRIBE MuscleGroup;
DESCRIBE Nutrition;
DESCRIBE Recovery;
DESCRIBE Equipment;

-- Count rows in each table
SELECT 'Workouts' as Table_Name, COUNT(*) as Row_Count FROM Workout
UNION ALL SELECT 'MuscleGroups', COUNT(*) FROM MuscleGroup
UNION ALL SELECT 'Nutrition', COUNT(*) FROM Nutrition
UNION ALL SELECT 'Recovery', COUNT(*) FROM Recovery
UNION ALL SELECT 'Equipment', COUNT(*) FROM Equipment;

-- View all data from each table
SELECT * FROM Workout;
SELECT * FROM MuscleGroup;
SELECT * FROM Nutrition;
SELECT * FROM Recovery;
SELECT * FROM Equipment;

-- View latest 10 workouts
SELECT * FROM Workout ORDER BY workout_date DESC, workout_time DESC LIMIT 10;

-- View workouts with muscle group names (JOIN)
SELECT 
    w.workout_id,
    w.workout_date,
    w.workout_time,
    w.duration,
    w.type_description,
    w.calories_burned,
    w.rate_perceived_exhaustion,
    mg.name as muscle_group_name
FROM Workout w
LEFT JOIN MuscleGroup mg ON w.muscle_group_id = mg.muscle_group_id
ORDER BY w.workout_date DESC
LIMIT 10;

-- Exit MySQL
EXIT;
```

## Method 2: One-Line Commands (From Terminal)

View data without entering MySQL interactive mode:

```bash
# View all tables
mysql -u workout_user -pworkout_pass workout_tracker -e "SHOW TABLES;"

# View row counts
mysql -u workout_user -pworkout_pass workout_tracker -e "
SELECT 'Workouts' as Table_Name, COUNT(*) as Count FROM Workout
UNION ALL SELECT 'MuscleGroups', COUNT(*) FROM MuscleGroup
UNION ALL SELECT 'Nutrition', COUNT(*) FROM Nutrition
UNION ALL SELECT 'Recovery', COUNT(*) FROM Recovery
UNION ALL SELECT 'Equipment', COUNT(*) FROM Equipment;"

# View all workouts
mysql -u workout_user -pworkout_pass workout_tracker -e "SELECT * FROM Workout;"

# View latest 5 workouts
mysql -u workout_user -pworkout_pass workout_tracker -e "
SELECT * FROM Workout 
ORDER BY workout_date DESC, workout_time DESC 
LIMIT 5;"

# View workouts for a specific date
mysql -u workout_user -pworkout_pass workout_tracker -e "
SELECT * FROM Workout 
WHERE workout_date = '2026-01-20';"

# View all muscle groups
mysql -u workout_user -pworkout_pass workout_tracker -e "SELECT * FROM MuscleGroup;"

# View nutrition for a specific date
mysql -u workout_user -pworkout_pass workout_tracker -e "
SELECT * FROM Nutrition 
WHERE meal_date = '2026-01-20';"

# View recovery sessions by type
mysql -u workout_user -pworkout_pass workout_tracker -e "
SELECT * FROM Recovery 
WHERE type LIKE '%Yoga%';"

# View equipment by category
mysql -u workout_user -pworkout_pass workout_tracker -e "
SELECT * FROM Equipment 
WHERE category = 'Free Weights';"
```

## Method 3: MySQL Workbench (Graphical Interface)

MySQL Workbench provides a visual way to explore your database.

### Installation

```bash
# Linux Mint / Ubuntu
sudo apt install mysql-workbench

# Launch it
mysql-workbench
```

### Setup Connection

1. Open MySQL Workbench
2. Click the **+** button next to "MySQL Connections"
3. Enter connection details:
   - **Connection Name:** Workout Tracker
   - **Hostname:** localhost
   - **Port:** 3306
   - **Username:** workout_user
   - **Password:** Click "Store in Vault" and enter: workout_pass
   - **Default Schema:** workout_tracker
4. Click **Test Connection**
5. Click **OK**

### Using MySQL Workbench

1. **Double-click** your connection
2. In the left sidebar, expand **workout_tracker** → **Tables**
3. **Right-click** any table → **Select Rows - Limit 1000**
4. View and edit data in the spreadsheet-like interface
5. Run custom queries in the **Query** tab

**Advantages:**
- Visual table browser
- Easy data editing
- Export data to CSV/JSON/Excel
- ER diagrams
- Query builder

## Method 4: Create Custom View Scripts

I'll create some useful SQL scripts for you:

### View All Data Script

```bash
# Save this as view_all_data.sql
cat > view_all_data.sql << 'EOF'
-- View All Data from Workout Tracking System

-- Database Info
SELECT DATABASE() as Current_Database;

-- Table Statistics
SELECT 'STATISTICS' as '═════════════════════════════════════';
SELECT 
    'Workouts' as Table_Name, COUNT(*) as Row_Count FROM Workout
UNION ALL SELECT 'MuscleGroups', COUNT(*) FROM MuscleGroup
UNION ALL SELECT 'Nutrition', COUNT(*) FROM Nutrition
UNION ALL SELECT 'Recovery', COUNT(*) FROM Recovery
UNION ALL SELECT 'Equipment', COUNT(*) FROM Equipment;

-- Recent Workouts
SELECT 'RECENT WORKOUTS (Latest 10)' as '═════════════════════════════════════';
SELECT 
    workout_id as ID,
    workout_date as Date,
    workout_time as Time,
    duration as Minutes,
    type_description as Type,
    calories_burned as Calories,
    rate_perceived_exhaustion as RPE
FROM Workout 
ORDER BY workout_date DESC, workout_time DESC 
LIMIT 10;

-- All Muscle Groups
SELECT 'MUSCLE GROUPS' as '═════════════════════════════════════';
SELECT 
    muscle_group_id as ID,
    name as Name,
    days_per_week as Days_Week,
    sets as Sets,
    reps as Reps,
    weight_amount as Weight
FROM MuscleGroup
ORDER BY name;

-- Recent Nutrition
SELECT 'RECENT NUTRITION (Latest 10)' as '═════════════════════════════════════';
SELECT 
    nutrition_id as ID,
    family as Family,
    meal_date as Date,
    carbs as Carbs_g,
    protein as Protein_g,
    fat as Fat_g,
    (carbs * 4 + protein * 4 + fat * 9) as Total_Calories
FROM Nutrition
ORDER BY meal_date DESC
LIMIT 10;

-- Recent Recovery
SELECT 'RECENT RECOVERY (Latest 10)' as '═════════════════════════════════════';
SELECT 
    recovery_id as ID,
    recovery_date as Date,
    type as Type,
    duration as Minutes,
    helpers as Helpers
FROM Recovery
ORDER BY recovery_date DESC
LIMIT 10;

-- Equipment Summary
SELECT 'EQUIPMENT BY CATEGORY' as '═════════════════════════════════════';
SELECT 
    category as Category,
    COUNT(*) as Count
FROM Equipment
GROUP BY category
ORDER BY Count DESC;
EOF

# Run it
mysql -u workout_user -pworkout_pass workout_tracker < view_all_data.sql
```

### View Workout Statistics Script

```bash
# Save this as workout_stats.sql
cat > workout_stats.sql << 'EOF'
-- Workout Statistics

-- Workouts per day
SELECT 
    workout_date as Date,
    COUNT(*) as Num_Workouts,
    SUM(duration) as Total_Minutes,
    SUM(calories_burned) as Total_Calories,
    AVG(rate_perceived_exhaustion) as Avg_RPE
FROM Workout
GROUP BY workout_date
ORDER BY workout_date DESC
LIMIT 30;

-- Workouts by muscle group
SELECT 
    mg.name as Muscle_Group,
    COUNT(w.workout_id) as Num_Workouts,
    AVG(w.duration) as Avg_Duration,
    AVG(w.calories_burned) as Avg_Calories,
    AVG(w.rate_perceived_exhaustion) as Avg_RPE
FROM Workout w
JOIN MuscleGroup mg ON w.muscle_group_id = mg.muscle_group_id
GROUP BY mg.name
ORDER BY Num_Workouts DESC;

-- High intensity workouts (RPE >= 8)
SELECT 
    workout_date as Date,
    type_description as Type,
    rate_perceived_exhaustion as RPE,
    calories_burned as Calories
FROM Workout
WHERE rate_perceived_exhaustion >= 8
ORDER BY workout_date DESC;
EOF

# Run it
mysql -u workout_user -pworkout_pass workout_tracker < workout_stats.sql
```

## Method 5: Pretty Output with Column Mode

For better formatted output in the terminal:

```bash
# Create an alias for pretty MySQL output
alias mysqlpretty='mysql -u workout_user -pworkout_pass workout_tracker --table'

# Use it
mysqlpretty -e "SELECT * FROM MuscleGroup;"

# Or for even nicer output with vertical display
mysql -u workout_user -pworkout_pass workout_tracker -e "SELECT * FROM Workout LIMIT 1\G"
```

## Method 6: Export Data to Files

### Export to CSV

```bash
# Export workouts to CSV
mysql -u workout_user -pworkout_pass workout_tracker -e "
SELECT * FROM Workout;" | sed 's/\t/,/g' > workouts.csv

# Export all tables
for table in Workout MuscleGroup Nutrition Recovery Equipment; do
    mysql -u workout_user -pworkout_pass workout_tracker -e "SELECT * FROM $table;" \
    | sed 's/\t/,/g' > ${table}.csv
    echo "Exported $table to ${table}.csv"
done
```

### Export to Text File with Formatting

```bash
# Create a comprehensive report
mysql -u workout_user -pworkout_pass workout_tracker --table < view_all_data.sql > database_report.txt
cat database_report.txt
```

## Method 7: Create a Quick View Script

Create a handy script to view your data quickly:

```bash
# Create the script
cat > view_database.sh << 'EOF'
#!/bin/bash
# Quick Database Viewer for Workout Tracking System

DB_USER="workout_user"
DB_PASS="workout_pass"
DB_NAME="workout_tracker"

echo "╔════════════════════════════════════════════════════════╗"
echo "║     WORKOUT TRACKING SYSTEM - DATABASE VIEWER          ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

echo "=== TABLE STATISTICS ==="
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT 
    'Workouts' as Table_Name, COUNT(*) as Row_Count FROM Workout
UNION ALL SELECT 'MuscleGroups', COUNT(*) FROM MuscleGroup
UNION ALL SELECT 'Nutrition', COUNT(*) FROM Nutrition
UNION ALL SELECT 'Recovery', COUNT(*) FROM Recovery
UNION ALL SELECT 'Equipment', COUNT(*) FROM Equipment;"

echo ""
echo "=== LATEST 5 WORKOUTS ==="
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT 
    workout_id, workout_date, workout_time, duration, 
    type_description, calories_burned, rate_perceived_exhaustion
FROM Workout 
ORDER BY workout_date DESC, workout_time DESC 
LIMIT 5;"

echo ""
echo "=== ALL MUSCLE GROUPS ==="
mysql -u $DB_USER -p$DB_PASS $DB_NAME --table -e "
SELECT muscle_group_id, name, days_per_week, sets, reps, weight_amount 
FROM MuscleGroup 
ORDER BY name;"

echo ""
echo "Available commands:"
echo "  mysql -u $DB_USER -p$DB_PASS $DB_NAME    # Login to MySQL"
echo "  mysql-workbench                                # Launch GUI"
echo "  ./view_database.sh                             # Run this script again"
EOF

# Make it executable
chmod +x view_database.sh

# Run it
./view_database.sh
```

## Useful SQL Queries for Your Project

### Find Workouts by Date Range
```sql
SELECT * FROM Workout 
WHERE workout_date BETWEEN '2026-01-01' AND '2026-01-31'
ORDER BY workout_date, workout_time;
```

### Total Calories Burned This Month
```sql
SELECT 
    SUM(calories_burned) as Total_Calories,
    AVG(calories_burned) as Avg_Per_Workout,
    COUNT(*) as Num_Workouts
FROM Workout 
WHERE workout_date >= DATE_FORMAT(NOW(), '%Y-%m-01');
```

### Nutrition Summary by Date
```sql
SELECT 
    meal_date,
    SUM(carbs) as Total_Carbs,
    SUM(protein) as Total_Protein,
    SUM(fat) as Total_Fat,
    SUM(carbs * 4 + protein * 4 + fat * 9) as Total_Calories
FROM Nutrition
GROUP BY meal_date
ORDER BY meal_date DESC;
```

### Recovery Time by Type
```sql
SELECT 
    type,
    COUNT(*) as Sessions,
    SUM(duration) as Total_Minutes,
    AVG(duration) as Avg_Minutes
FROM Recovery
GROUP BY type
ORDER BY Sessions DESC;
```

### Equipment Inventory by Category
```sql
SELECT 
    category,
    COUNT(*) as Items,
    GROUP_CONCAT(name SEPARATOR ', ') as Equipment_List
FROM Equipment
GROUP BY category
ORDER BY category;
```

## Quick Reference Card

```
╔═══════════════════════════════════════════════════════════╗
║              MYSQL QUICK REFERENCE CARD                   ║
╠═══════════════════════════════════════════════════════════╣
║                                                           ║
║  Login:                                                   ║
║    mysql -u workout_user -pworkout_pass workout_tracker   ║
║                                                           ║
║  Show Tables:                                             ║
║    SHOW TABLES;                                           ║
║                                                           ║
║  View All Workouts:                                       ║
║    SELECT * FROM Workout;                                 ║
║                                                           ║
║  Count Rows:                                              ║
║    SELECT COUNT(*) FROM Workout;                          ║
║                                                           ║
║  View Latest 10:                                          ║
║    SELECT * FROM Workout ORDER BY workout_date DESC       ║
║    LIMIT 10;                                              ║
║                                                           ║
║  Launch GUI:                                              ║
║    mysql-workbench                                        ║
║                                                           ║
║  Exit MySQL:                                              ║
║    EXIT; or Ctrl+D                                        ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
```

## Summary

**Fastest way:** 
```bash
mysql -u workout_user -pworkout_pass workout_tracker
```
Then:
```sql
SHOW TABLES;
SELECT * FROM Workout LIMIT 10;
```

**Best GUI:** Install and use MySQL Workbench for visual exploration

**Best for scripts:** Create custom .sql files and run them

Choose the method that works best for you! 🚀
