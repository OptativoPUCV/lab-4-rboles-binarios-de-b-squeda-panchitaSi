#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * newMap = (TreeMap*)malloc(sizeof(TreeMap));
    if (newMap == NULL) return NULL;

    newMap->root = NULL;
    newMap->current = NULL;
    newMap->lower_than = lower_than;
    return newMap;
}

/*INSTRUCCIONES:
Esta función inserta un nuevo dato (key,value) en el árbol y hace que el current apunte al nuevo nodo.
Para insertar un dato, primero debe realizar una búsqueda para encontrar donde debería ubicarse.
Luego crear el nuevo nodo y enlazarlo.
Si la clave del dato ya existe retorne sin hacer nada*/
void insertTreeMap(TreeMap * tree, void* key, void * value) {
    tree->current = tree->root;
    while(1){
        if (is_equal(tree,tree->current->pair->key,key)){
            return;
        } 
        if (tree->lower_than(tree->current->pair->key,key)){
            if (tree->current->right == NULL){
                TreeNode* newNode = createTreeNode(key,value);
                newNode->parent = tree->current;
                tree->current->right = newNode;
                tree->current = newNode;
                break;
            } 
            tree->current = tree->current->right;
        } 
        else{
            if (tree->current->left == NULL){
                TreeNode* newNode = createTreeNode(key,value);
                newNode->parent = tree->current;
                tree->current->left = newNode;
                tree->current = newNode;
                break;
            } 
            tree->current = tree->current->left;
        } 
    }

}

TreeNode * minimum(TreeNode * x){

    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL){
        if (is_equal(tree,node->parent->left->pair->key,node->pair->key)) node->parent->left = NULL;
        else node->parent->right = NULL;
    }
    else if (node->left != NULL && node->right != NULL){
        TreeNode* minimo = minimum(node->right);
        node->pair = minimo->pair;
        removeNode(tree,minimo);
    }
    else{
        if(node->right == NULL){
            if(tree->lower_than(node->left->pair->key,node->parent->pair->key))node->parent->left = node->left;
            else node->parent->right = node->left;
            node->left->parent = node->parent;
        }
        else{
            if(tree->lower_than(node->right->pair->key,node->parent->pair->key))node->parent->left = node->right;
            else node->parent->right = node->right;
            node->right->parent = node->parent;
        }
    }
    free(node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}



/*INSTRUCCIONES:
busca el nodo con clave igual a key y retorna el Pair asociado
Si no se encuentra la clave retorna NULL.
Recuerde hacer que el current apunte al nodo encontrado.*/
Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL)return NULL;
    tree->current = tree->root;
    while(tree->current != NULL){
        if (is_equal(tree,tree->current->pair->key,key))return tree->current->pair;
        if (tree->lower_than(tree->current->pair->key,key)) tree->current = tree->current->right;
        else tree->current = tree->current->left;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current->right != NULL){
        tree->current = minimum(tree->current->right);
    }
    else{
        TreeNode* padre = tree->current->parent;
        TreeNode* aux = tree->current;
        while(padre != NULL && padre->right == aux){
            aux = padre;
            padre = padre->parent;
        }
        if (padre != NULL) tree->current = padre;
    }
    return tree->current->pair;
}
