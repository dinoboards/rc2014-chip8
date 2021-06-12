	PUBLIC	_manageTimers
	EXTERN	_timerMode, _JIFFY, _instructionCostCounter, __divuint_callee, _soundTimer
	EXTERN _audioStop, _delayTimer, _lastTimerTick, _reportPerformance

	SECTION CODE

_manageTimers:
ifdef _MEASURE_PERFORMANCE
	call	_reportPerformance
endif

;chip8/timers.c:70: if (soundTimer == 0 && delayTimer == 0)
	ld	a, (_soundTimer)
	or	a
	jr	NZ, l_manageTimers_00108
	ld	a, (_delayTimer)
	or	a
	ret	Z

;chip8/timers.c:71: return;

l_manageTimers_00108:
	; IF _lastTimerTick == JIFFY - return (low byte only)
	ld	a, (_JIFFY)
	LD	D, A
	LD	hl, _lastTimerTick
	ld	C, (hl)
	sub	a, c
	ret	z
	ld	(hl), d

;chip8/timers.c:28: if (soundTimer == 0)
	ld	hl, _soundTimer
	ld	a, (hl)
	or	a
	jr	Z, l_manageTimers_00116

;chip8/timers.c:31: soundTimer--;
;chip8/timers.c:33: if (soundTimer == 0)
	dec 	(hl)
	jr	NZ , l_manageTimers_00116

;chip8/timers.c:34: soundOff();
	call	_audioStop

;chip8/timers.c:78: tickSoundTimer();

l_manageTimers_00116:
; ;chip8/timers.c:38: if (delayTimer == 0)
	ld	hl, _delayTimer
	ld	a, (hl)
	or	a
	ret	Z  ;, l_manageTimers_00119

; ;chip8/timers.c:41: delayTimer--;
	dec	(hl)

;chip8/timers.c:79: tickDelayTimer();

; l_manageTimers_00119:
; ;chip8/timers.c:81: lastTimerTick = JIFFY;
; 	ld	hl, (_JIFFY)
; 	ld	(_lastTimerTick), hl

; l_manageTimers_00120:
;chip8/timers.c:82: }
	ret

	SECTION IGNORE
