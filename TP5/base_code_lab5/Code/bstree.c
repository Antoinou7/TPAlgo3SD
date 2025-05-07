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

typedef BinarySearchTree* (*AccessFunction)(const BinarySearchTree*);

typedef struct {
    AccessFunction first;
    AccessFunction second;
} ChildAccessors;


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

BinarySearchTree* find_next(const BinarySearchTree* x, ChildAccessors access){
    assert (!bstree_empty(x));
    BinarySearchTree* next = access.second((BinarySearchTree*)x);
    if (next){
        while(access.first(next)){
            next = access.first(next);
        }
    } else {
        next = bstree_parent(x);
        while(next && x == access.second(next)){
            x = next;
            next = bstree_parent(next);
        }
    }

    return next;

}



const BinarySearchTree* bstree_successor(const BinarySearchTree* x) {
    ChildAccessors access = {bstree_left , bstree_right};
    return find_next(x,access);
}

const BinarySearchTree* bstree_predecessor(const BinarySearchTree* x) {
    ChildAccessors access = {bstree_right , bstree_left};
    return find_next(x,access);
}

void bstree_swap_nodes(ptrBinarySearchTree* tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));
    if(bstree_parent(from)) {
        if(from->parent->left == from) from->parent->left = to;
        else from->parent->right = to;
    } 
    else *tree = to;
    
    if (bstree_parent(to)) {
        if(to->parent->left == to){
             to->parent->left = from;
        }
        else{
            to->parent->right = from;
        }
    }

    BinarySearchTree *new_parent = from->parent;
    from->parent = to->parent;
    to->parent = new_parent;
    if(from->left){
        from->left->parent = to;
    } 
    if(to->left) {
        to->left->parent = from;
    }
    new_parent = from->left;
    from->left = to->left;
    to->left = new_parent;
    if(from->right) {
        from->right->parent = to;
    }
    if(to->right){
        to->right->parent = from;
    }
    new_parent = from->right;
    from->right = to->right;
    to->right = new_parent;
}

// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree* t, ptrBinarySearchTree current) {
    assert(!bstree_empty(*t) && !bstree_empty(current));

    ptrBinarySearchTree temp;
    if(current->left == current->right) {
        temp = NULL;
    } else if (!current->left) {
        temp = current->right;
    } else if (!current->right) {
        temp = current->left;
    } else {
        ptrBinarySearchTree leaf = (BinarySearchTree*)bstree_successor(current);
        bstree_swap_nodes(t, current, leaf);
        temp = current->right;
    }
    if (temp) {
        temp->parent = current->parent;
    }
    if (!current->parent) {
        *t = temp;
    } else if (current->parent->left == current) {
        current->parent->left = temp;
    } else {
        current->parent->right = temp;
    }
    free(current);

}

void bstree_remove(ptrBinarySearchTree* t, int v) {
    BinarySearchTree *current = *t;
    while (current) {
        if (v < bstree_key(current)) {
            current = current->left;
        } else if (v > bstree_key(current)) {
            current = current->right;
        } else {
            bstree_remove_node(t, current);
            return;
        }
    }
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
    BinarySearchTree *next = bstree_parent(t);
    BinarySearchTree *prev = bstree_parent(t);
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
    while(e && bstree_left(e)){
        e = bstree_left(e);
    }
	return e;
}

/* maximum element of the collection */
const BinarySearchTree* goto_max(const BinarySearchTree* e) {
    while(e && bstree_right(e)){
        e = bstree_right(e);
    }
	return e;
}

/* constructor */
BSTreeIterator* bstree_iterator_create(const BinarySearchTree* collection, IteratorDirection direction) {
    BSTreeIterator* new_iterator = malloc(sizeof(BSTreeIterator));
    new_iterator->collection = collection;
    if (direction == forward){
        new_iterator->begin = goto_min;
        new_iterator->next = bstree_successor;
    } else {
        new_iterator->begin = goto_max;
        new_iterator->next = bstree_predecessor;
    }

    return new_iterator;
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

