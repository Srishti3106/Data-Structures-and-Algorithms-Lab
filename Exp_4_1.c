#include <stdio.h>


int main() {
    // 1. Get the size N from the user.
    int N; // N will be the size of our square matrices (NxN)
    int i, j, k; // Loop variables

    printf("--- Square Matrix Multiplication ---\n");
    printf("Enter the size N (e.g., 3 for a 3x3 matrix): ");
    
    // Read the size N from the user
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Error: Please enter a positive integer for the size N.\n");
        return 1; // Exit the program 
    }

    // 2. Declare the matrices using the size N provided by the user.
    
    int A[N][N]; // First matrix
    int B[N][N]; // Second matrix
    int C[N][N]; // Result matrix (C = A * B)
    

    // --- Input Step: Get values for Matrix A ---
    printf("\nEnter elements for Matrix A (%dx%d):\n", N, N);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("A[%d][%d]: ", i, j);
            scanf("%d", &A[i][j]);
        }
    }

    // --- Input Step: Get values for Matrix B ---
    printf("\nEnter elements for Matrix B (%dx%d):\n", N, N);
    
    for (i = 0; i < N; i++) { 
        for (j = 0; j < N; j++) {
            printf("B[%d][%d]: ", i, j);
            scanf("%d", &B[i][j]);
        }
    }


    // --- Code Algorithm: Square-Matrix-Multiply (A, B) ---
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            
            C[i][j] = 0; 
            
            for (k = 0; k < N; k++) {
                
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            }
        }
    }


    // --- Output Step: Print the Result Matrix C ---
    printf("\n--- Result Matrix C (A * B) ---\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            
            
            printf("%5d", C[i][j]); 
        }
        printf("\n"); 
    }

    return 0;
}
