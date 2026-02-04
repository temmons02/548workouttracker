-- Workout Tracking System - Test Data Script
-- Author: Claude, Therin Emmons
-- Date: 2026-01-28

-- Sample Data to Make sure that everything works.


-- Insert test data into MuscleGroup table
INSERT INTO MuscleGroup (name, description, days_per_week, sets, reps, weight_amount) VALUES
('Chest', 'Pectorals major and minor muscles', 2, 4, 12, 135.00),
('Back', 'Latissimus dorsi, trapezius, rhomboids', 2, 4, 10, 150.00),
('Legs', 'Quadriceps, hamstrings, glutes, calves', 2, 5, 15, 225.00),
('Shoulders', 'Deltoids - anterior, lateral, posterior', 2, 3, 12, 65.00),
('Arms', 'Biceps, triceps, forearms', 2, 3, 12, 80.00),
('Core', 'Abdominals, obliques, lower back', 3, 4, 20, 25.00),
('Glutes', 'Gluteus maximus, medius, minimus', 2, 4, 15, 185.00),
('Calves', 'Gastrocnemius and soleus muscles', 2, 4, 20, 95.00);

-- Insert test data into Workout table
INSERT INTO Workout (workout_date, workout_time, duration, type_description, calories_burned, rate_perceived_exhaustion, muscle_group_id) VALUES
('2026-01-20', '06:30:00', 60, 'Heavy Bench Press and Cable Flyes', 450.00, 8, 1),
('2026-01-21', '07:00:00', 45, 'Deadlifts and Pull-ups', 520.00, 9, 2),
('2026-01-22', '06:00:00', 75, 'Squats and Leg Press', 600.00, 9, 3),
('2026-01-23', '18:00:00', 40, 'Shoulder Press and Lateral Raises', 380.00, 7, 4),
('2026-01-24', '06:30:00', 50, 'Bicep Curls and Tricep Extensions', 320.00, 6, 5),
('2026-01-25', '07:30:00', 35, 'Planks and Russian Twists', 280.00, 7, 6),
('2026-01-26', '06:00:00', 55, 'Hip Thrusts and Glute Bridges', 420.00, 8, 7),
('2026-01-27', '17:00:00', 30, 'Calf Raises and Jumps', 240.00, 5, 8),
('2026-01-20', '18:00:00', 45, 'HIIT Cardio Session', 550.00, 9, 6),
('2026-01-23', '06:30:00', 60, 'Full Body Circuit Training', 580.00, 8, NULL);

-- Insert test data into Nutrition table
INSERT INTO Nutrition (family, water, carbs, fat, protein, sugar, meal_date) VALUES
('Mixed', 500.00, 45.00, 15.00, 30.00, 8.00, '2026-01-20'),
('Fruit', 200.00, 25.00, 0.50, 2.00, 20.00, '2026-01-20'),
('Meat', 300.00, 0.00, 20.00, 50.00, 0.00, '2026-01-21'),
('Vegetable', 400.00, 15.00, 1.00, 5.00, 3.00, '2026-01-21'),
('Dairy', 250.00, 12.00, 8.00, 15.00, 10.00, '2026-01-22'),
('Mixed', 600.00, 60.00, 25.00, 35.00, 12.00, '2026-01-22'),
('Fruit', 150.00, 30.00, 0.25, 1.50, 25.00, '2026-01-23'),
('Meat', 350.00, 5.00, 18.00, 55.00, 1.00, '2026-01-23'),
('Vegetable', 450.00, 20.00, 2.00, 7.00, 4.00, '2026-01-24'),
('Dairy', 300.00, 15.00, 10.00, 18.00, 12.00, '2026-01-24');

-- Insert test data into Recovery table
INSERT INTO Recovery (recovery_date, duration, type, helpers) VALUES
('2026-01-20', 30, 'Foam Rolling', 'Foam roller, massage balls'),
('2026-01-21', 60, 'Stretching Session', 'Yoga mat, resistance bands'),
('2026-01-22', 45, 'Ice Bath', 'Ice bath tub, ice packs'),
('2026-01-23', 90, 'Massage Therapy', 'Professional massage therapist'),
('2026-01-24', 20, 'Light Walking', 'None'),
('2026-01-25', 40, 'Yoga Session', 'Yoga mat, blocks, straps'),
('2026-01-26', 30, 'Compression Therapy', 'Compression boots'),
('2026-01-27', 60, 'Sauna Session', 'Infrared sauna');

-- Insert test data into Equipment table
INSERT INTO Equipment (name, description, category, target) VALUES
('Barbell', 'Olympic standard barbell 45lbs', 'Free Weights', 'Full Body'),
('Dumbbells', 'Set of adjustable dumbbells 5-50lbs', 'Free Weights', 'Full Body'),
('Bench Press', 'Adjustable weight bench', 'Bench', 'Chest, Arms'),
('Squat Rack', 'Power rack with safety bars', 'Rack', 'Legs, Back'),
('Cable Machine', 'Dual adjustable cable crossover', 'Machine', 'Full Body'),
('Treadmill', 'Commercial grade treadmill', 'Cardio', 'Legs, Cardiovascular'),
('Rowing Machine', 'Air resistance rowing machine', 'Cardio', 'Full Body'),
('Pull-up Bar', 'Wall-mounted pull-up bar', 'Bodyweight', 'Back, Arms'),
('Resistance Bands', 'Set of 5 different resistance levels', 'Accessories', 'Full Body'),
('Kettlebells', 'Set of kettlebells 10-50lbs', 'Free Weights', 'Full Body'),
('Leg Press Machine', 'Plate-loaded leg press', 'Machine', 'Legs'),
('Smith Machine', 'Guided barbell system', 'Machine', 'Full Body'),
('Foam Roller', 'High-density foam roller', 'Recovery', 'Full Body'),
('Yoga Mat', 'Non-slip exercise mat', 'Accessories', 'Core, Flexibility');
