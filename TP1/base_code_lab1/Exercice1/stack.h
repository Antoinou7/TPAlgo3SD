#ifndef __STACK_H__
#define __STACK_H__
#include <stdio.h>
#include <stdbool.h>

/** Opaque definition of type Stack */
typedef struct s_stack Stack;

/** Create and initialize an empty stack.
 * This operator creates an empty stack of fixed capacity.
 */
Stack* stack(void);

/** Push a value on the stack.
 * @param s : the Stack to push on.
 * @param e : the value to push on the stack.
 * @return the modified stack.
 * @pre  Due to fixed capacity, push(s, e) is defined if \f$\neg\f$ overflow(s)
 * @note implemented using side effect on the stack. After execution, s is the same than the returned stack.
 */
Stack* push(Stack* s, int e);

/** Test if a stack is empty.
 * @param s : the Stack to test.
 * @return : true if the stack is empty, else false.
 */
bool empty(const Stack* s);

/** Pop a value from the stack.
 * @pre pop(s) defined if \f$\neg\f$ empty(s)
 * @param s : the Stack to pop from.
 * @return the modified stack.
 * @note implemented using side effect on the stack. After execution, s is the same than the returned stack.
 */
Stack* pop(Stack* s);

/** Return the top of a stack.
 * @pre top(s) defined if \f$\neg\f$ empty(s)
 * @param s : the Stack to examine.
 * @return the value at the top of the stack.
 */
int top(const Stack* s);

/** Return true if the stack will overflow on the next push.
 * @param s : the Stack to examine.
 * @return true if the number of element in the stack is equal to the stack capacity, else false.
 */
bool overflow(const Stack* s);

/** Dump the stack (print it) on the given FILE*
 * Dump the stack using 2 lines of integers.
 * The first line indicates the number of element in the stack.
 * The second line contains the elements from top to bottom.
 */
void dump(const Stack* s, FILE* f);

#endif
