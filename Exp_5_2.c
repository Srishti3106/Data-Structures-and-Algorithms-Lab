#include <stdio.h>
#include <stdlib.h>

/* 
 * M[i][j] = 1 means person 'i' knows person 'j'.
 */
int knows(int i, int j, int n, int M[n][n]) {
    return M[i][j];
}

// --- The O(n) Optimal Solution ---

int find_celebrity(int n, int M[n][n]) {
    // 1. Finding a Candidate (First O(n) Pass)
    // Initialize two pointers, i (start) and j (end), to eliminate non-celebrities quickly.
    int i = 0; 
    int j = n - 1;

    while (i < j) {
        
        // If 'i' knows 'j', then 'i' cannot be the celebrity (Celebrity knows NO ONE).
        if (knows(i, j, n, M)) {
            i++; 
        } 
        // If 'i' does NOT know 'j', then 'j' cannot be the celebrity (Everyone must know the Celebrity).
        else {
            j--; 
        }
    }

    // After the loop, the remaining index 'i' is the only possible candidate.
    int candidate = i;

    // 2. Verification (Second O(n) Pass)
    
    for (int k = 0; k < n; k++) {
        if (k != candidate) {
            
            // Condition 1: Candidate must know NO ONE 
            if (knows(candidate, k, n, M)) {
                return -1; // Fail: Candidate knows someone.
            }

            // Condition 2: Candidate must be known by EVERYONE
            if (!knows(k, candidate, n, M)) {
                return -1; // Fail: Candidate is not known by person 'k'.
            }
        }
    }

    // If both verification checks pass, the candidate is the celebrity.
    return candidate;
}

int main() {
    int n;
    
    printf("--- The Celebrity Problem (Optimal O(n) Solution) ---\n");
    printf("Enter the number of people in the party (N): ");
    
    // Read N from user
    if (scanf("%d", &n) != 1 || n <= 1) {
        printf("Error: N must be an integer greater than 1.\n");
        return 1;
    }

    // Declare the relationship matrix
    int M[n][n]; 

    printf("\n--- Entering Relationships ---\n");
    printf("Enter 1 if person i knows person j, or 0 otherwise.\n");
    
    // Input the N x N relationship matrix from the user
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                // A person does not know themselves, set M[i][i] = 0 automatically
                M[i][j] = 0;
                printf("M[%d][%d] (i knows i): 0 (Auto-set)\n", i, j);
            } else {
                printf("M[%d][%d] (P%d knows P%d): ", i, j, i, j);
                if (scanf("%d", &M[i][j]) != 1 || (M[i][j] != 0 && M[i][j] != 1)) {
                     printf("Invalid input. Must be 0 or 1. Exiting.\n");
                     return 1;
                }
            }
        }
    }

    // Find the celebrity
    int celebrity_index = find_celebrity(n, M);

    printf("\n-----------------------------------------------------\n");
    printf("FINAL RESULT\n");

    if (celebrity_index != -1) {
        printf("The Celebrity is Person P%d (Index %d).\n", celebrity_index, celebrity_index);
    } else {
        printf("No celebrity exists in this party.\n");
    }
    printf("Time Complexity: O(n)\n");
    printf("-----------------------------------------------------\n");

    return 0;
}
