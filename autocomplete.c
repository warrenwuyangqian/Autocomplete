#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "autocomplete.h"

/* The function allocates memory for new node and insert each character of
the word into the node. By recursively inserting nodes according to thier
values into relevant branches,  it finally returns a pointer to the header */
tree_ptr tree_insert (tree_ptr pNode, char *word, int weight) {
    /* If the pointer to the node is NULL, allocate memory for a new node and
    initialise the node by assigning values into relevant fields */
    if (pNode == NULL) {
        pNode = malloc (sizeof (*pNode));
        assert (pNode);
        pNode -> data = word [0];
        pNode -> weight = 0;
        pNode -> end_of_key = FALSE;
        pNode -> left = NULL;
        pNode -> equal = NULL;
        pNode -> right = NULL;
    }
    /* If the character in the node is greater than OR the same as OR smaller
    than the one in word travel to left, equal, right branch respectively */
    if (pNode -> data > word [0]) {
        pNode -> left = tree_insert (pNode -> left, word, weight);
    }
    else if (pNode -> data == word [0]) {
        /* If it is the last character in the word, assign the weight and
        adjust value of the flag to indicate the end of the key */
        if (word [1] == '\0') {
            pNode -> weight = weight;
            pNode -> end_of_key = TRUE;
        }
        else {
            pNode -> equal = tree_insert (pNode -> equal, word + 1, weight);
        }
    }
    else {
        pNode -> right = tree_insert (pNode -> right, word, weight);
    }
    return pNode;
}


/* The function traverses through the ternary tree in attempt to find the
prefix while counting the number of character comparisons. It also indicates
whether the prefix has been found. In the end, the function returns a pointer
pointing to the header of a linked list of keys that consist of the prefix */
list_ptr find_and_traverse (tree_ptr pNode, list_ptr pList_node,
    char *prefix, int *found_prefix, int *char_comparisons) {

    char *temp_prefix = prefix; // To keep prefix unchanged after the iteration
    int buffer_index = 0;
    /* equal_comparisons is the number of character comparisons that are
    evaluated to equal, both temp_node and equal_comparisons will be used
    in the case where prefix is one and the only key in the tree */
    tree_ptr temp_node;
    int equal_comparisons = 0;
    // Buffer contains the characters that are the same as ones in the prefix
    char *buffer = (char*) malloc (sizeof (char) * MAX_CHARACTER);
    assert (buffer);

    /* Iterate over the prefix until it is the end of the prefix or the tree.
    If the character is the same as the one in prefix, store it into buffer
    and create a temporary pointer pointing to the node containing the same
    data in case of prefix itself being one and the only key in the tree */
    while (temp_prefix [0] != '\0' && pNode != NULL) {
        if (temp_prefix [0] < pNode -> data) {
            pNode = pNode -> left;
            (*char_comparisons) ++;
            continue;
        }
        if (temp_prefix [0] > pNode -> data) {
            pNode = pNode -> right;
            (*char_comparisons) ++;
            continue;
        }
        if (temp_prefix [0] == pNode -> data) {
            buffer [buffer_index] = temp_prefix [0];
            buffer_index ++;
            temp_prefix ++;
            (*char_comparisons) ++;
            equal_comparisons ++;
            temp_node = pNode;
            pNode = pNode -> equal;
            continue;
        }
    }
    /* At this point, pNode should be pointing to the node where the prefix
    is countained. */
    if (pNode != NULL) {
        /* If the prefix is also a key, insert the key to a linked list
        and then traverse through the rest of characters for other keys */
        if (pNode -> end_of_key == TRUE) {
            buffer [strlen (prefix)] = '\0';
            pList_node = list_insert (pList_node, buffer, pNode -> weight);
        }
        pList_node = traverse (pNode, pList_node, buffer, strlen (prefix));
    }
    /* pNode being NULL indicates that either there is no key consisting of
    the prefix, OR the prefix is one and the only key in the tree */
    else {
        if (equal_comparisons != strlen (prefix)) {
            *found_prefix = FALSE;
        } else {
            buffer [strlen (prefix)] = '\0';
            pList_node = list_insert (pList_node, buffer, temp_node -> weight);
        }
    }
    return pList_node;
}


/* The function traverses through the tree to find all the keys consist of the
prefix and it returns a pointer pointing to the header of the linked list that
contains those keys */
list_ptr traverse (tree_ptr pNode, list_ptr pList_node, char *buffer,
    int depth) {
    // End of a subtree
    if (pNode == NULL) {
        return pList_node;
    }
    // Search in the left branch first and insert keys into the list
    pList_node = traverse (pNode -> left, pList_node, buffer, depth);
    buffer [depth] = pNode -> data;
    if (pNode -> end_of_key == TRUE) {
        buffer [depth + 1] = '\0';
        pList_node = list_insert (pList_node, buffer, pNode -> weight);
    }
    // Search in the equal branch and then the right branch
    pList_node = traverse (pNode -> equal, pList_node, buffer, depth + 1);
    pList_node = traverse (pNode -> right, pList_node, buffer, depth);
    return pList_node;
}


/* The function creates a node and inserts key into the empty node of a linked
list and finally returns a pointer pointing to the header node of the list */
list_ptr list_insert (list_ptr pList_node, char* key, int weight) {
    if (pList_node == NULL) {
        pList_node = malloc (sizeof (*pList_node));
        assert (pList_node);
        pList_node -> weight = weight;
        strcpy(pList_node -> data, key);
        pList_node -> next = NULL;
    }
    else {
        pList_node -> next = list_insert (pList_node -> next, key, weight);
    }
    return pList_node;
}


/* Sorting the linked list in decreasing order of weights given the pointer
pointing to the header of the list using Selection Sort, the function then
returns the number of weight comparisons that have been done */
int list_sort (list_ptr pList_node) {
	list_ptr header = pList_node;
	list_ptr traverse; // Used to traverse through every key in the list
	list_ptr max; // The key with largest weight
    int weight_comparisons = 0; // It includes the number of self-comparisons
	while (header -> next != NULL) {
		max = header;
		traverse = header -> next;
		while (traverse != NULL) {
			/* Find the key with maximum weight */
			if (max -> weight < traverse -> weight) {
				max = traverse;
			}
			traverse = traverse -> next;
            weight_comparisons ++;
		}
        // Swap the positions of node with highest weight with current header
        int temp_weight = header -> weight;
        char temp_data [MAX_CHARACTER];
        strcpy (temp_data, header -> data);
        header -> weight = max -> weight;
        strcpy (header -> data, max -> data);
        max -> weight = temp_weight;
        strcpy (max -> data, temp_data);

		header = header -> next;
	}
    return weight_comparisons;
}


// The function traverses through a givin list and writes into a file
void print_list (list_ptr pList_node, FILE* output) {
    list_ptr header = pList_node;
    while (header != NULL) {
        fprintf (output, "key:  %s --> weight:  %d\n", header -> data,
        header -> weight);
        header = header -> next;
    }
}


/* The function opens file in specified mode. It gives an error message and
exits the program if it fails to open the file */
FILE* open_file (char* filename, char* mode) {
    FILE* file = fopen (filename, mode);
    if (file == NULL) {
        fprintf (stderr, "Failed to open the file %s\n", filename);
        exit (EXIT_FAILURE);
    }
    return file;
}


/* Print the keys one by one from the unsorted linked list and the number of
character and weight comparisons. Finally free the memory allocated to store
for each node in the linked list after each iteration of searching prefix */
void write_file (list_ptr pList_node, char *prefix, int char_comparisons,
    int weight_comparisons, int found_prefix, int stage_two, FILE *output) {
    fprintf (output, "Prefix:  %s\n", prefix);
    if (found_prefix) {
        print_list (pList_node, output);
        fprintf (stdout, "Prefix: %s found with %d char comparsions\n",
        prefix, char_comparisons);
    } else {
        fprintf (output, "NOTFOUND\n");
        fprintf (stdout, "Prefix: %s found with %d char comparsions\n",
        prefix, char_comparisons);
    }
    if (stage_two) {
        fprintf (stdout, "Selection Sort: %d weight comparsions\n",
        weight_comparisons);
    }
    free_list_node (pList_node);
    fprintf (output, "\n");
}


/* Both free_list_node and free_tree_node attempt to free the memory allocated
for the tree nodes and list nodes respectively. They give error messages if
failling to free the memory */
void free_list_node (list_ptr pList_node) {
   if (pList_node != NULL) {
       free_list_node (pList_node -> next);
       free (pList_node);
   }
   else {
       return;
   }
}


void free_tree_node (tree_ptr pNode) {
   if (pNode != NULL) {
       free_tree_node (pNode -> left);
       free_tree_node (pNode -> right);
       free_tree_node (pNode -> equal);
       free (pNode);
   }
   else {
       return;
   }
}
