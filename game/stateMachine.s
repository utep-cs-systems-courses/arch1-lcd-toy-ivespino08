.text
	.balign 2
	.global check_state
	.word check_state
	.text

	.extern state
	.extern buzzer_set_period

check_state:
	cmp 	#0, &state
	jz 	STATE0
	cmp     #1, &state
	jz	STATE1
	cmp	#2, &state
	jz	STATE2
	ret
	
STATE0:
	mov 	#0x3E8, R12
	CALL 	#buzzer_set_period
	mov	#1, &state
	ret

STATE1:
	mov 	#0x5DC, R12
	CALL 	#buzzer_set_period
	mov 	#2, &state
	ret

STATE2:
	mov 	#0x7D0, R12
	CALL 	#buzzer_set_period
	mov	#0, &state
	ret
