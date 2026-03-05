#!/usr/bin/env python3
"""
Complexity Analyzer - Automated Validation Script
Runs all test cases and compares with expected outputs
"""

import subprocess
import json
import sys

# Test cases with expected outputs
TEST_CASES = {
    "bubble_sort.c": {
        "name": "Bubble Sort",
        "expected": {
            "complexity": "O(n^2)",
            "loops": 2,
            "decisions": 1,
            "cyclomatic_complexity": 4,
            "recursion": 0,
        }
    },
    "bsearch.c": {
        "name": "Binary Search",
        "expected": {
            "complexity": "O(log n)",
            "loops": 1,
            "decisions": 3,
            "cyclomatic_complexity": 4,
            "recursion": 0,
        }
    },
    "merge_sort.c": {
        "name": "Merge Sort",
        "expected": {
            "complexity": "O(n log n)",
            "loops": 3,
            "decisions": 2,
            "cyclomatic_complexity": 6,
            "recursion": 1,
        }
    },
    "test.c": {
        "name": "Nested Loops",
        "expected": {
            "complexity": "O(n^2)",
            "loops": 2,
            "decisions": 1,
            "cyclomatic_complexity": 4,
            "recursion": 0,
        }
    },
    "recursion.c": {
        "name": "Factorial (Recursion)",
        "expected": {
            "complexity": "O(2^n)",
            "loops": 0,
            "decisions": 1,
            "cyclomatic_complexity": 2,
            "recursion": 1,
        }
    },
    "max_test.c": {
        "name": "Max Function",
        "expected": {
            "complexity": "O(1)",
            "loops": 0,
            "decisions": 1,
            "cyclomatic_complexity": 2,
            "recursion": 0,
        }
    }
}

def run_analyzer(filename):
    """Run analyzer on a file and return results"""
    try:
        result = subprocess.run(
            ["./analyzer.exe", filename],
            capture_output=True,
            text=True,
            timeout=10
        )
        
        with open("result.json", "r") as f:
            data = json.load(f)
        return data
    except Exception as e:
        print(f"Error analyzing {filename}: {e}")
        return None

def validate_results():
    """Run all tests and validate against expected outputs"""
    print("=" * 70)
    print("COMPLEXITY ANALYZER - VALIDATION TEST SUITE")
    print("=" * 70)
    print()
    
    passed = 0
    failed = 0
    total = len(TEST_CASES)
    
    for filename, test_info in TEST_CASES.items():
        print(f"Testing: {test_info['name']} ({filename})")
        print("-" * 70)
        
        result = run_analyzer(filename)
        if not result:
            print("❌ FAILED - Could not analyze file")
            print()
            failed += 1
            continue
        
        expected = test_info['expected']
        all_match = True
        
        # Check each metric
        checks = [
            ("Complexity", "complexity", result.get("complexity"), expected.get("complexity")),
            ("Loops", "loops", result.get("loops"), expected.get("loops")),
            ("Decisions", "decisions", result.get("decisions"), expected.get("decisions")),
            ("CC", "cyclomatic_complexity", result.get("cyclomatic_complexity"), expected.get("cyclomatic_complexity")),
            ("Recursion", "recursion", result.get("recursion"), expected.get("recursion")),
        ]
        
        for label, key, actual, expected_val in checks:
            match = "✅" if actual == expected_val else "❌"
            if actual != expected_val:
                all_match = False
            print(f"  {match} {label:12} | Got: {actual:12} | Expected: {expected_val}")
        
        if all_match:
            passed += 1
            print("✅ PASSED")
        else:
            failed += 1
            print("❌ FAILED")
        print()
    
    # Summary
    print("=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"Total Tests: {total}")
    print(f"Passed: ✅ {passed}")
    print(f"Failed: ❌ {failed}")
    print(f"Success Rate: {(passed/total)*100:.1f}%")
    print()
    
    if failed == 0:
        print("🎉 ALL TESTS PASSED! Analyzer is working correctly.")
        return 0
    else:
        print(f"⚠️  {failed} test(s) failed. Review the output above.")
        return 1

if __name__ == "__main__":
    sys.exit(validate_results())
