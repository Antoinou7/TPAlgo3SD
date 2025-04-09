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

#define SIZE_of_s l->size

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement* previous;
	struct s_LinkedElement* next;
} LinkedElement;

typedef struct s_SubList {
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

SubList list_mergesort(SubList l, OrderFunctor f);


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
	return l->size;
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
	if(list_is_empty(l) || list_size(l)==1) {
		return l;			// To improve complexity of the function, we check if the list is empty or if it has only one element, in those cases we don't need to use the function.
	}

	SubList new_sublist;
	new_sublist.head = l->sentinel->next;
	new_sublist.tail = l->sentinel->previous;

	new_sublist.head->previous = NULL; 
	new_sublist.tail->next = NULL;

	new_sublist = list_mergesort(new_sublist, f); // We use the function list_mergesort to sort the list

	List* listsorted = list_create(); 
	new_sublist.head->previous = listsorted->sentinel; 
	new_sublist.tail->next = listsorted->sentinel; 

	listsorted->sentinel->next = new_sublist.head;
	listsorted->sentinel->previous = new_sublist.tail;
	listsorted->size = l->size;

	free(l->sentinel);
	free(l); 



	return listsorted;
}


/*-----------------------------------------------------------------*/

SubList list_split(SubList l) {
    SubList new_subList; 

	if(l.head == NULL || l.head->next == NULL) {
		new_subList.head = NULL;
		new_subList.tail = NULL;
		return new_subList;
	}

	LinkedElement* head2 = l.head;
	LinkedElement* tail2 = l.head->next;
	
	while(tail2 != NULL && tail2->next != NULL) {
		head2 = head2->next;
		tail2 = tail2->next->next;
	}

    new_subList.head = head2;
    new_subList.tail = head2->next;
	
	head2->next->previous = NULL; // We set the previous pointer of the head of the new sublist to NULL
	head2->next = NULL; // We set the next pointer of the head of the new sublist to NULL
	
    return new_subList;
}	



/*-----------------------------------------------------------------*/

SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f) {
    SubList new_sublist;

	LinkedElement* left = leftlist.head;
	LinkedElement* right = rightlist.head;
	LinkedElement* head = NULL;
	LinkedElement* tail = NULL;


    while (left != NULL && right != NULL) {
        LinkedElement* next_element = NULL;
        if (f(left->value,right->value)) {
            next_element = left;
            left = left->next;
        } else {
            next_element = right;
            right = right->next;
        }
		next_element->previous = tail;

		if(tail !=NULL){
			tail->next = next_element;
		} else {
			head = next_element;
		}
		tail = next_element;
    }

	while(left != NULL) {
		left->previous = tail;
		if (tail){
			tail->next = left;
		}
		else {
			head = left;
		}
		tail = left;
		left = left->next;
	}
	while(right != NULL) {
		right->previous = tail; 
		if(tail){
			tail->next = right;
		}
		else {
			head = right;
		}
		tail = right;
		right = right->next;
	}

	if(tail){
		tail->next = NULL; // We set the next pointer of the tail of the new sublist to NULL
	}

	new_sublist.head = head;
	new_sublist.tail = tail;

    return new_sublist;
}


/*-----------------------------------------------------------------*/

SubList list_mergesort(SubList l, OrderFunctor f) {
	if (l.head == NULL || l.head->next == NULL) {
		return l;
	}


	SubList splitlist = list_split(l); // As the subject asked, we split the list in two
	
	SubList leftlist;
	leftlist.head = l.head;
	leftlist.tail = splitlist.head;
	
	
	SubList rightlist;
	rightlist.head = splitlist.tail;
	rightlist.tail = l.tail;

	leftlist = list_mergesort(leftlist, f); // We use the function list_mergesort to sort the two separated lists
	rightlist = list_mergesort(rightlist, f);

	SubList finallist = list_merge(leftlist,rightlist,f); // We use the function list_merge to merge the two separated lists

	return finallist; // We return the final list
}