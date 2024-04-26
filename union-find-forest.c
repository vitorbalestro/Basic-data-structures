#include <stdio.h>
#include <stdlib.h>


typedef struct tree_node {
    struct tree_node *parent;
    int rank;
    int label;

    struct tree_node **children;
    int num_children;
}TNode;

TNode *make_set(int i, TNode **pointers_dict, int n) {
    TNode *new_node;
    
    new_node = (TNode*)malloc(sizeof(TNode));

    new_node->parent = new_node;
    new_node->rank = 0;
    new_node->label = i;
    new_node->num_children = 0;
    new_node->children = (TNode**)malloc(n * sizeof(TNode));

    pointers_dict[i] = new_node;
    return new_node;
}

TNode *find_set(int i, TNode **pointers_dict) {
    TNode *node = pointers_dict[i];
    while(node != node->parent) {
       node = node->parent;
    }
    return node;
}

/* TNode *find_set_compression(int i, TNode **pointers_dict) {
    TNode *node = pointers_dict[i];
    if(node != node->parent){
        node->parent = find_set_compression((node->parent->)label, pointers_dict);
    }
    return node->parent;
} */

void sets_union(int i, int j, TNode **pointers_dict){ 
    TNode *set_i = find_set(i, pointers_dict);
    TNode *set_j = find_set(j, pointers_dict);

    if(set_i->rank > set_j->rank) {
        set_j->parent = set_i;
        set_i->children[set_i->num_children] = set_j;
        set_i->num_children += 1;
    }

    else {
        set_i->parent = set_j;
        set_j->children[set_j->num_children] = set_i;
        set_j->num_children += 1;
        if(set_i->rank == set_j->rank){
            set_j->rank += 1;
        }
    }
}

void print_from_node(TNode *node) {
    printf("%d -- ", node->label);
    for(int i = 0; i < node->num_children; i++) {
        print_from_node(node->children[i]);
    }
}

void print_set(int i, TNode **pointers_dict) {
    TNode *node = pointers_dict[i];
    print_from_node(node);

}


int main(int argc, char *argv[]){

    int n = 10;
    TNode **pointers_dict;
    pointers_dict = (TNode**)malloc(n * sizeof(TNode));

    for(int i = 0; i < n; i++){
        make_set(i, pointers_dict, n);
    }

    sets_union(0,1,pointers_dict);
    sets_union(0,2, pointers_dict);

    sets_union(3,4,pointers_dict);
    sets_union(4,9,pointers_dict);
    
    sets_union(3,0,pointers_dict);
    
    sets_union(6,7, pointers_dict);

    for(int i = 0; i < n; i++){
        if(pointers_dict[i]->parent == pointers_dict[i]){
            print_set(i,pointers_dict);
            printf("\n");
        }
    }

    return 0;
}