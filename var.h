
#define ALPHABET_SIZE 26

typedef struct Node{
	int scor;
	char *id;
	struct Node *prev, *next;
} Node;

typedef struct Node_refs{
	void *file;
	struct Node_refs *next;
} Refs;

typedef struct Node_trie{
	struct Node_trie *children[ALPHABET_SIZE];
	Refs *file_ref;
	Refs *last_file_ref;
	int ok;
} Trie;

typedef struct Heap_max{
	char *id;
	int scor;
} Heap_max;

Trie *init_node_trie();
void add_trie(Trie *root, char *cuvant, Node *tail);
Trie *search(Trie *root, char *cuvant);
void del_file_trie(Trie *root, char *id);
int has_children(Trie* node);
Trie *search_prefix(Trie *node, char *cuv);
Trie *del_word_trie(Trie *node_curr, char *cuvant, int depth, int is_root);
void empty_words(Trie *node_curr, char buffer[101], int depth, char **v, int *s);
void verify_trie(Trie *root, char buffer[101]);
void print_trie(Trie *root, char buffer[101], int depth, FILE *fout);
void add_file(Trie *node, char **v, int *s, int depth);
void freeTrie(Trie *root);

Refs *add_refs(Refs *list, Node *tail);
Refs *del_refs(Refs *list, char *id);
void sort_refs(Refs *node, FILE *fout);
void free_refs(Refs *node);

void swap(Heap_max *a, Heap_max *b);
void add_heap(Heap_max *h, int *size, Node *file);
void siftup(Heap_max *h, int i);
void siftdown(Heap_max *h, int size, int i);
int is_empty(Heap_max *h);