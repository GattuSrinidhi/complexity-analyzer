# Complexity Analyzer - Validation Test Suite

## Test Cases & Expected Outputs

### 1. Binary Search - O(log n)
**Expected:**
- Time Complexity: O(log n)
- Loops: 1
- Decisions: 3 (two if statements + embedded else)
- CC: 4
- Recursion: No

**Test Code:**
```c
int binarySearch(int arr[], int low, int high, int x) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == x) return mid;
        if (arr[mid] < x) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}
```

---

### 2. Bubble Sort - O(n²)
**Expected:**
- Time Complexity: O(n²)
- Loops: 2
- Decisions: 1 (only one if)
- CC: 4
- Recursion: No

**Test Code:**
```c
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
```

---

### 3. Merge Sort - O(n log n)
**Expected:**
- Time Complexity: O(n log n)
- Functions: 2
- Loops: 3 (three while loops in merge)
- Decisions: 2
- CC: 6
- Recursion: Yes

**Test Code:**
```c
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int left[100], right[100];
    
    int i = 0;
    while (i < n1) {
        left[i] = arr[l + i];
        i++;
    }
    
    int j = 0;
    while (j < n2) {
        right[j] = arr[m + 1 + j];
        j++;
    }
    
    int k = l;
    i = 0;
    j = 0;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
```

---

### 4. Factorial (Recursion) - O(2^n)
**Expected:**
- Time Complexity: O(2^n)
- Loops: 0
- Decisions: 1
- CC: 2
- Recursion: Yes

**Test Code:**
```c
int fibonacci(int n) {
    if (n <= 1)
        return n;
    return fibonacci(n-1) + fibonacci(n-2);
}
```

---

### 5. Linear Search - O(n)
**Expected:**
- Time Complexity: O(n)
- Loops: 1
- Decisions: 1 (one if)
- CC: 3
- Recursion: No

**Test Code:**
```c
int linearSearch(int arr[], int n, int x) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == x)
            return i;
    }
    return -1;
}
```

---

### 6. If-Else Statement - O(1)
**Expected:**
- Time Complexity: O(1)
- Loops: 0
- Decisions: 1 (one if, else not counted)
- CC: 2
- Recursion: No

**Test Code:**
```c
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}
```

---

### 7. Nested Loops - O(n²)
**Expected:**
- Time Complexity: O(n²)
- Loops: 2
- Decisions: 1 (one if)
- CC: 4
- Recursion: No

**Test Code:**
```c
int main(){
    int n = 10;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j && j > 0) {
                printf("Hello");
            }
        }
    }
    return 0;
}
```

---

## Validation Checklist

✅ **Time Complexity Detection:**
- [ ] O(1) - Constant time
- [ ] O(n) - Linear
- [ ] O(n²) - Quadratic
- [ ] O(log n) - Binary search pattern
- [ ] O(n log n) - Merge/divide-and-conquer
- [ ] O(2^n) - Pure recursion

✅ **Loop Counting:**
- [ ] Counts `for` loops with spaces: `for (` and `for(`
- [ ] Counts `while` loops with spaces: `while (` and `while(`
- [ ] Counts `do-while` loops
- [ ] Correctly tracks nested loops

✅ **Decision Statements:**
- [ ] Counts `if` statements
- [ ] Counts `else if` statements
- [ ] Counts standalone `else` statements
- [ ] Counts `switch` cases
- [ ] Counts ternary operators

✅ **Other Metrics:**
- [ ] Cyclomatic Complexity calculated correctly
- [ ] Loop nesting depth tracked
- [ ] Function detection working
- [ ] Recursion detection working

---

## How to Test

1. **Open dashboard:** http://127.0.0.1:5000
2. **Paste each test code** from above
3. **Compare results** with expected values
4. **Mark ✓ or ✗** in the checklist
5. **Record any mismatches**

---

## Known Limitations

- Only detects recursion when function calls itself (direct recursion)
- Loop patterns rely on keyword matching (may miss unconventional patterns)
- Assumes standard C syntax
- Comments are removed before analysis
- Does not do full AST parsing (regex-based analysis)

