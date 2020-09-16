	PUBLIC	_manageTimers, _configureManageTimerFunction
	EXTERN	_timerMode, _timerTick, _timerTick, _hbSysGetTimer16, _instructionCostCounter, __divuint_callee, _soundTimer
	EXTERN _soundOff, _delayTimer, _lastTimerTick, _reportPerformance

	SECTION CODE

; define MEASURE_PERFORMANCE

_configureManageTimerFunction:
	ld	a, (_timerMode)
	sub	a,0x02
	RET	Z

	XOR	A
	ld	(l_manageTimers_00105-2), a
	ld	(l_manageTimers_00105-1), a

	RET

_manageTimers:
;chip8/timers.c:57: if (timerMode == TMS_TIMER_MODE)
	jr	manageTimerTMSTicks		; CHANGE TO NOP WHEN NOT TMS TICKS

l_manageTimers_00105:
;chip8/timers.c:60: else if (timerMode == HBIOS_TIMER_MODE)
	ld	a, (_timerMode)
	sub	a, 0x03
	jr	NZ, l_manageTimers_00102

;chip8/timers.c:61: timerTick = hbSysGetTimer16();
	call	_hbSysGetTimer16
	ld	c, l
	ld	b, h
	ld	(_timerTick), bc
	jr	l_manageTimers_00106

l_manageTimers_00102:
;chip8/timers.c:64: timerTick = instructionCostCounter / 48;
	ld	hl, 0x0030
	push	hl
	ld	hl, (_instructionCostCounter)
	push	hl
	call	__divuint_callee
	ld	c, l
	ld	b, h
	ld	(_timerTick), bc

	jr	l_manageTimers_00106

;chip8/timers.c:58: timerTick = timerTick;

manageTimerTMSTicks:
l_manageTimers_00106:

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
	call	_soundOff

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
