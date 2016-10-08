#pragma once

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct dmodule_node {
    struct rb_node node;
    bool debug;
    char* name;
};

extern struct dmodule_node* dmodule_search(struct rb_root* root, char* name);

extern int register_dmodule(char* dmodule_name);
extern int unregister_dmodule(char* dmodule_name);

extern int get_dmodule_number(int* num);
extern int get_dmodule_length(int* length);
extern int get_dmodule_info(char** dmodule_name, bool* dmodule_debug);

extern int enable_dmodule(char* dmodule_name);
extern int disable_dmodule(char* dmodule_name);

extern void preorder_traverse(struct rb_node* node);
extern void inorder_traverse(struct rb_node* node);
extern void postorder_traverse(struct rb_node* node);

extern struct rb_root dmodule_tree;
