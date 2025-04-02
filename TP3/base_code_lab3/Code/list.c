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

typedef struct s_SubList {
	int value;
	struct s_LinkedElement* head;
	struct s_LinkedElement* tail;
} SubList;

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
	SIZE_of_s = 0; //Sentinel has no value
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
	LinkedElement* new_element = malloc(sizeof(LinkedElement)); //We allocate the new element
	new_element->value = v; //We set the value of the new element to the value we want to add
	new_element->previous=l->sentinel; //The new element is going to be the first element of the list, so its previous element is the sentinel
	new_element->next=new_element->previous->next; //The new element points to the first element of the list (which is the sentinel for now)

	new_element->next->previous=new_element; //The first element of the list points to the new element
	new_element->previous->next=new_element; //The sentinel points to the new element
	SIZE_of_s++; //We increment the size of the list

	return l;
}

/*-----------------------------------------------------------------*/

int list_front(const List* l) {
	return l->sentinel->next->value; //We return the value of the first element of the list
}

/*-----------------------------------------------------------------*/

int list_back(const List* l) {
	return l->sentinel->previous->value; //We return the value of the last element of the list
}

/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) {
	assert(!list_is_empty(l));
	LinkedElement* e = l->sentinel->next;
	 l->sentinel->next = e->next;
	 l->sentinel->next->previous = l->
	 	sentinel;
	 --(SIZE_of_s);
	 free(e);
	return l;
}

/*-----------------------------------------------------------------*/

List* list_pop_back(List* l){
	assert(!list_is_empty(l));
	LinkedElement* e = l->sentinel->previous;
	 l->sentinel->previous = e->previous;
	 l->sentinel->previous->next = l->
	 	sentinel;
	 --(SIZE_of_s);
	 free(e);
	return l;
}

/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
		assert((0<=p) && (p <= SIZE_of_s));
		LinkedElement* insert = l->sentinel->next;
		while (p--) insert = insert->next;
		LinkedElement* e = malloc(sizeof(LinkedElement));
		e->value = v; 
		e->next = insert;
		e->previous = insert->previous;
		e->previous->next = e;
		e->next->previous = e;
		++(SIZE_of_s);
		return l;
		} 

/*-----------------------------------------------------------------*/

List* list_remove_at(List* l, int p) {
	assert((0 <= p) && (p < SIZE_of_s));
	LinkedElement* remove = l->sentinel->next;
	while (p--) remove = remove->next;
	remove->previous->next = remove->next;
	remove->next->previous = remove->previous;
	free(remove);
	--(SIZE_of_s);
	return l;
	}



/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
	assert(!list_is_empty(l) && (0 <= p) && p < SIZE_of_s);
 LinkedElement* e = l->sentinel->next;
 for (;p;--p, e = e->next);
 return e->value;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(const List* l) {
return l->size==0;
}

/*-----------------------------------------------------------------*/

int list_size(const List* l) {
	return SIZE_of_s;
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


/*-----------------------------------------------------------------*/

SubList list_split(SubList l) {
	SubList* new_sublist = malloc(sizeof(SubList));
	new_sublist->head = l->head;
	new_sublist->tail = l->tail;



	// We browse the list until we reach the middle and we split it
	while(new_sublist->head->next != new_sublist->tail && new_sublist->head != new_sublist->tail) {
		new_sublist->head = new_sublist->head->next; // This line allows us to go to the next element
		if(new_sublist->head->next != new_sublist->tail) {
			new_sublist->tail = new_sublist->tail->previous; // This line allows us to go to the previous element
		}
	}

	return new_sublist;

}


/*-----------------------------------------------------------------*/

SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f) {
	SubList* new_sublist = malloc(sizeof(SubList));
	new_sublist->head = NULL;
	new_sublist->tail = NULL;


	while(leftlist.head != NULL || rightlist.head != NULL ) {
		if (leftlist.head == NULL) {
			if(new_sublist->head == NULL) {
				new_sublist->head = rightlist.head;
				new_sublist->tail= rightlist.tail;
			} else {
				new_sublist->tail->next = rightlist.head;
				rightlist.head->previous = new_sublist->tail;
				new_sublist->tail = rightlist.tail;
			}
			break; // We break the loop since we have reached the end of the left list
		} else if 	(rightlist.head == NULL){
			if (new_sublist->head == NULL) {
				new_sublist->head = leftlist.head;
				new_sublist->tail= leftlist.tail;
			} else {
				new_sublist->tail->next = leftlist.head;
				leftlist.head->previous = new_sublist->tail;
				new_sublist->tail = leftlist.tail;
			}
			break; // We break the loop since we have reached the end of the right list
		} else if (f(leftlist.head->value, rightlist.head->value)) {
			if (new_sublist->head == NULL) {
				new_sublist->head = leftlist.head;
				new_sublist->tail = leftlist.tail;
				leftlist.head = leftlist.head->next;
				new_sublist->tail->next = NULL;
			} else {
				new_sublist->tail->next = leftlist.head;
				leftlist.head->previous = new_sublist->tail;
				new_sublist->tail = leftlist.head;
				leftlist.head = leftlist.head->next;
				new_sublist->tail->next = NULL;

			}
		} else {
			if(new_sublist->head == NULL) {
				new_sublist->head = rightlist.head;
				new_sublist->tail = rightlist.head;
				rightlist.head = rightlist.head->next;
				new_sublist->tail->next = NULL;
			} else {
				new_sublist->tail->next = rightlist.head;
				rightlist.head->previous = new_sublist->tail;
				new_sublist->tail = rightlist.head;
				rightlist.head = rightlist.head->next;
				new_sublist->tail->next = NULL;
			}
		}
	}
	return new_sublist;
}
