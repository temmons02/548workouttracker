# REST API Setup and Hosting Guide

## Overview

This guide covers setting up and hosting the Workout Tracking System REST API on both Linux and Windows.

**Architecture:**

```
┌─────────────┐    HTTP/REST    ┌──────────────┐
│  API Client │ ──────────────> │  API Server  │
│ (ApiClient) │ <────────────── │  (Port 8080) │
└─────────────┘    JSON         └──────┬───────┘
                                       │
                               ┌───────▼────────┐
                               │ WorkoutManager │
                               │ Business Layer │
                               └───────┬────────┘
                                       │
                               ┌───────▼────────┐
                               │  WorkoutDAO    │
                               │  Data Layer    │
                               └───────┬────────┘
                                       │
                               ┌───────▼────────┐
                               │     MySQL      │
                               │   Database     │
                               └────────────────┘
```

## Prerequisites

### Both Platforms

- MySQL database set up with workout_tracker database
- C++11 compatible compiler
- cpp-httplib library

### Linux Specific

- g++ compiler
- libssl-dev (for HTTPS support - optional)
- pthread library

### Windows Specific

- Visual Studio 2017 or later
- MySQL Connector/C++
- Windows Sockets (ws2_32.lib)

---

## Linux (Ubuntu/Mint) Setup

### Step 1: Install Dependencies

```bash
# Update package list
sudo apt-get update

# Install build tools and SSL library
sudo apt-get install build-essential libssl-dev

# MySQL should already be installed from earlier steps
```

### Step 2: Install cpp-httplib Library

```bash
# Download cpp-httplib (header-only library)
cd /tmp
git clone https://github.com/yhirose/cpp-httplib.git


# Copy header to system include directory
sudo cp cpp-httplib/httplib.h /usr/local/include/

# Verify installation
ls -l /usr/local/include/httplib.h
```

### Step 3: Compile the REST API Server

```bash
# Navigate to your project directory
cd /path/to/workout-tracker


# Compile the REST API server (write in 'ServiceLayer' Directory)
g++ -std=c++17 -I/usr/include/mysql -I/usr/local/include RestApiServer.cpp ../Workout.cpp ../MuscleGroup.cpp ../Nutrition.cpp ../Recovery.cpp ../Equipment.cpp ../WorkoutDAO.cpp ../BusinessLayer/WorkoutManager.cpp -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -o rest_api_server

# Check if compilation succeeded
ls -lh rest_api_server
```

### Step 4: Compile the API Client(USE Crudfrontend MENTIONED IN THE OTHER .md FILE)

```bash
# Compile the API client
g++ -std=c++17 -I/usr/local/include ApiClient.cpp -lpthread \
    -o api_client

# Check if compilation succeeded
ls -lh api_client
```

### Step 5: Run the Server

```bash
# Start the REST API server
./rest_api_server

# You should see:
# [SERVER] Starting REST API server...
# [SERVER] Listening on http://localhost:8080
```

**The server is now running on port 8080!**

### Step 6: Test with Client(Also check out Crudfrontend documentation)

Open a **new terminal** and run:

```bash
# Test the API
./api_client
```

### Step 7: Test with curl (Alternative)

```bash
# Test health endpoint
curl http://localhost:8080/health

# Get all workouts
curl http://localhost:8080/api/workouts

# Get workout by ID
curl http://localhost:8080/api/workouts/1

# Get all muscle groups
curl http://localhost:8080/api/musclegroups

# Save a workout (POST)
curl -X POST http://localhost:8080/api/workouts

# Get all nutrition
curl http://localhost:8080/api/nutrition

# Get all recovery
curl http://localhost:8080/api/recovery

# Get all equipment
curl http://localhost:8080/api/equipment
```

### Step 8: Test in Browser

Open your web browser and navigate to:

- <http://localhost:8080> (Welcome page with endpoints)
- <http://localhost:8080/api/workouts> (JSON data)
- <http://localhost:8080/api/musclegroups> (JSON data)

---

## Windows Setup

### Step 1: Download cpp-httplib

1. Go to <https://github.com/yhirose/cpp-httplib>
2. Click "Code" → "Download ZIP"
3. Extract the ZIP file
4. Copy `httplib.h` to your project directory

### Step 2: Create Visual Studio Project

1. Open Visual Studio
2. Create New Project → Console App (C++)
3. Add all files to project:
   - RestApiServer.cpp
   - ApiClient.cpp (separate project)
   - JsonHelper.h
   - All model files (.cpp and .h)
   - WorkoutDAO files
   - WorkoutManager files
   - httplib.h

### Step 3: Configure Project Properties

**For RestApiServer project:**

1. Right-click project → Properties
2. Configuration: All Configurations

**C/C++ Settings:**

- General → Additional Include Directories:

  ```
  C:\Program Files\MySQL\MySQL Server 8.0\include
  $(ProjectDir)
  ```

**Linker Settings:**

- General → Additional Library Directories:

  ```
  C:\Program Files\MySQL\MySQL Server 8.0\lib
  ```

- Input → Additional Dependencies:

  ```
  libmysql.lib
  ws2_32.lib
  ```

1. Click Apply → OK

**For ApiClient project:**

- Same as above, but only needs `ws2_32.lib` (no MySQL)

### Step 4: Build the Projects

1. Build → Build Solution (Ctrl+Shift+B)
2. Verify no errors

### Step 5: Copy MySQL DLL

```cmd
# Copy libmysql.dll to your Debug or Release folder
copy "C:\Program Files\MySQL\MySQL Server 8.0\lib\libmysql.dll" Debug\
```

### Step 6: Update Database Credentials

Edit `RestApiServer.cpp` line ~70:

```cpp
dao = std::make_unique<WorkoutDAO>("localhost", "workout_user", "workout_pass", "workout_tracker");
```

Update with your MySQL credentials.

### Step 7: Run the Server

1. In Visual Studio, set RestApiServer as startup project
2. Press F5 or Ctrl+F5 to run
3. Server will start on <http://localhost:8080>

### Step 8: Run the Client

1. Open a new Visual Studio instance or Command Prompt
2. Navigate to your build directory
3. Run:

   ```cmd
   Debug\ApiClient.exe
   ```

### Step 9: Test with PowerShell

```powershell
# Test health endpoint
Invoke-WebRequest -Uri http://localhost:8080/health | Select-Object -ExpandProperty Content

# Get all workouts
Invoke-WebRequest -Uri http://localhost:8080/api/workouts | Select-Object -ExpandProperty Content

# Save a workout (POST)
Invoke-WebRequest -Uri http://localhost:8080/api/workouts -Method POST
```

### Step 10: Test in Browser

Navigate to:

- <http://localhost:8080>
- <http://localhost:8080/api/workouts>
- <http://localhost:8080/api/musclegroups>

---

## Production Hosting Options

### Option 1: Run as Linux Service (systemd)

Create a systemd service file:

```bash
sudo nano /etc/systemd/system/workout-api.service
```

Add:

```ini
[Unit]
Description=Workout Tracking REST API
After=network.target mysql.service

[Service]
Type=simple
User=yourusername
WorkingDirectory=/path/to/workout-tracker
ExecStart=/path/to/workout-tracker/rest_api_server
Restart=on-failure
RestartSec=10

[Install]
WantedBy=multi-user.target
```

Enable and start:

```bash
sudo systemctl daemon-reload
sudo systemctl enable workout-api
sudo systemctl start workout-api
sudo systemctl status workout-api
```

### Option 2: Use nginx as Reverse Proxy

Install nginx:

```bash
sudo apt-get install nginx
```

Configure:

```bash
sudo nano /etc/nginx/sites-available/workout-api
```

Add:

```nginx
server {
    listen 80;
    server_name your-domain.com;

    location /api/ {
        proxy_pass http://localhost:8080/api/;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

Enable:

```bash
sudo ln -s /etc/nginx/sites-available/workout-api /etc/nginx/sites-enabled/
sudo nginx -t
sudo systemctl restart nginx
```

### Option 3: Docker Container

Create `Dockerfile`:

```dockerfile
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    build-essential \
    libmysqlclient-dev \
    libssl-dev
COPY . /app
WORKDIR /app
RUN g++ -std=c++11 -I/usr/include/mysql RestApiServer.cpp *.cpp -lmysqlclient -lpthread -o rest_api_server
EXPOSE 8080
CMD ["./rest_api_server"]
```

Build and run:

```bash
docker build -t workout-api .
docker run -p 8080:8080 workout-api
```

### Option 4: Windows Service

Use NSSM (Non-Sucking Service Manager):

```cmd
# Download NSSM from nssm.cc
nssm install WorkoutAPI "C:\path\to\rest_api_server.exe"
nssm start WorkoutAPI
```

---

## API Endpoints Reference

### Workout Endpoints

- `GET  /api/workouts` - Get all workouts
- `GET  /api/workouts/:id` - Get workout by ID
- `POST /api/workouts` - Save workout

### MuscleGroup Endpoints

- `GET  /api/musclegroups` - Get all muscle groups
- `GET  /api/musclegroups/:id` - Get muscle group by ID
- `POST /api/musclegroups` - Save muscle group

### Nutrition Endpoints

- `GET  /api/nutrition` - Get all nutrition entries
- `GET  /api/nutrition/:id` - Get nutrition by ID
- `POST /api/nutrition` - Save nutrition

### Recovery Endpoints

- `GET  /api/recovery` - Get all recovery sessions
- `GET  /api/recovery/:id` - Get recovery by ID
- `POST /api/recovery` - Save recovery

### Equipment Endpoints

- `GET  /api/equipment` - Get all equipment
- `GET  /api/equipment/:id` - Get equipment by ID
- `POST /api/equipment` - Save equipment

### Utility Endpoints

- `GET  /health` - Health check
- `GET  /` - API documentation page

---

## Troubleshooting

### Linux Issues

**"httplib.h: No such file or directory"**

```bash
# Reinstall cpp-httplib
cd /tmp
git clone https://github.com/yhirose/cpp-httplib.git
sudo cp cpp-httplib/httplib.h /usr/local/include/
```

**"Cannot connect to MySQL"**

```bash
# Check MySQL is running
sudo systemctl status mysql

# Test connection
mysql -u workout_user -pworkout_pass workout_tracker
```

**"Address already in use"**

```bash
# Port 8080 is in use, kill the process
sudo lsof -i :8080
sudo kill -9 <PID>
```

### Windows Issues

**"Cannot open include file: 'httplib.h'"**

- Ensure httplib.h is in project directory
- Check Additional Include Directories in project properties

**"Unresolved external symbol mysql\_\*"**

- Add libmysql.lib to Additional Dependencies
- Verify library path in Additional Library Directories

**"ws2_32.lib not found"**

- This should be automatically available in Windows SDK
- Reinstall Visual Studio with C++ desktop development workload

---

## Testing Checklist

- [ ] Server starts without errors
- [ ] Can access <http://localhost:8080> in browser
- [ ] Health check returns `{"status":"healthy"}`
- [ ] GET /api/workouts returns JSON array
- [ ] GET /api/workouts/1 returns single workout
- [ ] POST /api/workouts creates new workout
- [ ] All 5 controllers (Workout, MuscleGroup, Nutrition, Recovery, Equipment) work
- [ ] API client can connect and test all endpoints
- [ ] Server logs requests in terminal

---

## Quick Reference

### Start Server (Linux)

```bash
./rest_api_server
```

### Start Client (Linux)

```bash
./api_client
```

### Test Endpoint (curl)

```bash
curl http://localhost:8080/api/workouts
```

### Test Endpoint (PowerShell)

```powershell
Invoke-WebRequest -Uri http://localhost:8080/api/workouts
```

### View in Browser

```
http://localhost:8080/api/workouts
```

---

## Next Steps

1. Add authentication (JWT tokens)
2. Implement proper JSON parsing (use nlohmann/json library)
3. Add input validation
4. Implement HTTPS/SSL
5. Add rate limiting
6. Create API documentation (Swagger/OpenAPI)
7. Add logging to files
8. Implement caching (Redis)
9. Add API versioning
10. Deploy to cloud (AWS, Azure, Google Cloud)

---

**Your REST API is ready to use!** 🚀
