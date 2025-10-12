#include <stdio.h>
#include <stdlib.h> 

// --- 1. Define the Item Structure ---
typedef struct {
    float profit;
    float weight;
    float ratio; // Profit-to-Weight ratio (p/w)
} Item;

// --- 2. Comparison Function for Sorting  ---
int compareItems(const void *a, const void *b) {
    // Cast the void pointers back to Item pointers
    const Item *itemA = (const Item *)a;
    const Item *itemB = (const Item *)b;

    // The core of the greedy algorithm: check the ratio.
    if (itemA->ratio < itemB->ratio) {
        return 1; 
    } else if (itemA->ratio > itemB->ratio) {
        return -1; 
    } else {
        return 0; 
    }
}


// --- 3. The Main Fractional Knapsack Logic ---
float fractionalKnapsack(float W, Item items[], int n) {
    
    float maxProfit = 0.0;
    
    float currentCapacity = W;

    
    for (int i = 0; i < n; i++) {
    
        if (items[i].weight > 0) {
            items[i].ratio = items[i].profit / items[i].weight;
        } else {
            items[i].ratio = 0.0; 
        }
    }

    
    qsort(items, n, sizeof(Item), compareItems);

    printf("\n--- Selection Order (Highest Ratio First) ---\n");
    for(int i = 0; i < n; i++) {
        printf("Item %d: P=%.2f, W=%.2f, Ratio=%.2f\n", i + 1, items[i].profit, items[i].weight, items[i].ratio);
    }
    printf("--------------------------------------------\n");


    
    for (int i = 0; i < n; i++) {
        
        if (currentCapacity <= 0) {
            break;
        }

        
        if (items[i].weight <= currentCapacity) {
            
            currentCapacity -= items[i].weight;
            maxProfit += items[i].profit;
            printf("TAKE WHOLE Item %d (P: %.2f, W: %.2f). Remaining Capacity: %.2f\n",
                   i + 1, items[i].profit, items[i].weight, currentCapacity);
        }
        
        else {
            
            float fraction = currentCapacity / items[i].weight;
            float profitToAdd = items[i].profit * fraction;

            maxProfit += profitToAdd;
            currentCapacity = 0; 

            printf("TAKE FRACTIONAL Item %d (Fraction: %.2f, Profit Added: %.2f). Knapsack is full.\n",
                   i + 1, fraction, profitToAdd);
            
            
            break;
        }
    }

    return maxProfit;
}

// --- 4. Main Function  ---
int main() {
    int n = 0; 
    float capacity = 0.0; 
    Item *items = NULL;
    float result = 0.0;

    // 1. Get Knapsack Capacity
    printf("Enter the total Knapsack Capacity (W): ");
    if (scanf("%f", &capacity) != 1 || capacity < 0) {
        printf("Invalid capacity input. Exiting.\n");
        return 1;
    }

    // 2. Get Number of Items
    printf("Enter the number of items (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of items. Exiting.\n");
        return 1;
    }
    
    // 3. Dynamically Allocate Memory for Items
    items = (Item *)malloc(n * sizeof(Item));
    if (items == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return 1;
    }

    // 4. Get Profit and Weight for Each Item
    printf("\n--- Enter Item Details ---\n");
    for (int i = 0; i < n; i++) {
        printf("Item %d:\n", i + 1);
        printf("  Enter Profit (P): ");
        if (scanf("%f", &items[i].profit) != 1) {
             printf("Invalid profit input. Exiting.\n");
             free(items);
             return 1;
        }
        
        printf("  Enter Weight (W): ");
        if (scanf("%f", &items[i].weight) != 1) {
             printf("Invalid weight input. Exiting.\n");
             free(items);
             return 1;
        }
        // Initialize ratio to 0.0
        items[i].ratio = 0.0;
    }
    printf("--------------------------\n");

    // Display collected inputs
    printf("\nKnapsack Capacity (W): %.2f\n", capacity);
    printf("Items to consider:\n");
    for(int i = 0; i < n; i++) {
        printf("- Item %d: Profit=%.2f, Weight=%.2f\n", i + 1, items[i].profit, items[i].weight);
    }
    
    // 5. Call the function to find the maximum profit
    result = fractionalKnapsack(capacity, items, n);

    // 6. Display the final result
    printf("\n\n*** Maximum Profit Achieved: %.2f ***\n", result);

    // 7. Free the allocated memory
    free(items);

    return 0;
}
