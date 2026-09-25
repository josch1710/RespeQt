;  pclink.asm - SpartaDOS PCLink Protocol CLI Utility
;
;  Command-line client for SpartaDOS / SpartaDOS X to interact with
;  RespeQt's PCLink Server protocol over Atari SIO ($6F).
;
;  Usage:
;    PCLINK PROBE             - Probe PCLink SIO device and display speed
;    PCLINK INIT              - Reset and initialize all PCLink units
;    PCLINK DFREE <unit>      - Query volume label & free space on unit (1-15)
;    PCLINK CWD <unit>        - Display current working directory of unit
;    PCLINK CD <unit> <path>  - Change working directory on unit
;    PCLINK MD <unit> <name>  - Create a directory on unit
;    PCLINK RD <unit> <name>  - Remove an empty directory on unit

    icl 'equ.asm'

    org $4000

Start
    jsr printf
    .byte 'SpartaDOS PCLink Utility v1.0',155
    .byte '(c) RespeQt Project',155,155,0

    ; Verify SpartaDOS environment
    lda $0700
    cmp #'S'
    beq IsSpartaDos

    jsr printf
    .byte 'Error: SpartaDOS is required!',155,0
    rts

IsSpartaDos
    ; Setup Crunch routine from SpartaDOS COMTAB
    ldy #5
    ldx #2
@
    lda (comtab),y
    sta Crunch,x
    dey
    dex
    bne @-

    ; Get first argument (Command)
    jsr Crunch
    bne GotFirstArg
    jmp ShowUsage

GotFirstArg
    ; Parse command string
    jsr ParseCommand
    rts

; -------------------------------------------------------------
; Command Parser
; -------------------------------------------------------------
ParseCommand
    ldy #comfnam+2

    lda (comtab),y
    cmp #'P'            ; PROBE
    beq DoProbe
    cmp #'p'
    beq DoProbe

    cmp #'I'            ; INIT
    beq DoInit
    cmp #'i'
    beq DoInit

    cmp #'D'            ; DFREE
    beq DoDFree
    cmp #'d'
    beq DoDFree

    cmp #'C'            ; CWD or CD
    beq CheckCD
    cmp #'c'
    beq CheckCD

    cmp #'M'            ; MD (mkdir)
    beq DoMkdir
    cmp #'m'
    beq DoMkdir

    cmp #'R'            ; RD (rmdir)
    beq DoRmdir
    cmp #'r'
    beq DoRmdir

ShowUsage
    jsr printf
    .byte 'Usage:',155
    .byte '  PCLINK PROBE              - Check PCLink connection',155
    .byte '  PCLINK INIT               - Initialize all PCLink units',155
    .byte '  PCLINK DFREE <unit>       - Query drive free space (1-15)',155
    .byte '  PCLINK CWD <unit>         - Show working directory (1-15)',155
    .byte '  PCLINK CD <unit> <path>   - Change directory',155
    .byte '  PCLINK MD <unit> <dirname>- Create directory',155
    .byte '  PCLINK RD <unit> <dirname>- Remove directory',155,0
    rts

CheckCD
    iny
    lda (comtab),y
    cmp #'D'
    beq DoChdir
    cmp #'d'
    beq DoChdir
    cmp #'W'
    beq DoGetCwd
    cmp #'w'
    beq DoGetCwd
    jmp ShowUsage

; -------------------------------------------------------------
; Action Handlers
; -------------------------------------------------------------

; --- PROBE ---
DoProbe
    jsr printf
    .byte 'Probing PCLink device ($6F)... ',0

    lda #PCLINK_DEV
    sta DDEVIC
    lda #1
    sta DUNIT
    lda #CMD_POLL       ; '?' Speed poll
    sta DCOMND
    lda #$40            ; SIO read data frame
    sta DSTATS
    ldax SpeedBuf
    stax DBUFLO
    lda #7              ; 7 seconds timeout
    sta DTIMLO
    lda #1              ; 1 byte data
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    sta DAUX2

    jsr SIOV
    bmi @Error

    jsr printf
    .byte 'FOUND!',155
    .byte 'High-Speed Byte: $%x',155,0
    .word SpeedBuf

    rts

@Error
    jsr printf
    .byte 'NOT RESPONDING (Error %d)',155,0
    .word StatusErr
    rts

; --- INIT ---
DoInit
    jsr printf
    .byte 'Sending PCLink INIT... ',0

    jsr ClearParBuf
    lda #FNO_INIT
    sta ParBuf_Fno

    lda #1
    sta UnitNum
    jsr SendParBuf
    bmi @InitErr

    jsr printf
    .byte 'OK (Units Reset)',155,0
    rts

@InitErr
    jsr printf
    .byte 'FAILED (Error %d)',155,0
    .word StatusErr
    rts

; --- DFREE ---
DoDFree
    jsr GetUnitArg
    bcs @DfreeDone

    jsr ClearParBuf
    lda #FNO_DFREE
    sta ParBuf_Fno
    jsr SendParBuf
    bmi @DfreeErr

    ; Execute 'R' command to fetch 64-byte DFREE block
    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_EXEC
    sta DCOMND
    lda #$40
    sta DSTATS
    ldax DFreeBuf
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #64
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    lda UnitNum
    sta DAUX2

    jsr SIOV
    bmi @DfreeErr

    ; Copy volume label (offset 14, 8 chars)
    ldy #0
@LabelLoop
    lda DFreeBuf+14,y
    sta VolLabel,y
    iny
    cpy #8
    bne @LabelLoop
    lda #0
    sta VolLabel+8

    jsr printf
    .byte 'PCLink Unit %d Volume: [%s]',155
    .byte 'Free Sectors : %d',155
    .byte 'Total Sectors: %d',155,0
    .word UnitNum
    .word VolLabel
    .word DFreeBuf     ; Free sectors (low word)
    .word DFreeBuf+2   ; Total sectors (low word)
@DfreeDone
    rts

@DfreeErr
    jsr printf
    .byte 'DFREE Error %d on Unit %d',155,0
    .word StatusErr
    .word UnitNum
    rts

; --- GET CWD ---
DoGetCwd
    jsr GetUnitArg
    bcs @CwdDone

    jsr ClearParBuf
    lda #FNO_GETCWD
    sta ParBuf_Fno
    jsr SendParBuf
    bmi @CwdErr

    ; Fetch CWD string via 'R' frame
    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_EXEC
    sta DCOMND
    lda #$40
    sta DSTATS
    ldax PathBuf
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #64
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    lda UnitNum
    sta DAUX2

    jsr SIOV
    bmi @CwdErr

    ; Ensure null-terminated
    lda #0
    sta PathBuf+64

    jsr printf
    .byte 'Unit %d CWD: %s',155,0
    .word UnitNum
    .word PathBuf
@CwdDone
    rts

@CwdErr
    jsr printf
    .byte 'Get CWD Error %d on Unit %d',155,0
    .word StatusErr
    .word UnitNum
    rts

; --- CHDIR ---
DoChdir
    jsr GetUnitArg
    bcs @CdDone

    jsr Crunch
    beq @MissingPath

    jsr ClearParBuf
    lda #FNO_CHDIR
    sta ParBuf_Fno

    ; Copy path argument to ParBuf_Path
    ldy #0
@CopyPath
    lda (comtab),y
    sta ParBuf_Path,y
    beq @PathCopied
    iny
    cpy #63
    bne @CopyPath
    lda #0
    sta ParBuf_Path,y
@PathCopied

    jsr SendParBuf
    bmi @CdErr

    jsr printf
    .byte 'Unit %d Directory Changed to: %s',155,0
    .word UnitNum
    .word ParBuf_Path
@CdDone
    rts

@MissingPath
    jsr printf
    .byte 'Error: Missing path argument',155,0
    rts

@CdErr
    jsr printf
    .byte 'CHDIR Error %d on Unit %d',155,0
    .word StatusErr
    .word UnitNum
    rts

; --- MKDIR ---
DoMkdir
    jsr GetUnitArg
    bcs @MdDone

    jsr Crunch
    beq @MissingDir

    jsr ClearParBuf
    lda #FNO_MKDIR
    sta ParBuf_Fno
    jsr CopyNameToParBuf
    jsr SendParBuf
    bmi @MdErr

    jsr printf
    .byte 'Directory Created on Unit %d',155,0
    .word UnitNum
@MdDone
    rts

@MissingDir
    jsr printf
    .byte 'Error: Missing directory name',155,0
    rts

@MdErr
    jsr printf
    .byte 'MKDIR Error %d on Unit %d',155,0
    .word StatusErr
    .word UnitNum
    rts

; --- RMDIR ---
DoRmdir
    jsr GetUnitArg
    bcs @RdDone

    jsr Crunch
    beq @MissingDirR

    jsr ClearParBuf
    lda #FNO_RMDIR
    sta ParBuf_Fno
    jsr CopyNameToParBuf
    jsr SendParBuf
    bmi @RdErr

    jsr printf
    .byte 'Directory Removed on Unit %d',155,0
    .word UnitNum
@RdDone
    rts

@MissingDirR
    jsr printf
    .byte 'Error: Missing directory name',155,0
    rts

@RdErr
    jsr printf
    .byte 'RMDIR Error %d on Unit %d',155,0
    .word StatusErr
    .word UnitNum
    rts

; -------------------------------------------------------------
; Helpers
; -------------------------------------------------------------

GetUnitArg
    jsr Crunch
    bne @ParseUnit
    jsr printf
    .byte 'Error: Missing unit number (1-15)',155,0
    sec
    rts

@ParseUnit
    ldy #comfnam+2
    lda (comtab),y
    sec
    sbc #'0'
    cmp #10
    bcc @SingleDigit
    ; Check 10..15
    lda #1
    sta UnitNum
    iny
    lda (comtab),y
    sec
    sbc #'0'
    clc
    adc #10
    sta UnitNum
    jmp @CheckRange

@SingleDigit
    sta UnitNum

@CheckRange
    lda UnitNum
    cmp #1
    bcc @InvalidUnit
    cmp #16
    bcs @InvalidUnit
    clc
    rts

@InvalidUnit
    jsr printf
    .byte 'Error: Invalid unit %d (Must be 1-15)',155,0
    .word UnitNum
    sec
    rts

ClearParBuf
    ldx #0
    txa
@Loop
    sta ParBuf,x
    inx
    cpx #100
    bne @Loop
    rts

CopyNameToParBuf
    ; Converts 8.3 name to NNNNNNNNXXX raw format in ParBuf_Name
    ldx #0
    lda #$20
@FillSpaces
    sta ParBuf_Name,x
    inx
    cpx #11
    bne @FillSpaces

    ldy #comfnam+2
    ldx #0
@NameLoop
    lda (comtab),y
    beq @DoneName
    cmp #'.'
    beq @ExtPart
    sta ParBuf_Name,x
    iny
    inx
    cpx #8
    bne @NameLoop

@FindDot
    lda (comtab),y
    beq @DoneName
    cmp #'.'
    beq @ExtPart
    iny
    bne @FindDot

@ExtPart
    iny
    ldx #8
@ExtLoop
    lda (comtab),y
    beq @DoneName
    sta ParBuf_Name,x
    iny
    inx
    cpx #11
    bne @ExtLoop

@DoneName
    rts

SendParBuf
    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_PARAM       ; 'P'
    sta DCOMND
    lda #$80             ; Write data frame
    sta DSTATS
    ldax ParBuf
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #100             ; Size of PARBUF
    sta DBYTLO
    lda #0
    sta DBYTHI
    lda #100
    sta DAUX1
    lda UnitNum
    sta DAUX2

    jsr SIOV
    sta StatusErr
    rts

; -------------------------------------------------------------
; Dynamic Buffers & Variables
; -------------------------------------------------------------

Crunch      .ds 3

SpeedBuf    .ds 1

VolLabel    .ds 9
PathBuf     .ds 65
DFreeBuf    .ds 64

; 100-byte PARBUF structure
ParBuf
ParBuf_Fno      .ds 1
ParBuf_Handle   .ds 1
ParBuf_F1       .ds 1
ParBuf_F2       .ds 1
ParBuf_F3       .ds 1
ParBuf_F4       .ds 1
ParBuf_F5       .ds 1
ParBuf_F6       .ds 1
ParBuf_Fmode    .ds 1
ParBuf_Fatr1    .ds 1
ParBuf_Fatr2    .ds 1
ParBuf_Name     .ds 12
ParBuf_Names    .ds 12
ParBuf_Path     .ds 65
