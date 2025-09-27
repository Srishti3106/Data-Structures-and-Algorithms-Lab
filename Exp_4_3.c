#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>



// Checks if n is a power of 2 (required for Strassen's method)
int is_power_of_two(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

// Function to fill a matrix with random values (0-9)
void fill_random(int n, int (*matrix)[n], unsigned int seed_val) {
    //  random number generator only once
    srand(seed_val); 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Use small values (0-9) 
            matrix[i][j] = rand() % 10; 
        }
    }
}

// Function to add two square matrices (C = A + B)
void add(int n, int (*a)[n], int (*b)[n], int (*result)[n]) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            result[i][j] = a[i][j] + b[i][j];
}

// Function to subtract two square matrices (C = A - B)
void subtract(int n, int (*a)[n], int (*b)[n], int (*result)[n]) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            result[i][j] = a[i][j] - b[i][j];
}


// --- Algorithm 1: Traditional Cubic Time ) ---

void traditional_multiply(int n, int (*A)[n], int (*B)[n], int (*C)[n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0; 
            for (int k = 0; k < n; k++) {
                C[i][j] += (A[i][k] * B[k][j]);
            }
        }
    }
}

// --- Algorithm 2: Strassen's Algorithm ) ---

void Strassen_Multiply(int n, int (*A)[n], int (*B)[n], int (*C)[n]) {
    
    // Base Case
    if (n <= 64) {
        traditional_multiply(n, A, B, C);
        return;
    }

    int k = n / 2; 

    
    #define ALLOCATE_MATRIX(name) int (*name)[k] = malloc(sizeof(int[k][k])); 
    
    // Allocate 21 temporary matrices: 
    ALLOCATE_MATRIX(A11); ALLOCATE_MATRIX(A12); ALLOCATE_MATRIX(A21); ALLOCATE_MATRIX(A22);
    ALLOCATE_MATRIX(B11); ALLOCATE_MATRIX(B12); ALLOCATE_MATRIX(B21); ALLOCATE_MATRIX(B22);
    ALLOCATE_MATRIX(P1); ALLOCATE_MATRIX(P2); ALLOCATE_MATRIX(P3); ALLOCATE_MATRIX(P4); 
    ALLOCATE_MATRIX(P5); ALLOCATE_MATRIX(P6); ALLOCATE_MATRIX(P7);
    ALLOCATE_MATRIX(T1); ALLOCATE_MATRIX(T2); ALLOCATE_MATRIX(T3); // Used for P calculations
    ALLOCATE_MATRIX(T4); ALLOCATE_MATRIX(T5); ALLOCATE_MATRIX(T6); // Used for C combinations (NEW!)

    // 1. Partition matrices A and B
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];     B11[i][j] = B[i][j];
            A12[i][j] = A[i][j + k]; B12[i][j] = B[i][j + k];
            A21[i][j] = A[i + k][j]; B21[i][j] = B[i + k][j];
            A22[i][j] = A[i + k][j + k]; B22[i][j] = B[i + k][j + k];
        }
    }

    // 2. Calculate the 7 products (P1 through P7) via 7 recursive multiplications
    
    // P1 = A11 * (B12 - B22)
    subtract(k, B12, B22, T1); 
    Strassen_Multiply(k, A11, T1, P1); 

    // P2 = (A11 + A12) * B22
    add(k, A11, A12, T1); 
    Strassen_Multiply(k, T1, B22, P2); 

    // P3 = (A21 + A22) * B11
    add(k, A21, A22, T1); 
    Strassen_Multiply(k, T1, B11, P3); 

    // P4 = A22 * (B21 - B11)
    subtract(k, B21, B11, T1); 
    Strassen_Multiply(k, A22, T1, P4); 

    // P5 = (A11 + A22) * (B11 + B22)
    add(k, A11, A22, T1); 
    add(k, B11, B22, T2); 
    Strassen_Multiply(k, T1, T2, P5); 

    // P6 = (A12 - A22) * (B21 + B22)
    subtract(k, A12, A22, T1); 
    add(k, B21, B22, T2); 
    Strassen_Multiply(k, T1, T2, P6); 

    // P7 = (A11 - A21) * (B11 + B12)
    subtract(k, A11, A21, T1); 
    add(k, B11, B12, T2); 
    Strassen_Multiply(k, T1, T2, P7); 


    // 3. Combine products to get the four C quadrants (Using T4, T5, T6 as dedicated temps)
    
    // C11 = P5 + P4 - P2 + P6  
    add(k, P5, P4, T4);     
    subtract(k, T4, P2, T5);   
    add(k, T5, P6, T6);    
    
    // C12 = P1 + P2
    add(k, P1, P2, T4);        
    
    // C21 = P3 + P4
    add(k, P3, P4, T5);      

    // C22 = P5 + P1 - P3 - P7
    add(k, P5, P1, T3);         
    subtract(k, T3, P3, T2);   
    subtract(k, T2, P7, T1);  

    
    // 4. Combining the quadrants into C
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++) {
            C[i][j] = T6[i][j];          
            C[i][j + k] = T4[i][j];    
            C[i + k][j] = T5[i][j];      
            C[i + k][j + k] = T1[i][j];  
        }
    }

    // Cleanup memory
    #define FREE_MATRIX(name) free(name);
    FREE_MATRIX(A11); FREE_MATRIX(A12); FREE_MATRIX(A21); FREE_MATRIX(A22);
    FREE_MATRIX(B11); FREE_MATRIX(B12); FREE_MATRIX(B21); FREE_MATRIX(B22);
    FREE_MATRIX(P1); FREE_MATRIX(P2); FREE_MATRIX(P3); FREE_MATRIX(P4); 
    FREE_MATRIX(P5); FREE_MATRIX(P6); FREE_MATRIX(P7);
    FREE_MATRIX(T1); FREE_MATRIX(T2); FREE_MATRIX(T3);
    FREE_MATRIX(T4); FREE_MATRIX(T5); FREE_MATRIX(T6);
}

// --- Main Program and Comparison Logic ---

int main() {
    int N;
    int i, j;
    
    printf("--- Strassen's Comparative Analysis ---\n");
    printf("Enter the size N (must be power of 2, e.g., 64, 128, 256, 512, 1024): ");
    
    if (scanf("%d", &N) != 1 || N <= 0 || !is_power_of_two(N)) {
        printf("Error: N must be a positive integer and a power of 2.\n");
        return 1;
    }

    // Allocate matrices on the heap for large N testing
    int (*A)[N] = malloc(sizeof(int[N][N]));
    int (*B)[N] = malloc(sizeof(int[N][N]));
    int (*C_trad)[N] = malloc(sizeof(int[N][N])); // Result for Traditional
    int (*C_strassen)[N] = malloc(sizeof(int[N][N]));  // Result for Strassen's

    if (!A || !B || !C_trad || !C_strassen) {
        printf("Error: Memory allocation failed for N=%d.\n", N);
        return 1;
    }
    
    // Automatic Random Input
    unsigned int fixed_seed = 123;
    fill_random(N, A, fixed_seed);
    fill_random(N, B, fixed_seed);
    printf("\nMatrices A and B automatically filled with random data (0-9).\n");

    // Calculate theoretical memory usage
    long long total_memory = (long long)N * N * 4 * 4; 

    // --- 1. Run Traditional Algorithm (O(n^3)) ---
    clock_t start_trad = clock();
    traditional_multiply(N, A, B, C_trad);
    clock_t end_trad = clock();
    double time_trad = ((double)(end_trad - start_trad)) / CLOCKS_PER_SEC * 1000.0;

    // --- 2. Run Strassen's Algorithm (7T(n/2)) ---
    clock_t start_strassen = clock();
    Strassen_Multiply(N, A, B, C_strassen);
    clock_t end_strassen = clock();
    double time_strassen = ((double)(end_strassen - start_strassen)) / CLOCKS_PER_SEC * 1000.0;
    
    // --- Output Comparison ---
    
    printf("\n======================================================\n");
    printf("         STRASSEN'S VS TRADITIONAL ANALYSIS (N=%d)      \n", N);
    printf("======================================================\n");
    printf("Theoretical Space Used (4 Matrices): %.2f MB\n", (double)total_memory / (1024 * 1024));
    printf("------------------------------------------------------\n");
    printf("| Algorithm | Time Complexity | Runtime (ms) |\n");
    printf("------------------------------------------------------\n");
    printf("| Traditional | O(n^3)          | %10.2f |\n", time_trad);
    printf("| Strassen's | O(n^2.807)      | %10.2f |\n", time_strassen);
    printf("------------------------------------------------------\n");
    
    // Verification 
    int mismatch = 0;
    for(i=0; i<N; i++) {
        for(j=0; j<N; j++) {
            if (C_trad[i][j] != C_strassen[i][j]) {
                mismatch = 1;
                break;
            }
        }
    }
    printf("Results Match: %s\n", mismatch ? "NO (ERROR IN ALGORITHM)" : "YES");
    printf("======================================================\n");


    // Clean up memory
    free(A); free(B); free(C_trad); free(C_strassen);
    return 0;
}
