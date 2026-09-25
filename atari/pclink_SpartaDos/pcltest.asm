;  pcltest.asm - SpartaDOS PCLink Protocol Test Suite
;
;  Automated Atari-side test runner and counterpart to the RespeQt
;  PclinkTest unit test suite.
;
;  Executes end-to-end SIO functional tests against RespeQt's PCLink
;  protocol implementation on Device $6F.
;
;  Usage:
;    PCLTEST [unit]     - Run test suite on unit 1-15 (default: 1)
;

    icl 'equ.asm'

    org $4000

Start
    jsr printf
    .byte '====================================',155
    .byte 'SpartaDOS PCLink Protocol Test Suite',155
    .byte 'Counterpart to RespeQt Unit Tests',155
    .byte '====================================',155,155,0

    ; Check for SpartaDOS
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
@SetupCrunch
    lda (comtab),y
    sta Crunch,x
    dey
    dex
    bne @SetupCrunch

    ; Default to unit 1
    lda #1
    sta UnitNum

    ; Check if optional unit argument provided
    jsr Crunch
    beq RunTests

    ; Parse unit argument
    ldy #comfnam+2
    lda (comtab),y
    sec
    sbc #'0'
    cmp #10
    bcc @SingleDigit
    ; Two-digit unit (10..15)
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
    bcc @BadUnit
    cmp #16
    bcc RunTests

@BadUnit
    jsr printf
    .byte 'Error: Invalid unit (must be 1-15)',155,0
    rts

; -------------------------------------------------------------
; Test Suite Execution
; -------------------------------------------------------------
RunTests
    lda #0
    sta PassCount
    sta FailCount
    sta CurrentTest

    jsr printf
    .byte 'Target: SIO Device $6F, Unit %d',155
    .byte 'Running protocol verification tests...',155,155,0
    .word UnitNum

    ; Run all test cases in sequence
    jsr Test01_SpeedPoll
    jsr Test02_ServerInit
    jsr Test03_DFree
    jsr Test04_GetCwd
    jsr Test05_MkdirRmdir
    jsr Test06_FileCreateWrite
    jsr Test07_FileOpenRead
    jsr Test08_FLenSeekTell
    jsr Test09_DirListing
    jsr Test10_RenameRemove
    jsr Test11_ErrorHandling

    ; Print Summary
    jsr PrintSummary
    rts

; -------------------------------------------------------------
; TEST 1: Speed Poll ('?')
; -------------------------------------------------------------
Test01_SpeedPoll
    inc CurrentTest
    jsr printf
    .byte '[TEST  1] SIO Speed Poll ............ ',0

    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_POLL
    sta DCOMND
    lda #$40
    sta DSTATS
    ldax SpeedBuf
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #1
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    sta DAUX2

    jsr SIOV
    bmi @Fail

    ; Check received speed divisor > 0
    lda SpeedBuf
    beq @Fail
    sta DivisorVal

    jsr RecordPass
    jsr printf
    .byte '(Divisor: $%x)',155,0
    .word DivisorVal
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 2: Server Init ($08)
; -------------------------------------------------------------
Test02_ServerInit
    inc CurrentTest
    jsr printf
    .byte '[TEST  2] Server Init (Unit Reset) .. ',0

    jsr ClearParBuf
    lda #FNO_INIT
    sta ParBuf_Fno
    jsr SendParBuf
    bmi @Fail

    jsr QueryStatus
    bmi @Fail
    lda StatusErr
    cmp #1              ; 1 = Success
    bne @Fail

    jsr RecordPass
    jsr printf
    .byte 155,0
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 3: DFree Volume Info ($13)
; -------------------------------------------------------------
Test03_DFree
    inc CurrentTest
    jsr printf
    .byte '[TEST  3] DFree Volume Info ......... ',0

    jsr ClearParBuf
    lda #FNO_DFREE
    sta ParBuf_Fno
    jsr SendParBuf
    bmi @Fail

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
    bmi @Fail

    ; Extract 8-char label
    ldy #0
@CopyLabel
    lda DFreeBuf+14,y
    sta VolLabel,y
    iny
    cpy #8
    bne @CopyLabel
    lda #0
    sta VolLabel+8

    jsr RecordPass
    jsr printf
    .byte '(Label: "%s")',155,0
    .word VolLabel
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 4: Get Working Directory ($11)
; -------------------------------------------------------------
Test04_GetCwd
    inc CurrentTest
    jsr printf
    .byte '[TEST  4] Get Working Directory ..... ',0

    jsr ClearParBuf
    lda #FNO_GETCWD
    sta ParBuf_Fno
    jsr SendParBuf
    bmi @Fail

    ; Fetch CWD string
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
    bmi @Fail

    lda #0
    sta PathBuf+64

    jsr RecordPass
    jsr printf
    .byte '(CWD: "%s")',155,0
    .word PathBuf
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 5: Mkdir ($0E) & Rmdir ($0F)
; -------------------------------------------------------------
Test05_MkdirRmdir
    inc CurrentTest
    jsr printf
    .byte '[TEST  5] Create & Remove Directory . ',0

    ; Step A: Mkdir TESTDIR
    jsr ClearParBuf
    lda #FNO_MKDIR
    sta ParBuf_Fno
    jsr SetNameTestDir
    jsr SendParBuf
    bmi @Fail

    jsr QueryStatus
    bmi @Fail
    lda StatusErr
    cmp #1
    bne @Fail

    ; Step B: Rmdir TESTDIR
    jsr ClearParBuf
    lda #FNO_RMDIR
    sta ParBuf_Fno
    jsr SetNameTestDir
    jsr SendParBuf
    bmi @Fail

    jsr QueryStatus
    bmi @Fail
    lda StatusErr
    cmp #1
    bne @Fail

    jsr RecordPass
    jsr printf
    .byte '(TESTDIR OK)',155,0
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 6: File Create ($09), Write ($01) & Close ($07)
; -------------------------------------------------------------
Test06_FileCreateWrite
    inc CurrentTest
    jsr printf
    .byte '[TEST  6] Create, Write & Close File  ',0

    ; Step A: FOpen mode 8 (create/write)
    jsr ClearParBuf
    lda #FNO_FOPEN
    sta ParBuf_Fno
    lda #8              ; Write/Create mode
    sta ParBuf_Fmode
    jsr SetNameTestFile
    jsr SendParBuf
    bmi @Fail

    ; Read 24-byte PCLDBF response
    jsr ReadOpenResponse
    bmi @Fail

    lda DBuf
    sta FileHandle
    cmp #16
    bcs @Fail

    ; Step B: FWrite 20 bytes
    jsr ClearParBuf
    lda #FNO_FWRITE
    sta ParBuf_Fno
    lda FileHandle
    sta ParBuf_Handle
    lda #20
    sta ParBuf_F1
    lda #0
    sta ParBuf_F2
    sta ParBuf_F3
    jsr SendParBuf
    bmi @Fail

    ; Send 20 data bytes
    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_EXEC
    sta DCOMND
    lda #$80             ; Write data frame
    sta DSTATS
    ldax WritePayload
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #20
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    lda UnitNum
    sta DAUX2

    jsr SIOV
    bmi @Fail

    ; Step C: FClose
    jsr CloseHandle
    bmi @Fail

    jsr RecordPass
    jsr printf
    .byte '(TESTPCL.TXT, 20b)',155,0
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 7: File Open ($09), Read ($00) & Integrity Check
; -------------------------------------------------------------
Test07_FileOpenRead
    inc CurrentTest
    jsr printf
    .byte '[TEST  7] Open, Read & Verify ....... ',0

    ; Step A: FOpen mode 4 (read)
    jsr ClearParBuf
    lda #FNO_FOPEN
    sta ParBuf_Fno
    lda #4              ; Read mode
    sta ParBuf_Fmode
    jsr SetNameTestFile
    jsr SendParBuf
    bmi @Fail

    jsr ReadOpenResponse
    bmi @Fail

    lda DBuf
    sta FileHandle
    cmp #16
    bcs @Fail

    ; Step B: FRead 20 bytes
    jsr ClearParBuf
    lda #FNO_FREAD
    sta ParBuf_Fno
    lda FileHandle
    sta ParBuf_Handle
    lda #20
    sta ParBuf_F1
    lda #0
    sta ParBuf_F2
    sta ParBuf_F3
    jsr SendParBuf
    bmi @Fail

    ; Receive 20 data bytes
    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_EXEC
    sta DCOMND
    lda #$40             ; Read data frame
    sta DSTATS
    ldax ReadBuffer
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #20
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    lda UnitNum
    sta DAUX2

    jsr SIOV
    bmi @Fail

    ; Step C: Verify payload match
    ldy #0
@VerifyLoop
    lda ReadBuffer,y
    cmp WritePayload,y
    bne @Fail
    iny
    cpy #20
    bne @VerifyLoop

    ; Step D: FClose
    jsr CloseHandle
    bmi @Fail

    jsr RecordPass
    jsr printf
    .byte '(Match OK)',155,0
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 8: File Length ($04), Seek ($02) & Tell ($03)
; -------------------------------------------------------------
Test08_FLenSeekTell
    inc CurrentTest
    jsr printf
    .byte '[TEST  8] FLen, FSeek & FTell ....... ',0

    ; Open file in read mode
    jsr ClearParBuf
    lda #FNO_FOPEN
    sta ParBuf_Fno
    lda #4
    sta ParBuf_Fmode
    jsr SetNameTestFile
    jsr SendParBuf
    bmi @Fail

    jsr ReadOpenResponse
    bmi @Fail
    lda DBuf
    sta FileHandle

    ; Step A: FLen query
    jsr ClearParBuf
    lda #FNO_FLEN
    sta ParBuf_Fno
    lda FileHandle
    sta ParBuf_Handle
    jsr SendParBuf
    bmi @Fail

    jsr Read4ByteResponse
    bmi @Fail

    ; Verify length == 20
    lda DBuf
    cmp #20
    bne @Fail

    ; Step B: FSeek to position 7
    jsr ClearParBuf
    lda #FNO_FSEEK
    sta ParBuf_Fno
    lda FileHandle
    sta ParBuf_Handle
    lda #7              ; Seek offset 7
    sta ParBuf_F1
    lda #0
    sta ParBuf_F2
    sta ParBuf_F3
    sta ParBuf_Fmode    ; 0 = SEEK_SET
    jsr SendParBuf
    bmi @Fail

    jsr Read4ByteResponse
    bmi @Fail

    ; Step C: FTell query
    jsr ClearParBuf
    lda #FNO_FTELL
    sta ParBuf_Fno
    lda FileHandle
    sta ParBuf_Handle
    jsr SendParBuf
    bmi @Fail

    jsr Read4ByteResponse
    bmi @Fail

    ; Verify position == 7
    lda DBuf
    cmp #7
    bne @Fail

    ; Close file
    jsr CloseHandle
    bmi @Fail

    jsr RecordPass
    jsr printf
    .byte '(Len=20, Pos=7)',155,0
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 9: Directory Enumeration ($0A / $06)
; -------------------------------------------------------------
Test09_DirListing
    inc CurrentTest
    jsr printf
    .byte '[TEST  9] Directory Listing (FFirst)  ',0

    ; FFirst with wildcard *.* in directory mode ($10)
    jsr ClearParBuf
    lda #FNO_FFIRST
    sta ParBuf_Fno
    lda #$10            ; Directory mode
    sta ParBuf_Fmode
    jsr SetWildcardName
    jsr SendParBuf
    bmi @Fail

    jsr ReadOpenResponse
    bmi @Fail

    lda DBuf
    sta DirHandle
    cmp #16
    bcs @Fail

    ; Scan entries using FNEXT to find TESTPCL.TXT
    lda #0
    sta MatchFound
    lda #30             ; Max 30 entries
    sta LoopCount

@NextLoop
    ; Check if current entry matches TESTPCL TXT
    jsr CheckDirEntryMatch
    beq @FoundFile

    ; Fetch next entry via FNEXT
    jsr ClearParBuf
    lda #FNO_FNEXT
    sta ParBuf_Fno
    lda DirHandle
    sta ParBuf_Handle
    jsr SendParBuf
    bmi @CloseDir

    jsr ReadOpenResponse
    bmi @CloseDir

    dec LoopCount
    bne @NextLoop

@CloseDir
    lda DirHandle
    sta FileHandle
    jsr CloseHandle
    lda MatchFound
    bne @PassDir
    jmp @Fail

@FoundFile
    lda #1
    sta MatchFound
    jmp @CloseDir

@PassDir
    jsr RecordPass
    jsr printf
    .byte '(Found TESTPCL.TXT)',155,0
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 10: Rename ($0B) & Remove ($0C)
; -------------------------------------------------------------
Test10_RenameRemove
    inc CurrentTest
    jsr printf
    .byte '[TEST 10] Rename & Remove File ...... ',0

    ; Step A: Rename TESTPCL.TXT -> RENAMED.TXT
    jsr ClearParBuf
    lda #FNO_RENAME
    sta ParBuf_Fno
    jsr SetNameTestFile
    jsr SetNamesRenamedFile
    jsr SendParBuf
    bmi @Fail

    jsr QueryStatus
    bmi @Fail
    lda StatusErr
    cmp #1
    bne @Fail

    ; Step B: Remove RENAMED.TXT
    jsr ClearParBuf
    lda #FNO_REMOVE
    sta ParBuf_Fno
    jsr SetNameRenamedFile
    jsr SendParBuf
    bmi @Fail

    jsr QueryStatus
    bmi @Fail
    lda StatusErr
    cmp #1
    bne @Fail

    jsr RecordPass
    jsr printf
    .byte '(OK)',155,0
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; TEST 11: Error Handling & Negative Tests
; -------------------------------------------------------------
Test11_ErrorHandling
    inc CurrentTest
    jsr printf
    .byte '[TEST 11] Protocol Error Handling ... ',0

    ; Subtest A: Open non-existent file for read -> Err 170 (FileNotFound)
    jsr ClearParBuf
    lda #FNO_FOPEN
    sta ParBuf_Fno
    lda #4              ; Read
    sta ParBuf_Fmode
    jsr SetNameNonExistent
    jsr SendParBuf

    ; Read response; status should report error 170
    jsr QueryStatus
    lda StatusErr
    cmp #170            ; ErrFileNotFound
    bne @Fail

    ; Subtest B: Read from invalid handle ($FE) -> Err 134 (BadHandle)
    jsr ClearParBuf
    lda #FNO_FREAD
    sta ParBuf_Fno
    lda #$FE            ; Invalid handle
    sta ParBuf_Handle
    lda #10
    sta ParBuf_F1
    jsr SendParBuf

    jsr QueryStatus
    lda StatusErr
    cmp #134            ; ErrBadHandle
    bne @Fail

    jsr RecordPass
    jsr printf
    .byte '(Err 170 & 134 OK)',155,0
    rts

@Fail
    jsr RecordFail
    rts

; -------------------------------------------------------------
; Result Recording & Output
; -------------------------------------------------------------
RecordPass
    inc PassCount
    jsr printf
    .byte '[PASS] ',0
    rts

RecordFail
    inc FailCount
    jsr printf
    .byte '[FAIL]',155,0
    rts

PrintSummary
    jsr printf
    .byte '====================================',155
    .byte 'Test Results: %d Passed, %d Failed',155,0
    .word PassCount
    .word FailCount

    lda FailCount
    beq @AllPassed

    jsr printf
    .byte 'STATUS: FAILED (Some tests failed)',155,0
    rts

@AllPassed
    jsr printf
    .byte 'STATUS: ALL TESTS PASSED SUCCESSFULLY',155,0
    rts

; -------------------------------------------------------------
; SIO & Buffer Helper Routines
; -------------------------------------------------------------

ClearParBuf
    ldx #0
    txa
@ClearLoop
    sta ParBuf,x
    inx
    cpx #100
    bne @ClearLoop
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

QueryStatus
    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_STATUS      ; 'S'
    sta DCOMND
    lda #$40             ; Read data frame
    sta DSTATS
    ldax StatBuf
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #4               ; 4 bytes
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    sta DAUX2

    jsr SIOV
    bmi @StatErr
    lda StatBuf+1
    sta StatusErr
    lda #1
    rts

@StatErr
    lda #$FF
    sta StatusErr
    rts

ReadOpenResponse
    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_EXEC        ; 'R'
    sta DCOMND
    lda #$40             ; Read data frame
    sta DSTATS
    ldax DBuf
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #24              ; PCLDBF response size
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    lda UnitNum
    sta DAUX2

    jsr SIOV
    rts

Read4ByteResponse
    lda #PCLINK_DEV
    sta DDEVIC
    lda UnitNum
    sta DUNIT
    lda #CMD_EXEC        ; 'R'
    sta DCOMND
    lda #$40
    sta DSTATS
    ldax DBuf
    stax DBUFLO
    lda #7
    sta DTIMLO
    lda #4
    sta DBYTLO
    lda #0
    sta DBYTHI
    sta DAUX1
    lda UnitNum
    sta DAUX2

    jsr SIOV
    rts

CloseHandle
    jsr ClearParBuf
    lda #FNO_FCLOSE
    sta ParBuf_Fno
    lda FileHandle
    sta ParBuf_Handle
    jsr SendParBuf
    rts

; -------------------------------------------------------------
; Filename Formatters
; -------------------------------------------------------------

SetNameTestFile
    ; Name: "TESTPCL TXT"
    ldx #0
@CopyT1
    lda NameTestFileStr,x
    sta ParBuf_Name,x
    inx
    cpx #11
    bne @CopyT1
    rts

SetNameTestDir
    ; Name: "TESTDIR    "
    ldx #0
@CopyT2
    lda NameTestDirStr,x
    sta ParBuf_Name,x
    inx
    cpx #11
    bne @CopyT2
    rts

SetNameRenamedFile
    ; Name: "RENAMED TXT"
    ldx #0
@CopyT3
    lda NameRenamedStr,x
    sta ParBuf_Name,x
    inx
    cpx #11
    bne @CopyT3
    rts

SetNamesRenamedFile
    ; Names: "RENAMED TXT"
    ldx #0
@CopyT4
    lda NameRenamedStr,x
    sta ParBuf_Names,x
    inx
    cpx #11
    bne @CopyT4
    rts

SetNameNonExistent
    ; Name: "NOFILE  XYZ"
    ldx #0
@CopyT5
    lda NameNoFileStr,x
    sta ParBuf_Name,x
    inx
    cpx #11
    bne @CopyT5
    rts

SetWildcardName
    ; Name: "*   *  "
    ldx #0
@CopyT6
    lda NameWildcardStr,x
    sta ParBuf_Name,x
    inx
    cpx #11
    bne @CopyT6
    rts

CheckDirEntryMatch
    ; Compare DBuf+1 (name in DIRENTRY) with "TESTPCL TXT"
    ldx #0
@CmpLoop
    lda DBuf+1,x
    cmp NameTestFileStr,x
    bne @NoMatch
    inx
    cpx #11
    bne @CmpLoop
    lda #0              ; Match found (Z=1)
    rts
@NoMatch
    lda #1              ; No match (Z=0)
    rts

; -------------------------------------------------------------
; Data & Buffers
; -------------------------------------------------------------

NameTestFileStr .byte 'TESTPCL TXT'
NameTestDirStr  .byte 'TESTDIR    '
NameRenamedStr  .byte 'RENAMED TXT'
NameNoFileStr   .byte 'NOFILE  XYZ'
NameWildcardStr .byte '*   *  '

WritePayload    .byte 'PCLINK-TEST-PAYLOAD!'

PassCount       .ds 1
FailCount       .ds 1
CurrentTest     .ds 1
DivisorVal      .ds 1
FileHandle      .ds 1
DirHandle       .ds 1
MatchFound      .ds 1
LoopCount       .ds 1

Crunch          .ds 3
SpeedBuf        .ds 1
StatBuf         .ds 4
DBuf            .ds 24
DFreeBuf        .ds 64
PathBuf         .ds 65
ReadBuffer      .ds 24
VolLabel        .ds 9

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

    icl 'printf.asm'
