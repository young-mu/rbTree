#include "dmodule.h"

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
        struct dmodule_node* dmodule = dmodule_search(&dmodule_tree, LOG_TAG);  \
        if (dmodule != NULL) {                                                  \
            if (dmodule->debug || LOG_PRI == RUFF_LOG_ERROR) {                  \
                printf("(%s:%d) " fmt "\n", func, line, ##args);                \
            }                                                                   \
        } else {                                                                \
            printf("Error: dmodule '%s' is not registered!\n", LOG_TAG);        \
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
    register_dmodule("haha.haha");

    // list modules
    int num = get_dmodule_number();
    int* length = (int*)malloc(sizeof(int) * num);
    get_dmodule_length(length);

    char** dmodules_name = (char**)malloc(sizeof(char*) * num);
    bool* dmodules_debug = (bool*)malloc(sizeof(bool*) * num);
    for (int i = 0; i < num; i++) {
        dmodules_name[i] = (char*)malloc(sizeof(char) * (length[i] + 1));
        dmodules_debug[i] = (bool)malloc(sizeof(bool) * length[i]);
    }

    get_dmodule_info(dmodules_name, dmodules_debug);

    for (int i = 0; i < num; i++) {
        printf("%s:%d\n", dmodules_name[i], dmodules_debug[i]);
    }
    printf("\n");


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

    printf("unregister dmodule pwm\n");
    unregister_dmodule("pwm");
    printf("\n");

    printf("unregister dmodule i2c\n");
    unregister_dmodule("i2c");
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
