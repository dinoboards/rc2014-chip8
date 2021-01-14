
	PUBLIC	_hbSysGetTimer16, _hbSysBankCopy, _hbSysIntInfo, _hbSysIntSet

	SECTION CODE

include "hbios_sys.inc"

	;extern uint16_t hbSysGetTimer() __z88dk_fastcall;
_hbSysGetTimer16:
	PUSH	IX
	LD	BC, 0xF8D0
	RST	08

	POP	IX
	RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; typedef struct {
;   uint8_t destBank;
;   uint8_t sourceBank;
;   uint16_t byteCount;
;   void* destAddr;
;   void* sourceAddr;
; } hbiosBankCopy;
; uint8_t hbSysBankCopy(hbiosBankCopy* pData)  __z88dk_fastcall;

_hbSysBankCopy:
	PUSH	IX

	LD	B, BF_SYSSETCPY
	LD	D, (HL)
	INC	HL
	LD	E, (HL)
	INC	HL
	LD	A, (HL)
	INC	HL
	PUSH	HL
	LD	H, (HL)
	LD	L, A

	RST	08		;
	POP	HL
	OR	A
	JR	NZ, _hbSysBankCopyErr

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	INC	HL
	LD	A, (HL)
	INC	HL
	LD	H, (HL)
	LD	L, A
	LD	B, BF_SYSBNKCPY
	RST	08

_hbSysBankCopyErr:
	LD	L, A
	LD	H, 0
	POP	IX
	RET



; typedef struct {
;   uint8_t sizeOfInterruptVectorTable;
;   uint8_t interruptMode;
; } hbSysParams;
; extern uint8_t hbSysIntInfo(hbSysParams*) __z88dk_fastcall;
_hbSysIntInfo:
	PUSH	IX

	LD	BC, BF_SYSINT * 256 + BF_SYSINT_INFO
	PUSH	HL
	RST	08
	POP	HL

	LD	(HL), E
	INC	HL
	LD	(HL), D
	LD	L, A

	POP	IX
	RET


_hbSysIntSet:
	PUSH	IX

	LD	BC, BF_SYSINT * 256 + BF_SYSINT_SET

	LD	E, (HL)
	INC	HL
	LD	A, (HL)
	INC	HL
	PUSH	HL
	LD	H, (HL)
	LD	L, A

	RST	08
	POP	BC

	EX	DE, HL
	POP	HL
	INC	HL
	LD	(HL), E
	INC	HL
	LD	(HL), D

	LD	L, A

	POP	IX
	RET

	SECTION IGNORE
