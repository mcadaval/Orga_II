/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

/*

dpl = descriptor privilege level
Specifies the privilege level of the segment. The privilege level can range from 0 to 3, with 0 being
the most privileged level. The DPL is used to control access to the segment.

g = granularity
-If the granularity flag is clear, the segment size can range from 1 byte to 1 MByte, in byte incre-
ments.
-If the granularity flag is set, the segment size can range from 4 KBytes to 4 GBytes, in 4-KByte
increments.

s = (descriptor type) flag
Specifies whether the segment descriptor is for a system segment (S flag is clear) or a code or data
segment (S flag is set).

avl = Available and reserved bits
Bit 20 of the second doubleword of the segment descriptor is available for use by system software.

L = (64-bit code segment) flag
In IA-32e mode, bit 21 of the second doubleword of the segment descriptor indicates whether a
code segment contains native 64-bit code. A value of 1 indicates instructions in this code segment
are executed in 64-bit mode. A value of 0 indicates the instructions in this code segment are
executed in compatibility mode. If L-bit is set, then D-bit must be cleared. When not in IA-32e mode
or for non-code segments, bit 21 is reserved and should always be set to 0.

P = (segment-present) flag
Indicates whether the segment is present in memory (set) or not present (clear). If this flag is clear,
the processor generates a segment-not-present exception (#NP) when a segment selector that
points to the segment descriptor is loaded into a segment register.

type = Type field
Indicates the segment or gate type and specifies the kinds of access that can be made to the
segment and the direction of growth. The interpretation of this field depends on whether the
descriptor type flag specifies an application (code or data) descriptor or a system descriptor.

base = Base address fields
Defines the location of byte 0 of the segment within the 4-GByte linear address space. The
processor puts together the three base address fields to form a single 32-bit value. Segment base
addresses should be aligned to 16-byte boundaries. Although 16-byte alignment is not required,
this alignment allows programs to maximize performance by aligning code and data on 16-byte
boundaries.

*/


#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [GDT_IDX_IDLE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    GDT_TSS_BASE1(tarea_idle),                            /* base[0:15]   */
        (unsigned char)     GDT_TSS_BASE2(tarea_idle),                        /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     GDT_TSS_BASE3(tarea_idle)                             /* base[31:24]  */
    },
    [GDT_IDX_TASK1_CODE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_navios[TSS_IDX_TASK1_CODE] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (((int) &tss_navios[TSS_IDX_TASK1_CODE]) & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK1_CODE] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK1_FLAG] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_banderas[TSS_IDX_TASK1_FLAG] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK1_FLAG] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK1_FLAG] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK2_CODE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    (((int) &tss_navios[TSS_IDX_TASK2_CODE]) & 0xFFFF),              /* base[0:15]   */
        (unsigned char)     (((int) &tss_navios[TSS_IDX_TASK2_CODE]) & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK2_CODE] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK2_FLAG] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_banderas[TSS_IDX_TASK2_FLAG] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK2_FLAG] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK2_FLAG] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK3_CODE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    (((int) &tss_navios[TSS_IDX_TASK3_CODE]) & 0xFFFF),              /* base[0:15]   */
        (unsigned char)     (((int) &tss_navios[TSS_IDX_TASK3_CODE]) & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (((int) &tss_navios[TSS_IDX_TASK3_CODE]) & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK3_FLAG] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_banderas[TSS_IDX_TASK3_FLAG] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK3_FLAG] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK3_FLAG] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK4_CODE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    (((int) &tss_navios[TSS_IDX_TASK4_CODE]) & 0xFFFF),              /* base[0:15]   */
        (unsigned char)     (((int) &tss_navios[TSS_IDX_TASK4_CODE]) & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK4_CODE] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK4_FLAG] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_banderas[TSS_IDX_TASK4_FLAG] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK4_FLAG] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK4_FLAG] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK5_CODE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    (((int) &tss_navios[TSS_IDX_TASK5_CODE]) & 0xFFFF),              /* base[0:15]   */
        (unsigned char)     (((int) &tss_navios[TSS_IDX_TASK5_CODE]) & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK5_CODE] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK5_FLAG] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_banderas[TSS_IDX_TASK5_FLAG] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK5_FLAG] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK5_FLAG] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK6_CODE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_navios[TSS_IDX_TASK6_CODE] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK6_CODE] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK6_CODE] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK6_FLAG] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_banderas[TSS_IDX_TASK6_FLAG] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK6_FLAG] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK6_FLAG] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK7_CODE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_navios[TSS_IDX_TASK7_CODE] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK7_CODE] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK7_CODE] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK7_FLAG] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_banderas[TSS_IDX_TASK7_FLAG] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK7_FLAG] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK7_FLAG] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK8_CODE] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_navios[TSS_IDX_TASK8_CODE] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK8_CODE] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_navios[TSS_IDX_TASK8_CODE] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_TASK8_FLAG] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    &tss_banderas[TSS_IDX_TASK8_FLAG] & 0xFFFF,              /* base[0:15]   */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK8_FLAG] & 0xFF0000) >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (&tss_banderas[TSS_IDX_TASK8_FLAG] & 0xFF000000) >> 24,  /* base[31:24]  */
    },
    [GDT_IDX_KERNEL_CODE] = (gdt_entry) {
        (unsigned short)    0xFFFF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */ 
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x06,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [GDT_IDX_USER_CODE] = (gdt_entry) {
        (unsigned short)    0xFFFF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x06,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [GDT_IDX_KERNEL_DATA] = (gdt_entry) {
        (unsigned short)    0xFFFF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x06,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [GDT_IDX_USER_DATA] = (gdt_entry) {
        (unsigned short)    0xFFFF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x06,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [GDT_IDX_KERNEL_SCREEN] = (gdt_entry) {
        (unsigned short)    0x0F9F,         /* limit[0:15]  */
        (unsigned short)    0x8000,         /* base[0:15]   */
        (unsigned char)     0x0B,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [GDT_IDX_INIT_TASK] = (gdt_entry) {
        (unsigned short)    0x0067,                                                /* limit[0:15]  */
        (unsigned short)    (((int) &tarea_inicial) & 0xFFFF),                               /* base[0:15]   */
        (unsigned char)     (((int) &tarea_inicial) & 0xFF0000) >> 16,                     /* base[23:16]  */
        (unsigned char)     0x09,                                                  /* type         */ 
        (unsigned char)     0x00,                                                  /* s            */
        (unsigned char)     0x00,                                                  /* dpl          */
        (unsigned char)     0x01,                                                  /* p            */
        (unsigned char)     0x00,                                                  /* limit[16:19] */
        (unsigned char)     0x00,                                                  /* avl          */
        (unsigned char)     0x00,                                                  /* l            */
        (unsigned char)     0x00,                                                  /* db           */
        (unsigned char)     0x00,                                                  /* g            */
        (unsigned char)     (((int) &tarea_inicial) & 0xFF000000) >> 24,                   /* base[31:24]  */
    }
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};
