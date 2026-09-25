;  equ.asm - PCLINK SpartaDOS client equate definitions
;
;  Device and SIO equates for Atari SpartaDOS X PCLink Protocol

DDEVIC      equ $0300
DUNIT       equ $0301
DCOMND      equ $0302
DSTATS      equ $0303
DBUFLO      equ $0304
DBUFHI      equ $0305
DTIMLO      equ $0306
DUNUSE      equ $0307
DBYTLO      equ $0308
DBYTHI      equ $0309
DAUX1       equ $030A
DAUX2       equ $030B
NOCKSM      equ $003C

SIOV        equ $E459
DOSVEC      equ $0A
DOSINI      equ $0C

; CIO Equates
iccom       equ $0342
icbadr      equ $0344
icptl       equ $0346
icpth       equ $0347
icblen      equ $0348
icaux1      equ $034A
icaux2      equ $034B
ciov        equ $E456

; SpartaDOS Command line table
comfnam     equ $21
comtab      equ $0A

; PCLink Device ID
PCLINK_DEV  equ $6F     ; 'o' (111)

; PCLink Commands (DCOMND)
CMD_PARAM   equ 'P'     ; Send parameter block ($50)
CMD_EXEC    equ 'R'     ; Execute / Transfer data ($52)
CMD_STATUS  equ 'S'     ; Read status frame ($53)
CMD_POLL    equ '?'     ; Speed poll ($3F)

; PCLink Function Numbers (PARBUF fno)
FNO_FREAD   equ $00
FNO_FWRITE  equ $01
FNO_FSEEK   equ $02
FNO_FTELL   equ $03
FNO_FLEN    equ $04
FNO_FNEXT   equ $06
FNO_FCLOSE  equ $07
FNO_INIT    equ $08
FNO_FOPEN   equ $09
FNO_FFIRST  equ $0A
FNO_RENAME  equ $0B
FNO_REMOVE  equ $0C
FNO_CHMOD   equ $0D
FNO_MKDIR   equ $0E
FNO_RMDIR   equ $0F
FNO_CHDIR   equ $10
FNO_GETCWD  equ $11
FNO_SETBOOT equ $12
FNO_DFREE   equ $13
FNO_CHVOL   equ $14

; Zero page working variables
            org $80

Temp1       .ds 2
Temp2       .ds 2
Temp3       .ds 2
Temp4       .ds 2
ArgIndex    .ds 1
UnitNum     .ds 1
StatusErr   .ds 1

; Helper macros
.macro ldxy
    ldx #< :1
    ldy #> :1
.endm

.macro ldax
    lda #< :1
    ldx #> :1
.endm

.macro stax
    sta :1
    stx :1+1
.endm
