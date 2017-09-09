#ifndef _ASS1_H_
#define _ASS1_H_


#define MAX_CHARACTER 250
#define TRUE 1
#define FALSE 0


// Ternary Tree Node
typedef struct treenode *tree_ptr;
struct treenode {
    char data;
    int weight;
    int end_of_key;
    tree_ptr left;
    tree_ptr equal;
    tree_ptr right;
};

// Linked List Node
typedef struct listnode *list_ptr;
struct listnode {
	char data[MAX_CHARACTER];
	int weight;
	list_ptr next;
};


FILE* open_file (char* filename, char* mode);
tree_ptr tree_insert (tree_ptr pNode, char *word, int weight);
list_ptr find_and_traverse (tree_ptr pNode, list_ptr pList_node, char *prefix,
    int *found_prefix, int *char_comparisons);
list_ptr traverse (tree_ptr pNode, list_ptr pList_node, char *buffer,
    int depth);
list_ptr list_insert (list_ptr pList_node, char *key, int weight);
int list_sort (list_ptr pList_node);
void print_list (list_ptr pList_node, FILE *output);
void write_file (list_ptr pList_node, char *prefix, int char_comparisons,
    int weight_comparisons, int found_prefix, int stage_two, FILE *output);
void free_tree_node (tree_ptr pNode);
void free_list_node (list_ptr pList_node);


#endif
