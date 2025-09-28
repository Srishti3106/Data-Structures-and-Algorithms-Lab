#include <stdio.h>
#include <time.h>
#include <math.h> 


// ---  Naïve Algorithm  ---
/*
 * Time Complexity: O(n)
 */
long long power_naive(int a, int n) {
    // 1. Base case: a^0 = 1
    if (n == 0) {
        return 1;
    }

    // 2. Initialize result
    long long result = 1;
    
    // 3. Loop through n times, multiplying the result by the base 'a' each time.
    for (int i = 1; i <= n; i++) {
        result = result * a; // This is the core multiplication step
    }

    // 4. Return the final power
    return result;
}

// ---  Fast Exponentiation Algorithm ---
/*
 * Time Complexity: O(log n) 
 */
long long power_fast(int a, int n) {
    // 1. Base case: a^0 = 1
    if (n == 0) {
        return 1;
    }

    // 2. Recursive Step (Divide)
    long long half_power = power_fast(a, n / 2);

    // 3. Combine Step (Conquer)
    long long result = half_power * half_power;

    // 4. Handle Odd Exponents
    if (n % 2 != 0) {
        result = result * a;
    }

    return result;
}

int main() {
    int base, exponent;
    
    printf("--- Naive vs. Fast Exponentiation Comparison ---\n");
    printf("Enter the base (a): ");
    scanf("%d", &base);
    printf("Enter the non-negative exponent (n): ");
    scanf("%d", &exponent);

    if (exponent < 0) {
        printf("Error: Exponent must be a non-negative integer.\n");
        return 1;
    }

    long long result_naive, result_fast;
    clock_t start, end;
    double time_naive, time_fast;
    
    // --- Run Naïve Method ---
    start = clock();
    result_naive = power_naive(base, exponent);
    end = clock();
    time_naive = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

    // --- Run Fast Method ---
    start = clock();
    result_fast = power_fast(base, exponent);
    end = clock();
    time_fast = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    // --- Output Comparison ---
    
    printf("\n========================================================\n");
    printf("| Power Calculation (Base=%d, Exponent=%d) |\n", base, exponent);
    printf("========================================================\n");
    
    // Naive Output
    printf("1. Naive Method (O(n))\n");
    printf("   Result: %lld\n", result_naive);
    printf("   Runtime: %.6f milliseconds\n", time_naive);
    printf("--------------------------------------------------------\n");
    
    // Fast Output
    printf("2. Fast Method (O(log n))\n");
    printf("   Result: %lld\n", result_fast);
    printf("   Runtime: %.6f milliseconds\n", time_fast);
    printf("========================================================\n");

    // Theoretical Analysis Summary
    printf("\nAnalysis:\n");
    printf("The Naive method requires %d multiplications.\n", exponent);
    printf("The Fast method requires approximately %d multiplications (log2(n)).\n", (int)ceil(log2(exponent == 0 ? 1 : exponent)));
    
    return 0;
}
