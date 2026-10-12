
#include <stdio.h>
#include <stdlib.h>
#include "var.h"
#include <string.h>

// functia interschimba doua valori intre ele
void swap(Heap_max *a, Heap_max *b) {
	Heap_max aux = *a;
	*a = *b;
	*b = aux;
}

// functia adauga un fisier in heap, modificandu-i marimea
void add_heap(Heap_max *h, int *size, Node *file)
{
	for(int i = 0; i < *size; i++)
		if(strcmp((h + i)->id, file->id) == 0)
			return;
	(h + (*size))->id = strdup(file->id);
	(h + (*size))->scor = file->scor;
	siftup(h,*size);
	(*size) ++;
}

// functia adauga elementul in heap, asigurand ca toate elementele sunt sortate
void siftup(Heap_max *h, int i)
{
	int parent = (i - 1) / 2;
	while (i > 0) {
		if((h + i)->scor == (h + parent)->scor){
			if(strcmp((h + i)->id, (h + parent)->id) < 0) {
				swap(h + i, h + parent);
				i = parent;
				parent = (i - 1) / 2;
			}
			else break;
		} else if((h + i)->scor > (h + parent)->scor){
			swap(h + i, h + parent);
			i = parent;
			parent = (i - 1) / 2;
		} else
			break;
	}
}

// extragerea corecta a maximului din heap
void siftdown(Heap_max *h, int size, int i)
{
	int m = i;
	int left = 2*i + 1;
	int right = 2*i + 2;
	if(left < size) {
		if((h + left)->scor > (h + m)->scor)
			m = left;
		else if((h + left)->scor == (h + m)->scor && strcmp((h + left)->id, (h + m)->id) < 0)
			m = left;
	}
	if(right < size) {
		if((h+ right)->scor > (h + m)->scor)
			m = right;
		else if((h + right)->scor == (h + m)->scor && strcmp((h + right)->id, (h + m)->id) < 0)
			m = right;
	}
	if(m != i) {
		swap(h + i, h + m);
		siftdown(h, size, m);
	}
}

// functia verifica daca heap-ul este gol
int is_empty(Heap_max *h)
{
	if(h->id == NULL)
		return 1;
	return 0;
}