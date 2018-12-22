FIVM_VERSION = 1.0
SCALA_VERSION = 2.12

# Compilers
FRONTEND_BIN = scala ../frontend/target/scala-$(SCALA_VERSION)/FIVM-assembly-$(FIVM_VERSION).jar
BACKEND_BIN = ../backend/dbtoaster/bin/dbtoaster
BACKEND_FLAGS ?= -l vcpp -O3 -xhashmap -xruntime -F HEURISTICS-DECOMPOSE-OVER-TABLES
CC = g++
CFLAGS = -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++11 -pedantic

# Runtime arguments
NUMBER_OF_RUNS ?= 1
BATCH_SIZE ?= 1000

INPUT_DIR_SQL = queries
SQL_FILES := $(shell find $(INPUT_DIR_SQL) -name "*.sql")

# SQL to M3 compilation
OUTPUT_DIR_M3 = code/generated/m3
M3_FILES_BATCH := $(patsubst $(INPUT_DIR_SQL)/%.sql, $(OUTPUT_DIR_M3)/%_BATCH.m3, $(SQL_FILES))
M3_FILES_SINGLE := $(patsubst $(INPUT_DIR_SQL)/%.sql, $(OUTPUT_DIR_M3)/%_SINGLE.m3, $(SQL_FILES))

# M3 to CPP compilation
OUTPUT_DIR_CPP = code/generated/cpp
CPP_FILES_BATCH := $(patsubst $(OUTPUT_DIR_M3)/%.m3, $(OUTPUT_DIR_CPP)/%.hpp, $(M3_FILES_BATCH))
CPP_FILES_SINGLE := $(patsubst $(OUTPUT_DIR_M3)/%.m3, $(OUTPUT_DIR_CPP)/%.hpp, $(M3_FILES_SINGLE))

# CPP compilation
OUTPUT_DIR_EXE = bin
EXE_FILES_BATCH := $(patsubst $(OUTPUT_DIR_CPP)/%.hpp, $(OUTPUT_DIR_EXE)/%_$(BATCH_SIZE), $(CPP_FILES_BATCH))
EXE_FILES_SINGLE := $(patsubst $(OUTPUT_DIR_CPP)/%.hpp, $(OUTPUT_DIR_EXE)/%, $(CPP_FILES_SINGLE))

.PHONY: all m3_batch m3_single m3 cpp_batch cpp_single cpp exe_batch exe_single exe clean

all: exe

$(M3_FILES_BATCH): $(OUTPUT_DIR_M3)/%_BATCH.m3: $(INPUT_DIR_SQL)/%.sql
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(FRONTEND_BIN) --batch -o $@ $<

$(M3_FILES_SINGLE): $(OUTPUT_DIR_M3)/%_SINGLE.m3: $(INPUT_DIR_SQL)/%.sql
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(FRONTEND_BIN) --single -o $@ $<

$(CPP_FILES_BATCH): $(OUTPUT_DIR_CPP)/%.hpp : $(OUTPUT_DIR_M3)/%.m3
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(BACKEND_BIN) $(BACKEND_FLAGS) --batch -o $@ $<

$(CPP_FILES_SINGLE): $(OUTPUT_DIR_CPP)/%.hpp : $(OUTPUT_DIR_M3)/%.m3
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(BACKEND_BIN) $(BACKEND_FLAGS) -o $@ $<

$(EXE_FILES_BATCH): $(OUTPUT_DIR_EXE)/%_$(BATCH_SIZE) : $(OUTPUT_DIR_CPP)/%.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(eval TEMPLATE := $(patsubst $(OUTPUT_DIR_EXE)/%/,%, $(dir $@)))
	$(CC) $(CFLAGS) code/main.cpp -I ../backend/lib -I code -I code/lib -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) -include $< -include code/templates/$(TEMPLATE)_template.hpp -o $@ 

$(EXE_FILES_SINGLE): $(OUTPUT_DIR_EXE)/% : $(OUTPUT_DIR_CPP)/%.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(eval TEMPLATE := $(patsubst $(OUTPUT_DIR_EXE)/%/,%, $(dir $@)))
	$(CC) $(CFLAGS) code/main.cpp -I ../backend/lib -I code -I code/lib -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -include $< -include code/templates/$(TEMPLATE)_template.hpp -o $@ 

m3_batch: $(M3_FILES_BATCH)

m3_single: $(M3_FILES_SINGLE)

m3: m3_batch m3_single 

cpp_batch: $(CPP_FILES_BATCH) 

cpp_single: $(CPP_FILES_SINGLE)

cpp: cpp_batch cpp_single

exe_batch: $(EXE_FILES_BATCH) 

exe_single: $(EXE_FILES_SINGLE)

exe: exe_batch exe_single

clean: 
	rm -fr $(OUTPUT_DIR_M3) $(OUTPUT_DIR_CPP) $(OUTPUT_DIR_EXE)
