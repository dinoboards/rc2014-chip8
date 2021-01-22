	PUBLIC	_manageTimers
	EXTERN	_timerMode, _timerTick, _timerTick, _instructionCostCounter, __divuint_callee, _soundTimer
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

;chip8/timers.c:71: return;
	ret	Z

l_manageTimers_00108:
;chip8/timers.c:73: const byte diff = (byte)(timerTick - lastTimerTick);
	ld	a, (_timerTick)
	ld	hl, _lastTimerTick
	ld	c, (hl)
	sub	a, c

;chip8/timers.c:75: if (diff == 0)
	; or	a, a

;chip8/timers.c:76: return;
	RET	Z

;chip8/timers.c:28: if (soundTimer == 0)
	ld	hl, _soundTimer
	ld	a, (hl)
	or	a
	jr	Z, l_manageTimers_00116

;chip8/timers.c:31: soundTimer--;
;chip8/timers.c:33: if (soundTimer == 0)
	dec 	(hl)
	jr	NZ, l_manageTimers_00116

;chip8/timers.c:34: soundOff();
	call	_audioStop

;chip8/timers.c:78: tickSoundTimer();

l_manageTimers_00116:
;chip8/timers.c:38: if (delayTimer == 0)
	ld	hl, _delayTimer
	ld	a, (hl)
	or	a
	jr	Z, l_manageTimers_00119

;chip8/timers.c:41: delayTimer--;
	dec	(hl)

;chip8/timers.c:79: tickDelayTimer();

l_manageTimers_00119:
;chip8/timers.c:81: lastTimerTick = timerTick;
	ld	hl, (_timerTick)
	ld	(_lastTimerTick), hl

l_manageTimers_00120:
;chip8/timers.c:82: }
	ret

	SECTION IGNORE
