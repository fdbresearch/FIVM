# Compilers
CODEGEN_BIN = ../scripts/generate-code.sh
COMPILE_CXX_BIN = ../scripts/build-generated-cpp.sh
EXTRA_ARGS = -I include

# Directories
SRC_DIR = src
SQL_DIR = queries
M3_OUT_DIR = generated/m3
CPP_OUT_DIR = generated/cpp
EXE_OUT_DIR = bin

# Find all SQL queries
SQL_FILES := $(shell find $(SQL_DIR) -name "*.sql")

# SQL to M3 compilation
M3_FILES := $(patsubst $(SQL_DIR)/%.sql, $(M3_OUT_DIR)/%.m3, $(SQL_FILES))

# M3 to CPP compilation
CPP_FILES := $(patsubst $(M3_OUT_DIR)/%.m3, $(CPP_OUT_DIR)/%.hpp, $(M3_FILES))

# CPP compilation
EXE_FILES := $(patsubst $(CPP_OUT_DIR)/%.hpp, $(EXE_OUT_DIR)/%, $(CPP_FILES))

.PHONY: all m3 cpp exe clean

all: exe

$(M3_FILES): $(M3_OUT_DIR)/%.m3: $(SQL_DIR)/%.sql
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CODEGEN_BIN) -l m3 --batch -o $@ $<

$(CPP_FILES): $(CPP_OUT_DIR)/%.hpp : $(SQL_DIR)/%.sql
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CODEGEN_BIN) -l cpp --batch -o $@ $<

$(EXE_FILES): $(EXE_OUT_DIR)/% : $(CPP_OUT_DIR)/%.hpp ../runtime/src/main.cpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	EXTRA_ARGS="$(EXTRA_ARGS)" $(COMPILE_CXX_BIN) $< $@

m3: $(M3_FILES)

cpp: $(CPP_FILES)

exe: $(EXE_FILES)

clean: 
	rm -fr $(M3_OUT_DIR) $(CPP_OUT_DIR) $(EXE_OUT_DIR)
