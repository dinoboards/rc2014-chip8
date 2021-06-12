
	SECTION CODE

	EXTERN	_xprintf

	PUBLIC	_applicationExit
; extern void applicationExit(const char* message, ...)

_applicationExit:
	JP	_xprintf
