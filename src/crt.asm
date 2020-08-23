
	SECTION IGNORE

PUBLIC STDIO_MSG_PUTC
PUBLIC STDIO_MSG_WRIT
PUBLIC STDIO_MSG_GETC
PUBLIC STDIO_MSG_EATC
PUBLIC STDIO_MSG_READ
PUBLIC STDIO_MSG_SEEK
PUBLIC STDIO_MSG_ICTL
PUBLIC STDIO_MSG_FLSH
PUBLIC STDIO_MSG_CLOS

defc STDIO_MSG_PUTC = 1
defc STDIO_MSG_WRIT = 2
defc STDIO_MSG_GETC = 3
defc STDIO_MSG_EATC = 4
defc STDIO_MSG_READ = 5
defc STDIO_MSG_SEEK = 6
defc STDIO_MSG_ICTL = 7
defc STDIO_MSG_FLSH = 8
defc STDIO_MSG_CLOS = 9

defc __clib_malloc_heap_size = 4096

public	mtx_plain, mtx_recursive, mtx_timed, thrd_success, thrd_error

defc mtx_plain = 0x01
defc mtx_recursive = 0x02
defc mtx_timed = 0x04
defc thrd_success = 0x00
defc thrd_error = 0x01

         SECTION data_clib
         SECTION data_stdio

         PUBLIC __stdio_open_file_list
         __stdio_open_file_list:  defw 0

; if MALLOC
; ;   create malloc heap in bss section

; $#%#$%@#$%#$%

;    SECTION data_clib
;    SECTION data_alloc_malloc

;    PUBLIC __malloc_heap

;    __malloc_heap:              defw __malloc_block

;    SECTION bss_clib
;    SECTION bss_alloc_malloc

;    __malloc_block:             defs __clib_malloc_heap_size

;    SECTION code_crt_init

;    ld hl,__malloc_block
;    ld bc,__clib_malloc_heap_size

;    EXTERN asm_heap_init
;    call asm_heap_init

; endif

   SECTION bss_clib
   SECTION bss_stdlib

   PUBLIC __exit_stack
   defc __exit_stack_size = 0

   __exit_stack:               defb 0
                               defs __exit_stack_size * 2


PUBLIC STDIO_SEEK_CUR
defc STDIO_SEEK_CUR = 1

; _Noreturn void _exit_fastcall(int status)

SECTION code_clib
SECTION code_fcntl

PUBLIC __exit_fastcall

EXTERN __Exit

defc __exit_fastcall = __Exit


; _Noreturn void exit_fastcall(int status)

SECTION code_clib
SECTION code_stdlib

PUBLIC _exit_fastcall

EXTERN asm_exit

defc _exit_fastcall = asm_exit

	SECTION IGNORE
