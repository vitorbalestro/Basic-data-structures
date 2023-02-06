#include<stdio.h>
#include<limits.h>
#include<stdlib.h>


/* We study "binary heaps", which are arrays that can be seen as binary trees. We do that
as it follows: we identify the parent of the element vec[i] as vec[i-1/2], if i > 0 (vec[0] is the root).
The left child of vec[i] is vec[2*i+1], if it exists. The right child of vec[i] is vec[2*i+2], if it exists.

There are two types of heaps: the max heaps and the min heaps. In a max heap, the key of a parent is always greater than
or equal to the keys of its children. In a min heap, the opposite occurs: the key of a parent is always smaller than or equal
to the keys of its children. 

It is immediate to notice that not any array represents a heap (min or max), since the heap properties described above might
not be satisfied.    */

/* STRUCTURAL FUNCTIONS */

int parent(int i){
    if(i > 0) return (i-1)/2;
    return -1;
}

int left_child(int i){
    return 2*i+1;
}

int right_child(int i){
    return 2*i+2;
}

/* It is useful to have a function which reorders a given array transforming it in a maximum heap. To do that, we first
implement a function which guarantees that a subtree with root in a given index i will have the (maximum or minimum) heap 
property, given that its left and right subtrees have this property.  */

void max_heapfy(int *vec,int n,int i){
    int left = left_child(i);
    int right = right_child(i);
    int largest = i;
    if(left < n && vec[left] > vec[i]) largest = left;
    if(right < n && vec[right] > vec[largest]) largest = right;
    if(i != largest){
        int aux = vec[i];
        vec[i] = vec[largest];
        vec[largest] = aux;
        max_heapfy(vec,n,largest);
    }
}

/* Now, to transform an array into a maximum heap, we just need to "heapfy" all of its entries but the "leafs". */

void build_max_heap(int *vec, int n){
    int i, last_parent = parent(n-1);
    for(i=last_parent;i>=0;i--) max_heapfy(vec,n,i);
   
}

/* We also define an insertion operation. Initially, we insert an element in a heap putting it in the (n+1)-th position
of the heap array. Then, we have to compare it to its parent to check whether the heap property is satisfied. If this is
not the case, then we swap the new element with its parent. Of course, this may break the heap property of the level above, 
and so on. Hence, we must "go up" through the heap in a level-wise path always making sure the heap property is preserved
(that is, doing swaps if necessary) until we reach the root. Once we do one comparison by level, the time complexity is O(logn).
*/

void max_heap_insert(int *vec, int n, int new){
    /* The parameter n is the current size of the heap. We can make the insertion provided the array has enough memory, that is,
    we must be able to define vec[n]. */
    vec[n] = new;
    int aux, i = n, j = parent(i);
    while(j >= 0 && vec[i] > vec[j]) {
        aux = vec[i];
        vec[i] = vec[j];
        vec[j] = aux;
        i = j;
        j = parent(i);
    }
}

/* For the sake of completeness, we also write a function to remove the first element of a max heap (which is 
the greatest element of the heap, of course). This is very simple: we simply put the last heap element in the first 
position and call the max heapfy function for n-1 elements up to the first. In this implementation, we must keep track
of the size of the heap. However, if after the remotion we fill the last position (which is not part of the heap anymore)
with -INT_MAX, then we will always know when the heap ends. */

int max_heap_remove(int *vec, int n){
    int output = vec[0];
    vec[0] = vec[n-1];
    vec[n-1] = -INT_MAX;
    max_heapfy(vec,n-1,0);
    return output;
}

/* Now we do the same for minimum heaps. */

void min_heapfy(int *vec,int n,int i){
    int left = left_child(i);
    int right = right_child(i);
    int smallest = i;
    if(left < n && vec[left] < vec[i]) smallest = left;
    if(right < n && vec[right] < vec[smallest]) smallest = right;
    if(i != smallest){
        int aux = vec[i];
        vec[i] = vec[smallest];
        vec[smallest] = aux;
        min_heapfy(vec,n,smallest);
    }
}

void build_min_heap(int *vec, int n){
    int i, last_parent = parent(n-1);
    for(i=last_parent;i>=0;i--) min_heapfy(vec,n,i);
}

void min_heap_insert(int *vec, int n, int new){
    /* The parameter n is the current size of the heap. We can make the insertion provided the array has enough memory, that is,
    we must be able to define vec[n]. */
    vec[n] = new;
    int aux, i = n, j = parent(i);
    while(j >= 0 && vec[i] < vec[j]) {
        aux = vec[i];
        vec[i] = vec[j];
        vec[j] = aux;
        i = j;
        j = parent(i);
    }

}

int min_heap_remove(int *vec, int n){
    int output = vec[0];
    vec[0] = vec[n-1];
    vec[n-1] = INT_MAX;
    min_heapfy(vec,n-1,0);
    return output;
}


/* Of course, we may introduce functions to check whether a given array is a (max or min) heap. */

int check_max_heap(int *vec, int n){
    int last_parent = parent(n-1);
    int i;
    for(i=0;i<=last_parent;i++){
        if(vec[left_child(i)] > vec[i] || (right_child(i) < n && vec[right_child(i)] > vec[i])) return 0;
    }
    return 1;
}


int check_min_heap(int *vec, int n){
    int last_parent = parent(n-1);
    int i;
    for(i=0;i<=last_parent;i++){
        if(vec[left_child(i)] < vec[i] || (right_child(i) < n && vec[right_child(i)] < vec[i])) return 0;
    }
    return 1;
}

/* The heapsort algorithm sorts an array in O(nlogn) time complexity. The strategy is as follows: 
we start swapping the first and last elements guaranteeing, then, that the largest element is in the last
position. Once the last element is settled, we look to the n-1 first positions of the resulting array. Looking as a binary tree
in the usual way, the right and left subtrees of the root are heaps. Hence, if we call max_heapfy function to the root, 
the resulting array will be a max heap. Then, the largest element (of the (n-1)-sized vector that we are considering right now) is
now in the root. So, we repeat the operation: we swap the first and last elements (of the (n-1)-sized array), and then the 
second largest element in the original vector is now the last-but-one when we look to all the n positions. Moving forward, 
we simply need to repeat it until we look to a vector of a single element.  */

void heapsort(int *vec,int n){
    build_max_heap(vec,n);
    int k;
    int aux;
    for(k=n-1;k>=0;k--){
        aux = vec[0];
        vec[0] = vec[k];
        vec[k] = aux;
        max_heapfy(vec,k,0);
    }

}

/* We can also use this strategy to obtain the k-th largest element in an array (although this is not the optimal algorithm).*/

int kth_largest(int *vec, int n, int k){
    
    build_max_heap(vec,n);
    int j;
    int aux;
    for(j=n-1;j>=n-k+1;j--){
        aux = vec[0];
        vec[0] = vec[j];
        vec[j] = aux;
        max_heapfy(vec,j,0);
    }
    return vec[0];
}

/* Let us make it using a min heap. We construct a min heap with the k first array elements. We compare each one of the remaining
elements to the root of the min heap. If the element is greater than the root, then we replace the root by the element
heapfy from the root (both subtrees will already be heaps). This operation takes O(log(k)) time complexity, from where we have 
O(nlog(k)) time complexity. At the end, the heap will contain the k largest elements of the array, with the minimum among them in the 
root. Hence, we just return the root element. */


int kth_largest_with_heap(int *vec,int n, int k){
    int heap[k];
    int j;
    for(j=0;j<k;j++) heap[j] = vec[j];
    build_min_heap(heap,k);
    for(j=k;j<n;j++){
        if(vec[j] > heap[0]){
            heap[0] = vec[j];
            min_heapfy(heap,k,0);
        }
    }
    return heap[0];
}

/* An array is said to be "k-sorted" if each element is at most k positions away from its correct sorted position. 
Below, we write an algorithm to sort a k-sorted array. Notice that for each j from 0 to n-k-1, if the array
is sorted until the position j-1, then the correct sorted element of array[j] is the minimum among the entries 
array[j], array[j+1], ..., array[j+k]. Thus, we construct a minimum heap with the
first k+1 elements of the array, and its root is clearly the first sorted element of the array.
Then, we traverse the array as follows: for j = k+1 to n, we replace the root of the current heap by array[k], and call
min_heapify for the root (since we only replaced the root, the right and left subtrees are heaps). After that, the root of the
heap will contain the minimum among the entries from j-k to j and, provided the array is already sorted until the (j-k-1)-th element,
this root will be the correct sorted (j-k)-th element. Once the first position is already correctly sorted after constructing
the min heap, finite induction guarantees that at each step the "backward" array is already sorted. When we reach the n-th term,
we will have the first n-k positions sorted. For the remaining, we simply use heapsort. */

void k_sorted_sort(int *vec, int n, int k){
    int min_heap[k+1];
    int j;
    for(j=0;j<=k;j++) min_heap[j] = vec[j];
    build_min_heap(min_heap,k+1);
    vec[0] = min_heap[0];
    for(j=k+1;j<n;j++){
        min_heap[0] = vec[j];
        min_heapfy(min_heap,k+1,0);
        vec[j-k] = min_heap[0];
    }

    heapsort(&vec[n-k],k);

}

/* Let us say that we want to merge m sorted lists in a way that the resulting list is also sorted. 
We can do that using a heap. However, we need to keep track of from what list each element becomes. 
For this and for other applications, sometimes it is useful to have a "label" within a heap node. We 
implement this idea next. */

typedef struct labeled_heap_node{
    int key;
    int label;
}LH;

void min_labeled_heapfy(LH *vec,int n,int i){
    int left = left_child(i);
    int right = right_child(i);
    int smallest = i;
    if(left < n && vec[left].key < vec[i].key) smallest = left;
    if(right < n && vec[right].key < vec[smallest].key) smallest = right;
    if(i != smallest){
        LH aux = vec[i];
        vec[i] = vec[smallest];
        vec[smallest] = aux;
        min_labeled_heapfy(vec,n,smallest);
    }
}

void build_min_labeled_heap(LH *vec, int n){
    int i, last_parent = parent(n-1);
    for(i=last_parent;i>=0;i--) min_labeled_heapfy(vec,n,i);
}

/* In this merge function, we will assume that all arrays have the same size n.
The m arrays are the lines of the (m x n) matrix arrays[][], which is passed as **arrays. */

void merge_sorted_lists(int **arrays, int m, int n, int *output){
    LH **aux_arrays;
    aux_arrays = malloc(m * sizeof(LH));
    int i,j;
    for(i=0;i<m;i++){
        aux_arrays[i] = malloc(n * sizeof(LH));
        for(j=0;j<n;j++){
            aux_arrays[i][j].key = arrays[i][j];
            aux_arrays[i][j].label = i;
        }
    }
    LH aux_heap[m];
    for(i=0;i<m;i++) aux_heap[i] = aux_arrays[i][0];
    build_min_labeled_heap(aux_heap,m);

    int vec_indexes[m];
    for(i=0;i<m;i++) vec_indexes[i] = 1;
    int output_index = 0;
    int next_input_from;
    while(output_index < m * n){
        *(output+output_index) = aux_heap[0].key;
        output_index++;
        next_input_from = aux_heap[0].label;
        if(vec_indexes[next_input_from] < n) {
            aux_heap[0] = aux_arrays[next_input_from][vec_indexes[next_input_from]];
            vec_indexes[next_input_from]++;
        }
        else if(vec_indexes[(next_input_from + 1) % 3] < n) {
            next_input_from = (next_input_from + 1) % 3;
            aux_heap[0] = aux_arrays[next_input_from][vec_indexes[next_input_from]];
            vec_indexes[next_input_from]++;
        }
        else if(vec_indexes[(next_input_from + 2) % 3] < n) {
            next_input_from = (next_input_from + 2) % 3;
            aux_heap[0] = aux_arrays[next_input_from][vec_indexes[next_input_from]];
            vec_indexes[next_input_from]++;
        }
        else{
            LH *big_node;
            big_node = malloc(sizeof(LH));
            big_node->key = INT_MAX;
            big_node->label = -1;
            aux_heap[0] = *big_node;
        }
        min_labeled_heapfy(aux_heap,m,0);
    }
}

/* REMARK: When there is no more array elements to insert into the heap, we simply put INT_MAX in the 
root and min heapfy again. This is not the most efficient way, since we could just heapfy smaller heaps.
However, this does not modify the time complexity of the solution, which is O(mnlog(m)).
We could also use this same idea to merge m sorted arrays with varying sizes. We just need to complete
all arrays but the longer one with INT_MAX in the missing ending positions. */

/* The next algorithm uses a labeled min heap to replace each element of an array by its rank. The "rank" of
an element is its sorted position in the array. That is, the minimum element of the array has rank 1, the
second minimum element has rank 2, and so on. */

void replace_by_rank(int *vec,int n){
    LH aux_vec[n];
    int i;
    for(i=0;i<n;i++){
        aux_vec[i].key = vec[i];
        aux_vec[i].label = i;
    }
    int rank = 1;
    build_min_labeled_heap(aux_vec,n);
    for(i=0;i<n;i++){
        vec[aux_vec[0].label] = rank++;
        aux_vec[0].key = INT_MAX;
        min_labeled_heapfy(aux_vec,n,0);
        
    }
}

/* M-HEAPS*/

/* A m-heap is a (min or max) heap in which each node has m children (except the last parent and the leaves, of course).
Below, we implement max m-heaps. */

int m_parent(int i, int m){
    if(i > 0){
        return (i-1)/m;
    }
    return -1;
}

int m_child(int i, int m, int k){
    /* returns the k-th child of the i-th node; k must be between 1 and m. */
    return m*i+k;
}

void m_max_heapfy(int m, int *vec, int n, int i){
    int largest = i, k, aux;
    for(k=1;k<=m;k++){
        if(m_child(i,m,k) < n && vec[m_child(i,m,k)] > vec[largest]) largest = m_child(i,m,k);
    }
    if(largest != i){
        aux = vec[largest];
        vec[largest] = vec[i];
        vec[i] = aux;
        m_max_heapfy(m,vec,n,largest);
    }
}

void build_m_max_heap(int m, int *vec, int n){
    int i, last_parent = parent(n-1);
    for(i=last_parent;i>=0;i--) m_max_heapfy(m,vec,n,i);
}

/* Insert and remove operations are also very similar to the case of binary heaps. */

void m_heap_insert(int m, int *vec, int n, int new){
    /* This code is exactly the same as the version for binary heaps -- notice that the parameter m is not even used. */
    vec[n] = new;
    int aux, i = n, j = parent(i);
    while(j >= 0 && vec[i] > vec[j]) {
        aux = vec[i];
        vec[i] = vec[j];
        vec[j] = aux;
        i = j;
        j = parent(i);
    }
}

int m_heap_remove(int m,int *vec, int n){
    int output = vec[0];
    vec[0] = vec[n-1];
    vec[n-1] = -INT_MAX;
    m_max_heapfy(m,vec,n-1,0);
    return output;
}


int main(void){

    /* TEST FOR merge_sorted_lists() 

    int **lists;
    lists = malloc(3 * sizeof(int));
    int i;
    for(i=0;i<3;i++){
        lists[i] = malloc(4*sizeof(int));
    }
    lists[0][0] = 5;
    lists[0][1] = 8;
    lists[0][2] = 10;
    lists[0][3] = 12;
    
    lists[1][0] = 1;
    lists[1][1] = 4;
    lists[1][2] = 11;
    lists[1][3] = 15;
    
    lists[2][0] = 3;
    lists[2][1] = 7;
    lists[2][2] = 9;
    lists[2][3] = 20;
    
    int *output;
    output = malloc(13 * sizeof(int));
    merge_sorted_lists(lists, 3, 4, output);

    for(int i=0;i<12;i++) printf("%d--",output[i]);

    END OF TEST FOR merged_sorted_lists() */


    /* TEST FOR INSERT AND REMOVE 

    int vec[11] = {10,6,8,5,4,3,1,2,7,9};
    build_min_heap(vec,10);
    int i;
    for(i=0;i<10;i++) printf("%d--",vec[i]);
    printf("\n");
    min_heap_insert(vec,10,0);
    for(i=0;i<11;i++) printf("%d--",vec[i]);
    printf("\n");
    printf("%d\n",min_heap_remove(vec,11));
    for(i=0;i<10;i++) printf("%d--",vec[i]);

    END OF TEST FOR INSERT AND REMOVE */

    /*TEST FOR m-HEAPS 

    int vec[11] = {5,4,7,8,2,3,9,6,1,10};
    build_m_max_heap(3,vec,10);
    for(int i=0;i<10;i++) printf("%d--",vec[i]);
    printf("\n");
    m_heap_insert(3,vec,10,12);
    for(int i=0;i<11;i++) printf("%d--",vec[i]);
    m_heap_remove(3,vec,11);
    printf("\n");
    for(int i=0;i<10;i++) printf("%d--",vec[i]);

    END OF TEST FOR m-HEAPS */

    return 0;
}