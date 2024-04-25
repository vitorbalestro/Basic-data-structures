#include <stdio.h>
#include <stdlib.h>

typedef struct element_node {
    int label;
    struct element_node *next;
    struct head_node *head;
}TElement;

typedef struct head_node {
    TElement *first;
    TElement *tail;
    int num_elems; 
    int exists_flag;  // 0 -> the set has been deleted by a union; 1 -> the set still exists. 
}THead;


THead *create_head() {
    THead *new_head;
    new_head = (THead*)malloc(sizeof(THead));
    new_head->first = NULL;
    new_head->tail = NULL;
    new_head->exists_flag = 1;
    return new_head;
}

TElement *create_node(int i) {
    TElement *new_element;
    new_element = (TElement*)malloc(sizeof(TElement));
    new_element->label = i;
    new_element->head = NULL;
    new_element->next = NULL;
    return new_element;
}


THead *make_set(int i) {
    /* To make a set containing only i, we create a head and, then, its first node (having i as label).
    This is O(1). */
    THead *set_head = create_head();
    TElement *element_node = create_node(i);
    
    set_head->first = element_node;
    set_head->tail = element_node;
    set_head->num_elems = 1;
    element_node->head = set_head;
    element_node->next = NULL;
    
    return set_head;
}

THead *find_set(int i, TElement **pointers_dict, int n){
    /* To find the head of the set containing the element i, we pass the pointers dictionary.
    It contains the node address whose label is i. Thus, we simply have to get its head pointer.
    This is O(1). */
    if(i >= n){
        printf("Error! %d is not a label.\n", i);
    }
    return pointers_dict[i]->head;

}

void sets_union(int i, int j, TElement **pointers_dict, int n) {
    THead *set_i = find_set(i, pointers_dict, n);
    THead *set_j = find_set(j, pointers_dict, n);
    TElement *step_node;
    
    if(set_i->num_elems <= set_j->num_elems){
        step_node = set_i->first;
        do {
            step_node->head = set_j;
            step_node = step_node->next;
        } while(step_node != NULL);
        (set_j->tail)->next = set_i->first;
        set_j->tail = set_i->tail;
        set_j->num_elems = set_i->num_elems + set_j->num_elems;
        set_i->exists_flag = 0;
    } else {
        step_node = set_j->first;
        do {
            step_node->head = set_i;
            step_node = step_node->next;
        } while(step_node != NULL);
        (set_i->tail)->next = set_j->first;
        set_i->tail = set_j->tail;
        set_i->num_elems = set_i->num_elems + set_j->num_elems;
        set_j->exists_flag = 0;
    }

}

void print_set_complete(int i, TElement **pointers_dict, int n) {
    if(i >= n) {
        printf("Error! %d is not a label\n", i);
        return;
    }
    THead *set = find_set(i, pointers_dict, n);
    printf("Head address: %p\n", set);
    TElement *step_node = set->first;
    do {
        printf("Label: %d, Address: %p\n", step_node->label, step_node);
        step_node = step_node->next;
    } while(step_node != NULL);
    printf("Number of elements: %d\n\n", set->num_elems);
}

int sets_count(THead **sets_array, int n) { // counts how many sets still exist
    int count = 0;
    
    for(int i = 0; i < n; i++){
        if(sets_array[i]->exists_flag == 1) count++;        
    }
    
    return count;
}

int main(int argc, char *argv[]) {

    /* we assume that there are n elements enumerated from 0 to n-1. */

    int n = 10;

    TElement **pointers_dict; // array whose i-th element is the address of its corresponding TElement.
    pointers_dict = (TElement**)malloc(sizeof(TElement) * n);

    THead **sets_array; // array with the pointers of the initial heads of the sets.
    sets_array = (THead**)malloc(sizeof(THead) * n);

    /* we make the initial n sets */
    THead *set;
    for(int i = 0; i < n; i++) {
        set = make_set(i);
        pointers_dict[i] = set->first;
        sets_array[i] = set;
    }

    /* print all initial sets. */
    for(int i = 0; i < 10; i++) {
      printf("Label: %d\n Node address: %p\n Head address: %p\n", pointers_dict[i]->label, pointers_dict[i], pointers_dict[i]->head);
    }

    sets_union(1,2,pointers_dict,n);
    print_set_complete(1, pointers_dict, n);
    printf("\n");
    
    sets_union(1,3, pointers_dict,n);
    print_set_complete(1,pointers_dict,n);
    printf("\n");
    
    print_set_complete(2,pointers_dict,n);
    printf("\n");

    sets_union(0,3,pointers_dict,n);
    print_set_complete(0,pointers_dict,n);
    
    sets_union(4,5,pointers_dict,n);
    print_set_complete(4,pointers_dict,n);

    sets_union(0,4,pointers_dict,n);
    print_set_complete(0,pointers_dict,n);

    
    printf("%d\n", sets_count(sets_array,n));

    return 0;
}