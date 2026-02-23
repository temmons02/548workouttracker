# Makefile for Workout Tracking System
# Author: Claude
# Date: 2026-01-28

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -I/usr/include/mysql
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lmysqlclient

# Target executables
TARGET = workout_tracker
TEST_TARGET = test_crud_app

# Common source files
COMMON_SOURCES = Workout.cpp \
                 MuscleGroup.cpp \
                 Nutrition.cpp \
                 Recovery.cpp \
                 Equipment.cpp \
                 WorkoutDAO.cpp \
                 ./BusinessLayer/WorkoutManager.cpp

# Main application sources
MAIN_SOURCES = main.cpp $(COMMON_SOURCES)

# Test application sources
TEST_SOURCES = ./BusinessLayer/test_crud_app.cpp $(COMMON_SOURCES)

# Object files
MAIN_OBJECTS = $(MAIN_SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)
COMMON_OBJECTS = $(COMMON_SOURCES:.cpp=.o)

# Header files (for dependencies)
HEADERS = Workout.h \
          MuscleGroup.h \
          Nutrition.h \
          Recovery.h \
          Equipment.h \
          WorkoutDAO.h \
          ./BusinessLayer/WorkoutManager.h

# Default target - build both applications
all: $(TARGET) $(TEST_TARGET)

# Link main application
$(TARGET): $(MAIN_OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(MAIN_OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Link test application
$(TEST_TARGET): $(TEST_OBJECTS)
	@echo "Linking $(TEST_TARGET)..."
	$(CXX) $(TEST_OBJECTS) -o $(TEST_TARGET) $(LDFLAGS)
	@echo "Test application built! Run with: ./$(TEST_TARGET)"

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(MAIN_OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGET)
	@echo "Clean complete!"

# Rebuild everything from scratch
rebuild: clean all

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Run the test program
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Build only the test application
test-only: $(TEST_TARGET)

# Build only the main application
main-only: $(TARGET)

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
	@echo "  make          - Build both applications"
	@echo "  make all      - Build both applications (same as make)"
	@echo "  make main-only - Build only the main demo application"
	@echo "  make test-only - Build only the CRUD test application"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make rebuild  - Clean and rebuild both applications"
	@echo "  make run      - Build and run the main demo application"
	@echo "  make test     - Build and run the CRUD test application"
	@echo "  make install-deps - Install MySQL dependencies"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Applications:"
	@echo "  ./workout_tracker  - Main demonstration program"
	@echo "  ./test_crud_app    - CRUD test application (all 5 objects)"
	@echo ""
	@echo "Before building, ensure MySQL is installed and configured."
	@echo "Update database credentials in main.cpp and test_crud_app.cpp"

.PHONY: all clean rebuild run test test-only main-only install-deps help
