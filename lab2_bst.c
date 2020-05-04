/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32160000, 32162436
*	    Student name : 허전진, 신창
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"
/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order.
 *  @return                 : status (success or fail)
 */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void inorder(lab2_tree *tree, lab2_node *C_Node){ //add inorder fuction
    if (C_Node != NULL){ //C_node is not null
        inorder(tree, C_Node->left);
        inorder(tree, C_Node->right);
    }
}

int lab2_node_print_inorder(lab2_tree *tree)
{
    inorder(tree, tree->root);
    return LAB2_SUCCESS; //LAB2_SUCCESS`s value is 0
    // You need to implement lab2_node_print_inorder function.
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    lab2_tree *tree = (lab2_tree *)malloc(sizeof(lab2_tree)); //create tree by dynamic allocation
    tree->root = NULL;
    pthread_mutex_init(&tree->mutex, NULL);
    return tree;
    // You need to implement lab2_tree_create function.
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    lab2_node *node = (lab2_node *)malloc(sizeof(lab2_node)); //create node by dynamic allocation
    node->key = key;
    pthread_mutex_init(&node->mutex, NULL);
    return node;
    // You need to implement lab2_node_create function.
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node) {
    lab2_node *tmp = tree -> root;
    if(tmp == NULL) { //if tree is NULL
        tree -> root = new_node; // make root with new_node
    }
    else {
        while(1) {
            if(tmp -> key < new_node -> key) { // if inserting key < current key
                if(!(tmp -> right)) { // if current's right child is NULL
                    tmp -> right =  new_node; // Make new node at that point
                    break;
                }
                else{
                    tmp = tmp -> right; // if right child not NULL, re-search to right child
                }
            }
            
            else if(tmp -> key > new_node -> key) { // if inserting key > current key
                if(!(tmp -> left)) { // if current's left child is NULL
                    tmp -> left = new_node; // Make new node at that point
                    break;
                }
                else{
                    tmp = tmp -> left; // if left child not NULL, re-search to left child
                }
            }
            else break;
        }
    }
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
      // You need to implement lab2_node_insert_fg function.
    lab2_node *tmp = tree -> root;
    if(tmp == NULL) { //if tree is NULL
        pthread_mutex_lock(&tree -> mutex);
        tree -> root = new_node; // make root with new_node
        pthread_mutex_unlock(&tree -> mutex);
    }
    else {
        while(1) {
            if(tmp -> key < new_node -> key) { // if inserting key < current key
                if(!(tmp -> right)) { // if current's right child is NULL
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp -> right =  new_node; // Make new node at that point
                    pthread_mutex_unlock(&tmp -> mutex);
                    break;
                }
                else{
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp = tmp -> right; // if right child not NULL, re-search to right child
                    pthread_mutex_unlock(&tmp -> mutex);
                }
            }
            else if(tmp -> key > new_node -> key) { // if inserting key > current key
                if(!(tmp -> left)) { // if current's left child is NULL
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp -> left = new_node; // Make new node at that point
                    pthread_mutex_unlock(&tmp -> mutex);
                    break;
                }
                else{
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp = tmp -> left; // if right left not NULL, re-search to left child
                    pthread_mutex_unlock(&tmp -> mutex);
                }
            }
            else break;
        }
    }
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function. // lock 걸어줌
    pthread_mutex_lock(&mutex);
    lab2_node *tmp = tree -> root;
    if(tmp == NULL) { //if tree is NULL
        tree -> root = new_node; // make root with new_node
    }
    else {
        while(1) {
            if(tmp -> key < new_node -> key) { // if inserting key < current key
                if(!(tmp -> right)) { // if current's right child is NULL
                    tmp -> right =  new_node; // Make new node at that point
                    break;
                }
                else{
                tmp = tmp -> right; // if right child not NULL, re-search to right child
                }
            }
            else if(tmp -> key > new_node -> key) { // if inserting key > current key
                if(!(tmp -> left)) { // if current's left child is NULL
                    tmp -> left = new_node; // Make new node at that point
                    break;
                }
                else{
                tmp = tmp -> left; // if left child not NULL, re-search to left child
                }
            }
            else break;
        }
    }
    pthread_mutex_unlock(&mutex);
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
    lab2_node *temp = tree -> root;
    lab2_node *parent = NULL , *child, *succ, *succ_p;
    while(temp != NULL && (temp -> key != key)) {
        parent = temp;
        if(temp -> key < key) {
            temp = temp -> right;
        } else {
            temp = temp -> left;
        }
    }
    if(temp == NULL) {
        return LAB2_SUCCESS;
    }
    if((temp -> left == NULL) && (temp -> right == NULL)) { // ?꾨옒???먯떇 ?몃뱶媛 ?놁쓣 寃쎌슦
        if (parent != NULL)
        {
            if(parent -> left == temp) {
                parent -> left = NULL;
            } else {
                parent -> right = NULL;
            }
        } else {
            tree -> root = NULL;
        }
        
    } else if(temp -> left == NULL || temp -> right == NULL) { // ?꾨옒??1媛쒖쓽 ?먯떇 ?몃뱶媛 ?덉쓣 寃쎌슦
        child = (temp -> left != NULL) ? temp -> left : temp -> right;
        if(parent != NULL) {
            if(parent -> left == temp) {
                parent -> left = child;
            } else {
                parent -> right = child;
            }
        } else {
            tree -> root = child;
        }
    } else {
          // ?꾨옒??2媛쒖쓽 ?먯떇 ?몃뱶媛 ?덉쓣 寃쎌슦
        succ_p = temp;
        succ = temp -> right;
        while(succ -> left != NULL) {
            succ_p = succ;
            succ = succ -> left;
        }
        if(succ_p -> left == succ) {
            succ_p -> left = succ -> right;
        } else {
            succ_p -> right = succ -> right;
        }
        temp -> key = succ -> key;
        temp = succ;
    }
    lab2_node_delete(temp);
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_fg function.
    lab2_node *temp = tree -> root;
    lab2_node *parent = NULL , *child, *succ, *succ_p;
    
    pthread_mutex_lock(&tree -> mutex);
    while(temp != NULL && temp -> key != key) {
        parent = temp;
        if(temp -> key < key) {
            temp = temp -> right;
        } else {
            temp = temp -> left;
        }
    }
    pthread_mutex_unlock(&tree -> mutex);
    
    if(temp == NULL) {
        return LAB2_SUCCESS;
    }
    pthread_mutex_lock(&tree -> mutex);
    
    if((temp -> left == NULL) && (temp -> right == NULL)) { // ?꾨옒???먯떇 ?몃뱶媛 ?놁쓣 寃쎌슦
        if (parent != NULL)
        {
            
            if(parent -> left == temp) {
                parent -> left = NULL;
            } else {
                parent -> right = NULL;
            }
            pthread_mutex_unlock(&tree -> mutex);
        } else {
            tree -> root = NULL;
            pthread_mutex_unlock(&tree -> mutex);
        }
    } else if(temp -> left == NULL || temp -> right == NULL) { // ?꾨옒??1媛쒖쓽 ?먯떇 ?몃뱶媛 ?덉쓣 寃쎌슦
        child = (temp -> left != NULL) ? temp -> left : temp -> right;
        if(parent != NULL) {
            if(parent -> left == temp) {
                parent -> left = child;
            } else {
                parent -> right = child;
            }
            pthread_mutex_unlock(&tree ->mutex);
        } else {
            tree -> root = child;
            pthread_mutex_unlock(&tree ->mutex);
        }
    } else {
          // ?꾨옒??2媛쒖쓽 ?먯떇 ?몃뱶媛 ?덉쓣 寃쎌슦
        succ_p = temp;
        succ = temp -> right;
        while(succ -> left != NULL) {
            succ_p = succ;
            succ = succ -> left;
        }
        if(succ_p -> left == succ) {
            succ_p -> left = succ -> right;
        } else {
            succ_p -> right = succ -> right;
        }
        temp -> key = succ -> key;
        temp = succ;
        pthread_mutex_unlock(&tree -> mutex);
    
    }
    pthread_mutex_lock(&tree -> mutex);
    lab2_node_delete(temp);
    pthread_mutex_unlock(&tree -> mutex);
    return LAB2_SUCCESS;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    pthread_mutex_lock(&mutex);
    lab2_node *temp = tree -> root;
    lab2_node *parent = NULL , *child, *succ, *succ_p;
    while(temp != NULL && temp -> key != key) {
        parent = temp;
        if(temp -> key < key) {
            temp = temp -> right;
        } else {
            temp = temp -> left;
        }
    }
    if(temp == NULL) {
        pthread_mutex_unlock(&mutex);
        return LAB2_SUCCESS;
    }
    if((temp -> left == NULL) && (temp -> right == NULL)) { // ?꾨옒???먯떇 ?몃뱶媛 ?놁쓣 寃쎌슦
        if (parent != NULL)
        {
            if(parent -> left == temp) {
                parent -> left = NULL;
            } else {
                parent -> right = NULL;
            }
        } else {
            tree -> root = NULL;
        }
        
    } else if(temp -> left == NULL || temp -> right == NULL) { // ?꾨옒??1媛쒖쓽 ?먯떇 ?몃뱶媛 ?덉쓣 寃쎌슦
        child = (temp -> left != NULL) ? temp -> left : temp -> right;
        if(parent != NULL) {
            if(parent -> left == temp) {
                parent -> left = child;
            } else {
                parent -> right = child;
            }
        } else {
            tree -> root = child;
        }
    } else {
          // ?꾨옒??2媛쒖쓽 ?먯떇 ?몃뱶媛 ?덉쓣 寃쎌슦
        succ_p = temp;
        succ = temp -> right;
        while(succ -> left != NULL) {
            succ_p = succ;
            succ = succ -> left;
        }
        if(succ_p -> left == succ) {
            succ_p -> left = succ -> right;
        } else {
            succ_p -> right = succ -> right;
        }
        temp -> key = succ -> key;
        temp = succ;
    }
    lab2_node_delete(temp);
    pthread_mutex_unlock(&mutex);
    return LAB2_SUCCESS;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {//delete tree fuction
    lab2_node *temp = tree -> root;
    if(!temp) return;
    while(temp) {
        int key = temp -> key;
        lab2_node_remove(tree, key);
        temp = tree -> root;
    }
    // You need to implement lab2_tree_delete function.
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) { //delete node fuction
    free(node);
    node = NULL;
    // You need to implement lab2_node_delete function.
}