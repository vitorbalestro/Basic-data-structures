#include<stdio.h>
#include<stdlib.h>

/* In this material, we implement linked lists which are "singly" linked. This means that each node has a pointer to the
next node, but not to the previous. Also, we consider only non-circular lists. The ending of a list will be represented by a 
null pointer for the next node. */

/* The reference to a linked list is made always to the pointer of its first node. */

typedef struct linked_list_node {
    int key;
    struct linked_list_node *next;
}LL;

LL *LL_initialize(){
    return NULL;
}

/* STRUCTURAL FUNCTIONS */

/* We introduce the basic functions to construct linked lists. First, we implement functions to insert elements in a linked list.
There are two basic ways of doing that (without considering ordering of the elements): we may insert an element in the head of the list
or in the tail of the list. */

/* Inserting an element in the head of the list forces us to change the address of the first node of the list. For that reason, 
we implement a function which returns the new starting address of the list. */

LL *LL_insert_head(LL *l, int n){
    LL *new_node = (LL*)malloc(sizeof(LL));
    new_node->key = n;
    new_node->next = l;
    return new_node;
}

/* To insert an element in the tail of a linked list, we also consider a function which returns the address of the first node. We do that 
because the list under consideration may be empty at first. */

LL *LL_insert_tail(LL *l, int n){
    LL *new_node = (LL*)malloc(sizeof(LL));
    new_node->key = n;
    new_node->next = NULL;
    if(l == NULL) return new_node;
    LL *aux = l;
    while(aux->next != NULL) aux = aux->next;
    aux->next = new_node;
    return l;
}

/* We can also insert elements respecting an increasing (or decreasing) order. We implement the increasing order insertion, 
and the decreasing order insertion is completely analogous.*/

LL *LL_insert_increasing(LL *l, int n){
    LL *new_node = (LL*)malloc(sizeof(LL));
    new_node->key = n;
    new_node->next = NULL;
    if(l == NULL) return new_node;
    if(l->key >= n) {
        new_node->next = l;
        return new_node;
    }
    LL *pre = NULL;
    LL *aux = l;
    while(aux != NULL && aux->key < n){
        pre = aux;
        aux = aux->next;
    }
    new_node->next = aux;
    pre->next = new_node;
    return l;
}

/* The next functions searches for a given element in a given list. It returns the address of the first node where the element appears.
If the element is not found, then the function returns NULL. */

LL *LL_search(LL *l, int n){
    LL *aux = l;
    while(aux != NULL){
        if(aux->key == n) return aux;
        aux = aux->next;
    }
    return NULL;
}

/* We also may implement functions to remove a given element from a linked list. We implement first a function
which removes the first ocurrence of the given element, and after we write a function which remove all ocurrences 
of the given element.
Again, the address of the list may be modified, since the element to be removed might be in the first node.
Hence we construct a function which returns the first node of the resulting list. */

LL *LL_remove(LL *l, int n){
    LL *rem = LL_search(l,n);
    if(!rem) return l;
    LL *aux;
    if(l == rem){
        aux = l->next;
        free(l);
        return aux;
    }
    LL *pre = NULL;
    aux = l;
    while(aux != rem){
        pre = aux;
        aux = aux->next;
    }
    pre->next = aux->next;
    free(rem);
    return l;
}

/* The next function removes all ocurrences of a given integer in a linked list. */

LL *LL_remove_all_ocurrences(LL *l, int n){
    if(!l) return NULL;
    LL *aux = l->next, *pre = l, *rem;
    while(aux != NULL){
        if(aux->key == n){
            pre->next = aux->next;
            rem = aux;
            aux = aux->next;
            free(rem);

        }
        else{
            pre = aux;
            aux = aux->next;
        }
    }
    if(l->key == n){
        aux = l->next;
        free(l);
        return aux;
    }
    else return l;
}

/* Now we implement a function which removes all duplicate values of a list. Only the first ocurrence of each element remains in
the list. */

LL *LL_remove_duplicates(LL *l){
    LL *aux = l;
    while(aux != NULL){
        aux->next = LL_remove_all_ocurrences(aux->next, aux->key);
        aux = aux->next;
    }
    return l;
}

/* The function below deallocates the memory reserved to a given linked list. */

void LL_free(LL *l){
    LL *aux = l, *pre = NULL;
    while(aux!=NULL){
        pre = aux;
        aux = aux->next;
        free(pre);
    }
}

/* We can also use a recursive function to deallocate the memory reserved to a given linked list.
This is done below. */

void LL_free_rec(LL *l){
    if(l){
        LL_free_rec(l->next);
        free(l);
    }
}

/* END OF STRUCTURAL FUNCTIONS */

/* Now we write a simple function to print the elements of a linked list (in the order that they appear). Notice that
it is trivial to modify this code to get backwards printing. */

void LL_print(LL *l){
    if(l){
        printf("%d--",l->key);
        LL_print(l->next);
    }
}

/* The next function returns a copy of a given list. */

LL *LL_copy(LL *l){
    LL *iter = l;
    LL *output = LL_initialize();
    while(iter){
        output = LL_insert_tail(output, iter->key);
        iter = iter->next;
    }
    return output;
}

void LL_recursive_copy(LL *l, LL **output){
    if(l){
        *output = LL_insert_tail(*output,l->key);
        LL_recursive_copy(l->next,&(*output));
    }
}

int LL_number_of_nodes(LL *l){
    if(!l) return 0;
    LL *aux = l;
    int count = 0;
    while(aux!=NULL){
        count++;
        aux = aux->next;
    }
    return count;
}

/* The next function reverses the order of a given linked list. We do it using the original nodes of the list. 
Hence, the memory requirements restrict to the auxiliary pointers. */

LL *LL_reverse(LL *l){
    if(l == NULL || l->next == NULL) return l;
    LL *first = l, *aux = l->next;
    while(aux != NULL){
        l->next = aux->next;
        aux->next = first;
        first = aux;
        aux = l->next;
    }
    return first;
}

/* We can also use a recursive function to revert a linked list. The idea of the algorithm is as follows.
At each node p, let the "forward list p" be the list starting at p->next. For each node p, if we can get the
reverted forward list of p, then we just have to move p to the ending of that list. As the "basis" of the recursion, we
notice that if the list contains a single node, then its reverse list is itself. */

LL *LL_revert_rec(LL *l){
    if(l){
        if(l->next){
            LL *output = LL_revert_rec(l->next);
            (l->next->next) = l;
            l->next = NULL;
            return output;
        }
        else return l;
    }
    else return NULL;
}

/* The function below merge two sorted (in increasing order) lists. It returns a new list containing all 
elements of the two given lists in increasing order. */

LL *LL_merge_sorted_lists(LL *l1, LL*l2){
    LL *aux1 = l1, *aux2 = l2;
    LL *output_list = LL_initialize();
    
    while (aux1 && aux2){
        while(aux1 && aux2 && (aux1->key <= aux2->key)){
            output_list = LL_insert_tail(output_list,aux1->key);
            aux1 = aux1->next;
        }
        while(aux2 && aux1 && (aux2->key <= aux1->key)){
            output_list = LL_insert_tail(output_list,aux2->key);
            aux2 = aux2->next;
        }
    }
    while(aux1 != NULL){
        output_list = LL_insert_tail(output_list, aux1->key);
        aux1 = aux1->next;
    }
    while(aux2 != NULL){
        output_list = LL_insert_tail(output_list, aux2->key);
        aux2 = aux2->next;
    }
    return output_list;
}

/* Next, we implement a function to find the middle element of a linked list in only one pass. 
If there is an even number of nodes (n, say), then the function returns the node in the (n/2+1)-th position. */

LL *LL_middle_node(LL *l){
    LL *aux = l, *candidate = l;
    int candidate_update = 0;
    while(aux != NULL){
        if(!candidate_update) candidate_update = 1;
        else{
            candidate = candidate->next;
            candidate_update = 0;
        }
        aux = aux->next;
    }
    return candidate;
}

/* Now that we have a way of finding the middle node of a linked list, we may insert a new node in the middle. We convention that 
if the list has a unique element, then the new element is inserted in the tail of the list. We also convention that if the list has an odd 
number of nodes, the new node is inserted between the middle node and its predecessor. */

LL *LL_insert_middle(LL *l, int n){
    LL *new_node = (LL*)malloc(sizeof(LL));
    new_node->key = n;
    new_node->next = NULL;
    if(!l) return new_node;
    if(!l->next){
        l->next = new_node;
        return l;
    }
    LL *aux = l, *candidate = l, *pre_candidate = LL_initialize();
    int candidate_update = 0;
    while(aux != NULL){
        if(!candidate_update) candidate_update = 1;
        else{
            candidate_update = 0;
            pre_candidate = candidate;
            candidate = candidate->next;
        }
        aux = aux->next;
    }
    pre_candidate->next = new_node;
    new_node->next = candidate;
    return l;
}

/* Next, we write a function which returns the n-th node from the end of a given linked list. We do that in a 
single pass through the list. We convention that the last node is the first from the end. 
If the list has less than n nodes, then the function returns NULL. Roughly speaking,
the strategy is as follows: we initialize a pointer "candidate" to the first node of the list, and iterate it from the
(n+1)-th node until a NULL pointer is found. At each iteration, we update the candidate pointer to the next node. */

LL *LL_nth_from_end(LL *l,int n){
    LL *candidate = l, *iter = l;
    int i;
    for(i=0;i<n-1;i++){
        if(iter != NULL) {
            iter = iter->next;
        }
        else return NULL;
    }
    if(iter == NULL) return NULL;
    while(iter->next != NULL){
        iter = iter->next;
        candidate = candidate->next;
    }
    return candidate;
}

/* Now that we know how to find the n-th node from the end, we can also write a code which remove this node. 
If the list has less than n nodes, then the list remains unchanged. */

LL *LL_remove_nth_from_end(LL *l,int n){
    LL *candidate = l, *iter = l, *pre = NULL;
    int i;
    for(i=0;i<n-1;i++){
        if(iter != NULL) {
            iter = iter->next;
        }
        else return l;
    }
    if(iter == NULL) return l;
    while(iter->next != NULL){
        iter = iter->next;
        pre = candidate;
        candidate = candidate->next;
    }
    if(candidate == l){
        LL *aux = l->next;
        free(l);
        return aux;
    }
    pre->next = candidate->next;
    free(candidate);
    return l;
}

/* For the sake of completeness, we present bubble sort algorithm for linked lists. Unlike other sorting methods, 
bubble sort does not demand random access to the list's nodes. 
Hence, bubble sort is easy to implement for linked lists (there are more efficient sorting methods, although)). */

LL *LL_bubble_sort(LL *l){
    if(!l) return NULL;
    LL *outer_iter = l, *inner_iter = l->next;
    int aux;
    while(outer_iter->next){
        inner_iter = outer_iter->next;
        while(inner_iter){
            if(inner_iter->key < outer_iter->key){
                aux = inner_iter->key;
                inner_iter->key = outer_iter->key;
                outer_iter->key = aux;
            }
            inner_iter = inner_iter->next;
        }
        outer_iter = outer_iter->next;
    }
    return l;
}

/* The next function is not really a function to manipulate a linked list. 
Imagine that linked lists represent numbers. For example, the linked list 2-->3-->4-->NULL represents
the number 234. We want to construct a function which receives two such linked lists and returns the linked
list representing the sum of the numbers represented by them. We can do that using a stack.*/

/* IMPLEMENTING STACKS USING LINKED LISTS */

/* The top of the stack is the first element of the linked list. Hence, the stack push function is simply
and insertion in the head of the linked list.  */

typedef struct stack_node {
    int key;
    struct stack_node *below;
}ST;

ST *ST_initialize(){
    return NULL;
}

ST *ST_push(ST *s,int n){
    ST *new_node = (ST*)malloc(sizeof(ST));
    new_node->key = n;
    new_node->below = s;
    return new_node;
}

ST *ST_pop(ST **s){
    if(*s){
        ST *aux = *s;
        *s = (*s)->below;
        return aux;
    }
    return NULL;
}

void ST_print(ST *s){
    if(s){
        printf("%d-->", s->key);
        ST_print(s->below);
    }
}

/* END OF THE STACK IMPLEMENTATION*/

LL *LL_representation_sum(LL *l1, LL *l2){

    int l1_size = LL_number_of_nodes(l1);
    int l2_size = LL_number_of_nodes(l2);
    if(l1_size > l2_size){
        for(int i = 0; i< l1_size-l2_size;i++) l2 = LL_insert_head(l2,0);
    }
    if(l2_size > l1_size){
        for(int i = 0; i<l2_size-l1_size;i++) l1 = LL_insert_head(l1,0);
    }
    
    LL *aux1 = l1, *aux2 = l2, *output = LL_initialize();
    int amount, alg, carry = 0;
    ST *aux_stack = ST_initialize();

    while(aux1 || aux2){
        if(aux1 && aux2){
            aux_stack = ST_push(aux_stack,aux1->key+aux2->key);
            aux1 = aux1->next;
            aux2 = aux2->next;
        }
        else if(aux1){
            aux_stack = ST_push(aux_stack,aux1->key);
            aux1 = aux1->next;
        }
        else if(aux2){
            aux_stack = ST_push(aux_stack,aux2->key);
            aux2 = aux2->next;
        }
    }
    while(aux_stack){
        amount = ST_pop(&aux_stack)->key + carry;
        alg = amount % 10;
        carry = (int)(amount - alg) / (int)10;
        output = LL_insert_head(output, alg);

    }
    return output;
}

/* We can also use a stack to check whether a given linked list is a palindrome. We store the first half of our list
in a stack, and then we simultaneously pop the stack and iterate in the second half of the list comparing the elements. Notice that 
we have to pass through the list twice: the first time to count how many elements it has, and the second time to compare the first half
with the second half (using the stack). The memory requirement is half the size of the list (to store the auxiliary stack). */

int LL_check_palindrome(LL *l){
    int size = LL_number_of_nodes(l);
    LL *aux = l;
    ST *first_half = ST_initialize();
    for(int i = 0;i<size/2;i++){
        first_half = ST_push(first_half,aux->key);
        aux = aux->next;
    }
    if(size % 2 == 1){
        aux = aux->next;
    }
    while(first_half){
        if(ST_pop(&first_half)->key != aux->key) return 0;
        aux = aux->next;
    }
    return 1;
}

/* Now we want to implement a function which does the following: given a list l and an integer x, obtain a reordering of the list in such a way
that any element lesser than or equal to x appears before any element greater than x. We use a stack to do that: we push all elements of the 
list to an auxiliary stack, and then we pop each element positioning it in the head or in the tail of the output list according to them being
lesser or greater than x. */

LL *LL_below_above_reorder(LL *l, int x){
    LL *output = LL_initialize();
    ST *aux_stack = ST_initialize();
    LL *iter = l;
    int element;
    while(iter){
        aux_stack = ST_push(aux_stack,iter->key);
        iter = iter->next;
    }
    while(aux_stack){
        element = ST_pop(&aux_stack)->key;
        if(element <= x) output = LL_insert_head(output, element);
        else output = LL_insert_tail(output, element);
    }
    return output;
}

/* Of course, there is also an easy way of reversing a linked list using a stack. However, the nodes of the stack must
contain a pointer to a linked list node, instead of an integer.*/

typedef struct stl_node{
    LL *key;
    struct stl_node *below;
}STL;

STL *STL_initialize(){
    return NULL;
}

STL *STL_push(STL *s, LL *ll_node){
    STL *new_node = (STL*)malloc(sizeof(STL));
    new_node->key = ll_node;
    new_node->below = s;
    return new_node;
}

STL *STL_pop(STL **s){
    if(*s){
        STL *aux = *s;
        *s = (*s)->below;
        return aux;
    }
    return NULL;
}

LL *LL_reverse_with_stack(LL *l){
    LL *iter = l;
    STL *aux_stack = STL_initialize();
    while(iter){
        aux_stack = STL_push(aux_stack,iter);
        iter = iter->next;
    }
    LL *output_iter = STL_pop(&aux_stack)->key;
    LL *output = output_iter;
    while(aux_stack){
        output_iter->next = STL_pop(&aux_stack)->key;
        output_iter = output_iter->next;
    }
    output_iter->next = NULL;
    return output;
    
}

int main(void){

    
    return 0;
}