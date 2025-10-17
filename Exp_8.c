#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

// Define the maximum size for our alphabet (256 standard ASCII characters)
#define MAX_ALPHABET_SIZE 256
// Define the maximum length for the input string.
#define MAX_INPUT_LENGTH 1000

// --- 1. Define the Node Structure --
typedef struct Node {
    char data;          
    int freq;           
    struct Node *left; 
    struct Node *right;
} Node;

char codeTable[MAX_ALPHABET_SIZE][MAX_ALPHABET_SIZE];

// --- 2. Node Creation ---
Node* createNode(char data, int freq) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    // Initialize the new node with the provided data and frequency.
    newNode->data = data;
    newNode->freq = freq;
    newNode->left = NULL; // Initialize pointers to NULL (no children yet)
    newNode->right = NULL;
    return newNode;
}

// --- 3. Sorting Function (Simulates Min-Heap Extraction) ---
// Used by qsort to sort nodes by frequency in ASCENDING order.
int compareNodes(const void *a, const void *b) {
    // (pointers to Node pointers).
    const Node* nodeA = *(const Node**)a;
    const Node* nodeB = *(const Node**)b;
    
    // Returns a negative number if nodeA is smaller, which sorts in ASCENDING order.
    return (nodeA->freq - nodeB->freq);
}

// --- 4. The Huffman Algorithm (Greedy Implementation) ---
Node* buildHuffmanTree(Node** nodes, int n) {
    int currentSize = n;

    // The loop runs n-1 times to combine all 'n' initial nodes into one tree root.
    for (int i = 0; i < n - 1; i++) {
        
        // 1. Sort (Simulate Extract-Min)
        qsort(nodes, currentSize, sizeof(Node*), compareNodes);

        // 2. Extract-Min: Pick the two smallest frequency nodes
        Node* x = nodes[0]; // Smallest 
        Node* y = nodes[1]; // Second smallest

        // 3. Create new internal node (z)
        // Its frequency is the sum of its children's frequencies.
        Node* z = createNode('$', x->freq + y->freq); 
        
        // Connect the two smallest nodes as its children
        z->left = x;
        z->right = y;

        // 4. Insert: Replace the two extracted nodes with the new parent node (z)
        nodes[1] = z;

        nodes[0] = nodes[currentSize - 1]; 
        currentSize--; 
    }

    // The final node remaining is the Root of the entire tree.
    return nodes[0];
}

// --- 5. Generate and Store Huffman Codes ---
// Uses recursion to traverse the tree and record the binary code for each character.
void generateCodes(Node* root, char code[], int top) {
    // If we move left, add '0' to the code.
    if (root->left) {
        code[top] = '0';
        generateCodes(root->left, code, top + 1);
    }

    // If we move right, add '1' to the code.
    if (root->right) {
        code[top] = '1';
        generateCodes(root->right, code, top + 1);
    }

    // If it's a leaf node, we found a character and its full code.
    if (!root->left && !root->right) {
        code[top] = '\0'; // Null-terminate the string

        // Store the code in the global table, indexed by the ASCII value of the character.
        strcpy(codeTable[(unsigned char)root->data], code); 
        
        printf("  '%c' (Freq: %d): %s\n", root->data, root->freq, code);
    }
}

// --- 6. Helper function for recursive tree cleanup (Good Practice) ---
void freeHuffmanTree(Node* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}


// --- 7. Main Program Execution ---
int main() {
    char inputString[MAX_INPUT_LENGTH];
    int charCounts[MAX_ALPHABET_SIZE] = {0}; // Initialize all counts to 0
    int distinctChars = 0;
    
    // Get the input string from the user
    printf("Enter the text to encode (max %d characters):\n", MAX_INPUT_LENGTH - 1);
    if (fgets(inputString, MAX_INPUT_LENGTH, stdin) == NULL) {
        printf("Error reading input.\n");
        return EXIT_FAILURE;
    }
    
    
    size_t len = strlen(inputString);
    if (len > 0 && inputString[len - 1] == '\n') {
        inputString[len - 1] = '\0';
        len--;
    }
    
    // --- Step 1: Calculate Frequencies ---
    printf("\n--- i. Character Frequencies ---\n");
    for (int i = 0; inputString[i] != '\0'; i++) {
        unsigned char c = inputString[i];
        if (charCounts[c] == 0) {
            distinctChars++;
        }
        charCounts[c]++;
    }
    
    if (distinctChars < 2) {
        printf("Error: Need at least two distinct characters for encoding.\n");
        return EXIT_FAILURE;
    }

    // --- Step 2: Build Initial Nodes ---
    Node** nodes = (Node**)malloc(distinctChars * sizeof(Node*));
    if (nodes == NULL) {
        perror("Initial memory allocation failed");
        return EXIT_FAILURE;
    }


    int nodeIndex = 0;
    for (int i = 0; i < MAX_ALPHABET_SIZE; i++) {
        if (charCounts[i] > 0) {
            
            printf("  '%c': %d\n", (char)i, charCounts[i]);
            nodes[nodeIndex++] = createNode((char)i, charCounts[i]);
        }
    }

    // --- Step 3: Build the Huffman Tree (Greedy) ---
    Node* root = buildHuffmanTree(nodes, distinctChars);

    // --- Step 4: Generate Huffman Codes ---
    printf("\n--- ii. Corresponding Huffman Codes ---\n");
    char codeBuffer[MAX_ALPHABET_SIZE]; 
    generateCodes(root, codeBuffer, 0);

    // --- Step 5: Encoded Binary String  ---
    printf("\n--- iii. Encoded Binary String ---\n");
    printf("Original Text: %s\n", inputString);
    printf("Encoded String: ");

    int totalBits = 0;
    for (int i = 0; inputString[i] != '\0'; i++) {
        unsigned char c = inputString[i];
        char* code = codeTable[c];
        printf("%s", code);
        totalBits += (int)strlen(code);
    }
    
    printf("\nTotal Bits: %d\n", totalBits);

    // --- Step 6: Decoded Text ---
    printf("\n--- iv. Decoded (Original) Text ---\n");
    printf("Decoded Text: ");

    Node* current = root;
    // We traverse the tree based on the generated codes to simulate decoding.
    for (int i = 0; inputString[i] != '\0'; i++) {
        unsigned char c = inputString[i];
        char* code = codeTable[c];
        
        for (int j = 0; code[j] != '\0'; j++) {
            if (code[j] == '0') {
                current = current->left;
            } else { 
                current = current->right;
            }
            
            // If we hit a leaf, print the character and restart from the root.
            if (current->left == NULL && current->right == NULL) {
                printf("%c", current->data);
                current = root;
            }
        }
    }
    printf("\n-----------------------------------\n");

    // --- Final Cleanup ---
    freeHuffmanTree(root);
    free(nodes);
    
    return 0;
}
