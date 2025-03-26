/*!
 * Copyright (c) 2025, ErBW_s
 * All rights reserved.
 * 
 * @author  Baohan
 */

#ifndef NODES_MQ2_H
#define NODES_MQ2_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

typedef struct {
    float smoke_dense;
    bool detected;
} mq2_info_t;

mq2_info_t mq2_read();

#ifdef __cplusplus
}
#endif

#endif
