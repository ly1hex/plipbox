VERSION		EQU	38
REVISION	EQU	1
DATE	MACRO
		dc.b	'1.4.98'
	ENDM
VERS	MACRO
		dc.b	'linplip 38.1'
	ENDM
VSTRING	MACRO
		dc.b	'linplip 38.1 (1.4.98)',13,10,0
	ENDM
VERSTAG	MACRO
		dc.b	0,'$VER: linplip 38.1 (1.4.98)',0
	ENDM