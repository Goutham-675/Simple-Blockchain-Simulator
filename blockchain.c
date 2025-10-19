
/* main.c - Simple Blockchain Simulator in C
   Author: R. Goutham
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BLOCKS 20
#define DATA_LEN 128
#define HASH_LEN 64

typedef struct {
    int index;
    char timestamp[32];
    char data[DATA_LEN];
    char previous_hash[HASH_LEN];
    char current_hash[HASH_LEN];
    int nonce;
} Block;

Block blockchain[MAX_BLOCKS];
int block_count = 0;

// Simple hash function (for demo only)
void simple_hash(Block *b) {
    unsigned long hash = 0;
    for(int i=0; i<strlen(b->data); i++) hash += b->data[i]*31;
    hash += b->index*17 + b->nonce*13;
    snprintf(b->current_hash, HASH_LEN, "%lx", hash);
}

// Get current timestamp
void get_timestamp(char *buffer, int size) {
    time_t now = time(NULL);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

// Create a new block
void add_block(const char *data) {
    if(block_count >= MAX_BLOCKS) {
        printf("Blockchain full!\n");
        return;
    }
    Block *b = &blockchain[block_count];
    b->index = block_count;
    get_timestamp(b->timestamp, sizeof(b->timestamp));
    strncpy(b->data, data, DATA_LEN-1);
    b->data[DATA_LEN-1] = '\0';
    if(block_count == 0) strcpy(b->previous_hash, "0");
    else strcpy(b->previous_hash, blockchain[block_count-1].current_hash);
    b->nonce = rand()%1000; // simple PoW demo
    simple_hash(b);
    block_count++;
    printf("Block %d added!\n", b->index);
}

// List blockchain
void list_blocks() {
    printf("\nBlockchain:\n");
    printf("Index\tTimestamp\t\tData\tPrevious\tCurrent\tNonce\n");
    printf("-----\t---------\t\t----\t--------\t-------\t-----\n");
    for(int i=0;i<block_count;i++){
        Block *b = &blockchain[i];
        printf("%d\t%s\t%s\t%s\t%s\t%d\n",
               b->index, b->timestamp, b->data, b->previous_hash, b->current_hash, b->nonce);
    }
    printf("\n");
}

// Export CSV
void export_csv(const char *filename) {
    FILE *f = fopen(filename, "w");
    if(!f) { printf("Failed to open %s\n", filename); return; }
    fprintf(f,"index,timestamp,data,previous_hash,current_hash,nonce\n");
    for(int i=0;i<block_count;i++){
        Block *b = &blockchain[i];
        fprintf(f,"%d,%s,%s,%s,%s,%d\n",
                b->index, b->timestamp, b->data, b->previous_hash, b->current_hash, b->nonce);
    }
    fclose(f);
    printf("CSV exported to %s\n", filename);
}

// Export JSON
void export_json(const char *filename) {
    FILE *f = fopen(filename,"w");
    if(!f) { printf("Failed to open %s\n", filename); return; }
    fprintf(f,"{\n  \"chain\": [\n");
    for(int i=0;i<block_count;i++){
        Block *b = &blockchain[i];
        fprintf(f,"    {\n");
        fprintf(f,"      \"index\": %d,\n", b->index);
        fprintf(f,"      \"timestamp\": \"%s\",\n", b->timestamp);
        fprintf(f,"      \"data\": \"%s\",\n", b->data);
        fprintf(f,"      \"previous_hash\": \"%s\",\n", b->previous_hash);
        fprintf(f,"      \"current_hash\": \"%s\",\n", b->current_hash);
        fprintf(f,"      \"nonce\": %d\n", b->nonce);
        fprintf(f,"    }%s\n", (i==block_count-1)?"":",");
    }
    fprintf(f,"  ]\n}\n");
    fclose(f);
    printf("JSON exported to %s\n", filename);
}

int main() {
    srand(time(NULL));
    char choice[8];
    char data[DATA_LEN];
    char csv_file[64] = "blockchain.csv";
    char json_file[64] = "blockchain.json";

    while(1){
        printf("\nBlockchain Simulator - Menu\n");
        printf("1. Add Block\n2. List Blocks\n3. Export CSV\n4. Export JSON\n5. Quit\nChoose: ");
        if(!fgets(choice,sizeof(choice),stdin)) continue;
        int ch = atoi(choice);
        switch(ch){
            case 1:
                printf("Enter block data: ");
                fgets(data,DATA_LEN,stdin);
                data[strcspn(data,"\n")] = '\0';
                add_block(data);
                break;
            case 2: list_blocks(); break;
            case 3:
                printf("CSV filename (or press Enter for default): ");
                fgets(csv_file,sizeof(csv_file),stdin);
                csv_file[strcspn(csv_file,"\n")] = '\0';
                if(strlen(csv_file)==0) strcpy(csv_file,"blockchain.csv");
                export_csv(csv_file);
                break;
            case 4:
                printf("JSON filename (or press Enter for default): ");
                fgets(json_file,sizeof(json_file),stdin);
                json_file[strcspn(json_file,"\n")] = '\0';
                if(strlen(json_file)==0) strcpy(json_file,"blockchain.json");
                export_json(json_file);
                break;
            case 5: printf("Exiting.\n"); return 0;
            default: printf("Invalid choice.\n");
        }
    }
}
