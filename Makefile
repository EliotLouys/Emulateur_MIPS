# Name of the emulator executable to be produced.
TARGET=emulmips

# Name of executables used for compilation / linking / doc production.
# Feel free to change gcc -> clang in case you want to.
CC=`which gcc`
LD=`which gcc`
RM=`which rm` -f
DOXYGEN=`which doxygen`

MIPS_AS=`which mips-linux-gnu-as mips64-linux-gnu-as 2>&1 | grep -v which`
MIPS_AS_FLAGS=-no-trap -g2 -mips32r2  -32 -no-mips16 -mno-pdr -mno-shared -no-relax --traditional-format


# Compilation and linking options.
INCLUDE=-I$(INCDIR)
CFLAGS=-Wall -Wextra $(INCLUDE)
LFLAGS=-lreadline -lm -lcurses
CFLAGS_DBG=$(CFLAGS) -g -DNDEBUG
CFLAGS_RLS=$(CFLAGS) -O2


# Source / destination directories.
SRCDIR=src
INCDIR=include
DOCDIR=doc

# Construct list of source files.
SRC =$(wildcard $(SRCDIR)/*.c)
SRC+=$(wildcard $(SRCDIR)/commands/*.c)
SRC+=$(wildcard $(SRCDIR)/insns/*.c)
SRC+=$(wildcard $(SRCDIR)/elf/*.c)

# Construct list of includes (for Doxygen)
INC =$(wildcard $(INCDIR)/*.h)
INC+=$(wildcard $(INCDIR)/elf/*.h)

# Debug (resp. release) objects end in .dbg.o (resp. .rls.o).
OBJ_DBG=$(SRC:.c=.dbg.o)
OBJ_RLS=$(SRC:.c=.rls.o)


# Build MIPS ELF files
MIPS_ASM=$(wildcard elf/*.s)
MIPS_OBJ=$(MIPS_ASM:.s=.o)

# By default, make executes the first target.
# We use it to print the Makefile help.
all :
	@echo ""
	@echo "Usage:"
	@echo ""
	@echo "make debug         => build DEBUG   version"
	@echo "make release       => build RELEASE version"
	@echo "make documentation => build documentation in doc/html/ directory"
	@echo "make mipself       => build MIPS ELF files from assembly in elf/ directory"
	@echo "make clean         => clean everything"
#	@echo "make tarball       => produce .tgz archive in ../ directory"

debug   : $(OBJ_DBG)
	$(LD) $^ $(LFLAGS) -o $(TARGET)

release : $(OBJ_RLS)
	$(LD) $^ $(LFLAGS) -o $(TARGET)

mipself : $(MIPS_OBJ)

%.dbg.o : %.c
	$(CC) $< $(CFLAGS_DBG) -c -o $(basename $<).dbg.o

%.rls.o : %.c
	$(CC) $< $(CFLAGS_RLS) -c -o $(basename $<).rls.o

%.o     : %.s
	$(MIPS_AS) $(MIPS_AS_FLAGS) $< -o $@

documentation : $(SRC) $(INC)
	$(DOXYGEN)

clean :
	@$(RM) $(TARGET) $(SRCDIR)/*.o $(SRCDIR)/elf/*.o $(SRCDIR)/commands/*.o
	@find . -type f -name "*~" -delete

# Create archive with your code so you can send it to your advisor.
# Only send one of these archive files in your emails, nothing else!
#tarball : clean
#	tar -czvf ../$(notdir $(PWD) )-`whoami`-`date +%d-%m-%H-%M`.tgz .
#	echo "Fichier archive ../emulmips-`whoami`-`date +%d-%m-%H-%M`.tgz genere"
