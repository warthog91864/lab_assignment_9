#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Linked list node
struct ListNode
{
    struct RecordType data;
    struct ListNode* next;
};

// Hash table structure with separate chaining
struct HashType
{
    struct ListNode** buckets;
    int size;
};

// Compute the hash function
int hash(int x, int size)
{
    // A simple hash function to distribute data evenly in the array
    return x % size;
}

// Inserts a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id, hashTable->size);

    // Create a new node for the record
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed for hash table node.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = record;
    newNode->next = NULL;

    // Insert the node into the appropriate bucket
    if (hashTable->buckets[index] == NULL)
    {
        hashTable->buckets[index] = newNode;
    }
    else
    {
        struct ListNode* current = hashTable->buckets[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Display records in the hash structure
// Skip the indices which are free
// The output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* hashTable)
{
    for (int i = 0; i < hashTable->size; ++i)
    {
        if (hashTable->buckets[i] != NULL)
        {
            printf("Index %d -> ", i);
            struct ListNode* current = hashTable->buckets[i];
            while (current != NULL)
            {
                printf("%d, %c, %d -> ", current->data.id, current->data.name, current->data.order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}
// Free memory used by the hash table
void freeHashTable(struct HashType* hashTable)
{
    for (int i = 0; i < hashTable->size; ++i)
    {
        struct ListNode* current = hashTable->buckets[i];
        while (current != NULL)
        {
            struct ListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->buckets);
    free(hashTable);
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    int hashSz = 10; // Choose an appropriate hash table size

    recordSz = parseData("input.txt", &pRecords);
    if (recordSz <= 0)
    {
        printf("Error reading data from the input file.\n");
        return 1;
    }

    // Create the hash table
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType));
    if (hashTable == NULL)
    {
        printf("Memory allocation failed for hash table.\n");
        return 1;
    }
    hashTable->size = hashSz;
    hashTable->buckets = (struct ListNode**)calloc(hashSz, sizeof(struct ListNode*));
    if (hashTable->buckets == NULL)
    {
        printf("Memory allocation failed for hash table buckets.\n");
        free(hashTable);
        return 1;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable);

    // Free memory
    freeHashTable(hashTable);
    free(pRecords);

    return 0;
}
