#ifndef _KERNEL_VERSION_H_
#define _KERNEL_VERSION_H_

/*  values come from cmake/version.cmake
 * BUILD_VERSION related  values will be 'git describe',
 * alternatively user defined BUILD_VERSION.
 */

#define ZEPHYR_VERSION_CODE 197731
#define ZEPHYR_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#define KERNELVERSION          0x3046300
#define KERNEL_VERSION_NUMBER  0x30463
#define KERNEL_VERSION_MAJOR   3
#define KERNEL_VERSION_MINOR   4
#define KERNEL_PATCHLEVEL      99
#define KERNEL_VERSION_STRING  "3.4.99"

#define BUILD_VERSION v3.4.99-ncs1-1
#include <ncs_version.h>

#endif /* _KERNEL_VERSION_H_ */
