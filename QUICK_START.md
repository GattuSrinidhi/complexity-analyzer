# Quick Start - Deploy to Render (Free)

## 5-Minute Deployment

### Step 1: Push to GitHub
```bash
git init
git add .
git commit -m "C Code Complexity Analyzer"
git branch -M main
git remote add origin https://github.com/<your-username>/complexity-analyzer.git
git push -u origin main
```

### Step 2: Deploy on Render
1. Go to https://render.com (sign up free)
2. Click "New +" → "Web Service"
3. Connect your GitHub repo
4. Settings:
   - **Name**: `complexity-analyzer`
   - **Environment**: Docker
   - **Plan**: Free
5. Click "Create Web Service"
6. Wait 5-10 minutes for build

### Step 3: Access Your App
Your live URL: `https://complexity-analyzer.onrender.com`

---

## Docker Local Testing

Test Docker before pushing:

```bash
# Build image
docker build -t complexity-analyzer .

# Run container
docker run -p 5000:5000 complexity-analyzer

# Access: http://localhost:5000
```

---

## Project Files Overview

| File | Purpose |
|------|---------|
| `server.py` | Flask API backend |
| `dashboard.html` | Frontend UI with Chart.js |
| `analyzer.exe` | Compiled C complexity analyzer |
| `requirements.txt` | Python dependencies (Flask, Werkzeug) |
| `Dockerfile` | Container configuration for cloud deployment |
| `.dockerignore` | Files to exclude from Docker image |
| `.gitignore` | Files to exclude from Git |

---

## Environment Variables

The app automatically uses:
- `PORT`: Server port (Render assigns this automatically)
- Default: 5000 (local development)

---

## What's Configured

✅ **server.py** - Uses environment variable `PORT`  
✅ **Dockerfile** - Builds Python 3.11 image with Flask  
✅ **requirements.txt** - Flask dependencies listed  
✅ **.dockerignore** - Excludes unnecessary files  
✅ **.gitignore** - Ready for GitHub  
✅ **Root route "/"** - Serves dashboard.html  

---

## API Endpoints

### POST /analyze
```bash
curl -X POST https://complexity-analyzer.onrender.com/analyze \
  -d "code=int main(){return 0;}" \
  -H "Content-Type: application/x-www-form-urlencoded"
```

Response: JSON with complexity metrics

---

## Troubleshooting

**Port already in use?**
```bash
# Windows
netstat -ano | findstr :5000
taskkill /PID <PID> /F

# Linux/Mac
lsof -i :5000
kill -9 <PID>
```

**analyzer.exe not found?**
- Ensure `analyzer.exe` is in project root
- It's already compiled, just needs to be pushed to GitHub

**PDF slower to start on free tier?**
- Render free tier spins down after 15 min of inactivity (cold start ~30s)
- Upgrade to paid plan for always-on deployment

---

## Next Steps

1. Create GitHub repo and push code
2. Sign up on Render.com (free)
3. Connect GitHub to Render
4. Deploy - done! 🚀

For detailed instructions, see `DEPLOYMENT.md`
