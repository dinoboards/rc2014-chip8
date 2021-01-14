
	PUBLIC	_hbSysBankCopy

	SECTION CODE

include "hbios_sys.inc"


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


	SECTION IGNORE
