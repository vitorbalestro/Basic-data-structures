#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

/* We implement binary search trees, which are binary trees such that each node is greater than or equal to its left child and
lesser than its right child (if they exist, of course). The biggest advantage of this data structure is that the search operation has 
O(logn) time complexity. 

As in the case of general binary trees, we define the structure of a node. A reference to a binary search tree is made, then, to 
(the address of) its root node. Actually, the structure for BST's is the same as for BT's; the difference between them is in the 
insertion and remotion operations. In fact, it is clear that the structural constraints leads us to be more careful with insertion and 
deletion operations. 

All the insertions are made in the leafs of the tree. To insert a new value n, we go through the nodes walking to left or right 
whether the key of the node is greater than or lesser than n. To remove a value, we must guarantee that the resulting tree has the 
structural constraint described above.  */

typedef struct bst_node{
    int key;
    struct bst_node *left, *right;
}BST;

/* REMARK: Each node is an address which contains a data (int key), but we will often use the word "node" to refer to the key, 
and not to the address. The context will always let clear what we are talking about. */

/* STRUCTURAL FUNCTIONS */

BST *BST_initialize(){
    return NULL;
}

BST *BST_create_node(int n, BST *left, BST *right){
    BST *new_node = (BST*)malloc(sizeof(BST));
    new_node->key = n;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

BST *BST_insert(BST *b, int n){
    if(b){
        if(n <= b->key) b->left = BST_insert(b->left,n);
        else b->right = BST_insert(b->right,n);
        return b;
    }
    else return BST_create_node(n, NULL, NULL);
}

BST *BST_search(BST *b,int n){
    if(b){
        if(n == b->key) return b;
        if(n < b->key) return BST_search(b->left,n);
        return BST_search(b->right,n);
    }
    return NULL;
}

BST *BST_remove(BST *b, int n){
    if(!b) return b;
    if(n < b->key) {
        b->left = BST_remove(b->left, n);
        return b;
    }
    if(n > b->key){
        b->right = BST_remove(b->right, n);
        return b;
    }
    if(b->left){
        BST *aux = b->left, *pre = b;
        if(!aux->right){
            aux->right = b->right;
            free(b);
            return aux;
        }
        while(aux->right){
            pre = aux;
            aux = aux->right;
        }
        pre->right = aux->left;
        aux->left = b->left;
        aux->right = b->right;
        free(b);
        return aux;
    }
    if(b->right){
        BST *aux = b->right, *pre = b;
        if(!aux->left){
            aux->left = b->left;
            free(b);
            return aux;
        }
        while(aux->left){
            pre = aux;
            aux = aux->left;
        }
        pre->left = aux->right;
        aux->right = b->right;
        aux->left = b->left;
        free(b);
        return aux;
    }
    return NULL;     
}

BST *BST_free(BST *b){
    if(b){
        BST_free(b->left);
        BST_free(b->right);
        free(b);
    }
}

/* END OF STRUCTURAL FUNCTIONS */

/* Before moving forward, let us implement a linked list structure. This will be helpful in some 
functions for binary search trees. We write only the needed functions for linked lists. */

/* AUXILIARY STRUCTURE -- LINKED LISTS */

typedef struct ll_node{
    int key;
    struct ll_node *next;
}LL;

LL *LL_initialize(){
    return NULL;
}

LL *LL_insert_head(LL *l, int n){
    LL *new_node = (LL*)malloc(sizeof(LL));
    new_node->key = n;
    new_node->next = l;
    return new_node;
}

LL *LL_insert_tail(LL *l, int n){
    LL *new_node = (LL*)malloc(sizeof(LL));
    new_node->key = n;
    new_node->next = NULL;
    if(!l) return new_node;
    LL *aux = l;
    while(aux->next){
        aux = aux->next;
    }
    aux->next = new_node;
    return l;
    
}

void LL_print(LL *l){
    if(l){
        printf("%d--", l->key);
        LL_print(l->next);
    }
}

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

/* END OF AUXILIARY STRUCTURE -- LINKED LISTS */

/* AUXILIARY STRUCTURE -- STACKS */

/* Stacks whose nodes contain BST node addresses may be very useful to iterative traverse of binary search trees.
For that reason, we implement them. With that data structure in hands, we will write iterative versions of recursive
algorithms. */

typedef struct stack_node{
    BST *bst_address;
    struct stack_node *below;
}STA; /*STA stands for Stack of Addresses*/

STA *STA_initialize(){
    return NULL;
}

STA *STA_push(STA *s, BST *b){
    STA *new_stack_node = (STA*)malloc(sizeof(STA));
    new_stack_node->bst_address = b;
    new_stack_node->below = s;
    return new_stack_node;
}

STA *STA_pop(STA **s){
    if(s){
        STA *aux = *s;
        *s = (*s)->below;
        return aux;
    }
    return NULL;
}

/* END OF AUXILIARY STRUCTURE -- STACKS */


/* PRINTING FUNCTIONS */

/* Printing functions are quite important to understand different modes of tree traversal. Notice that "printing" 
could be replaced by any other operation that we might want to do in the tree nodes. */

void BST_print_inorder(BST *b){
    if(b){
        BST_print_inorder(b->left);
        printf("%d--", b->key);
        BST_print_inorder(b->right);
    }
}



void BST_print_inorder_iterative(BST *b){
    BST *iter = b;
    STA *aux_stack = STA_initialize();
    while(aux_stack || iter){
        if(iter){
            aux_stack = STA_push(aux_stack,iter);
            iter = iter->left;
        }
        else{
            iter = STA_pop(&aux_stack)->bst_address;
            printf("%d--", iter->key);
            iter = iter->right;
        }
    }
}

void BST_print_preorder(BST *b){
    if(b){
        printf("%d--", b->key);
        BST_print_preorder(b->left);
        BST_print_preorder(b->right);
    }
}

void BST_print_preorder_iterative(BST *b){
    if(b){
        BST *aux = b;
        STA *aux_stack = STA_initialize();
        aux_stack = STA_push(aux_stack,aux);
        while(aux_stack){
            aux = STA_pop(&aux_stack)->bst_address;
            printf("%d--", aux->key);
            if(aux->right) aux_stack = STA_push(aux_stack,aux->right);
            if(aux->left) aux_stack = STA_push(aux_stack,aux->left);
        }
    }
}

void BST_print_postorder(BST *b){
    if(b){
        BST_print_postorder(b->left);
        BST_print_postorder(b->right);
        printf("%d--", b->key);
    }
}

void BST_print_postorder_iterative(BST *b){
    BST *aux = b, *last_visited_node = NULL, *peek_node = NULL;
    STA *aux_stack = STA_initialize();
    while(aux_stack || aux){
        if(aux){
            aux_stack = STA_push(aux_stack,aux);
            aux = aux->left;
        }
        else{
            peek_node = aux_stack->bst_address;
            if(peek_node->right && last_visited_node != peek_node->right){
                aux = peek_node->right;
            }
            else{         
                printf("%d--", peek_node->key);
                last_visited_node = STA_pop(&aux_stack)->bst_address;
            }
        }
    }

}

/* We also want to print a BST traversing it by levels (or "breadth-first"). That is, we print level-by-level (top-down), always 
from the left to the right at each level. For that sake, we need to implement a "queue" structure.*/

/* AUXILIARY STRUCTURE -- QUEUE OF BST ADDRESSES */

typedef struct queue_node{
    BST *bst_address;
    struct queue_node *next;
}QA; /* This stands for Queue of Addresses */

QA *QA_initialize(){
    return NULL;
}

QA *QA_enqueue(QA *q, BST *a){
    QA *new_node = (QA*)malloc(sizeof(QA));
    new_node->bst_address = a;
    new_node->next = q;
    return new_node;
}

QA *QA_dequeue(QA **q){
    if(!(*q)) return NULL;
    if(!(*q)->next){
        QA *output = *q;
        *q = NULL;
        return output;
    }
    
    QA *iter = *q, *pre = NULL;
    while((iter)->next){
        pre = iter;
        iter = iter->next;
    }
    pre->next = NULL;
    return iter;
}

/* END OF AUXILIARY STRUCTURE -- QUEUE OF BST ADDRESSES */

void BST_print_breadth_first(BST *b){
    if(b){
        BST *aux;
        QA *aux_queue = QA_initialize();
        aux_queue = QA_enqueue(aux_queue,b);
    
        while(aux_queue){
            aux = QA_dequeue(&aux_queue)->bst_address;
            if(aux->left) aux_queue = QA_enqueue(aux_queue,aux->left);
            if(aux->right) aux_queue = QA_enqueue(aux_queue,aux->right);
            printf("%d--", aux->key);
        }
    }
}

/* END OF PRINTING FUNCTIONS */


/* THE LIBRARY */

int BST_number_of_nodes(BST *b){
    if(!b) return 0;
    return 1 + BST_number_of_nodes(b->left) + BST_number_of_nodes(b->right);
}

/* A node for a BST has pointers to its children, but no pointer for its parent. The function below returns 
the parent node of a node in a BST b whose key is n. We consider the first 
ocurrence of n in the tree (from the root, of course). */

BST *BST_parent(BST *b, int n){
    if(!BST_search(b,n) || (b && b->key == n)) return NULL;
    BST *aux = b, *output = BST_initialize();
    while(aux && aux->key != n){
        output = aux;
        if(n < aux->key) aux = aux->left;
        else aux = aux->right; 
    }
    return output;
}

/* The minimum and maximum values in a BST are the leftmost and rightmost nodes, respectively. Hence, it is
easy to obtain them. */

BST *BST_maximum(BST *b){
    if(b){
        if(b->right) return BST_maximum(b->right);
        return b;
    }
}

BST *BST_minimum(BST *b){
    if(b){
        if(b->left) return BST_minimum(b->left);
        return b;
    }
}

/* The next function computes the sum of all keys of a given BST. */

int BST_sum(BST *b){
    if(b) return b->key + BST_sum(b->left) + BST_sum(b->right);
}

/* Given an integer n and a binary search tree containing n, we want to get, in a linked list, 
all the ancestors of n (just the key, not the addresses of the nodes). 
The ancestors of a given node are the nodes in the path from the root to it. */

LL *BST_ancestors(BST *b, int n){
    LL *output = LL_initialize();
    BST *node = BST_search(b,n);
    if(node){
        BST *aux = b;
        while(aux && aux->key != n){
            output = LL_insert_tail(output, aux->key); 
            if(aux->key > n) aux = aux->left;
            else aux = aux->right;
        }
    }
    return output;
}

/* In the next function, we put all the key of the nodes of a BST in a sorted LL. We pass the address of the 
output list as a parameter so we can use recursion. Hence, the address of the output list must be initialized 
before calling the function. */

void BST2LL(BST *b, LL **output){
    if(b){
        BST2LL(b->left,&(*output));
        *output = LL_insert_tail(*output,b->key);
        BST2LL(b->right,&(*output));
    }

}

/* Now we want a sorted list of the (keys of the) nodes of a BST b which are lesser than a given integer value n.
To use recursion, we pass the output list as a parameter, as in the function above. */

void BST_lesser_than_n(BST *b, int n, LL **output){
    if(b){
        BST_lesser_than_n(b->left,n,&(*output));
        if(b->key < n) *output = LL_insert_tail(*output,b->key);
        BST_lesser_than_n(b->right,n,&(*output));
    }
}

/* The predecessor a node n in a BST b is the largest node smaller than n in b. The next function
returns the address of the predecessor of a given node. */

BST *BST_predecessor(BST *b, int n){
    BST *node = BST_search(b,n);
    if(!node) return NULL;
    if(node->left) return BST_maximum(node->left);
    BST *output = NULL, *aux = b;
    while(aux->key != n){
        if(aux->key > n) aux = aux->left;
        else{
            output = aux;
            aux = aux->right;
        }
    }
    return output;
}

BST *BST_predecessor_with_stack(BST *b, int n){
    BST *aux = b;
    BST *pred = NULL;
    STA *aux_stack = STA_initialize();
    while(aux_stack || aux){
        if(aux){
            aux_stack = STA_push(aux_stack,aux);
            aux = aux->left;
        }
        else{
            aux = STA_pop(&aux_stack)->bst_address;
            if(aux->key == n) return pred;
            pred = aux;
            aux = aux->right;
        }
    }
    return NULL;
}

/* We can also find the sucessor of a given node n in a BST. This is the smallest node greater than n. */

BST *BST_sucessor(BST *b, int n){
    BST *node = BST_search(b,n);
    if(!node) return NULL;
    if(node->right) return BST_minimum(node->right);
    BST *output = NULL, *aux = b;
    while(aux->key != n){
        if(aux->key > n){
            output = aux;
            aux = aux->left;
        }
        else aux = aux->right;
    }
    return output;

}

BST *BST_sucessor_with_stack(BST *b, int n){
    BST *aux = b, *suc= NULL;
    STA *aux_stack = STA_initialize();
    while(aux_stack || aux){
        if(aux){
            aux_stack = STA_push(aux_stack,aux);
            aux = aux->right;
        }
        else{
            aux = STA_pop(&aux_stack)->bst_address;
            if(aux->key == n) return suc;
            suc = aux;
            aux = aux->left;
        }
    }
}

/* REMARK: The algorithms to find the predecessor and sucessor of a node presented above have O(logn) time complexity.
It would be easier to just put the BST in a sorted linked list and get the predecessor and the sucessor using that linked list.
However, the time complexity of that is O(n). */



/* Next, we implement a function which returns the lowest common ancestor of two distinct nodes of a BST. This is the last
common node in the paths from the root to these nodes. The parameters of the functions are the tree and the two integer keys, 
but it is trivial to obtain a version where the integer keys are replaced by the addresses of their nodes. */

BST *BST_lowest_common_ancestor(BST *b, int n, int m){
    if(!b) return NULL;
    BST *n_node = BST_search(b,n);
    BST *m_node = BST_search(b,m);
    if(!n_node || !m_node) return NULL;
    BST *lca = b, *n_iter = b, *m_iter = b;

    while(n_iter && m_iter && n_iter == m_iter){
        if(n < n_iter->key) n_iter = n_iter->left;
        else if (n > n_iter->key) n_iter = n_iter->right;

        if(m < m_iter->key) m_iter = m_iter->left;
        else if (m > m_iter->key) m_iter = m_iter->right;

        if(n_iter == m_iter) lca = n_iter;
    }

    return lca;
    
}

/* The next function transforms a given vector in a BST. For the sake of simplicity, we also pass the number of 
elements of the vector as a parameter. It is important to notice that the function only works if the input vector 
is sorted. Once the function is recursive, it is better to sort the vector before calling the function than 
sorting the vector inside the function. The latter would lead to useless effort (since the subvectors are already sorted
if the initial vector is sorted).

REMARK: This function actually does a little better: the resulting BST is "balanced". In our context, this means that 
for any node it holds that the difference between the heights of the left and right subtrees is at most 1. */

BST *VET2BST(int *vet, int n){
    if(n > 0){
        int mid = n/2;
        int *left_vet = (int*)malloc(mid * sizeof(int) + 1);
        int i;
        for(i = 0; i< mid; i++) left_vet[i] = vet[i];
        int *right_vet = (int*)malloc(mid * sizeof(int) + 1);
        
        if(n % 2 == 1){
            for(i=0;i<mid;i++) right_vet[i] = vet[mid+i+1];
            return BST_create_node(vet[mid],VET2BST(left_vet,mid),VET2BST(right_vet,mid));
        }
        else {
            for(i=0;i<mid-1;i++) right_vet[i] = vet[mid+i+1];
            return BST_create_node(vet[mid],VET2BST(left_vet,mid),VET2BST(right_vet,mid-1));
        }
    }
    return NULL;
}

/* The next function checks whether a given binary tree is a binary search tree. */

int BST_check(BST *b){
    if(b){
        if((b->left && (BST_maximum(b->left)->key > b->key)) || (b->right && BST_minimum(b->right)->key <= b->key)) return 0;
        return BST_check(b->left) && BST_check(b->right);
        
    }   
    return 1;
}

/* Having the function BST_check in mind, we may implement a function which counts how many subtrees of a BT are BST's. 
Here, a "subtree" of a tree b is a tree whose root is a node of b. To call this function, we need first to initialize an integer
"output" with value 0. This integer is passed as a parameter by reference, and 
whenever the recursive algorithm finds a BST subtree, the output is incremented. */

void BST_subtrees(BST *b, int *output){
    if(b){
        if(BST_check(b->left)) (*output)++;
        if(BST_check(b->right)) (*output)++;
        if(BST_check(b)) (*output)++;
    }
}

/* Now we want to transform a given binary tree in a binary search tree without modifying the original structure. 
Notice that we pass a BST as parameter, but we might create a general binary tree with node type BST using
the function BST_create_node. Indeed, the node structure of BST's is exactly the same as the structure node
of BT's. The difference between them is in the insertion/remotion methods. The BST's versions of these methods is what
guarantees that the constructed tree is indeed a BST.

The strategy is as follows: we put the addresses of the nodes of the input BT in a linked list in inorder. Then, we put the
keys of the nodes of the BT in another linked list. Finally, we sort the linked list of keys and "feed" the inorder addresses 
of the BT with the sorted keys.

We need some auxiliary tools. To construct the keys list, we may use the function BST2LL, even if the input binary tree
is not a BST, but a BT. Indeed, this function simply add the keys to a linked list in inorder. On the other hand, to construct 
the linked list of addresses, we need to define a structure of linked list where the key of each node is a BST pointer. Then,
we have to implement a function which returns the (inorder) linked list of the addresses of the nodes of a given 
input BST. Recall again that we may naturally apply these functions to BT input, instead of BST input.  */

typedef struct bst_ll_node{
    BST *key;
    struct bst_ll_node *next;
}LLA;

LLA *LLA_initialize(){
    return NULL;
}

LLA *LLA_insert_tail(LLA *l, BST *key){
    LLA *new_node = (LLA*)malloc(sizeof(LLA));
    new_node->key = key;
    new_node->next = NULL;
    if(!l) return new_node;
    LLA *aux = l;
    while(aux->next){
        aux = aux->next;
    }
    aux->next = new_node;
    return l;
}

void BST2LLA(BST *b, LLA **output){
    if(b){
        BST2LLA(b->left,&(*output));
        *output = LLA_insert_tail(*output,b);
        BST2LLA(b->right,&(*output));
    }

}

BST *BT2BST(BST *b){
    LL *keys_list = LL_initialize();
    LLA *addresses_list = LLA_initialize();
    BST2LL(b,&keys_list);
    keys_list = LL_bubble_sort(keys_list);
    BST2LLA(b,&addresses_list);
    LL *iter_key = keys_list;
    LLA *iter_address = addresses_list;
    while(iter_key && iter_address){
        (iter_address->key)->key = iter_key->key;
        iter_address = iter_address->next;
        iter_key = iter_key->next;
    }
    return b;
   
}

/* Below, we have a function to check whether two given BST's are equal (meaning that they have the same keys and the same
structure -- that is, each key has exactly the same children in both trees). */

int BST_check_equal(BST *b1, BST *b2){
    if(!b1 && !b2) return 1;
    if((b1 && !b2) || (b2 && !b1)) return 0;
    if(b1->key != b2->key) return 0;
    return BST_check_equal(b1->left, b2->left) && BST_check_equal(b1->right, b2->right);
}

/* In the next function, we update each node of a given BST to contain the sum of all greater keys. We do that in O(n) time 
complexity, which is better than just put all the keys in a list (or vector) and manipulating this list before updating each 
tree node with the obtained sums. We just have to pass an integer address as a parameter. This integer is initialized as 0, and
at each step this is incremented with the current key value when we traverse the tree in decrescent order. Hence at each step we 
keep track of the sum of all keys greater than the current key.   */

void BST_update_by_sum(BST *b, int *sum){
    if(b){
        BST_update_by_sum(b->right,&(*sum));
        int aux = b->key;
        b->key = *sum;
        *sum += aux;
        BST_update_by_sum(b->left,&(*sum));
    }
}

/* The next function returns the key of the node containing the k-th smallest element in a given BST b.
Again, we could simply put all elements in a sorted list (or vector), but this would not be the more efficient 
solution. Then, what we do is to traverse the tree in inorder until we reach the k-th element. To traverse the tree in inorder, 
we need to procceed recursively. Thus, we must keep an integer address (count) to keep track of the position of each visited node. 
When the k-th node is reached, we update the output integer pointer with its key value. */

void BST_kth_smallest(BST *b, int k, int *count, int *output){
    if(b){
        BST_kth_smallest(b->left,k,count,output);
        (*count)++;
        if(*count == k) *output = b->key;
        BST_kth_smallest(b->right,k,count,output);
    }
    
}

/* Now we use a stack to produce an iterative version. This version returns directly the (address of the) node where 
the k-th smallest key is. The function above could also be designed to give this node; we just would have to change the integer 
pointer *output parameter to a BST address parameter. That is, the signature of the function would be: 

BST_kth_smallest(BST *b,int k,int count, BST **output).

*/

BST *BST_kth_smallest_iterative(BST *b, int k){
    BST *iter = b;
    int count = 0;
    STA *aux_stack = STA_initialize();
    while(aux_stack || iter){
        if(iter){
            aux_stack = STA_push(aux_stack,iter);
            iter = iter->left;
        }
        else{
            iter = STA_pop(&aux_stack)->bst_address;
            count++;
            if(count==k) return iter;
            iter = iter->right;
        }
    }
    return NULL;
}

/* We can also write an analogue recursive function to obtain the k-th largest key in a BST. We simply have to traverse
the tree in reverse inorder. There is also a completely analogous iterative versions -- we just have to swap left and right
to traverse the tree in reverse inorder. We will not write this function explicitly. */

void BST_kth_largest(BST *b,int k, int *count, int *output){
    if(b){
        BST_kth_largest(b->right,k,count,output);
        (*count)++;
        if(*count == k) *output = b->key;
        BST_kth_largest(b->left,k,count,output);
    }
}

/* The next function removes all nodes of a BST b whose keys do not lie in a valid range. This range is passed 
as a parameter of the function: we consider the interval [min,max] as the valid range.*/

BST *BST_valid_range(BST *b, int min, int max){
    if(b){
        if(b->key < min){
            free(b->left);
            b->right = BST_valid_range(b->right,min,max);
            BST *aux = b->right;
            free(b);
            return aux;
        }
        else if(b->key > max){
            free(b->right);
            b->left = BST_valid_range(b->left,min,max);
            BST *aux = b->left;
            free(b);
            return aux;
        }
        else{
            b->left = BST_valid_range(b->left,min,max);
            b->right = BST_valid_range(b->right,min,max);
            return b;
        }
    }
    else return NULL;
}

/* Now we write a function which finds a pair in a BST having a given sum. Of course, we may put all elements in a vector and 
implement an O(n^2) algorithm using brute force. However, there is an O(n) solution. First, we put the keys in a sorted vector.
Then, we start by adding the greatest and the smallest keys. 
If this sum is greater than the desired sum, then the greatest key is replaced by its predecessor. Otherwise, if 
this sum is smaller than the desired sum, then the smallest key is replaced by its sucessor. We do that until we reach the 
desired sum. If the sum is not found, then the function returns NULL. To return the pair, we use a linked list of BST addresses. */

LLA *BST_pair_having_sum(BST *b, int sum){
    BST *iter = b;
    LLA *output = LLA_initialize();
    STA *aux_stack = STA_initialize();
    int n = BST_number_of_nodes(b);
    BST *vec[n];
    int i = 0;
    if(n < 2) return NULL;
    while(aux_stack || iter){
        if(iter){
            aux_stack = STA_push(aux_stack,iter);
            iter = iter->left;
        }
        else{
            iter = STA_pop(&aux_stack)->bst_address;
            vec[i++] = iter;
            iter = iter->right;
        }
    }
    i = 0; 
    int j = n-1;
    while(i<n && j>=0){
        if(vec[i]->key+vec[j]->key == sum){
            output = LLA_insert_tail(output,vec[i]);
            output = LLA_insert_tail(output,vec[j]);
            return output;
        }
        else if(vec[i]->key+vec[j]->key > sum) j--;
        else i++;
    }
    return NULL;
}

/* We can do the same to a sum of three keys in a BST. The difference is that we keep two pointers in the head 
of the sorted vector and one pointer in the tail. If the obtained sum is smaller than the desired, we move one of the head pointers.
We know which pointer to move according to the following rule: if the difference between the positions of the two head pointers is
greater than 1, then we move the first pointer. Otherwise, we move the second pointer.  */

LLA *BST_triple_having_sum(BST *b, int sum){
    BST *iter = b;
    LLA *output = LLA_initialize();
    STA *aux_stack = STA_initialize();
    int n = BST_number_of_nodes(b);
    BST *vec[n];
    int i = 0;
    if(n < 3) return NULL;
    while(aux_stack || iter){
        if(iter){
            aux_stack = STA_push(aux_stack,iter);
            iter = iter->left;
        }
        else{
            iter = STA_pop(&aux_stack)->bst_address;
            vec[i++] = iter;
            iter = iter->right;
        }
    }
    i = 0;
    int j = 1, k = n-1;
    while(j<n && k>=0){
        if(vec[i]->key + vec[j]->key + vec[k]->key == sum){
            output = LLA_insert_tail(output,vec[i]);
            output = LLA_insert_tail(output,vec[j]);
            output = LLA_insert_tail(output,vec[k]);
            return output;
        }
        else if(vec[i]->key + vec[j]->key + vec[k]->key > sum) k--;
        else{
            if(j-i == 1) j++;
            else i++;
        }
    }
    return NULL;
}

/* BONUS -- PREORDER ARRAYS*/

/* The "preorder traversal" of a binary search tree is the following sequence of recursive steps:

(1) visit the node (starting at the root);
(2) traverse (in preorder) the left subtree of the node
(3) traverse (in preorder) the right subtree of the node. 

Consider an array v of integers where all entries are distinct (for the sake of simplicity). From this array, 
we iteratively construct a BST b inserting each entry in the order that they appear. Thus, the following natural question
arises: is it possible to know, before constructing the b, that the preorder printing of b will be exactly the
same as the original array v? If an array has this property, then we call it a "preorder array".

It is actually easy to determine whether an array is a preorder array or not. Let [a1,...,an] be an array. Then, it
is a preorder array if and only if the following holds true for any index k in {1,...,n}: if j is the minimum index such that 
aj > ak, then ai > ak for every i from j to n. In other words, no entry which is smaller than a given entry ak appears after the 
first entry which is greater than ak. 

The explanation is quite simple: when we find the first entry greater than ak (aj, say), we start building the right subtree of ak. 
In preorder, this happens if and only if the left subtree of ak already had been entirely constructed. 
This happens if and only if all keys smaller than ak appeared before aj. 

We implement below a function which receives a vector and its size, and returns 1 if the vector is in preorder and 0 otherwise. 

*/

int is_array_in_preorder(int *vec, int n){
    int j,i;
    for(i=0;i<n;i++){
        j = i+1;
        while(vec[j] < vec[i] && j < n) j++;
        if(j<n-1){
            j++;
            while(j<n){
                if(vec[j] < vec[i]) return 0;
                j++;
            }
        }
    }
    return 1;
}

BST *BST_from_preorder_array(int *vec, int n){
    if(n==1) return BST_create_node(vec[0],NULL,NULL);
    int i = 1;
    while(vec[i] < vec[0] && i < n) i++;
    BST *node = (BST*)malloc(sizeof(BST));
    node->key = vec[0];
    if(i==n){
        node->left = BST_from_preorder_array(&vec[1],n-1);
        node->right = NULL;
    }
    else if(i==1){
        node->right = BST_from_preorder_array(&vec[1],n-1);
        node->left = NULL;
    }
    else{
        node->left = BST_from_preorder_array(&vec[1],i-1);
        node->right = BST_from_preorder_array(&vec[i],n-i);
    }
    return node;
}

/* BONUS 2 - POSTORDER ARRAYS */

/* We say that an array is a "postorder array" if represents the postorder traversing of a BST (left-right-node).   
The natural question here is whether we can construct the BST from its postorder array. The answer is yes. Let v be the 
postorder array of a BST b. We look at v reverted (w, say). Once the postorder runs first by the left and right subtrees, 
the first element of w (that is, the last one of v) is the root of b (r, say). The first element smaller than r in w is the root 
of the left subtree of r (l, say), and the keys of the right subtree of r are the elements between l and r. Hence, we may write a
recursive function to get the BST of a given postorder vector.
 */

BST *BST_from_postorder_array(int *vec, int n){
    if(n == 0) return NULL;
    if(n == 1) return BST_create_node(vec[0],NULL,NULL);
    int left_root_pos = n-1;
    while(vec[left_root_pos] >= vec[0] && left_root_pos >= 0) left_root_pos--;
    if(left_root_pos == n-2) return BST_create_node(vec[n-1],BST_from_postorder_array(&vec[0],n-1),NULL);
    else if(left_root_pos == -1) return BST_create_node(vec[n-1],NULL,BST_from_postorder_array(&vec[0],n-1));
    else return BST_create_node(vec[n-1],BST_from_postorder_array(&vec[0],left_root_pos-1), BST_from_postorder_array(&vec[left_root_pos],n-left_root_pos-2));
}

/* BONUS 3 -- different arrays yielding the same BST */

/* We consider the natural way of building a BST from an array: we just insert the array's elements in the BST in the sequence 
that they appear. Of course, two arrays having precisely the same elements, but in different orders, might yield different BST's. 
What we want to do is to write a function which receives two arrays with the same elements and returns whether or not these
arrays give rise to the same BST. However, we would like to do that without constructing the respective BST's. Roughly speaking, 
we want to do that "looking only to the arrays". For the sake of simplicity, we always assume that all entries of the arrays are 
mutually distinct.

What we can do is the following: given an element in an array, we may discover its parent and whether it is a left child or a right 
child only looking to the previous elements in the array. 

In fact, when we insert a given element of the array in the BST, we compare it to all previous elements of the array (which had been
already inserted).
By the insertion rule (smaller to left, greater to right), an element x (which is not the first; i.e., the root) 
is on the left of the minimum element before x which is greater than x (y, say), if such an element exists. 
Also, x is on the right of the maximum element before x which is smaller than x (z, say), if such an element exists.

We claim that x is either the left child of y (if y appears after z in the array) or the right child of z (otherwise).
To see this, we consider two cases: if x is the right child of its parent, then it has to be the right child of the greatest
element least than x inserted in the BST before x. If this is not the case, then we would have wrongly moved x to the left of a
smaller key before moving it to the right of its parent. The case where x is the left child of its parent is completely analogous.
The reason why the parent is who appears "later" is evident from the order in which we do the comparisons. For example, if
our array is 15, 10, 12 and we insert 12, then we first go to the left of 15, and then to the right of 10. 

Of course, two BST's are the same if and only if they have the same root and each non-root node has the same parent. 
Thus, to determine if two arrays yield the same BST we just have to check if each element has the same parent 
(of course, the first element of the arrays have to be the same, 
since otherwise the corresponding BST's do not have the same root). 

The function below receives an array and an element (which cannot be the first element of the array) and returns the parent of 
its element in the BST yielded by the array.
 */

int who_is_your_daddy(int *vec, int element){
    int elem_pos = 0;
    while(vec[elem_pos] != element) elem_pos++;
    int min_greater_before_pos = -1, i = 0;
    while(i < elem_pos){
        if(min_greater_before_pos == -1) {
            if(vec[i] > element) min_greater_before_pos = i;
        }
        else{
            if(vec[i] > element && vec[i] < vec[min_greater_before_pos]) {
                min_greater_before_pos = i;
            }
        }
        i++;
    }
    int max_smaller_before_pos = -1;
    i = 0;
    while(i < elem_pos){
        if(max_smaller_before_pos == -1) {
            if(vec[i] < element) max_smaller_before_pos = i;
        }
        else{
            if(vec[i] < element && vec[i] > vec[max_smaller_before_pos]) {
                max_smaller_before_pos = i;
            }
        }
        i++;
    }
    if(min_greater_before_pos > max_smaller_before_pos) return vec[min_greater_before_pos];
    else return vec[max_smaller_before_pos];
}

/* Now we write the desired function, which says whether or not two arrays with the same keys yield the same BST. 
For the sake of simplicity, we pass the size of these vectors as a parameter. We also assume that n > 0. */

int same_bst_from_arrays(int *vec1, int *vec2, int n){
    int i;
    if(vec1[0] != vec2[0]) return 0;
    for(i=1;i<n;i++){
        if(who_is_your_daddy(vec1,vec1[i]) != who_is_your_daddy(vec2,vec1[i])) return 0;
    }
    return 1;
}

int main(){

    
    return 0;
} 