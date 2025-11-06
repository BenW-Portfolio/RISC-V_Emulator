HW1 = S
HW2 = S
HW3 = S
HW4 = S
# HW1 = P
# HW2 = P
# HW3 = P
# HW4 = P

# Get a list of all .c files in the current directory
CFILES_ALL = $(wildcard *.c)
CFILES = $(CFILES_ALL)

# Set the C files to compile based on the mode 
ifeq ($(HW1), P)
	CFILES = $(filter-out instructions.c, $(C_FILES))
endif
ifeq ($(HW2), P)
	CFILES = $(filter-out registers.c, $(C_FILES))
endif
ifeq ($(HW3), P)
	CFILES = $(filter-out memory.c, $(C_FILES))
endif
ifeq ($(HW4), P)
	CFILES = $(filter-out control_flow.c, $(C_FILES))
endif

CFILES_EXCLUDE_MAIN = $(filter-out main.c, $(CFILES))
OFILES = $(patsubst %.c,%.o,$(CFILES_ALL))
DFILES = $(patsubst %.c,%.d,$(CFILES_ALL))
BINARY = r5emu
CC = gcc
CFLAGS = -O3 -I. -Wall -Werror -Wextra 

all:
# Check if Mode is valid
ifeq ($(and $(filter-out S P,$(HW1) $(HW2) $(HW3) $(HW4))),$(HW1)$(HW2)$(HW3)$(HW4))
	$(error Invalid values: HW1, HW2, HW3, and HW4 must be either "S" or "P")
endif
ifeq ($(HW1), P)
	$(info <<<<<<<<<< Using provided hw1 object file >>>>>>>>>>>>)
	@cp objects/instructions.o .
else
	$(info <<<<<<<<<< Using student's instructions.c >>>>>>>>>>>>)
endif
ifeq ($(HW2), P)
	$(info <<<<<<<<<< Using provided hw2 object file >>>>>>>>>>>>)
	@cp objects/registers.o .
else 
	$(info <<<<<<<<<< Using student's registers.c >>>>>>>>>>>>)
endif
ifeq ($(HW3), P)
	$(info <<<<<<<<<< Using provided hw3 object file >>>>>>>>>>>>)
	@cp objects/memory.o .
else 
	$(info <<<<<<<<<< Using student's memory.c >>>>>>>>>>>>)
endif
ifeq ($(HW4), P)
	$(info <<<<<<<<<< Using provided hw4 object file >>>>>>>>>>>>)
	@cp objects/control_flow.o .
else 
	$(info <<<<<<<<<< Using student's control_flow.c >>>>>>>>>>>>)
endif
	make $(BINARY) 

$(BINARY): $(OFILES)
	@$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

-include $(DFILES)

clean:
	@rm -rf $(BINARY) $(OFILES) $(DFILES) tests/test
	

