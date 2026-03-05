#include <stdio.h>
#include <stdlib.h>

// Function 1: Helper to find maximum
int max(int a, int b) {
    if (a > b) return a; // [Decision 1]
    return b;
}

// Function 2: Recursive Solver
int solveKnapsack(int W, int wt[], int val[], int n) {
    if (n == 0 || W == 0) { // [Decision 2]
        return 0;
    }

    if (wt[n - 1] > W) { // [Decision 3]
        return solveKnapsack(W, wt, val, n - 1);
    }

    return max(val[n - 1] + solveKnapsack(W - wt[n - 1], wt, val, n - 1),
               solveKnapsack(W, wt, val, n - 1));
}

// Function 3: Input Validator (Updated with two checks)
void validateInput(int n, int W) {
    if (n < 0) exit(1);   // [Decision 4]
    if (W < 0) exit(1);   // [Decision 5] (New)
}

// Function 4: Logger
void startLog() {
    printf("Starting Recursive Branching...\n");
}

// Function 5: Results Formatter
void printResult(int res) {
    if (res <= 0) printf("No value.\n"); // [Decision 6]
    else printf("Optimal Value: %d\n", res);
}

// Function 6: Entry Point
int main() {
    int val[] = {60, 100, 120};
    int wt[] = {10, 20, 30};
    int W = 50, n = 3;

    startLog();
    validateInput(n, W);
    printResult(solveKnapsack(W, wt, val, n));
    return 0;
}
