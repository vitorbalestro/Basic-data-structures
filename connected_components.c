#include <stdio.h>
#include <stdlib.h>

typedef struct graph {
    int num_vert;
    int **adjacency_matrix;
}TGraph;

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

TElement **make_initial_sets(int num_verts, THead **sets_array) {
    TElement **pointers_dict;
    pointers_dict = (TElement**)malloc(num_verts * sizeof(TElement));
    THead *created_set;
    for(int i = 0; i < num_verts; i++) {
        created_set = make_set(i);
        pointers_dict[i] = created_set->first;
        sets_array[i] = created_set;
    }

    return pointers_dict;
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

void connected_components(TGraph *G, TElement **pointers_dict,THead **sets_array) {
    int n = G->num_vert;
   // TElement **pointers_dict = make_initial_sets(n, sets_array);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(G->adjacency_matrix[i][j] == 1) {
                if(find_set(i,pointers_dict,n) != find_set(j,pointers_dict,n)){
                    sets_union(i,j,pointers_dict,n);
                }
            }
        }
    }
}

void print_connected_components(THead **sets_array, int n) {
    TElement *node;
    for(int i = 0; i < n; i++){
        if(sets_array[i]->exists_flag == 1) {
            node = sets_array[i]->first;
            while(node != NULL) {
                printf("%d --", node->label);
                node = node->next;
            }
            printf("\n");
        }
        
    }
}

int main(int argc, char *argv[]){

    int n = 6;

    THead **sets_array;
    sets_array = (THead**)malloc(n * sizeof(THead));
    TElement ** pointers_dict;
    pointers_dict = make_initial_sets(n,sets_array);

    TGraph *G;
    G = (TGraph*)malloc(sizeof(TGraph));
    G->num_vert = n;
    G->adjacency_matrix = (int**)malloc(n * n * sizeof(int));
    
    for(int i = 0; i < n; i++) {
        G->adjacency_matrix[i] = (int*)malloc(n * sizeof(int));
        for(int j = 0; j < n; j++) {
            G->adjacency_matrix[i][j] = 0;
        }
    }


    G->adjacency_matrix[1][2] = 1;
    G->adjacency_matrix[1][3] = 1;
    G->adjacency_matrix[4][5] = 1;

    connected_components(G,pointers_dict,sets_array);
    print_connected_components(sets_array,n);

    return 0;

}