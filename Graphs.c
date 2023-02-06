#include<stdio.h>
#include<stdlib.h>

/* We implement a graph with linked lists. The main list contains all nodes (type Gr below). Each
node has a list of its adjacent nodes (this is a representation of the edges). Notice that the
Gr structure has a pointer for the first adjacent node, and the Adj structure has a pointer to the next
Adj node. 

In this way, we can represent both directed and undirected graphs. A graph is directed if and only if
it contains the reverse edge of any of its edges. 

The reader will notice that the first functions of search, insert and remove edges regards directed edges. For an undirected
graph, one just has to call these functions twice, one for each direction of the edge. */

typedef struct adj_node{
    int key;
    struct adj_node *next_adj;
}Adj;

typedef struct graph_node{
    int key;
    Adj *first_adj;
    struct graph_node *next_node;
}Gr;

/* BEGINNING OF STRUCTURAL FUNCTIONS */

Gr *Gr_initialize(){
    return NULL;
}


/* It is also very useful to have search functions. Searching for nodes is the same as searching in a linked list. */

Gr *search_node(Gr *g, int key){
    Gr *aux = g;
    while(aux){
        if(aux->key == key) return aux;
        aux = aux->next_node;
    }
    return NULL;
}

/* Node insertion in a graph implemented in this way is the same as insertion in a linked list.  */


Gr *insert_node(Gr *g, int key){
    if(search_node(g,key)) return g;
    Gr *new_node = (Gr*)malloc(sizeof(Gr));
    new_node->key = key;
    new_node->next_node = g;
    new_node->first_adj = NULL;
    return new_node;
}

/* Before inserting edges, it is useful to have a search function which verifies if the edge is already there. */

Adj *search_edge(Gr *g, int key_from, int key_to){
    Gr *node_from = search_node(g,key_from);
    Gr *node_to = search_node(g,key_to);
    if(!node_from || !node_to) return NULL;
    Adj *aux = node_from->first_adj;
    while(aux){
        if(aux->key == key_to) return aux;
        aux = aux->next_adj;
    }

    return NULL;
}


void insert_edge(Gr *g, int key_from, int key_to){
    Adj *edge = search_edge(g,key_from,key_to);
    if(edge) return;
    Gr *node_from = search_node(g,key_from);
    Adj *new_adj = (Adj*)malloc(sizeof(Adj));
    new_adj->key = key_to;
    new_adj->next_adj = node_from->first_adj;
    node_from->first_adj = new_adj;

}

void remove_edge(Gr *g, int key_from, int key_to){
    Adj * edge = search_edge(g,key_from,key_to);
    if(!edge) return;
    Gr *node_from = search_node(g,key_from);
    if((node_from->first_adj)->key == key_to){
        Adj *garb = node_from->first_adj;
        node_from->first_adj = garb->next_adj;
        free(garb);
        return;
    }
    Adj *aux = node_from->first_adj, *pre = NULL;
    while(aux->key != key_to){
        pre = aux;
        aux = aux->next_adj;
    }
    pre->next_adj = aux->next_adj;
    free(aux);
    return;
    
}

/* Now we write insert and remove functions for edges in undirected graphs. We always check the previous existence
of only one direction, since in an undirected graph this direction exists if and only if the reverse direction also exists. */

void undirected_insert_edge(Gr *g, int key1, int key2){
    Adj *edge = search_edge(g,key1,key2);
    if(edge) return;
    insert_edge(g,key1,key2);
    insert_edge(g,key2,key1);

}

void undirected_remove_edge(Gr *g, int key1, int key2){
    Adj *edge = search_edge(g,key1,key2);
    if(!edge) return;
    remove_edge(g,key1,key2);
    remove_edge(g,key1,key2);
}

/* END OF STRUCTURAL FUNCTIONS */


void print_graph(Gr *g){
    Gr *aux = g;
    Adj *aux_adj;
    while(aux){
        printf("%d--",aux->key);
        aux_adj = aux->first_adj;
        while(aux_adj){
            printf("%d--",aux_adj->key);
            aux_adj = aux_adj->next_adj;
        }
        printf("\n");
        aux = aux->next_node;
    }
}

/* THE "LIBRARY" */

/* We first write functions to count the number of nodes and of edges of a graph. */

int number_of_nodes(Gr *g){
    Gr *aux = g;
    int count = 0;
    while(aux){
        count++;
        aux = aux->next_node;
    }
    return count;
}

int number_of_edges_directed(Gr *g){
    Gr *iter_nodes = g;
    Adj *iter_edges = NULL;
    int count = 0;
    while(iter_nodes){
        iter_edges = iter_nodes->first_adj;
        while(iter_edges){
            count++;
            iter_edges = iter_edges->next_adj;
        }
        iter_nodes = iter_nodes->next_node;
    }
    return count;
}

int number_of_edges_undirected(Gr *g){
    return number_of_edges_directed(g) / 2;
}

/* We say that a graph g1 contains a graph g2 if every edge of g2 is also an edge of g1. Below, 
we implement a function to determine whether a graph contains another graph. */

int graph_contains(Gr *g1, Gr *g2){
    /* returns 1 if g1 contains g2. */

    Gr *iter = g2;
    Adj *iter_adj;
    while(iter){
        iter_adj = iter->first_adj;
        while(iter_adj){
            if(!search_edge(g1,iter->key,iter_adj->key)) return 0;
            iter_adj = iter_adj->next_adj;
        }
        iter = iter->next_node;
    }
    
    return 1;
}

int check_graph_equality(Gr *g1, Gr *g2){
    return graph_contains(g1,g2) && graph_contains(g2,g1);
}

int main(){

    Gr *g = Gr_initialize();
    g = insert_node(g,1);
    g = insert_node(g,3);
    g = insert_node(g,4);
    g = insert_node(g,5);
    insert_edge(g,1,3);
    insert_edge(g,3,4);
    insert_edge(g,4,1);
    insert_edge(g,4,5);


    


    return 0;
}