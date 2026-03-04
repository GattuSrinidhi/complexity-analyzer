from flask import Flask, request, jsonify
import subprocess
import json
import os

app = Flask(__name__)

@app.route("/")
def index():
    with open('dashboard.html', 'r') as f:
        return f.read()

@app.route("/dashboard")
def dashboard():
    with open('dashboard.html', 'r') as f:
        return f.read()

@app.route("/analyze", methods=["POST"])
def analyze():
    try:
        code = request.form.get("code")
        if not code:
            return jsonify({"error": "No code provided"}), 400
        
        with open("temp.c", "w") as f:
            f.write(code)
        
        result = subprocess.run(["analyzer.exe", "temp.c"], capture_output=True, text=True, cwd=".")
        if result.returncode != 0:
            return jsonify({"error": f"Analysis failed: {result.stderr}"}), 500
        
        with open("result.json") as f:
            data = json.load(f)
        return jsonify(data)
    except FileNotFoundError as e:
        return jsonify({"error": f"File not found: {str(e)}"}), 500
    except json.JSONDecodeError as e:
        return jsonify({"error": f"Invalid JSON in result: {str(e)}"}), 500
    except Exception as e:
        return jsonify({"error": f"Server error: {str(e)}"}), 500

if __name__ == "__main__":
    port = int(os.environ.get('PORT', 5000))
    app.run(host='0.0.0.0', port=port, debug=False)
