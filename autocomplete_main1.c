#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "autocomplete.h"

int main(int argc, char *argv []) {
    tree_ptr pNode = NULL;  // Initialise the root of the tree to NULL
    char prefix [MAX_CHARACTER];
    char key [MAX_CHARACTER];
	int weight;
    int found_prefix;    // Indicate whether prefix been found
    int char_comparisons;

    /* Read the input file line by line to insert each key into a ternary tree
    that consists of nodes containing one character of the key in each, weight
    pointer to left, equal and right branches and a flag indicates the end of
    the key. The function then returns a pointer pointing to the root */
    FILE* input = open_file (argv [1], "r");
    FILE *output = open_file (argv [2], "w");
	while (fscanf (input, "%d;%[^\n]", &weight, key) == 2) {
		pNode = tree_insert (pNode, key, weight);
	}

    /* Stage 1: With prefix(es) read in, traverse through the ternary tree
    in attempt to find the keys that consists of the prefix, then store the
    keys into a linked list. At the same time, counts the number of character
    comparisons. The functions then return the header of the linked list. */

    // Use gets in order to be able to read white space in a prefix as well
    while (gets (prefix)) {
        // Initialise the root of a new list to NULL for each prefix
        list_ptr pList_node = NULL;
        char_comparisons = 0;
        found_prefix = TRUE;
        /* Traverse through the tree and returns a pointer pointing to the
        header node of a linked list for each prefix */
        pList_node = find_and_traverse (pNode, pList_node, prefix,
            &found_prefix, &char_comparisons);

        // Write information to the file with stage_two option being false
        write_file (pList_node, prefix, char_comparisons, FALSE, found_prefix,
            FALSE, output);
    }
    // Free the memory tree nodes in the end and close the files
    free_tree_node (pNode);
    fclose (input);
    fclose (output);
	return 0;
}
