#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct edge {
    int s_end;
    int g_end;
    int weight;
}TEdge;

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




int count_edges(TGraph *G) {
    int count = 0;
    int n = G->num_vert;
    for(int i = 0; i < n; i++) {
        for(int j = i+1; j < n; j++) {
            if(G->adjacency_matrix[i][j] != INT_MAX) {
                count = count + 1;
            }
        }
    }
    return count;
}

TEdge *create_edge(int s_end, int g_end, int weight) {
    TEdge *new_edge;
    new_edge = (TEdge*)malloc(sizeof(TEdge));
    new_edge->s_end = s_end;
    new_edge->g_end = g_end;
    new_edge->weight = weight;

    return new_edge;
}

TEdge **create_edges_array(TGraph *G) {
    int num_edges = count_edges(G);
    int n = G->num_vert;
    int index = 0;
    TEdge **edges_array;
    edges_array = (TEdge**)malloc(num_edges * sizeof(TEdge));
    for(int i = 0; i < n; i++) {
        for(int j = i+1; j < n; j++) {
            if(i < j && G->adjacency_matrix[i][j] != INT_MAX) {
                TEdge *edge = create_edge(i,j,G->adjacency_matrix[i][j]);
                edges_array[index++] = edge;
            }
        }
    }
    return edges_array;
}

void bubble_sort(TEdge **edges_vec, int n) {
    TEdge *temp;
    for(int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if(edges_vec[i]->weight > edges_vec[j]->weight){
                temp = edges_vec[i];
                edges_vec[i] = edges_vec[j];
                edges_vec[j] = temp;
            }
        }
    }
}


TEdge **kruskal_algorithm(TGraph *G, int *tree_size) {
    int num_edges = count_edges(G); // number of edges of G
    int n = G->num_vert; // number of vertices of G
    
    TEdge **edges_array = create_edges_array(G); // create array of edges
    bubble_sort(edges_array,num_edges); // sort array of edges by weight
    
    TEdge **mst;
    mst = (TEdge**)malloc(num_edges * sizeof(TEdge)); // declare and allocate memory for the output spanning tree
    int tree_count = 0;

    /* now, we prepare the data structure for disjoint sets */

    TElement **pointers_dict;
    pointers_dict = (TElement**)malloc(n * sizeof(TElement));
    THead *new_set;
    for(int i = 0; i < n; i++) {
        new_set = make_set(i);
        pointers_dict[i] = new_set->first;
    }

    /* main loop */

    for(int i = 0; i < num_edges; i++) {
        int u = edges_array[i]->s_end;
        int v = edges_array[i]->g_end;
        THead *set_u = find_set(u,pointers_dict,n);
        THead *set_v = find_set(v,pointers_dict,n);
        if(set_u != set_v) {
            mst[tree_count++] = edges_array[i];
            sets_union(u,v,pointers_dict,n);
        }
    }

    *tree_size = tree_count;

    return mst; 

}


int main(int argc, char *argv[]) {

    int n = 8;
    TGraph *G;
    G = (TGraph*)malloc(sizeof(TGraph));
    G->num_vert = n;
    G->adjacency_matrix = (int**)malloc(n * n * sizeof(int));
    for(int i = 0; i < n; i++) {
        G->adjacency_matrix[i] = (int*)malloc(n * sizeof(int));
        for(int j = 0; j < n; j++) {
            G->adjacency_matrix[i][j] = INT_MAX;
        }
    }

    G->adjacency_matrix[0][1] = 3;
    G->adjacency_matrix[1][3] = 1;
    G->adjacency_matrix[1][2] = 7;
    G->adjacency_matrix[2][4] = 1;
    G->adjacency_matrix[1][4] = 5;
    G->adjacency_matrix[0][5] = 2;
    G->adjacency_matrix[2][5] = 8;
    G->adjacency_matrix[4][5] = 3;
    G->adjacency_matrix[5][6] = 4;
    G->adjacency_matrix[5][7] = 11;

    TEdge **edges_array = create_edges_array(G);
    
    int num_edges = count_edges(G);

    bubble_sort(edges_array,num_edges);
    
    for(int i = 0; i < 10; i++) {
        printf("%d, %d, %d\n", edges_array[i]->s_end, edges_array[i]->g_end, edges_array[i]->weight);
    }
    printf("\n");

    int *tree_size = (int*)malloc(sizeof(int)); // this pointer will store the number of edges of the mst.
    TEdge **mst = kruskal_algorithm(G, tree_size);
    
    printf("\nMST:\n");
    for(int i = 0; i < *tree_size; i++){
        printf("%d, %d, %d\n", mst[i]->s_end, mst[i]->g_end, mst[i]->weight);

    }


    return 0;

}