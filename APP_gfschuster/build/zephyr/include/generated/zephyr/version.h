#ifndef _KERNEL_VERSION_H_
#define _KERNEL_VERSION_H_

/* The template values come from cmake/modules/version.cmake
 * BUILD_VERSION related template values will be 'git describe',
 * alternatively user defined BUILD_VERSION.
 */

#define ZEPHYR_VERSION_CODE 262755
#define ZEPHYR_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#define KERNELVERSION                   0x4026300
#define KERNEL_VERSION_NUMBER           0x40263
#define KERNEL_VERSION_MAJOR            4
#define KERNEL_VERSION_MINOR            2
#define KERNEL_PATCHLEVEL               99
#define KERNEL_TWEAK                    0
#define KERNEL_VERSION_STRING           "4.2.99"
#define KERNEL_VERSION_EXTENDED_STRING  "4.2.99+0"
#define KERNEL_VERSION_TWEAK_STRING     "4.2.99+0"

#define BUILD_VERSION 0d7e0b8969b4


#endif /* _KERNEL_VERSION_H_ */
