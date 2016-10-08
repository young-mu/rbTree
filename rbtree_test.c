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

static int _dmodule_insert(struct rb_root* root, struct dmodule_node* dmodule);
static void _dmodule_free(struct dmodule_node* dmodule);

struct dmodule_node* dmodule_search(struct rb_root* root, char* name);

int register_dmodule(char* dmodule_name);
int unregister_dmodule(char* dmodule_name);
int list_dmodule(void);

int enable_dmodule(char* dmodule_name);
int disable_dmodule(char* dmodule_name);

void preorder_traverse(struct rb_node* node);
void inorder_traverse(struct rb_node* node);
void postorder_traverse(struct rb_node* node);

struct rb_root dmodule_tree = RB_ROOT;

#define DEBUG 1

enum _LOG_PRI {
    RUFF_LOG_DEBUG = 1,
    RUFF_LOG_ERROR
};

#define LOGE(LOG_TAG, ...)   _LOG(RUFF_LOG_ERROR, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)
#define LOGD(LOG_TAG, ...)   _LOG(RUFF_LOG_DEBUG, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)

#if DEBUG
#define _LOG(LOG_PRI, LOG_TAG, func, line, fmt, args...)                        \
    do {                                                                        \
        struct dmodule_node* dmodule = dmodule_search(&dmodule_tree, LOG_TAG);   \
        if (dmodule != NULL) {                                                   \
            if (dmodule->debug || LOG_PRI == RUFF_LOG_ERROR) {                   \
                printf("(%s:%d) " fmt "\n", func, line, ##args);                \
            }                                                                   \
        } else {                                                                \
            printf("Error: dmodule '%s' is not registered!\n", LOG_TAG);         \
        }                                                                       \
    } while (0)
#else
#define _LOG(LOG_PRI, LOG_TAG, func, line, fmt, args...)                        \
    do {                                                                        \
        if (LOG_PRI == RUFF_LOG_ERROR) {                                        \
            printf("(%s:%d) " fmt "\n", func, line, ##args);                    \
        }                                                                       \
    } while (0)
#endif

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

int list_dmodule(void)
{
    struct rb_node* node = (&dmodule_tree)->rb_node;

    for (node = rb_first(&dmodule_tree); node; node = rb_next(node)) {
        struct dmodule_node* dmodule = rb_entry(node, struct dmodule_node, node);
        printf("%s:%d ", dmodule->name, dmodule->debug);
    }

    printf("\n");

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

int main(int argc, const char* argv[])
{
    register_dmodule("pwm");
    register_dmodule("pwm_jerry");
    register_dmodule("i2c");
    register_dmodule("i2c_jerry");
    register_dmodule("qei");
    register_dmodule("qei_jerry");
    register_dmodule("uart");
    register_dmodule("fs");
    register_dmodule("memory");

    enable_dmodule("pwm");
    disable_dmodule("pwm");
    enable_dmodule("pwm_jerry");
    enable_dmodule("xxx");

    LOGD("pwm", "[debug] pwm");
    LOGE("pwm", "[error] pwm");
    LOGD("pwm_jerry", "[debug] pwm_jerry");
    LOGE("pwm_jerry", "[error] pwm_jerry");
    LOGD("qei", "[debug] qei");
    LOGE("qei", "[error] qei");
    LOGD("ok", "[debug] ok");
    printf("\n");

    printf("list dmodules -> ");
    list_dmodule();
    printf("\n");

    printf("unregister dmodule pwm\n");
    unregister_dmodule("pwm");
    printf("\n");

    printf("unregister dmodule i2c\n");
    unregister_dmodule("i2c");
    printf("\n");

    printf("list dmodules -> ");
    list_dmodule();
    printf("\n");

    struct rb_node* node = (&dmodule_tree)->rb_node;
    printf("preorder_traverse: \n");
    preorder_traverse(node);
    printf("\n\n");

    printf("inorder_traverse: \n");
    inorder_traverse(node);
    printf("\n\n");

    printf("postorder_traverse: \n");
    postorder_traverse(node);
    printf("\n\n");

    return 0;
}
