;
MEMORY
{
  FLASH (rx) : ORIGIN = (0x0), LENGTH = (0x8000)
  RAM (rwx) : ORIGIN = (0x20000000), LENGTH = (0x8000)
}
__msp_stack_size__ = (0x00000800);
GROUP(libgcc.a libc.a libm.a libnosys.a libc_nano.a)
ENTRY(Reset_Handler)
SECTIONS
{
    .TFM_VECTORS : ALIGN(4)
    {
        __vectors_start__ = .;
        KEEP(*(.vectors))
        . = ALIGN(4);
        __vectors_end__ = .;
    } > FLASH
    ASSERT(. <= ADDR(.TFM_VECTORS) + (0x154), ".TFM_VECTORS section size overflow.")
    . = ADDR(.TFM_VECTORS) + (0x154);
    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
        LONG (LOADADDR(.TFM_DATA))
        LONG (ADDR(.TFM_DATA))
        LONG (SIZEOF(.TFM_DATA) / 4)
        LONG (LOADADDR(.TFM_PSA_ROT_LINKER_DATA))
        LONG (ADDR(.TFM_PSA_ROT_LINKER_DATA))
        LONG (SIZEOF(.TFM_PSA_ROT_LINKER_DATA) / 4)
        LONG (LOADADDR(.TFM_APP_ROT_LINKER_DATA))
        LONG (ADDR(.TFM_APP_ROT_LINKER_DATA))
        LONG (SIZEOF(.TFM_APP_ROT_LINKER_DATA) / 4)
        __copy_table_end__ = .;
    } > FLASH
    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
        LONG (ADDR(.TFM_BSS))
        LONG (SIZEOF(.TFM_BSS) / 4)
        LONG (ADDR(.TFM_PSA_ROT_LINKER_BSS))
        LONG (SIZEOF(.TFM_PSA_ROT_LINKER_BSS) / 4)
        LONG (ADDR(.TFM_APP_ROT_LINKER_BSS))
        LONG (SIZEOF(.TFM_APP_ROT_LINKER_BSS) / 4)
        LONG (ADDR(.TFM_SP_META_PTR))
        LONG (SIZEOF(.TFM_SP_META_PTR) / 4)
        __zero_table_end__ = .;
    } > FLASH
    .TFM_SP_LOAD_LIST : ALIGN(4)
    {
       KEEP(*(.part_load_priority_lowest))
       KEEP(*(.part_load_priority_low))
       KEEP(*(.part_load_priority_normal))
       KEEP(*(.part_load_priority_high))
    } > FLASH
    Image$$TFM_SP_LOAD_LIST$$RO$$Base = ADDR(.TFM_SP_LOAD_LIST);
    Image$$TFM_SP_LOAD_LIST$$RO$$Limit = ADDR(.TFM_SP_LOAD_LIST) + SIZEOF(.TFM_SP_LOAD_LIST);
    . = ALIGN(32);
    Image$$TFM_PSA_CODE_START$$Base = .;
    .TFM_PSA_ROT_LINKER : ALIGN(32)
    {
        *tfm_psa_rot_partition*:*(.text*)
        *tfm_psa_rot_partition*:*(.rodata*)
        *(TFM_*_PSA-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_PSA_ROT_LINKER$$RO$$Base = ADDR(.TFM_PSA_ROT_LINKER);
    Image$$TFM_PSA_ROT_LINKER$$RO$$Limit = ADDR(.TFM_PSA_ROT_LINKER) + SIZEOF(.TFM_PSA_ROT_LINKER);
    Image$$TFM_PSA_ROT_LINKER$$Base = ADDR(.TFM_PSA_ROT_LINKER);
    Image$$TFM_PSA_ROT_LINKER$$Limit = ADDR(.TFM_PSA_ROT_LINKER) + SIZEOF(.TFM_PSA_ROT_LINKER);
    Image$$TFM_PSA_CODE_END$$Base = .;
    Image$$TFM_APP_CODE_START$$Base = .;
    .TFM_APP_ROT_LINKER : ALIGN(32)
    {
        *tfm_app_rot_partition*:*(.text*)
        *tfm_app_rot_partition*:*(.rodata*)
        *(TFM_*_APP-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_APP_ROT_LINKER$$RO$$Base = ADDR(.TFM_APP_ROT_LINKER);
    Image$$TFM_APP_ROT_LINKER$$RO$$Limit = ADDR(.TFM_APP_ROT_LINKER) + SIZEOF(.TFM_APP_ROT_LINKER);
    Image$$TFM_APP_ROT_LINKER$$Base = ADDR(.TFM_APP_ROT_LINKER);
    Image$$TFM_APP_ROT_LINKER$$Limit = ADDR(.TFM_APP_ROT_LINKER) + SIZEOF(.TFM_APP_ROT_LINKER);
    Image$$TFM_APP_CODE_END$$Base = .;
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;
    .ER_TFM_CODE : ALIGN(4)
    {
        *startup*(.text*)
        *libplatform_s*:*(.text*)
        *libtfm_spm*:*(.text*)
        *libplatform_s*:*(.rodata*)
        *libtfm_spm*:*(.rodata*)
    } > FLASH
    .tfm_secure_data_start :
    {
        . = ABSOLUTE((0x20000000)) ;
    } > RAM
    .tfm_bl2_shared_data : ALIGN(32)
    {
        . += (0x400);
    } > RAM
    .msp_stack : ALIGN(32)
    {
        . += __msp_stack_size__ - 0x8;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.msp_stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);
    .msp_stack_seal_res :
    {
        . += 0x8;
    } > RAM
    __StackSeal = ADDR(.msp_stack_seal_res);
    .TFM_SP_META_PTR : ALIGN(32)
    {
        *(.bss.SP_META_PTR_SPRTL_INST)
        . = ALIGN(32);
    } > RAM
    Image$$TFM_SP_META_PTR$$ZI$$Base = ADDR(.TFM_SP_META_PTR);
    Image$$TFM_SP_META_PTR$$ZI$$Limit = ADDR(.TFM_SP_META_PTR) + SIZEOF(.TFM_SP_META_PTR);
    . = ALIGN(32);
    Image$$TFM_APP_RW_STACK_START$$Base = .;
    .TFM_APP_ROT_LINKER_DATA : ALIGN(32)
    {
        *tfm_app_rot_partition*:*(.data*)
        *(TFM_*_APP-ROT_ATTR_RW)
        . = ALIGN(4);
    } > RAM AT> FLASH
    Image$$TFM_APP_ROT_LINKER_DATA$$RW$$Base = ADDR(.TFM_APP_ROT_LINKER_DATA);
    Image$$TFM_APP_ROT_LINKER_DATA$$RW$$Limit = ADDR(.TFM_APP_ROT_LINKER_DATA) + SIZEOF(.TFM_APP_ROT_LINKER_DATA);
    .TFM_APP_ROT_LINKER_BSS : ALIGN(4)
    {
        start_of_TFM_APP_ROT_LINKER = .;
        *tfm_app_rot_partition*:*(.bss*)
        *tfm_app_rot_partition*:*(COMMON)
        *(TFM_*_APP-ROT_ATTR_ZI)
        . += (. - start_of_TFM_APP_ROT_LINKER) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_APP_ROT_LINKER_DATA$$ZI$$Base = ADDR(.TFM_APP_ROT_LINKER_BSS);
    Image$$TFM_APP_ROT_LINKER_DATA$$ZI$$Limit = ADDR(.TFM_APP_ROT_LINKER_BSS) + SIZEOF(.TFM_APP_ROT_LINKER_BSS);
    Image$$TFM_APP_RW_STACK_END$$Base = .;
    Image$$TFM_PSA_RW_STACK_START$$Base = .;
    .TFM_PSA_ROT_LINKER_DATA : ALIGN(32)
    {
        *tfm_psa_rot_partition*:*(.data*)
        *(TFM_*_PSA-ROT_ATTR_RW)
        . = ALIGN(4);
    } > RAM AT> FLASH
    Image$$TFM_PSA_ROT_LINKER_DATA$$RW$$Base = ADDR(.TFM_PSA_ROT_LINKER_DATA);
    Image$$TFM_PSA_ROT_LINKER_DATA$$RW$$Limit = ADDR(.TFM_PSA_ROT_LINKER_DATA) + SIZEOF(.TFM_PSA_ROT_LINKER_DATA);
    .TFM_PSA_ROT_LINKER_BSS : ALIGN(4)
    {
        start_of_TFM_PSA_ROT_LINKER = .;
        *tfm_psa_rot_partition*:*(.bss*)
        *tfm_psa_rot_partition*:*(COMMON)
        *(TFM_*_PSA-ROT_ATTR_ZI)
        . += (. - start_of_TFM_PSA_ROT_LINKER) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_PSA_ROT_LINKER_DATA$$ZI$$Base = ADDR(.TFM_PSA_ROT_LINKER_BSS);
    Image$$TFM_PSA_ROT_LINKER_DATA$$ZI$$Limit = ADDR(.TFM_PSA_ROT_LINKER_BSS) + SIZEOF(.TFM_PSA_ROT_LINKER_BSS);
    Image$$TFM_PSA_RW_STACK_END$$Base = .;
    .TFM_UNPRIV_CODE : ALIGN(32)
    {
        *(SFN)
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
         *crtbegin.o(.dtors)
         *crtbegin?.o(.dtors)
         *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
         *(SORT(.dtors.*))
         *(.dtors)
        *(.rodata*)
        KEEP(*(.eh_frame*))
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_UNPRIV_CODE$$RO$$Base = ADDR(.TFM_UNPRIV_CODE);
    Image$$TFM_UNPRIV_CODE$$RO$$Limit = ADDR(.TFM_UNPRIV_CODE) + SIZEOF(.TFM_UNPRIV_CODE);
    .gnu.sgstubs (ALIGN((0x4000)) - (0x400) + (. > (ALIGN((0x4000)) - (0x400)) ? (0x4000) : 0)) : { *(.gnu.sgstubs*) } > FLASH .sgstubs_end : ALIGN((0x4000)) { } > FLASH
    .TFM_DATA : ALIGN(4)
    {
        *(.data*)
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
        KEEP(*(.jcr*))
        . = ALIGN(4);
    } > RAM AT> FLASH
    Image$$ER_TFM_DATA$$RW$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$RW$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA);
    .TFM_BSS : ALIGN(4)
    {
        __bss_start__ = .;
        __partition_runtime_start__ = .;
        KEEP(*(.bss.part_runtime_priority_lowest))
        KEEP(*(.bss.part_runtime_priority_low))
        KEEP(*(.bss.part_runtime_priority_normal))
        KEEP(*(.bss.part_runtime_priority_high))
        __partition_runtime_end__ = .;
        . = ALIGN(4);
        __service_runtime_start__ = .;
        KEEP(*(.bss.serv_runtime_priority_lowest))
        KEEP(*(.bss.serv_runtime_priority_low))
        KEEP(*(.bss.serv_runtime_priority_normal))
        KEEP(*(.bss.serv_runtime_priority_high))
        __service_runtime_end__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM AT> RAM
    Image$$ER_TFM_DATA$$ZI$$Base = ADDR(.TFM_BSS);
    Image$$ER_TFM_DATA$$ZI$$Limit = ADDR(.TFM_BSS) + SIZEOF(.TFM_BSS);
    Image$$ER_PART_RT_POOL$$ZI$$Base = __partition_runtime_start__;
    Image$$ER_PART_RT_POOL$$ZI$$Limit = __partition_runtime_end__;
    Image$$ER_SERV_RT_POOL$$ZI$$Base = __service_runtime_start__;
    Image$$ER_SERV_RT_POOL$$ZI$$Limit = __service_runtime_end__;
    Image$$ER_TFM_DATA$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA) + SIZEOF(.TFM_BSS);
    Image$$ER_VENEER$$Base = ADDR(.gnu.sgstubs);
    Image$$VENEER_ALIGN$$Limit = ADDR(.sgstubs_end);
    ASSERT ((Image$$VENEER_ALIGN$$Limit - Image$$ER_VENEER$$Base) <= (0x400), "Veneer region overflowed")
    Load$$LR$$LR_NS_PARTITION$$Base = ((0x8000));
    PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
}
