/*
 * COPYRIGHT:       GPL - See COPYING in the top level directory
 * PROJECT:         ReactOS Virtual DOS Machine
 * FILE:            bios32.h
 * PURPOSE:         VDM 32-bit BIOS
 * PROGRAMMERS:     Aleksandar Andrejevic <theflash AT sdf DOT lonestar DOT org>
 */

#ifndef _BIOS32_H_
#define _BIOS32_H_

/* INCLUDES *******************************************************************/

#include "ntvdm.h"
#include "kbdbios32.h"
#include "vidbios32.h"

/**/ #include "callback.h" /**/

/* DEFINES ********************************************************************/

#define BIOS_PIC_MASTER_INT 0x08
#define BIOS_PIC_SLAVE_INT  0x70

#define BIOS_EQUIPMENT_INTERRUPT    0x11
#define BIOS_MEMORY_SIZE            0x12
#define BIOS_MISC_INTERRUPT         0x15
#define BIOS_TIME_INTERRUPT         0x1A
#define BIOS_SYS_TIMER_INTERRUPT    0x1C

#define BIOS_EQUIPMENT_LIST     0x2C // HACK: Disable FPU for now


#define BDA_SEGMENT     0x40
// #define BIOS_SEGMENT    0xF000

/*
 * BIOS Data Area at 0040:XXXX
 *
 * See: http://webpages.charter.net/danrollins/techhelp/0093.HTM
 * and: http://www.bioscentral.com/misc/bda.htm
 * for more information.
 */
#pragma pack(push, 1)
typedef struct
{
    WORD SerialPorts[4];                        // 0x00
    WORD ParallelPorts[3];                      // 0x08
    WORD EbdaSegment;                           // 0x0e - ParallelPort in PC/XT
    WORD EquipmentList;                         // 0x10
    BYTE Reserved0;                             // 0x12 - Errors in PCjr infrared keyboard link
    WORD MemorySize;                            // 0x13
    WORD Reserved1;                             // 0x15 - Scratch pad for manufacturing error tests
    WORD KeybdShiftFlags;                       // 0x17
    BYTE AlternateKeypad;                       // 0x19
    WORD KeybdBufferHead;                       // 0x1a
    WORD KeybdBufferTail;                       // 0x1c
    WORD KeybdBuffer[BIOS_KBD_BUFFER_SIZE];     // 0x1e
    BYTE DriveRecalibrate;                      // 0x3e
    BYTE DriveMotorStatus;                      // 0x3f
    BYTE MotorShutdownCounter;                  // 0x40
    BYTE LastDisketteOperation;                 // 0x41
    BYTE Reserved2[7];                          // 0x42
    BYTE VideoMode;                             // 0x49
    WORD ScreenColumns;                         // 0x4a
    WORD VideoPageSize;                         // 0x4c
    WORD VideoPageOffset;                       // 0x4e
    WORD CursorPosition[BIOS_MAX_PAGES];        // 0x50
    BYTE CursorEndLine;                         // 0x60
    BYTE CursorStartLine;                       // 0x61
    BYTE VideoPage;                             // 0x62
    WORD CrtBasePort;                           // 0x63
    BYTE CrtModeControl;                        // 0x65
    BYTE CrtColorPaletteMask;                   // 0x66
    BYTE CassetteData[5];                       // 0x67
    DWORD TickCounter;                          // 0x6c
    BYTE MidnightPassed;                        // 0x70
    BYTE BreakFlag;                             // 0x71
    WORD SoftReset;                             // 0x72
    BYTE LastDiskOperation;                     // 0x74
    BYTE NumDisks;                              // 0x75
    BYTE DriveControlByte;                      // 0x76
    BYTE DiskPortOffset;                        // 0x77
    BYTE LptTimeOut[4];                         // 0x78
    BYTE ComTimeOut[4];                         // 0x7c
    WORD KeybdBufferStart;                      // 0x80
    WORD KeybdBufferEnd;                        // 0x82
    BYTE ScreenRows;                            // 0x84
    WORD CharacterHeight;                       // 0x85
    BYTE EGAFlags[2];                           // 0x87
    BYTE VGAFlags[2];                           // 0x89
    DWORD Reserved3;                            // 0x8b
    BYTE Reserved4;                             // 0x8f
    BYTE Reserved5[2];                          // 0x90
    BYTE Reserved6[2];                          // 0x92
    BYTE Reserved7[2];                          // 0x94
    WORD Reserved8;                             // 0x96
    DWORD Reserved9;                            // 0x98
    DWORD Reserved10;                           // 0x9c
    DWORD Reserved11[2];                        // 0xa0
    DWORD EGAPtr;                               // 0xa8
    BYTE Reserved12[68];                        // 0xac
    BYTE Reserved13[16];                        // 0xf0

    DWORD Reserved14;                           // 0x100
    BYTE Reserved15[12];                        // 0x104
    BYTE Reserved16[17];                        // 0x110
    BYTE Reserved17[15];                        // 0x121
    BYTE Reserved18[3];                         // 0x130
} BIOS_DATA_AREA, *PBIOS_DATA_AREA;
#pragma pack(pop)

C_ASSERT(sizeof(BIOS_DATA_AREA) == 0x133);

/* FUNCTIONS ******************************************************************/

extern CALLBACK16 BiosContext;
#define RegisterBiosInt32(IntNumber, IntHandler) \
do { \
    BiosContext.NextOffset += RegisterInt32(MAKELONG(BiosContext.NextOffset,  \
                                                     BiosContext.Segment),    \
                                            (IntNumber), (IntHandler), NULL); \
} while(0);

extern PBIOS_DATA_AREA Bda;

/**HACK!!**/typedef VOID (WINAPI *EMULATOR_INT32_PROC)(LPWORD Stack);/**HACK!!**/

VOID EnableHwIRQ(UCHAR hwirq, EMULATOR_INT32_PROC func);
VOID PicIRQComplete(LPWORD Stack);

BOOLEAN Bios32Initialize(IN HANDLE ConsoleInput,
                         IN HANDLE ConsoleOutput);
VOID Bios32Cleanup(VOID);

#endif // _BIOS32_H_

/* EOF */
