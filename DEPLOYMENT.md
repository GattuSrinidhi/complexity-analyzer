# C Code Complexity Analyzer - Deployment Guide

This project is a Flask-based C code complexity analyzer with a Chart.js frontend. It can be deployed locally or on cloud platforms like Render.

## Project Structure

```
complexity_analyzer/
├── server.py                 # Flask backend API
├── dashboard.html            # Frontend UI
├── analyzer.exe              # Compiled Flex/C analyzer
├── analyzer.l                # Flex lexer source
├── requirements.txt          # Python dependencies
├── Dockerfile                # Docker configuration
├── .dockerignore              # Docker ignore file
└── test.c                    # Test C file
```

## Local Development

### Requirements
- Python 3.11+
- Flask and dependencies (see requirements.txt)
- analyzer.exe (pre-compiled Flex/C program)

### Setup

1. Navigate to project directory:
```bash
cd complexity_analyzer
```

2. Create a virtual environment (optional):
```bash
python -m venv .venv
.venv\Scripts\activate  # Windows
# or
source .venv/bin/activate  # Linux/Mac
```

3. Install dependencies:
```bash
pip install -r requirements.txt
```

4. Run the server:
```bash
python server.py
```

5. Access the application:
- Open browser to `http://localhost:5000`

## Docker Deployment

### Build Docker Image

```bash
docker build -t complexity-analyzer .
```

### Run Locally with Docker

```bash
docker run -p 5000:5000 complexity-analyzer
```

Access: `http://localhost:5000`

### Push to Docker Hub (optional)

```bash
docker tag complexity-analyzer:latest <your-docker-username>/complexity-analyzer:latest
docker push <your-docker-username>/complexity-analyzer:latest
```

## Deploy on Render

### Prerequisites
- Render account (free tier available)
- GitHub repository with this project

### Step-by-Step Deployment

1. **Push to GitHub**:
```bash
git init
git add .
git commit -m "Initial commit"
git branch -M main
git remote add origin https://github.com/<username>/<repo>.git
git push -u origin main
```

2. **Create Render Account**:
   - Go to https://render.com
   - Sign up with GitHub

3. **Deploy on Render**:
   - Click "New +" button
   - Select "Web Service"
   - Connect your GitHub repository
   - Fill in deployment settings:
     - **Name**: `complexity-analyzer`
     - **Environment**: Docker
     - **Region**: Choose closest to you
     - **Plan**: Free (or upgraded)
   - Click "Create Web Service"

4. **Configure Environment**:
   - In Render dashboard, go to your service
   - No need to set PORT (Render auto-assigns)
   - The app will be live at: `https://<service-name>.onrender.com`

### Render Deployment Notes
- Free tier supports up to 750 hours/month
- Service spins down after 15 minutes of inactivity (cold start delay)
- Paid plans provide always-on deployment

## Environment Variables

- **PORT**: Server port (default: 5000)
  - Example: `export PORT=8080`

## API Endpoints

### GET `/`
- Returns the dashboard HTML interface
- Access: `http://localhost:5000/`

### GET `/dashboard`
- Alternative dashboard route
- Access: `http://localhost:5000/dashboard`

### POST `/analyze`
- Analyzes C code
- Request body: `code=<C_code>`
- Response: JSON with analysis metrics

Example using curl:
```bash
curl -X POST http://localhost:5000/analyze \
  -d "code=int main(){for(int i=0;i<10;i++){printf(\"hello\");}return 0;}" \
  -H "Content-Type: application/x-www-form-urlencoded"
```

Response:
```json
{
  "functions": 1,
  "loops": 1,
  "decisions": 0,
  "max_depth": 1,
  "complexity": "O(n)",
  "recursion": 0
}
```

## Features

✅ Analyze C code complexity  
✅ Detect loops and nesting depth  
✅ Count functions and decision statements  
✅ Generate visual Chart.js bar graph  
✅ Upload .c files or paste code  
✅ Works with unlimited sequential analyses  
✅ Responsive HTML interface  

## Troubleshooting

### "Canvas is already in use"
- Fixed in latest version - chart is properly destroyed before re-rendering

### Port already in use
```bash
# Windows
netstat -ano | findstr :5000
taskkill /PID <PID> /F

# Linux/Mac
lsof -i :5000
kill -9 <PID>
```

### analyzer.exe not found
- Ensure `analyzer.exe` is in the project root
- Recompile from `analyzer.l` if needed

### Flask development server warning
- Normal for development. Use production WSGI server for production:
```bash
pip install gunicorn
gunicorn -w 4 -b 0.0.0.0:5000 server:app
```

## Production Deployment

For production, use a WSGI application server:

### Using Gunicorn with Render

1. Update requirements.txt to include gunicorn:
```
Flask==3.1.3
Werkzeug==3.1.6
gunicorn==21.2.0
```

2. Update Dockerfile:
```dockerfile
CMD ["gunicorn", "-w", "4", "-b", "0.0.0.0:$PORT", "server:app"]
```

3. Redeploy on Render

## Support

For issues or questions, refer to the Flask and Chart.js documentation:
- Flask: https://flask.palletsprojects.com
- Chart.js: https://www.chartjs.org
