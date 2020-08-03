
	SECTION code_crt_init
	ORG	$100
	SECTION	code_crt_main

	SECTION CODE
	SECTION code_compiler
	SECTION code_l_sdcc
	SECTION code_math
	SECTION code_error
	SECTION code_l

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; memory model ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	section interrupt_vectors
	section code_crt_start
	section code_crt_init
	section code_crt_main
	section code_crt_exit
	section code_crt_return
	section code_crt_common
	section interrupt_vectors
	section code_driver
	section code_font
	section code_clib

	;include "../../clib_code.inc"
	section code_adt_b_array
	section code_adt_b_vector
	section code_adt_ba_priority_queue
	section code_adt_ba_stack
	section code_adt_bv_priority_queue
	section code_adt_bv_stack
	section code_adt_p_forward_list
	section code_adt_p_forward_list_alt
	section code_adt_p_list
	section code_adt_p_queue
	section code_adt_p_stack
	section code_adt_w_array
	section code_adt_w_vector
	section code_adt_wa_priority_queue
	section code_adt_wa_stack
	section code_adt_wv_priority_queue
	section code_adt_wv_stack
	section code_alloc_balloc
	section code_alloc_malloc
	section code_alloc_obstack
	section code_arch
	section code_bifrost_h
	section code_bifrost_l
	section code_bifrost2
	section code_compress_aplib
	section code_compress_zx7
	section code_ctype
	section code_driver_general
	section code_driver_character_input
	section code_driver_character_output
	section code_driver_memstream
	section code_driver_terminal_input
	section code_driver_terminal_output
	section code_driver_tty
	section code_env
	section code_error
	section code_esxdos
	section code_fcntl
	section code_font_fzx
	section code_fp_genmath
	section code_fp_math48
	section code_fp_math32
	section code_im2
	section code_input
	section code_inttypes
	section code_l
	section code_l_sccz80
	section code_l_sdcc
	section code_locale
	section code_math
	section code_network
	section code_nirvanam
	section code_nirvanap
	section code_PSGlib
	section code_setjmp
	section code_SMSlib
	section code_sound_bit
	section code_sound_ay
	section code_stdio
	section code_stdlib
	section code_string
	section code_temp_sp1
	section code_threads
	section code_threads_mutex
	section code_time
	section code_z180
	section code_z80


	section code_lib
	section code_compiler
	section code_user

	section rodata_align_256
	align 256

	section rodata_align_128
	align 128

	section rodata_align_64
	align 64

	section rodata_align_32
	align 32

	section rodata_align_16
	align 16

	section rodata_align_8
	align 8

	section rodata_align_4
	align 4

	section rodata_align_2
	align 2

	section rodata_driver
	section rodata_font
	section rodata_clib
	;include "../../clib_rodata.inc"


	section rodata_align_256
	section rodata_align_128
	section rodata_align_64
	section rodata_align_32
	section rodata_align_16
	section rodata_align_8
	section rodata_align_4
	section rodata_align_2
	section rodata_arch
	section rodata_env
	section rodata_error_strings
	section rodata_error_string_end
	defb 0
	section rodata_fcntl
	section rodata_font_4x8
	section rodata_font_8x8
	section rodata_font_fzx
	section rodata_fp_math32
	section rodata_input
	section rodata_SMSlib
	section rodata_sound_ay
	section rodata_stdio
	section rodata_stdlib


	;;section rodata_error_strings
	;;section rodata_error_string_end
	;;defb 0
	section rodata_lib


	section rodata_user

	SECTION CODE_END

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	SECTION DATA

	section data_align_256
	align 256

	section data_align_128
	align 128

	section data_align_64
	align 64

	section data_align_32
	align 32

	section data_align_16
	align 16

	section data_align_8
	align 8

	section data_align_4
	align 4

	section data_align_2
	align 2

	section smc_jump_vectors
	section smc_driver
	section smc_font
	section smc_clib
	;  include "../../clib_smc.inc"

	section smc_fcntl
	section smc_l
	section smc_sound_bit
	section smc_sound_ay


	section smc_lib
	section smc_compiler
	section smc_user

	section data_driver
	section data_font
	section data_clib
	;include "../../clib_data.inc"

	section data_alloc_balloc
	section data_alloc_malloc
	section data_arch
	section data_fcntl
	section data_fcntl_stdio_heap_head
	section data_fcntl_stdio_heap_body
	section data_fcntl_stdio_heap_tail
	section data_fcntl_fdtable_body
	section data_PSGlib
	section data_SMSlib
	section data_sound_bit
	section data_sound_ay
	section data_stdio
	section data_stdlib
	section data_threads


	section data_lib
	section data_compiler
	section data_user

	SECTION DATA_END

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	SECTION BSS

	section bss_align_256
	align 256

	section bss_align_128
	align 128

	section bss_align_64
	align 64

	section bss_align_32
	align 32

	section bss_align_16
	align 16

	section bss_align_8
	align 8

	section bss_align_4
	align 4

	section bss_align_2
	align 2

	section bss_driver
	section bss_font
	section bss_clib
	;include "../../clib_bss.inc"
	section bss_arch
	section bss_alloc_balloc
	section bss_alloc_malloc
	section bss_compress_aplib
	section bss_error
	section bss_env
	section bss_esxdos
	section bss_fcntl
	section bss_input
	section bss_PSGlib
	section bss_SMSlib
	section bss_sound_bit
	section bss_sound_ay
	section bss_stdio
	section bss_stdlib
	section bss_string

	section bss_lib
	section bss_compiler
	section bss_user

	section rodata_compiler

	section BSS_UNINITIALIZED
	SECTION BSS_END
