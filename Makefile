# Makefile for Workout Tracking System
# Fixed for actual project structure
# Author: Claude
# Date: 2026-01-28

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -I/usr/include/mysql -I/usr/local/include
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread

# Directories
BUILD_DIR = build
SERVICE_DIR = ServiceLayer
BUSINESS_DIR = BusinessLayer
FRONTEND_DIR = Front-EndLayer
CGI_DIR = /usr/lib/cgi-bin

# Model sources (in root directory)
MODEL_SOURCES = Workout.cpp MuscleGroup.cpp Nutrition.cpp Recovery.cpp Equipment.cpp

# Data layer (in root)
DATA_SOURCES = WorkoutDAO.cpp

# Business layer
BUSINESS_SOURCES = $(BUSINESS_DIR)/WorkoutManager.cpp

# Service layer
SERVICE_SOURCES = $(SERVICE_DIR)/WorkoutService.cpp

# Common sources needed by most programs
COMMON_SOURCES = $(MODEL_SOURCES) $(DATA_SOURCES) $(BUSINESS_SOURCES)

# Executables
MAIN_TARGET = $(BUILD_DIR)/workout_tracker
TEST_TARGET = $(BUILD_DIR)/test_crud_app
API_SERVER = $(BUILD_DIR)/rest_api_server
CRUD_FRONTEND = $(BUILD_DIR)/crud_frontend
CGI_APP = workout.cgi

.PHONY: all clean rebuild help core api frontend cgi install-cgi

# Default target
all: core api
	@echo ""
	@echo "✓ Build complete!"
	@echo ""
	@echo "Built: $(MAIN_TARGET)"
	@echo "       $(TEST_TARGET)"
	@echo "       $(API_SERVER)"
	@echo ""
	@echo "Next: make cgi (build CGI)"
	@echo "      make install-cgi (deploy CGI)"
	@echo ""

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Core applications (each has its own main())
core: $(MAIN_TARGET) $(TEST_TARGET)

# Main app - ONLY compile main.cpp with common sources
$(MAIN_TARGET): main.cpp $(COMMON_SOURCES) | $(BUILD_DIR)
	@echo "Building main application..."
	$(CXX) $(CXXFLAGS) main.cpp $(COMMON_SOURCES) -o $(MAIN_TARGET) $(LDFLAGS)
	@echo "✓ Built: $(MAIN_TARGET)"

# Test app - ONLY compile test_crud_app.cpp with common sources
$(TEST_TARGET): $(BUSINESS_DIR)/test_crud_app.cpp $(COMMON_SOURCES) | $(BUILD_DIR)
	@echo "Building test application..."
	$(CXX) $(CXXFLAGS) $(BUSINESS_DIR)/test_crud_app.cpp $(COMMON_SOURCES) \
	-o $(TEST_TARGET) $(LDFLAGS)
	@echo "✓ Built: $(TEST_TARGET)"

# REST API
api: $(API_SERVER)

# REST API Server - ONLY compile RestApiServer.cpp with common sources
$(API_SERVER): $(SERVICE_DIR)/RestApiServer.cpp $(COMMON_SOURCES) | $(BUILD_DIR)
	@echo "Building REST API server..."
	$(CXX) $(CXXFLAGS) $(SERVICE_DIR)/RestApiServer.cpp $(COMMON_SOURCES) \
	-o $(API_SERVER) $(LDFLAGS)
	@echo "✓ Built: $(API_SERVER)"

# Frontend
frontend: $(CRUD_FRONTEND)

# CRUD Frontend - ONLY compile CrudFrontEnd.cpp
$(CRUD_FRONTEND): $(SERVICE_DIR)/CrudFrontEnd.cpp | $(BUILD_DIR)
	@echo "Building CRUD frontend..."
	$(CXX) $(CXXFLAGS) $(SERVICE_DIR)/CrudFrontEnd.cpp \
	-o $(CRUD_FRONTEND) -lpthread -lcurl
	@echo "✓ Built: $(CRUD_FRONTEND)"

# CGI Application
cgi: $(CGI_APP)

# CGI - ONLY compile workout_cgi.cpp with needed sources
$(CGI_APP): $(FRONTEND_DIR)/workout_cgi.cpp $(COMMON_SOURCES) $(SERVICE_SOURCES)
	@echo "Building CGI application..."
	@echo "⚠️  Note: If you get 'undefined reference' errors for CGI handlers,"
	@echo "    you need to use the complete workout_cgi.cpp file provided."
	$(CXX) $(CXXFLAGS) $(FRONTEND_DIR)/workout_cgi.cpp \
	$(COMMON_SOURCES) $(SERVICE_SOURCES) \
	-o $(CGI_APP) $(LDFLAGS)
	@chmod +x $(CGI_APP)
	@echo "✓ Built: $(CGI_APP)"

# Install CGI to Apache
install-cgi: $(CGI_APP)
	@echo "Installing CGI to Apache..."
	@sudo cp $(CGI_APP) $(CGI_DIR)/workout.cgi
	@sudo chmod +x $(CGI_DIR)/workout.cgi
	@sudo chown www-data:www-data $(CGI_DIR)/workout.cgi
	@echo "✓ Installed: $(CGI_DIR)/workout.cgi"
	@echo "Access: http://localhost/cgi-bin/workout.cgi"

# Run targets
run-main: $(MAIN_TARGET)
	@$(MAIN_TARGET)

run-test: $(TEST_TARGET)
	@$(TEST_TARGET)

run-server: $(API_SERVER)
	@echo "Starting REST API server..."
	@$(API_SERVER)

run-frontend: $(CRUD_FRONTEND)
	@$(CRUD_FRONTEND)

# Database
db-setup:
	@mysql -u workout_user -pworkout_pass workout_tracker < create_tables.sql
	@mysql -u workout_user -pworkout_pass workout_tracker < insert_test_data.sql
	@echo "✓ Database setup"

db-view:
	@mysql -u workout_user -pworkout_pass workout_tracker < $(BUSINESS_DIR)/view_all_data.sql

# Clean
clean:
	@rm -rf $(BUILD_DIR) $(CGI_APP)
	@echo "✓ Clean"

clean-all: clean
	@sudo rm -f $(CGI_DIR)/workout.cgi

rebuild: clean all

# Help
help:
	@echo "Makefile targets:"
	@echo "  make all          - Build core + api"
	@echo "  make core         - Build main + test"
	@echo "  make api          - Build REST API server"
	@echo "  make frontend     - Build CRUD frontend"
	@echo "  make cgi          - Build CGI"
	@echo "  make install-cgi  - Deploy CGI to Apache"
	@echo "  make run-main     - Run main"
	@echo "  make run-test     - Run tests"
	@echo "  make run-server   - Start API"
	@echo "  make clean        - Clean build"
	@echo ""
	@echo "Structure:"
	@echo "  Root:              Models, DAO, main.cpp"
	@echo "  $(BUSINESS_DIR):  WorkoutManager, test_crud_app"
	@echo "  $(SERVICE_DIR):   Service, API, frontend"
	@echo "  $(FRONTEND_DIR):  CGI"
