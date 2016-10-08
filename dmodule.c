#include "dmodule.h"

struct rb_root dmodule_tree = RB_ROOT;

static int _dmodule_insert(struct rb_root* root, struct dmodule_node* dmodule)
{
    struct rb_node** new = &(root->rb_node);
    struct rb_node* parent = NULL;

    // Figure out where to put new code
    while (*new) {
        struct dmodule_node* this = rb_entry(*new, struct dmodule_node, node);
        int result = strcmp(dmodule->name, this->name);
        parent = *new;
        if (result < 0) {
            new = &((*new)->rb_left);
        } else if (result > 0) {
            new = &((*new)->rb_right);
        } else {
            return 0;
        }
    }

    // Add new node and rebalance tree
    rb_link_node(&dmodule->node, parent, new);
    rb_insert_color(&dmodule->node, root);

    return 1;
}

static void _dmodule_free(struct dmodule_node* dmodule)
{
    if (dmodule != NULL) {
        if (dmodule->name != NULL) {
            free(dmodule->name);
            dmodule->name = NULL;
        }
        free(dmodule);
        dmodule = NULL;
    }
}

struct dmodule_node* dmodule_search(struct rb_root* root, char* name)
{
    struct rb_node* node = root->rb_node;
    while (node) {
        struct dmodule_node* dmodule = rb_entry(node, struct dmodule_node, node);
        int result;
        result = strcmp(name, dmodule->name);
        if (result < 0) {
            node = node->rb_left;
        } else if (result > 0) {
            node = node->rb_right;
        } else {
            return dmodule;
        }
    }
    return NULL;
}

int register_dmodule(char* dmodule_name)
{
    struct dmodule_node* dmodule = (struct dmodule_node*)malloc(sizeof(struct dmodule_node));
    if (dmodule == NULL) {
        printf("Error: malloc failed\n");
        return -1;
    }

    dmodule->name = (char*)malloc(sizeof(char) * (strlen(dmodule_name) + 1));
    snprintf(dmodule->name, strlen(dmodule_name) + 1, "%s", dmodule_name);
    dmodule->debug = false;
    _dmodule_insert(&dmodule_tree, dmodule);

    return 0;
}

int unregister_dmodule(char* dmodule_name)
{
    struct dmodule_node* dmodule = dmodule_search(&dmodule_tree, dmodule_name);
    if (dmodule) {
        rb_erase(&dmodule->node, &dmodule_tree);
        _dmodule_free(dmodule);
    }

    return 0;
}

int get_dmodule_number(int* num)
{
    struct rb_node* node = (&dmodule_tree)->rb_node;

    int index = 0;
    for (node = rb_first(&dmodule_tree); node; node = rb_next(node)) {
        index++;
    }

    *num = index;
    return 0;
}

int get_dmodule_length(int* length)
{
    struct rb_node* node = (&dmodule_tree)->rb_node;

    int num = 0;
    for (node = rb_first(&dmodule_tree); node; node = rb_next(node)) {
        struct dmodule_node* dmodule = rb_entry(node, struct dmodule_node, node);
        length[num] = (int)strlen(dmodule->name);
        num++;
    }

    return 0;
}

int get_dmodule_info(char** dmodule_name, bool* dmodule_debug)
{
    struct rb_node* node = (&dmodule_tree)->rb_node;

    int index = 0;
    for (node = rb_first(&dmodule_tree); node; node = rb_next(node)) {
        struct dmodule_node* dmodule = rb_entry(node, struct dmodule_node, node);
        if (dmodule_name != NULL) {
            snprintf(dmodule_name[index], strlen(dmodule->name) + 1, "%s", dmodule->name);
        }
        if (dmodule_debug != NULL) {
            dmodule_debug[index] = dmodule->debug;
        }
        index++;
    }

    return 0;
}

int enable_dmodule(char* dmodule_name)
{
    struct dmodule_node* dmodule = dmodule_search(&dmodule_tree, dmodule_name);
    if (dmodule == NULL) {
        printf("Error: no dmodule called '%s'!\n", dmodule_name);
        return -1;
    }

    dmodule->debug = true;

    return 0;
}

int disable_dmodule(char* dmodule_name)
{
    struct dmodule_node* dmodule = dmodule_search(&dmodule_tree, dmodule_name);
    if (dmodule == NULL) {
        printf("Error: no dmodule called '%s'!\n", dmodule_name);
        return -1;
    }

    dmodule->debug = false;

    return 0;
}

void preorder_traverse(struct rb_node* node)
{
    if (node) {
        struct dmodule_node* dmodule = rb_entry(node, struct dmodule_node, node);
        printf("%s:%d ", dmodule->name, dmodule->debug);
        preorder_traverse(node->rb_left);
        preorder_traverse(node->rb_right);
    }
}

void inorder_traverse(struct rb_node* node)
{
    if (node) {
        inorder_traverse(node->rb_left);
        struct dmodule_node* dmodule = rb_entry(node, struct dmodule_node, node);
        printf("%s:%d ", dmodule->name, dmodule->debug);
        inorder_traverse(node->rb_right);
    }
}

void postorder_traverse(struct rb_node* node)
{
    if (node) {
        postorder_traverse(node->rb_left);
        postorder_traverse(node->rb_right);
        struct dmodule_node* dmodule = rb_entry(node, struct dmodule_node, node);
        printf("%s:%d ", dmodule->name, dmodule->debug);
    }
}
