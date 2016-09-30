#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct module_node {
    struct rb_node node;
    bool debug;
    char *name;
};

struct module_node *module_search(struct rb_root *root, char *name);
int module_insert(struct rb_root *root, struct module_node *module);
void module_free(struct module_node *module);
void register_module(char *module_name);
void enable_module_debug(char *module_name);
void preorder_traverse(struct rb_node *node);
void inorder_traverse(struct rb_node *node);
void postorder_traverse(struct rb_node *node);

struct rb_root module_tree = RB_ROOT;

#define DEBUG 1

enum _LOG_PRI {
    RUFF_LOG_DEBUG = 1,
    RUFF_LOG_ERROR
};

#define LOGE(LOG_TAG, ...)   _LOG(RUFF_LOG_ERROR, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)
#define LOGD(LOG_TAG, ...)   _LOG(RUFF_LOG_DEBUG, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)

#if DEBUG
#define _LOG(LOG_PRI, LOG_TAG, func, line, fmt, args...)                    \
    do {                                                                    \
        struct module_node *node = module_search(&module_tree, LOG_TAG);    \
        if (node->debug || LOG_PRI == RUFF_LOG_ERROR) {                     \
            printf("(%s:%d) " fmt "\n", func, line, ##args);                \
        }                                                                   \
    } while (0)
#else
#define _LOG(LOG_PRI, LOG_TAG, func, line, fmt, args...)                    \
    do {                                                                    \
        if (LOG_PRI == RUFF_LOG_ERROR) {                                    \
            printf("(%s:%d) " fmt "\n", func, line, ##args);                \
        }                                                                   \
    } while (0)
#endif

struct module_node *module_search(struct rb_root *root, char *name)
{
    struct rb_node *node = root->rb_node;
    while (node) {
        struct module_node *module = rb_entry(node, struct module_node, node);
        int result;
        result = strcmp(name, module->name);
        if (result < 0) {
            node = node->rb_left;
        } else if (result > 0) {
            node = node->rb_right;
        } else {
            return module;
        }
    }
    return NULL;
}

int module_insert(struct rb_root *root, struct module_node *module)
{
    struct rb_node **new = &(root->rb_node);
    struct rb_node *parent = NULL;

    // Figure out where to put new code
    while (*new) {
        struct module_node *this = rb_entry(*new, struct module_node, node);
        int result = strcmp(module->name, this->name);
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
    rb_link_node(&module->node, parent, new);
    rb_insert_color(&module->node, root);

    return 1;
}

void module_free(struct module_node *module)
{
    if (module != NULL) {
        if (module->name != NULL) {
            free(module->name);
            module->name = NULL;
        }
        free(module);
        module = NULL;
    }
}

void register_module(char *module_name)
{
    struct module_node *module = (struct module_node*)malloc(sizeof(struct module_node));
    module->name = (char*)malloc(sizeof(char) * (strlen(module_name) + 1));

    snprintf(module->name, strlen(module_name) + 1, "%s", module_name);
    module->debug = false;

    module_insert(&module_tree, module);
}

void enable_module_debug(char *module_name)
{
    struct module_node *module = module_search(&module_tree, module_name);
    module->debug = true;
}


void preorder_traverse(struct rb_node *node)
{
    if (node) {
        struct module_node *module = rb_entry(node, struct module_node, node);
        printf("%s%d ", module->name, module->debug);
        preorder_traverse(node->rb_left);
        preorder_traverse(node->rb_right);
    }
}

void inorder_traverse(struct rb_node *node)
{
    if (node) {
        inorder_traverse(node->rb_left);
        struct module_node *module = rb_entry(node, struct module_node, node);
        printf("%s:%d ", module->name, module->debug);
        inorder_traverse(node->rb_right);
    }
}

void postorder_traverse(struct rb_node *node)
{
    if (node) {
        postorder_traverse(node->rb_left);
        postorder_traverse(node->rb_right);
        struct module_node *module = rb_entry(node, struct module_node, node);
        printf("%s:%d ", module->name, module->debug);
    }
}

int main(int argc, const char *argv[])
{
    register_module("pwm");
    register_module("pwm_jerry");
    register_module("i2c");
    register_module("i2c_jerry");
    register_module("qei");
    register_module("qei_jerry");
    register_module("uart");
    register_module("fs");
    register_module("memory");

    enable_module_debug("pwm");
    enable_module_debug("pwm_jerry");

//    struct rb_node *node = (&module_tree)->rb_node;
//    printf("preorder_traverse: \n");
//    preorder_traverse(node);
//    printf("\n");
//
//    printf("inorder_traverse: \n");
//    inorder_traverse(node);
//    printf("\n");
//
//    printf("postorder_traverse: \n");
//    postorder_traverse(node);
//    printf("\n");
//
//    /* search */
//    printf("\n");
//    for (node = rb_first(&module_tree); node; node = rb_next(node)) {
//        struct module_node *module = rb_entry(node, struct module_node, node);
//        printf("%s:%d\n", module->name, module->debug);
//    }
//    printf("\n");
//
//    /* delete */
//    printf("delete node pwm\n");
//    struct module_node *module = module_search(&module_tree, "pwm");
//    if (module) {
//        rb_erase(&module->node, &module_tree);
//        module_free(module);
//    }
//
//    printf("delete node i2c\n");
//    module = module_search(&module_tree, "i2c");
//    if (module) {
//        rb_erase(&module->node, &module_tree);
//        module_free(module);
//    }
//
//    /* search */
//    printf("\n");
//    for (node = rb_first(&module_tree); node; node = rb_next(node)) {
//        struct module_node *module = rb_entry(node, struct module_node, node);
//        printf("%s:%d\n", module->name, module->debug);
//    }
//    printf("\n");

    LOGD("pwm", "[debug] pwm");
    LOGE("pwm", "[error] pwm");

    LOGD("pwm_jerry", "[debug] pwm_jerry");
    LOGE("pwm_jerry", "[error] pwm_jerry");

    LOGD("qei", "[debug] qei");
    LOGE("qei", "[error] qei");

    return 0;
}
