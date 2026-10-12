

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "var.h"

//initializeaza lista de fisiere
void init_list(Node **head, Node **tail)
{
	Node *elem = malloc(sizeof(Node));
	elem->prev = NULL;
	elem->scor = 0;
	elem->next = NULL;
	elem->id = NULL;
	(*head) = elem;
	(*tail) = (*head);
}

// functia de adaugare a unui fisier nou
Node *add_list(Node *tail, int n, char *p)
{
	Node *elem = malloc(sizeof(Node));
	elem->scor = n;
	elem->id = strdup(p);
	elem->prev = tail;
	elem->next = NULL;
	tail->next = elem;
	tail = tail->next;
	return elem;
}

// functia de stergere a unui element din lista de fisiere
void del_list(Node *elem)
{
	if(elem->next && elem->prev){
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;
		if (elem->id) free(elem->id);
		free(elem);
	} else if(elem->prev){
		elem->prev->next = NULL;
		if (elem->id) free(elem->id);
		free(elem);
	} else {
		elem->scor = 0;
		if (elem->id) {
			free(elem->id);
			elem->id = NULL;
		}
	}
}

//verifica daca fisierul cu id ul dat se afla sau nu in lista
Node* verify_list(Node *head, char *p)
{
	Node *elem = head->next;
	while(elem != NULL)
	{
		if(strcmp(elem->id, p) == 0)
			return elem;
		elem = elem->next;
	}
	return NULL;
}

// functia elibereaza memoria alocata pt lista
void free_list(Node *head)
{
	Node *elem= head;
	while(elem)
	{
		Node *aux = elem;
		elem = elem->next;
		if(aux->id)
			free(aux->id);
		free(aux);
	}
}

Node *add(Node *tail, char *id, int scor, char **v, int t, Trie *root)
{
// se adauga fisierul in lista
	Node * new_tail = add_list(tail,scor,id);
// se adauga cuvintele-cheie
	for(int i = 0; i < t; i++)
		add_trie(root, v[i], new_tail);
	return new_tail;
}

void del(Node *head, char *id, Trie *root)
{
// se sterge fisierul de la referinte
	del_file_trie(root, id);
	char buffer[101];
// se verifica daca exista cuvinte-cheie fara referinte
	verify_trie(root, buffer);
// se gaseste elementul in lista si se sterge
	Node *elem = head->next;
	while(elem && strcmp(elem->id, id) != 0)
		elem = elem->next;
	if(elem)
		del_list(elem);
}

void addkw(Node *head, char *id, char cuv[101], Trie *root)
{
	Node *elem = verify_list(head,id);
	if(!elem) return;
	Trie *node = search(root, (char *)cuv);
// se adauga cuvantul cheie daca nu exista
// daca exista, se adauga doar fisierul in refs
	if(!node)
		add_trie(root, (char *)cuv, elem);
	else {
		node->ok = 1;
		node->file_ref = add_refs(node->file_ref, elem);
	}
}

void verify_trie_file(Trie *node_curr, char *id, int *ok)
{
	if (node_curr == NULL || (*ok) == 1) return;
// daca s-a ajuns la finalul cuvantului, se verifica daca contine fisierul in lista de referinte
	if (node_curr->ok) {
		Refs *elem = node_curr->file_ref;
		while(elem && (*ok) == 0)
		{
			Node *aux = (Node *)elem->file;
			if(aux && aux->id){
			if(strcmp(aux->id, id) == 0)
				(*ok) = 1;
			}
			elem = elem->next;
		}
	}
	for(int i = 0; i < ALPHABET_SIZE; i++)
		if(node_curr->children[i])
			verify_trie_file(node_curr->children[i], id, ok);
}

void delkw(Node *head, char *id, char cuv[101], Trie *root)
{
	Trie *node = search(root, (char *)cuv);
	if(node) {
		Refs *aux = node->file_ref;
		Refs *prev = NULL;
// se cauta id-ul fisierului in lista de referinte
// se sterge in functie de cate elemente sunt in lista
		while(aux){
			Node *aux_curr = (Node *)aux->file;
			if(aux_curr && aux_curr->id){
				if(strcmp(aux_curr->id, id) == 0)
				break;
			}
			prev = aux;
			aux = aux->next;
		}
		if(aux){
			if(!prev)
				node->file_ref = aux->next;
			else
				prev->next = aux->next;
			free(aux);
		}
		if(!node->file_ref)
			del_word_trie(root, (char *)cuv, 0, 1);
	}
	int ok = 0;
// functia verifica daca alte cuvinte au la referinte fisierul dat
	verify_trie_file(root, id, &ok);
// daca nu se gaseste, se sterge din lista de fisiere
	if(ok == 0){
		Node *elem = verify_list(head, id);
		del_list(elem);
	}
	
}

void find(Trie *root, char cuv[101], FILE *fout)
{
	Trie *node = search(root, (char *)cuv);
	if(node){
	Refs *elem = node->file_ref;
	char **v = malloc(sizeof(char *)*200);
	int i = 0, j;
// umplerea vectorului cu id urile fisierelor
	while(elem)
	{
		Node *aux = (Node *)elem->file;
		v[i] = strdup(aux->id);
		elem = elem->next;
		i++;
	}
	j = i;
// sortarea vectorului de fisiere in ordine lexicografica
	for(int i = 0; i < j - 1; i++) {
		for(int k = 0; k < j - i - 1; k++) {
			if(strcmp(v[k], v[k + 1]) > 0) {
				char *p = v[k];
				v[k] = v[k + 1];
				v[k + 1] = p;
			}
		}
	}
	fprintf(fout, "%d ", j);
	for(i = 0; i < j; i++) {
		fprintf(fout, "%s", v[i]);
		if(i < j - 1)
			fprintf(fout, " ");
	}
	fprintf(fout, "\n");
	for(i = 0; i < j; i++)
		free(v[i]);
	free(v);
	} else
		fprintf(fout, "EMPTY\n");
}

void topk(Trie *root, char cuv[101], int k, Heap_max *h, int *heap_size, FILE *fout)
{
	Trie *node = search(root, (char *)cuv);
	if (node == NULL) {
		fprintf(fout, "EMPTY\n");
		return;
	}
	Refs* elem = node->file_ref;
	for (int i = 0; i < *heap_size; i++) {
		if ((h + i)->id) {
			free((h + i)->id);
			(h + i)->id = NULL;
		}
	}
	int size = 0;
// se adauga elementele in heap
	while(elem)
	{
		add_heap(h, &size,(Node *)elem->file);
		elem = elem->next;
	}
// afisarea rezultatului
	if(is_empty(h))
		fprintf(fout, "EMPTY\n");
	else {
		int s;
		if(size < k)
			s = size;
		else
			s = k;
		fprintf(fout, "%d ", s);
		for(int i = 0; i < s; i++) {
			fprintf(fout, "%s", h[0].id);
			if(i < s - 1)
				fprintf(fout, " ");
			swap(h, h + (size - 1 - i));
			siftdown(h, size - 1 - i, 0);
		}
		fprintf(fout, "\n");
	}
	(*heap_size) = size;
}

void prefix(Trie *root, char cuv[101], FILE *fout)
{
	Trie *node = search_prefix(root, cuv);
	int s = 0;
	char **v = malloc(sizeof(char *)*500);
// sortarea elementelor din vector
	if(node){
		add_file(node, v, &s, 0);
		for(int i = 0; i < s - 1; i++)
		for(int k = 0; k < s - i - 1; k++) {
			if(strcmp(v[k], v[k + 1]) > 0) {
				char *p = v[k];
				v[k] = v[k + 1];
				v[k + 1] = p;
			}
		}
	}
	int j = 0;
	for( int i = 0; i < s; i++)
	{
		if(j == 0 || strcmp(v[i], v[j - 1]) != 0)
			v[j++] = v[i];
		else
			free(v[i]);
	}
	s = j;
// daca exista fisiere, se printeaza dupa ce au fost sortate
// in caz contrar, se afiseaza un mesaj
	if(s == 0)
		fprintf(fout, "EMPTY\n");
	else {
		fprintf(fout, "%d ", s);
		for(int i = 0; i < s; i++){
			fprintf(fout, "%s", v[i]);
			if(i < s - 1)
				fprintf(fout, " ");
		}
		fprintf(fout, "\n");
	}
	for(int i = 0; i < s; i++)
		free(v[i]);
	free(v);
}

int main()
{
	int q = 0, scor, t = 0, k = 0, i, j, heap_size = 0;
	char s[7], cuvant[101];

	Node *file, *last_file;
	init_list(&file, &last_file);
	Trie *root = init_node_trie();
	Heap_max *h = malloc(sizeof(Heap_max)*100);
	for (int i = 0; i < 100; i++)
		h[i].id = NULL;

	FILE *fin = fopen("indexare.in", "r");
	FILE *fout = fopen("indexare.out", "w");

	if (fin == NULL || fout == NULL) {
		free(h);
		free(root); 
		free(file); 
		if (fin) fclose(fin);
		if (fout) fclose(fout);
		return 1;
	}
	if(fscanf(fin, "%d", &q) != 1) {
		free(h);
		free(root);
		free(file);
		fclose(fin);
		fclose(fout);
		return 1;
	}
	for (i = 0; i < q; i++)
	{
		fscanf(fin, "%s", s);
		if (strcmp(s, "ADD") == 0) {
			char *id = malloc(sizeof(char) * 100);
			fscanf(fin, "%s", id);
			fscanf(fin, "%d", &scor);
			fscanf(fin, "%d", &t);
			char **v = malloc(sizeof(char*) * 200);
			for( j = 0; j < t; j++)
			{
				fscanf(fin, "%s", cuvant);
				v[j] = strdup(cuvant);
			}
			if(verify_list(file, id))
				fprintf(fout,"EXISTS\n");
			else {
				last_file = file;
				while(last_file->next)
					last_file = last_file->next;
				last_file = add(last_file, id, scor, v, t, root);
				fprintf(fout, "OK\n");
			}
			for(j = 0; j < t; j++)
				free(v[j]);
			free(v);
			free(id);
		}
		if (strcmp(s, "DEL") == 0) {
			char *id = malloc(sizeof(char) * 100);
			fscanf(fin, "%s", id);
			if(!verify_list(file, id))
				fprintf(fout,"NOT FOUND\n");
			else {
				del(file, id, root);
				fprintf(fout, "OK\n");
			}
			free(id);
		}
		if (strcmp(s, "ADDKW") == 0) {
			char *id = malloc(sizeof(char) * 100);
			fscanf(fin, "%s", id);
			fscanf(fin, "%s", cuvant);
			if(!verify_list(file, id))
				fprintf(fout,"NOT FOUND\n");
			else {
				addkw(file, id, cuvant, root);
				fprintf(fout, "OK\n");
			}
			free(id);
		}
		if (strcmp(s, "DELKW") == 0) {
			char *id = malloc(sizeof(char) * 100);
			fscanf(fin, "%s", id);
			fscanf(fin, "%s", cuvant);
			if(!verify_list(file, id))
				fprintf(fout,"NOT FOUND\n");
			else {
				delkw(file, id, cuvant, root);
				fprintf(fout, "OK\n");
			}
			free(id);
		}
		if (strcmp(s, "FIND") == 0) {
			fscanf(fin, "%s", cuvant);
			find(root, cuvant,fout);
		}
		if (strcmp(s, "TOPK") == 0) {
			fscanf(fin, "%s", cuvant);
			fscanf(fin, "%d", &k);
			topk(root, cuvant, k, h, &heap_size,fout);
		}
		if (strcmp(s, "PRINT") == 0) {
			char buffer[101];
			if(!has_children(root))
				fprintf(fout, "EMPTY\n");
			else
				print_trie(root,buffer,0,fout);
		}
		if (strcmp(s, "PREFIX") == 0) {
			fscanf(fin, "%s", cuvant);
			prefix(root, cuvant, fout);
		}
	}
	free(h);
	freeTrie(root);
	free_list(file);
	fclose(fin);
	fclose(fout);
	return 0;
}