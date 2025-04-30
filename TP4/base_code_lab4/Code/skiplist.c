#include <stdlib.h>
#include <assert.h>

#include "skiplist.h"
#include "rng.h"


typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement** next;
	struct s_LinkedElement** previous;
} LinkedElement;  //Struct LinkedElement from TP3


struct s_SkipList {
	int nblevels;
	int size;
	RNG rng;
	LinkedElement* sentinelle;
};

LinkedElement* sentinelle_create(int nblevels) {
	assert(nblevels > 0);
	LinkedElement* sentinelle = (LinkedElement*)malloc(sizeof(LinkedElement));
	sentinelle->value = 0;
	sentinelle->next = (LinkedElement**)malloc(nblevels * sizeof(LinkedElement*));
	sentinelle->previous = (LinkedElement**)malloc(nblevels * sizeof(LinkedElement*));
	for (int i = 0; i < nblevels; i++) {
		sentinelle->next[i] = NULL;
		sentinelle->previous[i] = NULL;
	}
	return sentinelle;
}



SkipList* skiplist_create(int nblevels) {
	SkipList* new_skip = (SkipList*)malloc(sizeof(SkipList));
	new_skip->nblevels = nblevels;
	new_skip->size = 0;
	new_skip->sentinelle = sentinelle_create(nblevels);
	new_skip->rng = rng_initialize(0, nblevels);
	return new_skip;
}

void skiplist_delete(SkipList** d) {
	if(d == NULL || *d == NULL) {
		return;
	}
	LinkedElement* current = (*d)->sentinelle->next[0];
	while(current != NULL) {
		LinkedElement* next = current->next[0];
		free(current);
		current = next;
	}


	free((*d)->sentinelle->next);
	free((*d)->sentinelle->previous);

	free((*d)->sentinelle);

	free(*d);
	*d = NULL;

}



unsigned int skiplist_size(const SkipList* d){
	return d->size;
}

int skiplist_at(const SkipList* d, unsigned int i) {
	assert (d != NULL);
	assert (i < skiplist_size(d));
	
	LinkedElement* current = d->sentinelle->next[0];
	for (unsigned int j = 0; j < i; j++) {
		current = current->next[0];
	}

	return current->value;
}


void skiplist_map(const SkipList* d, ScanOperator f, void *user_data) {
	assert (d != NULL);
	assert (f != NULL);
	LinkedElement* current = d->sentinelle->next[0];
	while (current != NULL) {
		f(current->value, user_data);
		current = current->next[0];
	}
}


SkipList* skiplist_insert(SkipList* d, int value) {
	assert (d != NULL);

	int level = rng_get_value(&(d->rng)) + 1;
	if(level > d->nblevels) {
		level = d->nblevels;
	}


LinkedElement* current = d->sentinelle;
	for(int i = d->nblevels - 1; i >= 0; i--) {
		while(current->next[i] != NULL && current->next[i]->value < value) {
			current = current->next[i];
		}
	

	if(current->next[i] != NULL && current->next[i]->value == value) {
		return d; 
	}
}

	LinkedElement* new_E = (LinkedElement*)malloc(sizeof(LinkedElement));
	new_E->value = value;
	new_E->next = (LinkedElement**)malloc(level * sizeof(LinkedElement*));
	new_E->previous = (LinkedElement**)malloc(level * sizeof(LinkedElement*));
	for (int i = 0; i < level; i++) {
		new_E->next[i] = NULL;
		new_E->previous[i] = NULL;
	}

	current = d->sentinelle;

	for(int i = level - 1;i>=0;i--){
		while(current->next[i] != NULL && current->next[i]->value < value ){
			current = current->next[i];
		}
	

	if(i<level){
		new_E->next[i]=current->next[i];
		new_E->previous[i]=current;


		if(current->next[i] != NULL){
			current->next[i]->previous[i]=new_E;
		}
		current->next[i]=new_E;
	}
}
	d->size++;
	return d;
}


bool skiplist_search(const SkipList* d, int value, unsigned int* nb_operations) {
	assert (d != NULL);

	bool found = false;
	*nb_operations = 0;
	LinkedElement* current = d->sentinelle;

	for(int i = d->nblevels - 1; i >= 0 && !found; i--) {
		if (current->next[i] != NULL && current->next[i]->value == value) {
			found = true;
		}

		while (current->next[i] != NULL && current->next[i]->value < value) {
			current = current->next[i];
			(*nb_operations)++;
		}
	}
	current = current->next[0];
	(*nb_operations)++;
	if (current != NULL && current->value == value) {
		found = true;
		return found;
	}
	return found;
}


struct s_SkipListIterator{
	SkipList * list;
	LinkedElement* current;
	int forwardorbackward;
};

SkipListIterator* skiplist_iterator_create(SkipList* d, IteratorDirection w){
	SkipListIterator * l =malloc(sizeof(SkipListIterator));
	l->list = d;
	if(w == FORWARD_ITERATOR){
		l->current = d->sentinelle->next[0];
		l->forwardorbackward = 1;
	}else{
		l->current = d->sentinelle;
		l->forwardorbackward = -1;
	}
	return l;
}


void skiplist_iterator_delete(SkipListIterator** l){
	free(*l);
	*l = NULL;
}


SkipListIterator* skiplist_iterator_begin(SkipListIterator* l){
	assert( l != NULL);
	if(l->forwardorbackward == 1){
		l->current = l->list->sentinelle->next[0];
	}else{
		l->current = l->list->sentinelle->previous[0];
	}
	return l;
}

bool skiplist_iterator_end(SkipListIterator* l){
	assert (l != NULL);
	if(l->forwardorbackward == 1){
		return l->current == NULL;
	}else{
		return l->current == l->list-> sentinelle;
	}
}

SkipListIterator* skiplist_iterator_next(SkipListIterator* l){
	assert (l != NULL);
	if(l->forwardorbackward == 1){
		l->current = l->current-> next[0];
	}else{
		l->current = l->current->previous[0];
	}
	return l;
}


int skiplist_iterator_value(SkipListIterator* l){
	assert(l != NULL);
	return l->current->value;
}


SkipList* skiplist_remove(SkipList* d, int value) {
	LinkedElement* current = d->sentinelle;
	LinkedElement* update[d->nblevels];
	int top = d->nblevels - 1;

	if (current->next[0] == NULL) {
		d->sentinelle->previous[0] = current->previous[0];
	}

	for(int i = top;i>=0;i--){
		while(current->next[i] != NULL && current->next[i]->value < value){
			current = current->next[i];
		}
		update[i] = current;
	}

	current = current->next[0];
	if(current == NULL || current->value != value){
		return d;
	}


	if(current != NULL && current->value == value){
	for(int i = 0;i< d->nblevels;i++){
		if(update[i]->next[i]==current){
			update[i]->next[i]=current->next[i];
			if(current->next[i] != NULL){
				current->next[i]->previous[i]=update[i];
			}
		}
	}
}
	free(current->next);
	free(current->previous);
	free(current);

	d->size--;
	return d;
}


