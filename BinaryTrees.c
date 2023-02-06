#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

/* A "binary tree" is a tree in which each node has at most two children. We call them the "left" and "right" children,
although this classification makes more sense when we are dealing with binary "search" trees. */

/* We implement a binary tree using a struct for its nodes. This struct contains
the label (or information) of the node and its left and right children. */

/* By doing that, any binary tree is referenced by using a pointer to its root. */


typedef struct binary_tree_node {
    int key;
    struct binary_tree_node *left;
    struct binary_tree_node *right;
}BT;

/* STRUCTURAL FUNCTIONS */

/* We introduce functions to initialize, construct and delete binary trees.
The first function simply initializes a null pointer for a binary tree. */

BT *BT_initialize(){
    return NULL;
}

/* Next, we create a node of a binary tree. We have to pass, as parameters, the label (information) of the node
and pointers for its left and right node children. */

BT *BT_create_node(int key, BT *left, BT *right){
    BT *new_node;
    new_node = (BT*)malloc(sizeof(BT));
    new_node->key = key;
    new_node->left = left;
    new_node->right = right;

    return new_node;

}

/* Last, we implement a function to free memory space reserved to a binary tree. This avoids
memory leak. */

void BT_free(BT *b){
    if(b){
        BT_free(b->left);
        BT_free(b->right);
        free(b);
    }
}

/* END OF STRUCTURAL FUNCTIONS */

/* Below, we implement some needed auxiliary data structures. */


/* LIST OF TREE NODES --> USED IN FUNCTION "BT_is_complete" BELOW */

typedef struct list_node {
    BT *tree_node;
    struct list_node *next;
}LTN;

LTN *LTN_initialize(){
    return NULL;
}

void LTN_insert(LTN **l, BT *b){
    LTN *new_node = (LTN*)malloc(sizeof(LTN));
    new_node->tree_node = b;
    new_node->next = NULL;
    if(*l == NULL) *l = new_node;
    else{
        LTN *aux = *l;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = new_node;
    }
}

/* END OF LIST OF TREE NODES */


/* THE LIBRARY */

/* The function below returns the number of nodes of a given binary tree b.
This is a recursive function based on the fact that the number of nodes of any binary tree
equals the sum of the number of nodes of the left and right subtrees of its root plus 1. */

int BT_number_of_nodes(BT *b){
    if(!b) return 0;
    return 1 + BT_number_of_nodes(b->left) + BT_number_of_nodes(b->right);
}

/* Below, we implement a function to compute the number of leafs of a binary tree b. Recall that a leaf
of a tree is a node which has no children. 
This is also a recursive function. If the root is not a leaf, then the number of leafs is the sum
of the number of leafs of its left and right subtrees. */

int BT_number_of_leafs(BT *b){
    if(!b) return 0;
    if(!b->left && !b->right) return 1;
    return BT_number_of_leafs(b->left) + BT_number_of_leafs(b->right);
}

/* The function below returns the "height" of a binary tree, which is the number of edges 
in the longest path from the root to a leaf.
We can use recursion, again. Indeed, the height of a tree is 1 plus the maximum height of its left and right subtrees.
REMARK 1: we use the convention that the height of an empty tree is -1, since the height of a tree which contains only one 
node is 0.
REMARK 2: we need an auxiliary function to compute the maximum between two integers. This is given right below. */

int max(int a,int b){
    if(a >= b) return a;
    return b;
}

int BT_height(BT *b){
    if(!b) return -1;
    return 1 + max(BT_height(b->left), BT_height(b->right));
}

/* A non-empty binary tree is "perfect" if each of its non-leaf nodes have exactly two children, and all 
the leafs are at the same level. The function below returns 1
if the given binary tree is perfect, and 0 otherwise.
It is readily seen that the number of nodes of a perfect tree is a sum of successive powers of 2. In fact,
a binary tree b whose height is k obviously have 1 + 2 + 2^2 + ... + 2^k (= 2^(k+1)-1) nodes. Hence, b is perfect if and only if its number
of nodes equals 2^(k+1)-1. 
REMARK: in the implementation below, we assume that b is non-empty. */

int BT_is_perfect(BT *b){
    if(BT_number_of_nodes(b) == ((int)pow(2,BT_height(b)+1))-1){
        return 1;
    }
    return 0;
}

/* A binary tree is "degenerate" if each of its non-leaf nodes have exactly one child. The function below
return 1 if the given binary tree is degenerate, and 0 otherwise.  
REMARK: we assume that b is non-empty. */

int BT_is_degenerate(BT *b){
    if(b){
        if(b->left && b->right) return 0;
        else if(b->left) return BT_is_degenerate(b->left);
        else if(b->right) return BT_is_degenerate(b->right);
    }
    return 1;
}

/* A binary tree is "complete" if the following conditions hold:

(1) all of its levels are completely filled, except possibly the last one, and
(2) the last level contains all elements on the left of its rightmost element. 

The function below returns 1 if the given binary tree is complete, and 0 otherwise.

A comment about the strategy is due. We walk through the tree level by level, top-down, left-right. 
We do that keeping track of two lists of binary tree nodes: a list of the current level and a list of 
the next level. Of course, the next level list contains the children of the nodes in the current level.
At the end of an iteration, the next level list becomes the current level list, and the next level list is
emptied. When a missing child is found, a flag is raised. If a non-empty child appears after that flag is raised, 
then the function return 0. Otherwise, the iteration proceeds until there is no more levels and the function returns 1. 
*/

int BT_is_complete(BT *b){
    if(!b) return 1; /* We assume that an empty tree is complete. */
    
    int null_child_found = 0;

    LTN *current_level = LTN_initialize();
    LTN *next_level = LTN_initialize();

    LTN_insert(&current_level,b);
    LTN *aux;
    while(current_level != NULL){
        aux = current_level;    
       
        while(aux!=NULL){

            if(null_child_found && (aux->tree_node)->left) return 0;
            else if((aux->tree_node)->left) LTN_insert(&next_level,(aux->tree_node)->left);
            else null_child_found = 1;
            
            if(null_child_found && (aux->tree_node)->right) return 0;
            else if((aux->tree_node)->right) LTN_insert(&next_level,(aux->tree_node)->right);
            else null_child_found = 1;

            aux = aux->next;
        }
        
        current_level = next_level;
        next_level = LTN_initialize();
    }
    return 1;
    
}

/* Next, given a binary tree b, we create a "mirror tree" of b (and return the pointer to its root).
A binary tree b' is a "mirror" for b when:

(1) b and b' have the same nodes and
(2) for each node of b, the corresponding node in b' have the same children, but with interchanged sides. */

BT *BT_mirror(BT *b){
    if(!b) return NULL;
    return BT_create_node(b->key, BT_mirror(b->right),BT_mirror(b->left));
}

/* Now we invert a binary tree b. That is, instead of creating a new binary tree which is a mirror of b, we 
modify b itself by interchanging the sides of the children of each node. */

void BT_invert(BT *b){
    if(b){
        BT *aux;
        aux = b->left;
        b->left = b->right;
        b->right = aux;

        BT_invert(b->left);
        BT_invert(b->right);

    }
}

/* Unlike binary search trees, general binary trees have no ordering properties. Hence, it is
much more costly to find the maximum (minimum) element. We do that below for the maximum. First, we get
the maximum value; after, we get the address of the maximum value (in left-root-right order of priority).
Corresponding functions for the minimum are completely analogous, and we will not explicit their implementations here.  */

int BT_maximum(BT *b){
    if(b){
        return max(b->key,max(BT_maximum(b->left),BT_maximum(b->right)));
    }
    return -INT_MAX;
}

/* For a variation, now we are interested in the address of the biggest element in the binary tree. */

BT *BT_maximum_address(BT *b){
    if(b){
        int aux_left;
        BT *max_left;
        int aux_right;
        BT *max_right;
        if(b->left){
            max_left = BT_maximum_address(b->left);
            aux_left = max_left->key;
        }
        if(b->right){
            max_right = BT_maximum_address(b->right);
            aux_right = max_right->key;
        }
        
        if(b->left && b->right){
            int maximum;
            maximum = max(aux_left,max(aux_right,b->key));
            if(maximum == max_left->key) return max_left;
            if(maximum == b->key) return b;
            return max_right;
        }
        else if(b->left){
            if(aux_left >= b->key) return max_left;
            return b;
        }
        else if(b->right){
            if(aux_right > b->key) return max_right;
            return b; 
        }
        else{
            return b;
        }
         
    }
    return NULL;
}

/* Next, we implement a searching function. It searches for a given integer in a binary tree and returns the
address of the node containing this integer. */

BT *BT_search(BT *b, int n){

    if(b && (b->key == n)) return b;
    if(b->left && BT_search(b->left,n)) return BT_search(b->left,n);
    if(b->right && BT_search(b->right,n)) return BT_search(b->right,n);
    return NULL;
    
}

/* The function below decides whether two binary trees are equal (meaning not only that they have the same nodes, but
the same entire structure). The recursion justifies because two given trees are equal if and only if all of their
corresponding subtrees are equal. Hence, the function returns 0 (false) if the information of two corresponding nodes is different, 
or if the corresponding left and right subtrees of a given node are distinct.*/

int BTs_are_equal(BT *b1, BT *b2){
    if((b1 && !b2) || (!b1 && b2)) return 0;
    if(!b1 && !b2) return 1;
    if(b1->key != b2->key) return 0;
    if(!BTs_are_equal(b1->left,b2->left) || !BTs_are_equal(b1->right,b2->right)) return 0;
    return 1;
}

/* The "diameter" of a binary tree is the maximum distance between two of its nodes (the distance between two nodes
is the number of edges of the path connecting these nodes; in a binary tree, such a path is unique). 
The function below computes the diameter of a binary tree. The algorithm is based on two facts:

(1) The diameter of a binary tree cannot be attained by a pair of nodes where one of the nodes has a child and a parent.
In fact, in this case we would be able to enlarge the diameter by moving to the child or to the parent, having then a contradiction.
Hence, the diameter of a binary tree is attained either by a pair of leafs, or by a leaf and the root (this case only can happen
if the root does not have two children).

(2) Each pair of leafs has a first common ancestor, which is the first ancestor that appears in both (backward) paths from the leafs to the root. 
The distance between two leafs is clearly the sum of the distances of each leaf to this common ancestor. 

Hence we may proceed as follows: for a given node q, we consider the number:

2 + (height of the left child of q) + (height of the right child of q).

This clearly gives the maximum distance between a pair of leafs whose first common ancestor is q. Finally, all we have to do
is to compute this number for each node of the tree and take the maximum among the obtained values. By doing that, we will
certainly get the maximum distance between a pair of leafs of the tree (or between a leaf and the root, since this will
be computed if the root does not have two children).

We actually don't need to compute node-by-node, keep track of the values and take the maximum among them. We can proceed 
recursively. We just take the maximum between the computations for the node and its children. 
  */

int BT_diameter(BT *b){
    if(b) return max(2 + BT_height(b->left)+ BT_height(b->right), max(BT_diameter(b->left),BT_diameter(b->right)));
    return -1; 
    /* Here, we agree that an empty tree has diameter -1. By doing that, we will not have a problem for computing
     the maximum above when we reach a leaf.*/
}


/* BONUS: BT from preorder and inorder arrays */

/* A tree is traversed in "preorder" if we always visit the node, then the left subtree, then the right subtree. 
A tree is traversed in "inorder" if we visit the left subtree, then the node, then the right subtree. Assume that for 
a given BT b we have the preorder and inorder arrays (i.e., arrays with the keys appearing in the order that they are 
visited according to each one of the traverses described above). Is it possible to retrieve b from these arrays?

The answer is yes. The explanation is as follows: by the inorder array, we know precisely who is in the left and in the right
subtrees of any given node. Once the traverse is left-node-right, the keys which appear before the given node in the array are the
ones in the left subtree ("inorder smaller" than), and those which appear after ("inorder greater" than) form the right subtree. 
On the other hand, in the preorder array, we have node-left-right. 
This means that the left subtree of a given node (a, say) will consist in the elements "inorder smaller" than a appearing 
after a. Once we find an element "inorder greater" than a, we know that we start traversing the right subtree of a.

Therefore, to reobtain a tree b from its inorder and preorder arrays, we proceed as follows: we start at the first element of the 
preorder array. This is the root r of b. The left subtree of r consist of all subsequent elements of the preorder array 
which are "inorder smaller" than r, until we find an element which is "inorder greater" than r. When such an element is found, 
all the remaining of the preorder array forms the right subtree of r. Thus, we may implement a recursive algorithm which call itself 
to construct the left and right subtrees. First, it is convenient to have a function to get the order relation given by the inorder
array. */

int inorder_smaller(int *inorder_vec, int n, int m){
    int n_pos = 0;
    while(inorder_vec[n_pos] != n) n_pos++;
    int m_pos = 0;
    while(inorder_vec[m_pos] != m) m_pos++;
    if(n_pos < m_pos) return 1;
    return 0;
}

BT *BT_from_arrays(int *inorder_vec, int *preorder_vec, int size_inorder, int size_preorder){
    /* the size of the preorder array changes at each recursive call; this does not happen for the 
    size of the inorder array. */
    if(size_preorder == 0) return NULL;
    if(size_preorder == 1) return BT_create_node(preorder_vec[0],NULL,NULL);
    int i = 1;
    while(i< size_preorder && inorder_smaller(inorder_vec,preorder_vec[i],preorder_vec[0])) i++;
    if(i== size_preorder){
        return BT_create_node(preorder_vec[0],BT_from_arrays(inorder_vec,&preorder_vec[1],size_inorder,size_preorder-1),NULL);
    }
    else if(i == 1){
        return BT_create_node(preorder_vec[0],NULL,BT_from_arrays(inorder_vec,&preorder_vec[1],size_inorder,size_preorder-1));
    }
    else{
        return BT_create_node(preorder_vec[0],BT_from_arrays(inorder_vec,&preorder_vec[1],size_inorder,i-1),
        BT_from_arrays(inorder_vec,&preorder_vec[i],size_inorder,size_preorder-i));
    }
}

/* Below, we implement printing functions for the sake of testing the code above. */

void BT_inorder_print(BT *b){
    if(b){
        BT_inorder_print(b->left);
        printf("%d--",b->key);
        BT_inorder_print(b->right);
    }
}

void BT_preorder_print(BT *b){
    if(b){
        printf("%d--",b->key);
        BT_preorder_print(b->left);
        BT_preorder_print(b->right);
    }
}



int main() {


    int preorder_array[9] = {1,2,3,4,5,6,7,8,9};
    int inorder_array[9] = {3,2,6,5,4,1,7,8,9};
    BT *b = BT_from_arrays(inorder_array,preorder_array,9,9);
    BT_inorder_print(b);
    printf("\n");
    BT_preorder_print(b);
    return 0;
}

