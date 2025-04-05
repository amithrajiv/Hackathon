#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct Block {
    int index;
    char data[256];
    char timestamp[64];
    char prevHash[64];
    char hash[64];
    struct Block* next;
} Block;

Block* head = NULL;

void calculateHash(char* data, char* prevHash, char* result) {
    unsigned long hash = 5381;
    while (*data) hash = ((hash << 5) + hash) + *data++;
    while (*prevHash) hash = ((hash << 5) + hash) + *prevHash++;

    sprintf(result, "%lx", hash);
}

void getCurrentTimestamp(char* buffer) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", t);
}

Block* createBlock(char* data, Block* prev) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->index = (prev == NULL) ? 0 : prev->index + 1;
    strcpy(newBlock->data, data);
    getCurrentTimestamp(newBlock->timestamp);

    if (prev == NULL)
        strcpy(newBlock->prevHash, "0");
    else
        strcpy(newBlock->prevHash, prev->hash);

    calculateHash(newBlock->data, newBlock->prevHash, newBlock->hash);
    newBlock->next = NULL;
    return newBlock;
}

void addBlock(char* data) {
    if (head == NULL) {
        head = createBlock(data, NULL);
    } else {
        Block* temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = createBlock(data, temp);
    }
    printf("✅ Block added successfully.\n");
}

void displayChain() {
    Block* temp = head;
    printf("\n======= BLOCKCHAIN LEDGER =======\n");
    while (temp != NULL) {
        printf("Block #%d\n", temp->index);
        printf("Data      : %s\n", temp->data);
        printf("Timestamp : %s\n", temp->timestamp);
        printf("Prev Hash : %s\n", temp->prevHash);
        printf("Hash      : %s\n", temp->hash);
        printf("-------------------------------\n");
        temp = temp->next;
    }
}

void verifyChain() {
    Block* temp = head;
    int valid = 1;
    while (temp && temp->next) {
        char expectedHash[64];
        calculateHash(temp->next->data, temp->hash, expectedHash);
        if (strcmp(temp->next->hash, expectedHash) != 0) {
            valid = 0;
            break;
        }
        temp = temp->next;
    }
    if (valid)
        printf("\n✅ Blockchain is VALID.\n");
    else
        printf("\n❌ Blockchain has been TAMPERED!\n");
}


int main() {
    int choice;
    char data[256];

    printf("===== Decentralized Ledger (Blockchain in C) =====\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Record (e.g., NFT/Transaction)\n");
        printf("2. Display Blockchain\n");
        printf("3. Verify Blockchain Integrity\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();  

        switch (choice) {
            case 1:
                printf("Enter data to store: ");
                fgets(data, 256, stdin);
                data[strcspn(data, "\n")] = 0; 
                addBlock(data);
                break;
            case 2:
                displayChain();
                break;
            case 3:
                verifyChain();
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
}