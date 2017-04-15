# HDR=include/callbacks.h include/resource.h
NAME=NMahou
OBJ=$(OBJDIR)/$(NAME).o $(OBJDIR)/resource.o
INC=-I.\inc -I.\res
CC=gcc
RC=windres
FLG=-lgdi32 -lcomctl32 -s -Os -DUNICODE -D_UNICODE
# ARCH=Architecture x86(x32) or x64. [-m32]
ifeq ($(ARCH), -m32)
	RCARCH=-F pe-i386
	OBJDIR=obj
	EXE=$(NAME).exe
else
	OBJDIR=obj64
	EXE=$(NAME)64.exe
endif
# DBG=Debug mode, don't use [-s] and [-Os] switches. [Y]
ifeq ($(DBG), Y)
	FLG:=$(filter-out -s -Os,$(FLG))
endif
# PACK=Pack executable with UPX. [Y]
ifeq ($(PACK), Y)
	UPXEXEC=&& upx -9 $(EXE)
endif
all: $(OBJDIR) $(OBJ)
	@echo Compiling objects into executable...
	@$(CC) $(ARCH) $(INC) $(OBJ) $(FLG) -o $(EXE) $(UPXEXEC)
	@echo Done.

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c
	@echo Compiling $@...
	@$(CC) $(ARCH) $(INC) -c $< -o $@
	@echo Done.

$(OBJDIR)/resource.o:
	@echo Compiling $@...
	@$(RC) $(RCARCH) $(INC) -i res/Window.rc -o $@
	@echo Done.

clean:
	@echo Removing build files...
	-@rm -f -r obj obj64 $(NAME).exe $(NAME)64.exe
	@echo Done.

run: all
	@echo Starting executable...
	@./$(EXE)
	@$(MAKE) clean
zip: all
	@echo Compressing executables...
	-@upx -9 $(EXE)
	@echo Done.
re: clean all
