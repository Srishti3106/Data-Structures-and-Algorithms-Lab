#include <stdio.h>
#include <stdlib.h>

int is_power_of_two(int n) {
    // This ensures the matrix can be evenly divided in half repeatedly.
    return (n > 0) && ((n & (n - 1)) == 0);
}

// Function to add two square matrices (C = A + B)
void add(int n, int a[n][n], int b[n][n], int result[n][n]) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            result[i][j] = a[i][j] + b[i][j];
}

// --- Recursive Function (SMMR: Square Matrix Multiply Recursive) ---

/* * Implements the Divide and Conquer algorithm :
 * T(n) = 8T(n/2) + O(n^2)
 */
void SMMR_Multiply(int n, int A[n][n], int B[n][n], int C[n][n]) {
    
    // 1. Base Case: If the matrix size is 1x1
    if (n == 1) {
        
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    // Determine the size of the sub-matrices (k = n/2)
    int k = n / 2; 

    // 2. Declare sub-matrices and temporary storage
    // These are created for partitioning A and B:
    int A11[k][k], A12[k][k], A21[k][k], A22[k][k];
    int B11[k][k], B12[k][k], B21[k][k], B22[k][k];
    
    // These store the final quadrants of C:
    int C11[k][k], C12[k][k], C21[k][k], C22[k][k];
    
    
    int temp1[k][k], temp2[k][k];

    // 3. Partition matrices A and B into four k x k sub-matrices
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++) {
            // Partition A
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            // Partition B
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // 4. Recursive Multiplications (8 calls) and Additions (4 calls)
    // This implements the four equations C11, C12, C21, C22 exactly.

    // C11 = A11*B11 + A12*B21
    SMMR_Multiply(k, A11, B11, temp1);
    SMMR_Multiply(k, A12, B21, temp2); 
    add(k, temp1, temp2, C11);    

    // C12 = A11*B12 + A12*B22
    SMMR_Multiply(k, A11, B12, temp1); 
    SMMR_Multiply(k, A12, B22, temp2); 
    add(k, temp1, temp2, C12);    

    // C21 = A21*B11 + A22*B21
    SMMR_Multiply(k, A21, B11, temp1); 
    SMMR_Multiply(k, A22, B21, temp2); 
    add(k, temp1, temp2, C21);    

    // C22 = A21*B12 + A22*B22
    SMMR_Multiply(k, A21, B12, temp1); 
    SMMR_Multiply(k, A22, B22, temp2); 
    add(k, temp1, temp2, C22);    

    // 5. Combining the quadrants (C11, C12, C21, C22) into the result matrix C
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++) {
            C[i][j] = C11[i][j];         
            C[i][j + k] = C12[i][j];     
            C[i + k][j] = C21[i][j];     
            C[i + k][j + k] = C22[i][j]; 
        }
    }
}

int main() {
    int n;
    
    printf("--- Divide and Conquer Matrix Multiplication ---\n");
    printf("Enter size of square matrix N (must be a power of 2): ");
    
    // Input size validation
    if (scanf("%d", &n) != 1 || n <= 0 || !is_power_of_two(n)) {
        printf("Error: N must be a positive integer and a power of 2 (e.g., 1, 2, 4, 8) for this algorithm.\n");
        return 1;
    }

    // Declare matrices (A, B, C) 
    int A[n][n], B[n][n], C[n][n];

    // Input data for Matrix A
    printf("\nEnter elements of Matrix A:\n");
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    // Input data for Matrix B
    printf("\nEnter elements of Matrix B:\n");
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%d", &B[i][j]);
            
    // Perform multiplication
    SMMR_Multiply(n, A, B, C);

    // Print the final result
    printf("\n--- Result Matrix C (A * B) ---\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%5d", C[i][j]); 
        }
        printf("\n");
    }

    return 0;
}
