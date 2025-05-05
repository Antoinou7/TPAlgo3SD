#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


/*------------------------  BSTreeType  -----------------------------*/

struct _bstree {
    BinarySearchTree* parent;
    BinarySearchTree* left;
    BinarySearchTree* right;
    int key;
};

/*------------------------  BaseBSTree  -----------------------------*/

BinarySearchTree* bstree_create(void) {
    return NULL;
}

/* This constructor is private so that we can maintain the oredring invariant on
 * nodes. The only way to add nodes to the tree is with the bstree_add function
 * that ensures the invariant.
 */
BinarySearchTree* bstree_cons(BinarySearchTree* left, BinarySearchTree* right, int key) {
    BinarySearchTree* t = malloc(sizeof(struct _bstree));
    t->parent = NULL;
    t->left = left;
    t->right = right;
    if (t->left != NULL)
        t->left->parent = t;
    if (t->right != NULL)
        t->right->parent = t;
    t->key = key;
    return t;
}

void freenode(const BinarySearchTree* t, void* n) {
    (void)n;
    free((BinarySearchTree*)t);
}

void bstree_delete(ptrBinarySearchTree* t) {
    bstree_depth_postfix(*t, freenode, NULL);
    *t=NULL;
}

bool bstree_empty(const BinarySearchTree* t) {
    return t == NULL;
}

int bstree_key(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->key;
}

BinarySearchTree* bstree_left(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->left;
}

BinarySearchTree* bstree_right(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->right;
}

BinarySearchTree* bstree_parent(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->parent;
}

/*------------------------  BSTreeDictionary  -----------------------------*/

/* Obligation de passer l'arbre par référence pour pouvoir le modifier */
void bstree_add(ptrBinarySearchTree* t, int v) {
    if(*t == NULL){
        *t = bstree_cons(NULL, NULL, v);
    } else {
        BinarySearchTree* cur = *t;
        BinarySearchTree* par = NULL;
        while(cur != NULL){
            par = cur;

            if (v<cur->key){
                cur = cur->left;
            }

            else if(v>cur->key){
                cur = cur->right;
            }
            else{
                return; // value already in the tree
            }
        }

        if (v<par->key){
            par->left = bstree_cons(NULL, NULL,v);
            par->left->parent = par;
        }

        else if (v>par->key){
            par->right = bstree_cons(NULL, NULL,v);
            par->right->parent = par;
        }   
    }
}

const BinarySearchTree* bstree_search(const BinarySearchTree* t, int v) {
    while(!bstree_empty(t)){
        if(v<t->key){
            t = bstree_left(t);
        }
        else if (v>t->key){
            t = bstree_right(t);
        }
        else{
            return t; // value found
        }
    }
    return bstree_create(); 
}

const BinarySearchTree* bstree_successor(const BinarySearchTree* x) {
    assert(!bstree_empty(x));
    BinarySearchTree* succ = NULL;
    if(bstree_right(x)){
        succ = bstree_right(x);
        while(succ->left){
            succ = succ->left;
        }
    } else {
        succ = bstree_parent(x);
        while(succ && x == bstree_right(succ)){
            x = succ;
            succ = bstree_parent(succ);
        }
    }
    return succ;
}

const BinarySearchTree* bstree_predecessor(const BinarySearchTree* x) {
    assert(!bstree_empty(x));
    BinarySearchTree* pred = NULL;
    if(bstree_left(x)){
        pred = bstree_left(x);
        while(bstree_right(pred)){
            pred = pred->right;
        }
    } else {
        pred = bstree_parent(x);
        while(pred && (x == bstree_left(pred))){
            x = pred;
            pred = bstree_parent(pred);
        }
    }
    return pred;
}

void bstree_swap_nodes(ptrBinarySearchTree* tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));
    (void)tree; (void)from; (void)to;
}

// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree* t, ptrBinarySearchTree current) {
    assert(!bstree_empty(*t) && !bstree_empty(current));
    (void)t; (void)current;
}

void bstree_remove(ptrBinarySearchTree* t, int v) {
    (void)t; (void)v;
}

/*------------------------  BSTreeVisitors  -----------------------------*/

void bstree_depth_prefix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if (t==NULL){
        return;
    }

    f(t,environment);

    bstree_depth_prefix(t->left, f, environment);
    bstree_depth_prefix(t->right, f, environment);

}

void bstree_depth_infix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if(t==NULL){
        return;
    }

    bstree_depth_infix(t->left, f, environment);
    f(t,environment);
    bstree_depth_infix(t->right, f, environment);
}

void bstree_depth_postfix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if (t==NULL){
        return;
    }

    bstree_depth_postfix(t->left, f, environment);
    bstree_depth_postfix(t->right, f, environment);

    f(t,environment);
}

void bstree_iterative_breadth(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if (bstree_empty(t)) {
        return;
    }
    
    Queue* queue = create_queue();
    queue_push(queue, t); 
    
    while (!queue_empty(queue)) {
        if (!queue_top(queue)){
            queue_pop(queue);
        }
        else {
            f(queue_top(queue), environment);
            queue_push(queue, bstree_left(queue_top(queue)));
            queue_push(queue, bstree_right(queue_top(queue)));
            queue_pop(queue);
        }
    }
    delete_queue(&queue);
}

void bstree_iterative_depth_infix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    BinarySearchTree *current = (BinarySearchTree*)t;
    BinarySearchTree *next = bstree_parent( t );
    BinarySearchTree *prev = bstree_parent( t );
    while (!bstree_empty(current)) {
        if (prev == bstree_parent(current) ) {
            prev = current; next = bstree_left(current);
        }
        if ( bstree_empty(next) || prev == bstree_left(current) ) {
            f(current,environment); 
            prev = current; next = bstree_right(current);
        }
        if (bstree_empty(next) || prev == bstree_right(current) ) {
            prev = current; next = bstree_parent( current );
        }
        current = next;
    }
}


/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
    /* the collection the iterator is attached to */
    const BinarySearchTree* collection;
    /* the first element according to the iterator direction */
    const BinarySearchTree* (*begin)(const BinarySearchTree* );
    /* the current element pointed by the iterator */
    const BinarySearchTree* current;
    /* function that goes to the next element according to the iterator direction */
    const BinarySearchTree* (*next)(const BinarySearchTree* );
};

/* minimum element of the collection */
const BinarySearchTree* goto_min(const BinarySearchTree* e) {
	(void)e;
	return NULL;
}

/* maximum element of the collection */
const BinarySearchTree* goto_max(const BinarySearchTree* e) {
	(void)e;
	return NULL;
}

/* constructor */
BSTreeIterator* bstree_iterator_create(const BinarySearchTree* collection, IteratorDirection direction) {
	(void)collection; (void)direction;
	return NULL;
}

/* destructor */
void bstree_iterator_delete(ptrBSTreeIterator* i) {
    free(*i);
    *i = NULL;
}

BSTreeIterator* bstree_iterator_begin(BSTreeIterator* i) {
    i->current = i->begin(i->collection);
    return i;
}

bool bstree_iterator_end(const BSTreeIterator* i) {
    return i->current == NULL;
}

BSTreeIterator* bstree_iterator_next(BSTreeIterator* i) {
    i->current = i->next(i->current);
    return i;
}

const BinarySearchTree* bstree_iterator_value(const BSTreeIterator* i) {
    return i->current;
}

