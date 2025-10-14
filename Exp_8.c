#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- 1. Define the Node Structure ---
typedef struct Node {
    char data;          // The character itself
    int freq;           // The frequency  of the character
    struct Node *left;  // Pointer to the left child node (0)
    struct Node *right; // Pointer to the right child node(1)
} Node;

// --- 2. Function to Create a New Node ---
Node* createNode(char data, int freq) {
    // Allocate memory for the new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        // error check for memory allocation failure
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the node properties
    newNode->data = data;
    newNode->freq = freq;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// --- 3. Sorting Function (Simulates Min-Heap Extraction)----
int compareNodes(const void *a, const void *b) {
    const Node* nodeA = *(const Node**)a;
    const Node* nodeB = *(const Node**)b;
    
    // Sort in ascending order . Smallest frequency comes first.
    return (nodeA->freq - nodeB->freq);
}

// --- 4. The Huffman Algorithm (Greedy Implementation) ---
Node* buildHuffmanTree(Node** nodes, int n) {
    // We will use 'nodes' as our simulated Min Heap Q.
    int currentSize = n;

    
    for (int i = 0; i < n - 1; i++) {
        
        // 1. Sort the array (Simulate Heap-Sort)
        
        qsort(nodes, currentSize, sizeof(Node*), compareNodes);

        // 2. Extract-Min 
        Node* x = nodes[0];
        Node* y = nodes[1]; 

        // 3. Create a new internal node (z)
        Node* z = createNode('$', x->freq + y->freq); 
        
        z->left = x;
        z->right = y;

        nodes[1] = z;

        nodes[0] = nodes[currentSize - 1]; 
        currentSize--;
    }

    
    return nodes[0];
}

// --- 5. function to print Huffman Code (The Output)----
void printCodes(Node* root, char code[], int top) {
    if (root->left) {
        code[top] = '0';
        printCodes(root->left, code, top + 1);
    }

    if (root->right) {
        code[top] = '1';
        printCodes(root->right, code, top + 1);
    }

    if (!root->left && !root->right) {
        code[top] = '\0'; 
        printf("  '%c' (Freq: %d): %s\n", root->data, root->freq, code);
    }
}

// --- 6. Main Program Execution ---
int main() {
    
    char chars[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int freqs[] = {5, 9, 12, 13, 16, 45};
    int n = sizeof(chars) / sizeof(chars[0]); // Calculates the number of items (6)


    Node** nodes = (Node**)malloc(n * sizeof(Node*));
    if (nodes == NULL) {
        perror("Initial memory allocation failed");
        return EXIT_FAILURE;
    }

    // Create the initial leaf nodes from the input data.
    printf("--- Input Data (Initial Set C) ---\n");
    for (int i = 0; i < n; i++) {
        nodes[i] = createNode(chars[i], freqs[i]);
        printf("  Char: '%c', Frequency: %d\n", chars[i], freqs[i]);
    }
    printf("----------------------------------\n");
    
   
    Node* root = buildHuffmanTree(nodes, n);

    printf("\n--- Huffman Codes (The Output) ---\n");
    
    char code[100]; 
    printCodes(root, code, 0);
    printf("----------------------------------\n");

    
    free(nodes);
    
    
    
    return 0;
}


        