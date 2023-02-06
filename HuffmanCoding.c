#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define max_input_size 1000
#define max_output_size 8*max_input_size

/* HUFFMAN CODING IMPLEMENTATION */

/* This is an implementation of Huffman Coding, which is used for data compression. The objective here is didactic, and 
in reality the code is useless for real data compression. This happens because the code uses characters '0' and '1' instead
of bits. Hence, the code is bigger that the input string. Any attempt to turn it into "real" code should start by correcting
this issue (as far as I know, there is no such a thing as "bit array" in C). Also, some algorithms may not be in optimal
time/space complexity. Improvements are welcome. */



/* We define three data structures:

(1) A coding tree, whose leaf nodes contain characters and their frequencies, and the non-leaf nodes contain 
sums of frequencies 

(2) A priority queue (minimum heap) which we use to iteratively construct the coding tree.

(3) A linked list of Node pointers to keep track of the memory address of the nodes containing each character. 
This list will be constructed at the same time as the coding tree.

Our first task is to build a coding tree from a given sorted frequency map. */

/* TREE NODE DATA STRUCTURE*/

typedef struct tree_node{
    int freq;
    int is_leaf;
    char character;
    struct tree_node *left, *right, *parent;
}Node;


Node *Node_initialize(){
    return NULL;
}

Node *create_nonleaf_node(int freq, Node *left, Node *right, Node *parent){
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->freq = freq;
    new_node->is_leaf = 0;
    new_node->left = left;
    new_node->right = right;
    new_node->parent = parent;
    return new_node;
}

Node *create_leaf_node(char c, int freq, Node *parent){
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->freq = freq;
    new_node->character = c;
    new_node->is_leaf = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = parent;
    return new_node;
}

int max(int a, int b){
    if(a>b) return a;
    return b;
}

int tree_height(Node *t){
    if(!t) return -1;
    return 1 + max(tree_height(t->left),tree_height(t->right));
}

/* END OF TREE NODE DATA STRUCTURE */


/* PRIORITY QUEUE DATA STRUCTURE */

typedef struct priority_queue{
    Node *queue[129]; /* ASCII has 128 characters */
    int size;
}PQ; /* PQ stands for Priority Queue */

PQ *PQ_initialize(){
    return NULL;
}

int parent(int i){
    if(i>0) return (i-1)/2;
    return -1;
}

int left_child(int i){
    return 2*i+1;
}

int right_child(int i){
    return 2*i+2;
}

void min_heapfy(PQ *q, int i){
    int smallest = i, left = left_child(i), right = right_child(i);
    if(left < q->size && q->queue[left]->freq < q->queue[smallest]->freq) smallest = left;
    if(right < q->size && q->queue[right]->freq < q->queue[smallest]->freq) smallest = right;
    if(smallest != i){
        Node *aux = q->queue[i];
        q->queue[i] = q->queue[smallest];
        q->queue[smallest] = aux;
        min_heapfy(q,smallest);
    } 
}

void build_priority_queue(PQ *q){
    int last_parent = parent(q->size-1);
    int j;
    for(j=last_parent;j>=0;j--) min_heapfy(q,j);
}

Node *remove_minimum(PQ *q){
    Node *output = q->queue[0];
    q->queue[0] = q->queue[q->size-1];
    min_heapfy(q,0);
    q->size = q->size-1;
    return output;
}

void priority_queue_insert(PQ *q,Node *node){
    q->queue[q->size] = node;
    q->size = q->size+1;
    build_priority_queue(q);
}

/* END OF PRIORITY QUEUE DATA STRUCTURE */


/* LINKED LIST OF NODES DATA STRUCTURE */

typedef struct list_node{
    Node *tree_node;
    struct list_node *next;
}LL; /* LL stands for Linked List. */

LL *LL_initialize(){
    return NULL;
}

LL *LL_insert(LL *l, Node *tree_node){
    LL *new_ll_node = (LL*)malloc(sizeof(LL));
    new_ll_node->tree_node = tree_node;
    new_ll_node->next = l;
    return new_ll_node;
}  

/* END OF LINKED LIST OF NODES DATA STRUCTURE */


/* CONSTRUCTION OF THE CODING TREE (FROM PRIORITY QUEUE) */

/* REMARK: Always declare a LL pointer to be passed as parameter by reference. This pointer
we hold the linked list of addresses of the character nodes. Do NOT allocate memory for the list of addresses
before calling the function. The memory is automatically allocated by the insert method.
Do as below (where q is the priority queue):

LL *list_of_addresses = LL_initialize();
Node *tree = tree_builder(q,&list_of_addresses);

*/

Node *tree_builder(PQ *q, LL **list_of_addresses){
    build_priority_queue(q); /* We make sure q is a priority queue. */
    if(q->size==1){
        *list_of_addresses = LL_insert(*list_of_addresses,q->queue[0]);
        return q->queue[0];
    }
    while(q->size > 1){
        
        Node *left = remove_minimum(q);
        build_priority_queue(q);
        Node *right = remove_minimum(q);
        Node *new_node = create_nonleaf_node(left->freq+right->freq,left,right,NULL);
        priority_queue_insert(q,new_node);
        build_priority_queue(q);
        left->parent = new_node;
        right->parent = new_node;
        if(left->is_leaf) *list_of_addresses = LL_insert(*list_of_addresses,left);
        if(right->is_leaf) *list_of_addresses = LL_insert(*list_of_addresses,right);
        
    }
    return remove_minimum(q);
}

/* END OF CONSTRUCTION OF THE CODING TREE */


/* GETTING THE CODE OF A GIVEN CHARACTER */

Node *char_address(char c, LL *list){
    LL *iter = list;
    while(iter && iter->tree_node->character != c) iter = iter->next;
    if(iter) return iter->tree_node;
    return NULL;
}

void get_code(char c, Node *t, LL *list, char *code){
    int i = 0;
    Node *address = char_address(c,list);
    Node *aux = address, *parent;
    if(aux == t){
        *code = '0';
        *(code+1) = '\0';
    }
    else{
        while(aux!=t){
            parent = aux->parent;
            if(aux == parent->left) *(code+i) = '0';
            else if(aux == parent->right) *(code+i) = '1';
            i++;
            aux = parent;
        }
        *(code+i) = '\0';

        /* Now we have to revert the obtained code!*/
        
        int j;
        char swap_aux;
        for(j=0;j<i/2;j++){
            swap_aux = *(code+j);
            *(code+j) = *(code+i-1-j);
            *(code+i-1-j) = swap_aux;
        }
    }
    

    
}

/* END OF "GETTING THE CODE OF A GIVEN CHARACTER" */


/* CONSTRUCTING THE PRIORITY QUEUE FROM AN INPUT STRING */

Node *search_char(PQ *q, char c){
    int i;
    for(i=0;i<q->size;i++){
        if(c == q->queue[i]->character) return q->queue[i];
    }
    return NULL;
}

void insert_char_into_PQ(PQ *q, char c){
    Node *new_node = create_leaf_node(c,1,NULL);
    q->queue[q->size] = new_node;
    q->size = q->size+1;
}

PQ *create_PQ_from_string(char *string){
    PQ *new_pq = PQ_initialize();
    new_pq = (PQ*)malloc(sizeof(PQ));
    Node *aux;
    int i = 0;
    while(*(string+i) != '\0'){
        if(!search_char(new_pq,*(string+i))) insert_char_into_PQ(new_pq,*(string+i));
        else{
            aux = search_char(new_pq,*(string+i));
            aux->freq = aux->freq+1;
        }
        i++;
    }
    build_priority_queue(new_pq);
    return new_pq;
}

/* END CONSTRUCTING THE PRIORITY QUEUE FROM AN INPUT STRING */


/* Now we can read an input string, transform it into a priority queue, construct the coding tree
and get the code of each character. Recall that the priority queue is "destroyed" in the process of constructing
the tree, but we keep track of the existing characters in the list of addresses (which is what we use to get
the code of each character, by the way). Our next step is to construct a map of codes, which inform the generated code
of each character.  */

/* MAP OF CODES */

typedef struct map_pair{
    char character;
    char *code;
}Pair;

typedef struct map_of_codes{
    Pair *map[129];
    int map_size;
}Map;

Pair *Pair_initialize(){
    return NULL;
}

Map *Map_initialize(){
    return NULL;
}

Map *build_map_of_codes(LL *list_of_addresses, Node *t){
    int max_code_size = tree_height(t);
    /* The height of the tree is the maximum length of a code. Once we have to allocate memory
    to each code, we use this number to avoid unnecessary memory allocation; */
    LL *iter = list_of_addresses;
    Map *output_map = Map_initialize();
    output_map = (Map*)malloc(sizeof(Map));
    output_map->map_size = 0;
    
    while(iter){
        Pair *new_pair = Pair_initialize();
        new_pair = (Pair*)malloc(sizeof(Pair));
        new_pair->character = (iter->tree_node)->character;
        new_pair->code = malloc((max_code_size+2)*sizeof(char));
        get_code((iter->tree_node)->character,t,list_of_addresses,new_pair->code);
        output_map->map[output_map->map_size] = new_pair;
        output_map->map_size++;
        iter = iter->next;
    }
    return output_map;
}

char *search_code(char c,Map *map_of_codes){
    int i;
    for(i=0;i<map_of_codes->map_size;i++){
        if(map_of_codes->map[i]->character == c) return map_of_codes->map[i]->code;
    }
    return NULL;
}

/* END OF MAP OF CODES */


/* THE ENCODING FUNCTION */

/* We pass a pointer to a Node address as a parameter so we can keep the enconding tree in memory. 
We need this tree for decoding. 

To call the function below, we just have to declare a Node pointer and pass its address as a parameter.
Do as below:

Node *tree;
char *output_code;
output_code = encode(input_string,&tree);

*/

char *encode(char *input_string, Node **tree){
    char *output_code;
    
    PQ *priority_queue = create_PQ_from_string(input_string);
    LL *list_of_addresses = LL_initialize();
    *tree = tree_builder(priority_queue,&list_of_addresses);
    Map *map_of_codes = Map_initialize();
    map_of_codes = build_map_of_codes(list_of_addresses,*tree);
    output_code = malloc(max_output_size*sizeof(char));
    int i = 0;
    while(*(input_string+i)!='\0'){
        strcat(output_code,search_code(*(input_string+i),map_of_codes));
        i++;
    }
    return output_code;

}

/* END OF THE ENCONDING FUNCTION */


/* THE DECODING FUNCTION */

/* To decode an encoded string, we only need the coding tree. This is passed as a parameter. */



char *decode(char *encoded_string, Node *tree){
    Node *iter = tree;
    char c;
    char *output;
    output = malloc(max_input_size * sizeof(char));
    int i = 0, j = 0;
    if(iter->left == NULL && iter->right == NULL){
        while(*(encoded_string+i) != '\0'){
            *(output+i) = iter->character;
            i++;
        }
        return output;
    }
    while(*(encoded_string+i) != '\0'){
        c = *(encoded_string+i);
        iter = tree;
        while(!iter->is_leaf){
            if(c == '1') iter = iter->right;
            else iter = iter->left;
            i++;
            c = *(encoded_string+i);
        }
        *(output+j) = iter->character;
        j++;
    }

    return output;
}

/* END OF THE DECODING FUNCTION */


 /* COMPUTING THE COMPRESSION FACTOR */

 /* The compression factor is the ratio between the respective bit sizes of the original string and of its
 Huffman code. */

float compression_factor(char *input_string, char *encoded_string){
    int input_size = 8*strlen(input_string);
    int code_size = strlen(encoded_string);
    float result = (float)code_size / (float) input_size;
    return result;
}

 /* END OF COMPUTING THE COMPRESSION FACTOR */


/* PRINTING FUNCTIONS (FOR TESTS)*/

void print_coding_tree(Node *t){
    if(t){
        if(t->is_leaf){
            printf("(%c)", t->character);
        }
        else{
            printf("(%d",t->freq);
            print_coding_tree(t->left);
            print_coding_tree(t->right);
            printf(")");
        }
    }
}

void print_priority_queue(PQ *q){
    int i;
    for(i=0;i<q->size;i++){
        printf("%c,%d--",q->queue[i]->character,q->queue[i]->freq);
    }
}

void print_linked_list(LL *l){
    if(l){
        printf("%c,%p--",(l->tree_node)->character,l->tree_node);
        print_linked_list(l->next);
    }
}

void print_map(Map *map){
    int i;
    for(i=0;i<map->map_size;i++){
        printf("%c,%s--", map->map[i]->character, map->map[i]->code);
    }
}
/* END OF PRINTING FUNCTIONS */


int main(){

    /* The main test is below. The user insert a string in the command line. The program constructs all
    data structures and returns the Huffman code of the string. After, the code is decoded back and the
    string is printed again (if everything goes right). */

    char input_string[max_input_size];
    scanf("%s",input_string);
    char *output_code;
    Node *tree;
    output_code = encode(input_string,&tree);
    printf("%s",output_code);
    printf("\n");
    char *get_string_back;
    get_string_back = decode(output_code,tree);
    printf("%s\n",get_string_back);
    printf("%f\n",compression_factor(input_string,output_code));


    /* SOME TESTS */

    /* PQ *q = PQ_initialize();
    q = (PQ*)malloc(sizeof(PQ));

    Node *node1 = create_leaf_node('a',6,NULL);
    Node *node2 = create_leaf_node('b',10,NULL);
    Node *node3 = create_leaf_node('c', 3,NULL);
    Node *node4 = create_leaf_node('d',11,NULL);
    q->queue[0] = node1;
    q->queue[1] = node2;
    q->queue[2] = node3;
    q->queue[3] = node4;
    q->size = 4;
    LL *list_of_addresses = LL_initialize();
    Node *tree = tree_builder(q,&list_of_addresses);
    
    Map *map;
    map = (Map*)malloc(sizeof(Map));
    map = build_map_of_codes(list_of_addresses,tree);
    printf("%s",search_code('a',map));
    printf("\n");
    print_coding_tree(tree);
    printf("\n"); */
    

    /* Let us get the code of 'c' */
    /* char code[20];
    get_code('a',tree,list_of_addresses,code);
    printf("%s",code); */


    
    /*Building all data structures from input string: 

    PQ *pq_test = PQ_initialize();
    pq_test = create_PQ_from_string(input_string);
    print_priority_queue(pq_test);
    LL *list_of_addresses = LL_initialize();
    Node *tree2 = tree_builder(pq_test,&list_of_addresses);
    print_coding_tree(tree2);
    LL *iter = list_of_addresses;
    while(iter){
        printf("%c--",iter->tree_node->character);
        iter = iter->next;
    }
    printf("\n");
    Map *map;
    map = build_map_of_codes(list_of_addresses,tree2);
    printf("\n");
    print_map(map);
    printf("\n");
   */

    return 0;
}