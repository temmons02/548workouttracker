# Makefile for Workout Tracking System
# Author: Claude
# Date: 2026-01-28

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I/usr/include/mysql
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lmysqlclient

# Target executable
TARGET = workout_tracker

# Source files
SOURCES = main.cpp \
          Workout.cpp \
          MuscleGroup.cpp \
          Nutrition.cpp \
          Recovery.cpp \
          Equipment.cpp \
          WorkoutDAO.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files (for dependencies)
HEADERS = Workout.h \
          MuscleGroup.h \
          Nutrition.h \
          Recovery.h \
          Equipment.h \
          WorkoutDAO.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Rebuild everything from scratch
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Install MySQL client libraries (Ubuntu/Debian)
install-deps:
	@echo "Installing MySQL client development libraries..."
	sudo apt-get update
	sudo apt-get install -y libmysqlclient-dev

# Help target
help:
	@echo "Workout Tracking System - Makefile Help"
	@echo "========================================"
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make all      - Build the project (same as make)"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make run      - Build and run the program"
	@echo "  make install-deps - Install MySQL dependencies"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Before building, ensure MySQL is installed and configured."
	@echo "Update database credentials in main.cpp before running."

.PHONY: all clean rebuild run install-deps help
