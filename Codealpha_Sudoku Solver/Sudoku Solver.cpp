#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const int SIZE = 9;

// ─── Display the grid ────────────────────────────────────────────────────────
void printGrid(int grid[SIZE][SIZE]) {
    cout << "\n  +-------+-------+-------+\n";
    for (int row = 0; row < SIZE; row++) {
        cout << "  | ";
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0)
                cout << ". ";
            else
                cout << grid[row][col] << " ";
            if ((col + 1) % 3 == 0) cout << "| ";
        }
        cout << "\n";
        if ((row + 1) % 3 == 0)
            cout << "  +-------+-------+-------+\n";
    }
}

// ─── Constraint checks ───────────────────────────────────────────────────────
bool isInRow(int grid[SIZE][SIZE], int row, int num) {
    for (int col = 0; col < SIZE; col++)
        if (grid[row][col] == num) return true;
    return false;
}

bool isInCol(int grid[SIZE][SIZE], int col, int num) {
    for (int row = 0; row < SIZE; row++)
        if (grid[row][col] == num) return true;
    return false;
}

bool isInBox(int grid[SIZE][SIZE], int startRow, int startCol, int num) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[startRow + row][startCol + col] == num) return true;
    return false;
}

bool isSafe(int grid[SIZE][SIZE], int row, int col, int num) {
    return !isInRow(grid, row, num) &&
           !isInCol(grid, col, num) &&
           !isInBox(grid, row - row % 3, col - col % 3, num);
}

// ─── Backtracking solver ─────────────────────────────────────────────────────
bool solveSudoku(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {           // Found an empty cell
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(grid, row, col, num)) {
                        grid[row][col] = num;    // Place number
                        if (solveSudoku(grid))   // Recurse
                            return true;
                        grid[row][col] = 0;      // Backtrack
                    }
                }
                return false;  // No valid number found → trigger backtrack
            }
        }
    }
    return true;  // No empty cell left → solved!
}

// ─── Validate initial puzzle input ───────────────────────────────────────────
bool isValidPuzzle(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            int val = grid[row][col];
            if (val < 0 || val > 9) return false;
            if (val != 0) {
                // Temporarily remove to check against others
                grid[row][col] = 0;
                if (!isSafe(grid, row, col, val)) {
                    grid[row][col] = val;
                    return false;
                }
                grid[row][col] = val;
            }
        }
    }
    return true;
}

// ─── User input ──────────────────────────────────────────────────────────────
void inputPuzzle(int grid[SIZE][SIZE]) {
    cout << "\nEnter the Sudoku puzzle row by row.\n";
    cout << "Use 0 for empty cells. Separate values with spaces.\n\n";

    for (int row = 0; row < SIZE; row++) {
        bool valid = false;
        while (!valid) {
            cout << "Row " << (row + 1) << ": ";
            valid = true;
            for (int col = 0; col < SIZE; col++) {
                if (!(cin >> grid[row][col]) || grid[row][col] < 0 || grid[row][col] > 9) {
                    cout << "[ERROR] Invalid input. Enter 9 digits (0-9) separated by spaces.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    valid = false;
                    break;
                }
            }
        }
    }
}

// ─── Load a built-in sample puzzle ───────────────────────────────────────────
void loadSample(int grid[SIZE][SIZE]) {
    int sample[SIZE][SIZE] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            grid[r][c] = sample[r][c];
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
    int grid[SIZE][SIZE] = {};
    int choice;

    cout << "==========================================\n";
    cout << "           SUDOKU SOLVER (C++)            \n";
    cout << "==========================================\n";
    cout << "  1. Enter your own puzzle\n";
    cout << "  2. Use sample puzzle\n";
    cout << "==========================================\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1) {
        inputPuzzle(grid);
    } else {
        loadSample(grid);
        cout << "\n[INFO] Sample puzzle loaded.\n";
    }

    cout << "\nOriginal Puzzle:";
    printGrid(grid);

    if (!isValidPuzzle(grid)) {
        cout << "\n[ERROR] The puzzle contains conflicts and cannot be solved.\n";
        return 1;
    }

    cout << "\nSolving...\n";

    if (solveSudoku(grid)) {
        cout << "\n[SUCCESS] Puzzle Solved!\n";
        cout << "\nSolved Grid:";
        printGrid(grid);
    } else {
        cout << "\n[ERROR] No solution exists for the given puzzle.\n";
    }

    return 0;
}