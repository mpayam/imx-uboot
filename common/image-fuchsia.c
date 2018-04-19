/*
 * Copyright (c) 2018 The Fuchsia Authors
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <image.h>
#include <fuchsia/bootdata.h>

int fuchsia_image_check_header(const void *hdr)
{
    const bootdata_t* bootdata = hdr;

    return (bootdata[0].type == BOOTDATA_CONTAINER &&
            bootdata[0].extra == BOOTDATA_MAGIC &&
            bootdata[0].magic == BOOTITEM_MAGIC &&
            bootdata[1].type == BOOTDATA_KERNEL &&
            bootdata[1].magic == BOOTITEM_MAGIC) ? 0 : -1;
}

/**
 * fuchsia_image_get_kernel() - processes kernel part of Fuchsia bootdata
 * @bootdata:   Pointer to bootdata.
 * @verify:     Checksum verification flag. Currently unimplemented.
 * @os_data:    Pointer to a ulong variable, will hold os data start
 *          address.
 * @os_len: Pointer to a ulong variable, will hold os data length.
 *
 * This function returns the os image's start address and length. Also,
 * it appends the kernel command line to the bootargs env variable.
 *
 * Return: Zero, os start address and length on success,
 *      otherwise on failure.
 */
int fuchsia_image_get_kernel(const void* hdr, int verify,
                 ulong *os_data, ulong *os_len)
{
    if (fuchsia_image_check_header(hdr))
        return -1;

    const bootdata_t* bootdata = hdr;

    *os_data = (ulong)bootdata;
    *os_len = (ulong) bootdata->length + sizeof(bootdata_t);

    return 0;
}

ulong fuchsia_image_get_end(const void *hdr)
{
    const bootdata_t* container_hdr = hdr;
    return (ulong)hdr + container_hdr->length + sizeof(bootdata_t);
}

ulong fuchsia_image_get_kload(const void *hdr)
{
    return 0x40480000;
}

ulong fuchsia_image_get_comp(const void *hdr)
{
    return IH_COMP_NONE;
}