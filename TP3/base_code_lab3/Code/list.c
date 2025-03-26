/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

#define SIZE_of_s l->sentinel->value

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement* previous;
	struct s_LinkedElement* next;
} LinkedElement;

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement* whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement* sentinel;
	int size;
};


/*-----------------------------------------------------------------*/

List* list_create(void) {
	//First, we allocate the List
	List* l = malloc(sizeof(List));

	//Then, we allocate the sentinel in the List
	l->sentinel = (LinkedElement*)malloc(sizeof(LinkedElement));

	//We initialize the sentinel as presented in the homework
	SIZE_of_s = 0 //Sentinel has no value
	l->sentinel->previous = l->sentinel->next = l->sentinel; //Sentinel points to itself since it is the only element in the list for now(list is obviously empty when creating the list)

	//Finally, we initialize the List
	l->size = 0; //List is empty for now

	return l;
}

/*-----------------------------------------------------------------*/

List* list_push_back(List* l, int v) {
	LinkedElement* new_element = malloc(sizeof(LinkedElement)); //We allocate the new element
	new_element->value = v; //We set the value of the new element to the value we want to add
	new_element->next=l->sentinel; //The new element is going to be the last element of the list, so it points to the sentinel
	new_element->previous=new_element->next->previous; //The new element points to the previous last element of the list
	
	new_element->previous->next=new_element; //The previous last element of the list points to the new element
	new_element->next->previous=new_element; //The sentinel points to the new element
	SIZE_of_s++; //We increment the size of the list
	return l;
}

/*-----------------------------------------------------------------*/

void list_delete(ptrList* l) {
if (l==NULL || *l==NULL)
{
	return;
}
	LinkedElement* e = (*l)->sentinel->next; //We start from the first element of the list
	while(e != (*l)->sentinel) //While we haven't reached the sentinel
	{
		LinkedElement* next = e->next; //We store the next element
		free(e); //We free the current element
		e = next; //We move to the next element
	}

	// When the elements of the list are freed, we can free the sentinel and the list
	free((*l)->sentinel);
	free(*l);

	*l=NULL;
}

/*-----------------------------------------------------------------*/

List* list_push_front(List* l, int v) {
	LinkedElement* e
	return l;
}

/*-----------------------------------------------------------------*/

int list_front(const List* l) {
	(void)l;
	return 0;
}

/*-----------------------------------------------------------------*/

int list_back(const List* l) {
	(void)l;
	return 0;
}

/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) {
	return l;
}

/*-----------------------------------------------------------------*/

List* list_pop_back(List* l){
	return l;
}

/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
	(void)v;
	(void)p;
	return l;
}

/*-----------------------------------------------------------------*/

List* list_remove_at(List* l, int p) {
	(void)p;
	return l;
}

/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
	(void)l;
	return p;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(const List* l) {
	(void)l;
	return false;
}

/*-----------------------------------------------------------------*/

int list_size(const List* l) {
	(void)l;
	return 0;
}

/*-----------------------------------------------------------------*/

List* list_map(List* l, ListFunctor f, void* environment) {
for (LinkedElement *e = l->sentinel->next; e != l->sentinel; e = e->next){
	e->value = f(e->value,environment);
}
	return l;
}

/*-----------------------------------------------------------------*/

List* list_sort(List* l, OrderFunctor f) {
	(void)f;
	return l;
}

