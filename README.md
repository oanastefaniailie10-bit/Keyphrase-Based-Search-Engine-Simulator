# Keyphrase-Based Search Engine Simulator

# Description

This project is a C application that simulates an indexing mechanism based on associated keyphrases.

# Data Structures Used

In this project, several data structures were used to store the provided information. A doubly linked list stores a unique identifier and a relevance score for the given files. The associated keyphrases are stored using a multiway retrieval tree (trie). The terminal node of each word contains a reference list of pointers to each file containing that keyphrase, as well as an 'ok' variable indicating whether it is terminal. A max-heap is also used to determine the top $k$ most relevant files associated with a given word.
The data structures are defined in the `var.h` file, along with the function prototypes used for the tree, reference list, and max-heap.

# Mechanism of Operation

Information is read from the input file `indexare.in` and outputted to `indexare.out`. The file contains various operations that can be performed on the system, followed by additional information where applicable:

* **ADD `<id>` `<score>` `<word_count>` `<keyphrases>**`: Adds a new file to the file list and its associated keyphrases to the tree. If the ID already exists, a corresponding message is displayed.
* **DEL `<id>**`: Deletes the file with the given identifier. If it does not exist, the message `NOT FOUND` is displayed.
* **ADDKW `<id>` `<keyphrase>**`: Adds a new keyphrase to an existing file. Upon successful execution of the operation, the message `OK` is displayed; otherwise, `NOT FOUND` is displayed.
* **DELKW `<id>` `<keyphrase>**`: Deletes a keyphrase from an existing file. If the file does not exist, the operation fails and `NOT FOUND` is displayed. If the file exists and already has that keyphrase associated with it, the operation does not modify the system, but is considered successful and `OK` is displayed.
* **FIND `<keyphrase>**`: Displays all files containing the given word in lexicographical order of their identifiers.
* **TOPK `<keyphrase>` `<k>**`: Determines the top $k$ most relevant files associated with the given word using a max-heap. The files are sorted based on their relevance score, and if two files have the same score, they will be added to the heap in lexicographical order.
* **PRINT**: Displays all keyphrases in lexicographical order, along with the number of associated files and the identifiers of those files in lexicographical order.
* **PREFIX `<prefix>**`: Receives a prefix and determines all files containing at least one keyphrase starting with the given prefix. The results are displayed in lexicographical order.

In addition to these operations, there are utility functions for managing the data structures. For the file list, the following operations can be performed: `init_list`, `add_list`, `del_list`, `verify_list`, and `free_list`. For the retrieval tree (trie), the functions used are: `init_node_trie`, `add_trie`, `search`, `del_file_trie`, `has_children`, `search_prefix`, `del_word_trie`, `verify_trie`, `print_trie`, `add_file`, and `free_trie`. For the reference list, the available operations are: `add_refs`, `del_refs`, `sort_refs`, and `free_refs`. For the max-heap, the supported operations are: `swap`, `add_heap`, `siftup`, and `is_empty`.

# Compilation and Execution

To compile the program using `gcc`, run `make build` in the terminal; to run the program, execute `make run`.