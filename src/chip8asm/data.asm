	PUBLIC _labelStorage, _programStorage
	SECTION BSS_UNINITIALIZED
_labelStorage:

_programStorage:	EQU _labelStorage + 4096

appEnd:	EQU	_programStorage + 6144

	SECTION IGNORE
