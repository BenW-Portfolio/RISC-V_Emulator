# Top level Makefile for building and testing the project
# Feel free to modify this file as needed

OBJCRT_DIR = object_create
STUDENT_DIR = src
TEST_DIR = tests
TEST_SRCOBJS = $(sort $(wildcard $(TEST_DIR)/assembly/*.S))

# Default cache settings (can be overridden by passing variables in the command line)
CACHE_LINES ?= 1
CACHE_SETS ?= 1

all: test_objects

v1: 
	make rebuild CACHE_LINES=8 CACHE_SETS=8
v2: 
	make rebuild CACHE_LINES=4 CACHE_SETS=4

rebuild: clean all

test1: 
	@make v1
	@echo "\nRunning Tests V1...."
	./run_tests.sh v1

test2: 
	@make v2
	@echo "\nRunning Tests V2...."
	./run_tests.sh v2

# Create all of the test objects
test_objects: $(wildcard $(OBJCRT_DIR)/*) $(TEST_SRCOBJS)
	$(info <<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>)
	$(info <<< Creating solution and all of the test objects  >>>)
	$(info <<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>)
	CACHE_NUM_LINES=$(CACHE_LINES) CACHE_SETS=$(CACHE_SETS) make --no-print-directory -C $(OBJCRT_DIR)
	@echo ""
	@echo "Compilation complete"
	

clean:
	make --no-print-directory -C $(STUDENT_DIR) clean
	make --no-print-directory -C $(OBJCRT_DIR) clean


.PHONY: all clean rebuild test1 test2 v1 v2 test_objects 

