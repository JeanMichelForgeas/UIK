# *****************************************************************************
# *                                                                           *
# *  Default Makefile used in order to build an UIK object.                   *
# *                                                                           *
# *  Created 03/31/93 Frederic DENIS, ©1993  The Software Laboratory          *
# *                                                                           *
# *  All Rights Reserved.                                                     *
# *                                                                           *
# *****************************************************************************

# -=* Constants *=-

OBJNAME           = Cycle

DEST              = UIK:Objects/
UIKINCLUDES       = UIKI:

OBJ_BASENAME      = UIKObj_
INCL_BASENAME     = Obj

DEBUG_FLAG        = #-dDEBUG -d5

C_COMPILER        = lc
C_COMPILER_OPTS   = -Hram:headers -s -O -ccstq -v -b0

ASSEMBLER         = genam
ASSEMBLER_OPTS    = -L -iINCLUDE: -vp=68030/68881/68851

LINKER            = BLink
LINKER_OPTS       = BUFSIZE 32768 SC SD ND VERBOSE

STARTUP           = LIB:c.o
LIBS              = LIB:debug.lib LIB:aps.lib LIB:uik.lib LIB:lcnb.lib \
                    LIB:amiga.lib

# -=* transformation rules *=-
.c.o:
   $(C_COMPILER) $(C_COMPILER_OPTS) $(DEBUG_FLAG) $*

.a.o:
   $(ASSEMBLER) $(ASSEMBLER_OPTS) -O$*.o $*.a

fake :   ClearScreen \
         $(DEST)$(OBJ_BASENAME)$(OBJNAME) \
         Test


# *****************************************************************************
# * Sources Files & Dependencies                                              *
# *****************************************************************************

$(OBJ_BASENAME)$(OBJNAME)_Head.o:   $(OBJ_BASENAME)$(OBJNAME)_Head.a

$(OBJ_BASENAME)$(OBJNAME).o :       $(OBJ_BASENAME)$(OBJNAME).c \
                                    $(UIKINCLUDES)$(INCL_BASENAME)$(OBJNAME).h

Test.o :                            Test.c
TestBM.o:                           TestBM.c

# *****************************************************************************
# * Actions                                                                   *
# *****************************************************************************

$(DEST)$(OBJ_BASENAME)$(OBJNAME):   $(OBJ_BASENAME)$(OBJNAME)_Head.o \
                                    $(OBJ_BASENAME)$(OBJNAME).o
   @$(LINKER) <with <
FROM $(OBJ_BASENAME)$(OBJNAME)_Head.o $(OBJ_BASENAME)$(OBJNAME).o TO \
$(DEST)$(OBJ_BASENAME)$(OBJNAME) LIB $(LIBS) $(LINKER_OPTS)
<
   FlushUIK

Test: Test.o TestBM.o
   @$(LINKER) <with <
FROM $(STARTUP) Test.o TestBM.o TO Test LIB $(LIBS) $(LINKER_OPTS)
<

ClearScreen:
   @Cls
