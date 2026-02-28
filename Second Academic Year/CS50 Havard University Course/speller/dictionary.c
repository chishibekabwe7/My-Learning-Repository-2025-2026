#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Node structure
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Bigger table = fewer collisions
#define N 50000

// Hash table
node *table[N];

// Word counter
unsigned int word_count = 0;

// Hash function (djb2 variant, case-insensitive)
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        c = tolower(c);
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % N;
}

// Load dictionary into memory
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocate memory for node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(file);
            return false;
        }

        // Copy word into node
        strcpy(new_node->word, word);

        // Hash word
        unsigned int index = hash(word);

        // Insert at beginning of linked list
        new_node->next = table[index];
        table[index] = new_node;

        word_count++;
    }

    fclose(file);
    return true;
}

// Check if word exists in dictionary
bool check(const char *word)
{
    unsigned int index = hash(word);

    node *cursor = table[index];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Return number of words in dictionary
unsigned int size(void)
{
    return word_count;
}

// Free memory
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}
