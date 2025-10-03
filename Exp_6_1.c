#include <stdio.h>
#include <stdlib.h>


// --- 1. Define the Activity Structure ---

typedef struct {
    int start;
    int finish;
    int index; // Original index to identify the selected activity
} Activity;

// --- 2. Comparison Function for Sorting ---

int compareActivities(const void *a, const void *b) {
    Activity *activityA = (Activity *)a;
    Activity *activityB = (Activity *)b;
    // Sort in non-decreasing order of finish time
    return activityA->finish - activityB->finish;
}

// --- 3. The Greedy Algorithm Implementation ---

void activity_selector(Activity activities[], int n) {
    if (n <= 0) {
        printf("No activities to select.\n");
        return;
    }

    
    qsort(activities, n, sizeof(Activity), compareActivities);

    printf("\n--- Sorted Activities (by Finish Time) ---\n");
    for (int i = 0; i < n; i++) {
        printf("A%d: (Start: %d, Finish: %d)\n", activities[i].index, activities[i].start, activities[i].finish);
    }
    
    printf("\n--- Selected Activities (Greedy Schedule) ---\n");
    
    
    printf("Selected Activity A%d (Start: %d, Finish: %d)\n", 
           activities[0].index, activities[0].start, activities[0].finish);

    
    int last_finish_time = activities[0].finish;
    int selected_count = 1;

    
    for (int i = 1; i < n; i++) {
        
        
        if (activities[i].start >= last_finish_time) {
            
            
            printf("Selected Activity A%d (Start: %d, Finish: %d)\n", 
                   activities[i].index, activities[i].start, activities[i].finish);
            
            
            last_finish_time = activities[i].finish;
            selected_count++;
        }
    }
    
    printf("\n========================================================\n");
    printf("Maximum number of non-overlapping activities selected: %d\n", selected_count);
}

int main() {
    int n;
    
    printf("--- Activity Selection Problem using Greedy Algorithm ---\n");
    printf("Enter the number of activities (n): ");
    
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Error: Please enter a positive number of activities.\n");
        return 1;
    }

    // Allocate memory dynamically for 'n' activities
    Activity *activities = (Activity *)malloc(n * sizeof(Activity));
    if (activities == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    printf("\n--- Enter Start and Finish Times ---\n");
    for (int i = 0; i < n; i++) {
        activities[i].index = i + 1; // Assign A1, A2, A3, etc.
        
        printf("Activity %d (A%d):\n", i + 1, i + 1);
        printf("  Start time (s[%d]): ", i + 1);
        if (scanf("%d", &activities[i].start) != 1) {
            printf("Invalid input. Exiting.\n");
            free(activities);
            return 1;
        }

        printf("  Finish time (f[%d]): ", i + 1);
        if (scanf("%d", &activities[i].finish) != 1 || activities[i].finish < activities[i].start) {
             printf("Invalid input. Finish time must be valid and >= Start time. Exiting.\n");
             free(activities);
             return 1;
        }
    }

    activity_selector(activities, n);

    printf("Time Complexity Analysis: O(n log n) due to the sorting step.\n");
    printf("========================================================\n");

    // Free the allocated memory
    free(activities);
    
    return 0;
}
