# HDR=include/callbacks.h include/resource.h
NAME=NMahou
BINDIR=bin/
BINS=$(BINDIR)jkl.dll
MAHOU_OBJ=$(OBJDIR)/$(NAME).o $(OBJDIR)/resource.o
OBJ=$(MAHOU_OBJ) $(OBJDIR)/jkl.dll.o
INC=-I.\inc -I.\res -I.\jkl
CC=gcc
RC=windres
XARCH=
DEBUG=0
FLG=-lgdi32 -lcomctl32 -s -Os -DUNICODE -D_UNICODE
dllflg=-DUNICODE -D_UNICODE -s -Os -shared -o 
exeflg=-DUNICODE -D_UNICODE -s -Os -o 
# ARCH=Architecture x86(x32) or x64. [-m32]
ifeq ($(ARCH), -m32)
	RCARCH=-F pe-i386
	OBJDIR=obj
	XARCH=-DSUPPORTX86
	EXE=$(NAME).exe
else
	OBJDIR=obj64
	OBJ := $(OBJ) $(OBJDIR)/jklx86.exe.o $(OBJDIR)/jklx86.dll.o
	EXE=$(NAME)64.exe
	BINS := $(BINS) $(BINDIR)jklx86.exe $(BINDIR)jklx86.dll
endif
ifeq ($(DEBUG), 1)
	DFLG = -DDEBUG
endif
# DBG=Debug mode, don't use [-s] and [-Os] switches. [Y]
ifeq ($(DBG), Y)
	FLG:=$(filter-out -s -Os,$(FLG))
endif
# PACK=Pack executable with UPX. [Y]
ifeq ($(PACK), Y)
	UPXEXEC=&& upx -9 $(EXE)
endif
all: $(OBJDIR) $(BINDIR) $(OBJ) $(BINS)
	@echo Compiling NMahou.exe objects into executable...
	@$(CC) $(ARCH) $(INC) $(MAHOU_OBJ) $(FLG) $(XARCH) $(DFLG) -o $(BINDIR)$(EXE) $(UPXEXEC)
	@echo Done.
 
$(OBJDIR)/jkl.dll.o:
	@echo Compiling jkl.dll.o...
	@$(CC) $(ARCH) jkl/jkl.cxx -c $(dllflg) $(OBJDIR)/jkl.dll.o
	
$(BINDIR)jkl.dll: $(OBJDIR)/jkl.dll.o
	@echo Compiling jkl.dll...
	@$(CC) $(ARCH) $(OBJDIR)/jkl.dll.o $(dllflg) $(BINDIR)jkl.dll
	
$(OBJDIR)/jklx86.dll.o:
	@echo Compiling jklx86.dll.o...
	@$(CC) -m32 jkl/jkl.cxx -c $(dllflg) $(OBJDIR)/jklx86.dll.o

$(BINDIR)jklx86.dll: $(OBJDIR)/jklx86.dll.o
	@echo Compiling jklx86.dll...
	@$(CC) -m32 $(OBJDIR)/jklx86.dll.o $(dllflg) $(BINDIR)jklx86.dll

$(OBJDIR)/jklx86.exe.o:
	@echo Compiling jklx86.exe.o...
	@$(CC) jkl/jklx86.c -Wl,-Bstatic -lpthread -c -m32 $(exeflg) $(OBJDIR)/jklx86.exe.o	

$(BINDIR)jklx86.exe: $(OBJDIR)/jklx86.exe.o
	@echo Compiling jklx86.exe...
	@$(CC) $(OBJDIR)/jklx86.exe.o -Wl,-Bstatic -lpthread -m32 $(exeflg) $(BINDIR)jklx86.exe

both: 
	@echo -e "\e[34mCompiling x32 executable...\e[0m"
	-@$(MAKE) -s all ARCH=-m32
	@echo -e "\e[33mCompiling x64 executable...\e[0m"
	-@$(MAKE) -s all ARCH=
	@echo -e "\e[32mBoth executable compilation done.\e[0m"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(OBJDIR)/%.o: %.c
	@echo Compiling $@...
	@$(CC) $(ARCH) $(XARCH) $(DFLG) $(INC) -c $< -o $@
	@echo Done.

$(OBJDIR)/resource.o:
	@echo Compiling $@...
	@$(RC) $(RCARCH) $(INC) -i res/Window.rc -o $@
	@echo Done.

clean:
	@echo Removing build files...
	-@rm -f -r obj obj64 bin
	@echo Done.

run:
	@echo Starting executable...
	@$(MAKE) -s all
	@$(BINDIR)$(EXE)
	@$(MAKE) clean

zip:
	@$(MAKE) -s all
	@echo Compressing executables...
	-@upx -9 $(BINDIR)$(NAME).exe $(BINDIR)$(NAME)64.exe $(BINDIR)jkl.dll $(BINDIR)jklx86.exe $(BINDIR)jklx86.dll
	@echo Done.

re:
	@$(MAKE) -s clean
	@$(MAKE) -s all
debug:
	@$(MAKE) all DEBUG=1
renm: 
	@echo Removing NMahou build files...
	-@rm -f obj/NMahou.o obj64/NMahou.o bin/NMahou.exe  obj64/resource.o  obj/resource.o
	@echo Done.
	@$(MAKE) all
reboth: 
	@$(MAKE) -s clean
	@$(MAKE) -s both
