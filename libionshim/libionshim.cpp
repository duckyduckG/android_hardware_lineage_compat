/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "libion_shim"

#include <stddef.h>
#include <stdint.h>
#include <log/log.h>
// idk what am i doing
extern "C" int ion_alloc_fd(int fd, size_t len, size_t align,
                             unsigned int heap_mask, unsigned int flags,
                             int* handle_fd);

extern "C" int ion_alloc_shim(int fd, size_t len, size_t align,
                               unsigned int heap_mask, unsigned int flags,
                               int* handle_fd) {
    if (heap_mask == 0x8000000) {
        ALOGD("ion_shim: fixing heap_mask 0x8000000 > 0x80");
        heap_mask = 0x80;
    }

    return ion_alloc_fd(fd, len, align, heap_mask, flags, handle_fd);
}
