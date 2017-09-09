#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "autocomplete.h"

int main(int argc, char *argv[]) {
    char key [MAX_CHARACTER];
	int weight;
    int found_prefix;
    int char_comparisons, weight_comparisons;
	char prefix[MAX_CHARACTER];
	tree_ptr pNode = NULL;

    /* Read the input file line by line to insert each key into a ternary
    tree that consists of nodes containing one character of the key. The
    function tree_insert returns a pointer pointing to the root */
    FILE* input = open_file (argv[1], "r");
    FILE *output = open_file (argv[2], "w");
	while (fscanf (input, "%d;%[^\n]", &weight, key) == 2) {
		pNode = tree_insert (pNode, key, weight);
	}

	/* Stage 2: Except building a ternary tree and store the keys that consist
    of the prefix into a linked list, sort the linked list in descending order
    using Selection Sort and return the number of weight comparisons */

    // Use gets in order to be able to read white space in a prefix as well
    while (gets (prefix)) {
        char_comparisons = 0;
        found_prefix = TRUE;
        list_ptr pList_node = NULL;
        pList_node = find_and_traverse (pNode, pList_node, prefix,
            &found_prefix, &char_comparisons);
        // Sort the list and write to file with stage_two option being true
        if (found_prefix) {
            weight_comparisons = list_sort (pList_node);
        } else {
            // The number of weight comparison is 0 if prefix not been found
            weight_comparisons = FALSE;
        }
        write_file (pList_node, prefix, char_comparisons, weight_comparisons,
            found_prefix, TRUE, output);
    }
    // Free the memory tree nodes in the end and close the files
    free_tree_node (pNode);
    fclose (input);
    fclose (output);
	return 0;
}
