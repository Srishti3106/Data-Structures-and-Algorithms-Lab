#include <stdio.h>
#include <stdlib.h>

// Helper function to find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to solve the 0/1 Knapsack problem using Dynamic Programming
int knapsackDP(int W, int weights[], int values[], int n) {
   
    int V[n + 1][W + 1]; 

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            // Base case: If no items (i=0) or no capacity (w=0), value is 0
            if (i == 0 || w == 0) {
                V[i][w] = 0;
            } 
            // Case 1: If weight of current item (weights[i-1]) is less than 
           
            else if (weights[i - 1] <= w) {
               
                V[i][w] = max(
                    V[i - 1][w], 
                    values[i - 1] + V[i - 1][w - weights[i - 1]]
                );
            } 
            // Case 2: If weight of current item is greater than 'w', 
            else {
                V[i][w] = V[i - 1][w];
            }
        }
    }

    // The maximum value
    return V[n][W];
}

// Struct to store item data for the Greedy approach
typedef struct {
    int weight;
    int value;
    float ratio; 
} Item;

// Comparison function for qsort: sorts items by ratio in descending order
int compareItems(const void *a, const void *b) {
    float ratioA = ((Item *)a)->ratio;
    float ratioB = ((Item *)b)->ratio;

    if (ratioA < ratioB) return 1;
    if (ratioA > ratioB) return -1;
    return 0;
}

// Function to solve the Knapsack problem using the Greedy approach
int knapsackGreedy(int W, int weights[], int values[], int n) {
    // 1. Create an array of Item structs
    Item items[n];
    for (int i = 0; i < n; i++) {
        items[i].weight = weights[i];
        items[i].value = values[i];
        items[i].ratio = (float)values[i] / weights[i];
    }

    // 2. Sort items by value-to-weight ratio in descending order
    qsort(items, n, sizeof(Item), compareItems);

    int currentWeight = 0;
    int totalValue = 0;

    // 3. Iterate through sorted items and take them if they fit
    for (int i = 0; i < n; i++) {
        if (currentWeight + items[i].weight <= W) {
            // Take the whole item
            currentWeight += items[i].weight;
            totalValue += items[i].value;
        }
    } 
    return totalValue; 
}

int main() {
    int n, W;

    // --- User Input ---
    printf("--- 0/1 Knapsack Problem Solver ---\n");
    printf("Enter the number of items (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of items.\n");
        return 1;
    }

    printf("Enter the knapsack capacity (W): ");
    if (scanf("%d", &W) != 1 || W <= 0) {
        printf("Invalid capacity.\n");
        return 1;
    }

    // Dynamically allocate arrays for weights and values
    int *weights = (int *)malloc(n * sizeof(int));
    int *values = (int *)malloc(n * sizeof(int));

    if (weights == NULL || values == NULL) {
        printf("Memory allocation failed.\n");
        free(weights);
        free(values);
        return 1;
    }

    printf("\nEnter item weights and values:\n");
    for (int i = 0; i < n; i++) {
        printf("Item %d: Weight (w[%d]): ", i + 1, i + 1);
        scanf("%d", &weights[i]);
        printf("Item %d: Value (v[%d]): ", i + 1, i + 1);
        scanf("%d", &values[i]);
    }

    // --- Dynamic Programming Solution ---
    printf("\n==================================\n");
    printf("Dynamic Programming Approach\n");
    int dp_result = knapsackDP(W, weights, values, n);
    printf("Maximum Value (Optimal Solution): %d\n", dp_result);
    printf("==================================\n");

    // --- Greedy Solution ---
    printf("\n==================================\n");
    printf("Greedy Approach (based on Value/Weight Ratio)\n");
    int greedy_result = knapsackGreedy(W, weights, values, n);
    printf("Maximum Value (Greedy Solution): %d\n", greedy_result);
    printf("==================================\n");

    // --- Performance Analysis ---
    printf("\nPerformance Analysis:\n");
    printf("The Dynamic Programming solution provides the GUARANTEED OPTIMAL result (%d).\n", dp_result);
    printf("The Greedy solution is faster (O(n log n)) but may be sub-optimal (%d).\n", greedy_result);
    if (dp_result != greedy_result) {
        printf("In this instance, the Greedy approach DID NOT find the optimal solution.\n");
    } else {
        printf("In this instance, the Greedy approach happened to find the optimal solution.\n");
    }

    // Clean up allocated memory
    free(weights);
    free(values);

    return 0;
}