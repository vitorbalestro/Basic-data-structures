#include <stdio.h>
#include <stdlib.h>

#define N 10

typedef struct tree_node {
    struct tree_node *parent;
    int rank;
    int label;

    struct tree_node *descendents[N-1];
    int num_descendents;
}TNode;

TNode *make_set(int i, TNode **pointers_dict) {
    TNode *new_node;
    
    new_node = (TNode*)malloc(sizeof(TNode));
    new_node->parent = new_node;
    new_node->rank = 0;
    new_node->label = i;
    new_node->num_descendents = 0;

    pointers_dict[i] = new_node;
    return new_node;
}

TNode *find_set(int i, TNode **pointers_dict) {
    TNode *node = pointers_dict[i];
    if(node != node->parent) {
        node->parent = find_set((node->parent)->label, pointers_dict);
    }
    return node;
}

void sets_union(int i, int j, TNode **pointers_dict){ 
    TNode *set_i = find_set(i, pointers_dict);
    TNode *set_j = find_set(j, pointers_dict);

    if(set_i->rank > set_j->rank) {
        set_j->parent = set_i;
        set_i->descendents[set_i->num_descendents] = set_j;
        set_i->num_descendents += 1;
    }

    else {
        set_i->parent = set_j;
        if(set_i->rank == set_j->rank){
            set_j->rank += 1;
        }
    }
}

void print_from_node(TNode *node) {
    printf("%d -- ", node->label);
    for(int i = 0; i < node->num_descendents; i++) {
        print_from_node(node->descendents[i]);
    }
}

void print_set(int i, TNode **pointers_dict) {
    TNode *node = pointers_dict[i];
    print_from_node(node);

}


int main(int argc, char *argv[]){

    TNode **pointers_dict;
    pointers_dict = (TNode**)malloc(N * sizeof(TNode));

    for(int i = 0; i < N; i++){
        make_set(i, pointers_dict);
    }
    sets_union(0,1,pointers_dict);
    
    for(int i = 0; i < N; i++){
        print_set(i,pointers_dict);
        printf("\n");
    }

    return 0;
}