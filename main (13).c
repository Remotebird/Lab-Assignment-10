#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct TrieNode {
    struct TrieNode *children[26]; // For each letter of the alphabet
    int count; // Number of times the word is inserted
};

struct Trie {
    struct TrieNode *root;
};


// Inserts the word to the trie structure
void insert(struct Trie *trie, char *word) {
    struct TrieNode *node = trie->root;
    while (*word) {
        int index = *word - 'a';
        if (!node->children[index]) {
            node->children[index] = malloc(sizeof(struct TrieNode));
            node->children[index]->count = 0;
            for (int i = 0; i < 26; i++) {
                node->children[index]->children[i] = NULL;
            }
        }
        node = node->children[index];
        word++;
    }
    node->count++;
}


// computes the number of occurances of the word
int numberOfOccurances(struct Trie *trie, char *word) {
    struct TrieNode *node = trie->root;
    while (*word) {
        int index = *word - 'a';
        if (!node->children[index]) {
            return 0; // Word not found
        }
        node = node->children[index];
        word++;
    }
    return node->count;
}


// deallocate the trie structure
void deallocateTrieNodes(struct TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            deallocateTrieNodes(node->children[i]);
        }
    }
    free(node);
}

struct Trie *deallocateTrie(struct Trie *trie) {
    deallocateTrieNodes(trie->root);
    free(trie);
    return NULL;
}


// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *trie = malloc(sizeof(struct Trie));
    trie->root = malloc(sizeof(struct TrieNode));
    trie->root->count = 0;
    for (int i = 0; i < 26; i++) {
        trie->root->children[i] = NULL;
    }
    return trie;
}


// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    char buffer[100];
    int count = 0;
    while (fgets(buffer, 100, file) && count < 256) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline
        pInWords[count] = strdup(buffer); // Duplicate the buffer
        count++;
    }
    fclose(file);
    return count;
}


int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}