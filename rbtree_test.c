#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct my_node {
    struct rb_node node;
    char *str;
};

struct rb_root my_tree = RB_ROOT;

struct my_node *mySearch(struct rb_root *root, char *str)
{
    struct rb_node *node = root->rb_node;
    while (node) {
        struct my_node *data = rb_entry(node, struct my_node, node);
        int result;
        result = strcmp(str, data->str);
        if (result < 0) {
            node = node->rb_left;
        } else if (result > 0) {
            node = node->rb_right;
        } else {
            return data;
        }
    }
    return NULL;
}

int myInsert(struct rb_root *root, struct my_node *data)
{
    struct rb_node **new = &(root->rb_node);
    struct rb_node *parent = NULL;

    /* Figure out where to put new code */
    while (*new) {
        struct my_node *this = rb_entry(*new, struct my_node, node);
        int result = strcmp(data->str, this->str);
        parent = *new;
        if (result < 0) {
            new = &((*new)->rb_left);
        } else if (result > 0) {
            new = &((*new)->rb_right);
        } else {
            return 0;
        }
    }

    /* Add new node and rebalance tree */
    rb_link_node(&data->node, parent, new);
    rb_insert_color(&data->node, root);

    return 1;
}

void myFree(struct my_node *node)
{
    if (node != NULL) {
        if (node->str != NULL) {
            free(node->str);
            node->str = NULL;
        }
        free(node);
        node = NULL;
    }
}

void PreOrder(struct rb_node *node)
{
    if (node) {
        struct my_node *data = rb_entry(node, struct my_node, node);
        printf("%s ", data->str);
        PreOrder(node->rb_left);
        PreOrder(node->rb_right);
    }
}

void InOrder(struct rb_node *node)
{
    if (node) {
        InOrder(node->rb_left);
        struct my_node *data = rb_entry(node, struct my_node, node);
        printf("%s ", data->str);
        InOrder(node->rb_right);
    }
}

void PostOrder(struct rb_node *node)
{
    if (node) {
        PostOrder(node->rb_left);
        PostOrder(node->rb_right);
        struct my_node *data = rb_entry(node, struct my_node, node);
        printf("%s ", data->str);
    }
}

int main(int argc, const char *argv[])
{
    struct my_node *nodes[8];

    /* insert */
    int i;
    for (i = 0; i < 8; i++) {
        nodes[i] = (struct my_node *)malloc(sizeof(struct my_node));
        nodes[i]->str = (char *)malloc(sizeof(char) * 4);
        sprintf(nodes[i]->str, "%d", i);
        myInsert(&my_tree, nodes[i]);
    }

    struct rb_node *node = (&my_tree)->rb_node;
    printf("PreOrder: \n");
    PreOrder(node);
    printf("\n");

    printf("InOrder: \n");
    InOrder(node);
    printf("\n");

    printf("PostOrder: \n");
    PostOrder(node);
    printf("\n");

    /* search */
    for (node = rb_first(&my_tree); node; node = rb_next(node)) {
        printf("key = %s\n", rb_entry(node, struct my_node, node)->str);
    }

    /* delete */
    printf("delete node 3\n");
    struct my_node *data = mySearch(&my_tree, "3");
    if (data) {
        rb_erase(&data->node, &my_tree);
        myFree(data);
    }

    printf("delete node 7\n");
    data = mySearch(&my_tree, "7");
    if (data) {
        rb_erase(&data->node, &my_tree);
        myFree(data);
    }

    /* search */
    for (node = rb_first(&my_tree); node; node = rb_next(node)) {
        printf("key = %s\n", rb_entry(node, struct my_node, node)->str);
    }

    return 0;
}
