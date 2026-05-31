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

/*
 * Heap ID bitmask remapping from 4.19 to 5.4 kernel.
 *
 * 4.19 kernel: drivers/staging/android/uapi/msm_ion.h
 * 5.4  kernel: include/uapi/linux/msm_ion_ids.h
 */
static unsigned int remap_heap_mask(unsigned int heap_mask) {
    static const struct {
        unsigned int old_mask;
        unsigned int new_mask;
        const char*  name;
    } heap_map[] = {
        { 1U << 27, 1U << 7,  "ION_QSECOM_HEAP_ID"          },
        { 1U << 26, 1U << 6,  "ION_USER_CONTIG_HEAP_ID"     },
        { 1U << 25, 1U << 25, "ION_SYSTEM_HEAP_ID"          },
        { 1U << 28, 1U << 8,  "ION_AUDIO_HEAP_ID"           },
        { 1U << 22, 1U << 4,  "ION_ADSP_HEAP_ID"            },
        { 1U << 20, 1U << 2,  "ION_CAMERA_HEAP_ID"          },
        { 1U << 19, 1U << 1,  "ION_QSECOM_TA_HEAP_ID"       },
        { 1U << 14, 1U << 15, "ION_SECURE_CARVEOUT_HEAP_ID" },
        { 1U << 13, 1U << 14, "ION_SPSS_HEAP_ID"            },
        { 1U << 10, 1U << 11, "ION_SECURE_DISPLAY_HEAP_ID"  },
        { 1U << 9,  1U << 10, "ION_SECURE_HEAP_ID"          },
        { 1U << 8,  1U << 9,  "ION_CP_MM_HEAP_ID"           },
    };

    for (size_t i = 0; i < sizeof(heap_map) / sizeof(heap_map[0]); i++) {
        if (heap_mask == heap_map[i].old_mask) {
            ALOGD("ion_shim: remapping %s 0x%x > 0x%x",
                  heap_map[i].name, heap_mask, heap_map[i].new_mask);
            return heap_map[i].new_mask;
        }
    }

    ALOGW("ion_shim: unknown heap_mask 0x%x passing through", heap_mask);
    return heap_mask;
}

extern "C" int ion_alloc_fd(int fd, size_t len, size_t align,
                             unsigned int heap_mask, unsigned int flags,
                             int* handle_fd);

extern "C" int ion_alloc_shim(int fd, size_t len, size_t align,
                               unsigned int heap_mask, unsigned int flags,
                               int* handle_fd) {
    heap_mask = remap_heap_mask(heap_mask);
    return ion_alloc_fd(fd, len, align, heap_mask, flags, handle_fd);
}
