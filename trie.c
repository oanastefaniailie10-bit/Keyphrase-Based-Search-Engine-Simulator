

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "var.h"

//functia creeaza un nod nou
Trie *init_node_trie()
{
	Trie *elem = malloc(sizeof(Trie));
	if (elem) {
		elem->file_ref = NULL;
		elem->last_file_ref = NULL;
		elem->ok = 0;
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			elem->children[i] = NULL;
		}
	}
	return elem;
}

//adaugarea unui cuvant in arbore
void add_trie(Trie *root, char *cuvant, Node *tail)
{
	Trie *elem = root;
	int length = strlen(cuvant);
	for (int i = 0; i < length; i++) {
		int index = cuvant[i] - 'a';
		if (elem->children[index] == NULL)
			elem->children[index] = init_node_trie();
		elem = elem->children[index];
	}
	elem->ok = 1;
	elem->file_ref = add_refs(elem->file_ref, tail);
}

// functia cauta daca exista cuvantul in arbore sau nu
Trie *search(Trie *root, char *cuvant)
{
	Trie *curr = root;
	int len = strlen(cuvant);
	int i;
	for (i = 0; i < len; i++)
	{
		int letter = cuvant[i] - 'a';
		if (!curr->children[letter])
			return NULL;
		curr = curr->children[letter];
	}
	return curr;
}

void del_file_trie(Trie *node_curr, char *id)
{
	if (!node_curr) return;
// valorea variabilelei ok semnaleaza daca s a ajuns la sfarsitul cuvantului
// se apeleaza functia pt stergerea fisierului din lista de referinte
	if (node_curr->ok)
		node_curr->file_ref = del_refs(node_curr->file_ref, id);
// parcurgerea recursiva a arborelui pentru a sterge fiecare fisier
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if(node_curr->children[i])
			del_file_trie(node_curr->children[i], id);
}

//functia verifica daca nodul are succesori
int has_children(Trie* node)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (node->children[i] != NULL)
		return 1;
	return 0;
}

// stergerea unui cuvant
Trie *del_word_trie(Trie *node_curr, char *cuvant, int depth, int is_root)
{
	if (!node_curr) return NULL;
// daca s a ajuns la lungimea cuvantului
// verifica daca este nod terminal sau daca mai are litere in jos
	if (depth == (int)strlen(cuvant)){
		if (node_curr->ok)
			node_curr->ok = 0;
// se elibereaza memoria din lista de referinte
		if (node_curr->file_ref){
			Refs *elem = node_curr->file_ref;
			while (elem) {
				Refs *aux = elem;
				elem = elem->next;
				free(aux);
			}
			node_curr->file_ref = NULL;
		}
// daca nodul nu este parinte, atunci se poate sterge
	if (!has_children(node_curr)) {
		free(node_curr);
		node_curr = NULL;
	}
	return node_curr;
	}
// se merge parcurge mai departe in adancime
	int index = cuvant[depth] - 'a';
	node_curr->children[index] = del_word_trie(node_curr->children[index], cuvant, depth + 1, 0);
// la intoarcere, daca nu are succesori si nu este nod terminal, poate fi eliberat nodul
	if (!has_children(node_curr) && !node_curr->ok && !is_root) {
		free(node_curr);
		node_curr = NULL;
	}
	return node_curr;
}

void empty_words(Trie *node_curr, char buffer[101], int depth, char **v, int *s)
{
	if (!node_curr) return;
	if (node_curr->ok) {
		buffer[depth] = '\0';
		if (!node_curr->file_ref) {
			v[*s] = strdup(buffer);
			(*s)++;
		}
	}
	for (int i = 0; i < ALPHABET_SIZE; i++)
	if (node_curr->children[i]) {
		buffer[depth] = 'a' + i;
		empty_words(node_curr->children[i], buffer, depth + 1, v, s);
	}
}

// se verifica daca exista vreun cuvant cu lista de referinte goala si se sterge
void verify_trie(Trie *root, char buffer[101])
{
	char **v = malloc(sizeof(char *) * 500);
	int s = 0;
	empty_words(root, buffer, 0, v, &s);
	for (int i = 0; i < s; i++)
	{
		del_word_trie(root, v[i], 0, 1);
		free(v[i]);
	}
	free(v);
}

// functia afiseaza continutul arborelui
void print_trie(Trie *node_curr, char buffer[101], int depth, FILE *fout)
{
	if (!node_curr) return;
// cand ajunge la finalul unui cuvant, se printeaza si se sorteaza fisierele
	if (node_curr->ok){
		buffer[depth] = '\0';
		fprintf(fout, "%s ", buffer);
		sort_refs(node_curr->file_ref, fout);
	}
// parcurgerea recursiva a arborelui si adaugarea in buffer a literelor pt a forma cuvantul
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (node_curr->children[i]) {
			buffer[depth] = 'a' + i;
			print_trie(node_curr->children[i], buffer, depth + 1, fout);
		}
}

void add_file(Trie *node, char **v, int *s, int depth)
{
	if (!node) return;
// daca a ajuns la finalul unui cuvant, adaugam fisierele din lista de referinte
	if (node->ok) {
		if(node->file_ref)
		{
			Refs *elem = node->file_ref;
			while (elem)
			{
				Node *aux = (Node *)elem->file;
				if(aux && aux->id){
					v[*s] = strdup(aux->id);
					(*s)++;
				}
				elem = elem->next;
			}
		}
	}
// parcurgem recursiv pentru a verifica fiecare cuvant
	for(int i = 0; i < ALPHABET_SIZE; i++)
		if(node->children[i])
			add_file(node->children[i], v, s, depth + 1);
}

// functia cauta prefixul dat in arbore si returneaza adresa de sfarsit
Trie *search_prefix(Trie *root, char *cuvant)
{
	Trie *curr = root;
	int len = strlen(cuvant);
	int i;
	for (i = 0; i < len; i++)
	{
		int letter = cuvant[i] - 'a';
		if (!curr->children[letter])
			return NULL;
		curr = curr->children[letter];
	}
	return curr;
}

// functia elibereaza memoria alocata pentru arbore
void freeTrie(Trie *root)
{
	if (root == NULL) return;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if(root->children[i])
			freeTrie(root->children[i]);
	if(root->file_ref)
		free_refs(root->file_ref);
	free(root);
}

// adaugarea unei referinte noi
Refs *add_refs(Refs *list, Node *tail)
{
// se verifica daca fisierul exista deja in refs
	Refs *aux = list;
	while(aux) {
		if((Node *)aux->file == tail)
		return list;
		aux = aux->next;
	}
	Refs *new_elem = malloc(sizeof(Refs));
	if(!new_elem)
		return NULL;
	new_elem->file = tail;
	new_elem->next = NULL;
// daca lista este goala, se returneaza fisierul nou
	if(!list)
		return new_elem;
	Refs *elem = list;
// se parcurge lista pt a ajunge la ultimul element
	while(elem->next != NULL)
		elem = elem->next;
	elem->next = new_elem;
	return list;
}

Refs* del_refs(Refs *list, char *id)
{
	Refs *elem = list;
	Refs *aux = NULL;
//se parcurge lista pana ajunge la id ul potrivit si se elimina referinta la fisier
	while(elem)
	{
		if(strcmp(((Node*)elem->file)->id, id) == 0){
			Refs *elem_2 = elem;
			if(!aux) { 
				list = elem->next;
				elem = list;
			} else {
				aux->next = elem->next;
				elem = elem->next;
			}
			free(elem_2);
		} else {
			aux = elem;
			elem = elem->next;
		}
	}
	return list;
}

// sortarea fisierelor pentru afisare
void sort_refs(Refs *node, FILE *fout)
{
	Refs *elem = node;
	char **v = malloc(sizeof(char *)*200);
	int i = 0, j;
// se adauga id-urile fisierelor in vector
	while(elem)
	{
		v[i] = strdup(((Node *)elem->file)->id);
		elem = elem->next;
		i++;
	}
	j = i;
// se sorteaza vectorul in ordine lexicografica
	for(int i = 0; i < j - 1; i++) {
		for(int k = 0; k < j - i - 1; k++) {
			if(strcmp(v[k], v[k + 1]) > 0) {
				char *p = v[k];
				v[k] = v[k + 1];
				v[k + 1] = p;
			}
		}
	}
// afisarea fisierelor
	fprintf(fout, "%d ", j);
	for(i = 0; i < j; i++){
		fprintf(fout, "%s", v[i]);
			fprintf(fout, " ");
	}
	fprintf(fout, "\n");
	for(i = 0; i < j; i++)
		free(v[i]);
	free(v);
}

// eliberarea memoriei
void free_refs(Refs *head)
{
	Refs *elem = head;
	while(elem)
	{
		Refs *aux = elem;
		elem = elem->next;
		free(aux);
	}
}