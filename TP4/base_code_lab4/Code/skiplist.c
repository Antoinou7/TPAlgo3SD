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
	assert (nb_operations != NULL);

	*nb_operations = 0;
	LinkedElement* current = d->sentinelle;

	for(int i = d->nblevels - 1; i >= 0; i--) {
		while(current->next[i] != NULL && current->next[i]->value < value) {
			current = current->next[i];
			(*nb_operations)++;
		}
	}

	if(current->next[0] != NULL && current->next[0]->value == value) {
		(*nb_operations)++;
		return true;
	}

	return false;
}
