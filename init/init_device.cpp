/*
   Copyright (c) 2013, The Linux Foundation. All rights reserved.
   Copyright (C) 2016 The CyanogenMod Project.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

#define RAW_ID_PATH     "/sys/devices/soc0/raw_id"
#define BUF_SIZE         64

static char tmp[BUF_SIZE];
static char buff_tmp[BUF_SIZE];

char const *device;
char const *family;
char const *product;
char const *heapstartsize;
char const *heapgrowthlimit;
char const *heapsize;
char const *heapminfree;

static int read_file2(const char *fname, char *data, int max_size)
{
    int fd, rc;

    if (max_size < 1)
        return 0;

    fd = open(fname, O_RDONLY);
    if (fd < 0) {
        ERROR("failed to open '%s'\n", fname);
        return 0;
    }

    rc = read(fd, data, max_size - 1);
    if ((rc > 0) && (rc < max_size))
        data[rc] = '\0';
    else
        data[0] = '\0';
    close(fd);

    return 1;
}

void vendor_load_properties()
{

    char b_description[PROP_VALUE_MAX], b_fingerprint[PROP_VALUE_MAX];
    char p_carrier[PROP_VALUE_MAX], p_device[PROP_VALUE_MAX], p_model[PROP_VALUE_MAX];
    unsigned long raw_id = -1;
    int rc;

    /* get raw ID */
    rc = read_file2(RAW_ID_PATH, tmp, sizeof(tmp));
    if (rc) {
        raw_id = strtoul(tmp, NULL, 0);
    }

    /* Z010D  */
    if (raw_id==1797) {

    /* Device Setting */
    family = "WW_Phone";
    device = "Z010D";
    product = "ZC550KL"; // need for Сamera Hal project ID check

    /* Heap Setting */
    heapstartsize = "8m";
    heapgrowthlimit = "192m";
    heapsize = "512m";
    heapminfree = "2m";

    property_set("ro.build.product", product);
    property_set("ro.product.model", "Zenfone Max");
    property_set("ro.build.product", "ZC550KL");

    /* Heap Set */
    property_set("dalvik.vm.heapstartsize", heapstartsize);
    property_set("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_set("dalvik.vm.heapsize", heapsize);
    property_set("dalvik.vm.heaptargetutilization", "0.75");
    property_set("dalvik.vm.heapminfree", heapminfree);
    property_set("dalvik.vm.heapmaxfree", "8m");

    } else {
        property_set("ro.product.model", "Zenfone"); // this should never happen.
    }

    INFO("Setting build properties for %s device\n", device, family);
}