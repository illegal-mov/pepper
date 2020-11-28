# Based on a Makefile from Job Vranish (2016)
TARGET_EXEC := pepper.bin
TARGET_LIB := libpepper.so

TEST_DIR := ./test
BUILD_DIR := ./build~
SRC_DIR := ./src
INC_DIR := ./include/headers/

MKDIR_P := mkdir -p

CFLAGS := -D _DEFAULT_SOURCE -std=c11 -Wall -Wextra -Werror -O0 -I $(INC_DIR)
CPPFLAGS := -MMD -MP -fpic -std=c++17 -Wall -Weffc++ -Wextra -Wsign-conversion -Werror -I $(INC_DIR)

MAIN := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
TEST := $(shell find $(TEST_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS_MAIN := $(MAIN:%=$(BUILD_DIR)/%.o)
OBJS_TEST := $(TEST:%=$(BUILD_DIR)/%.o)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: release
release: $(OBJS_MAIN)
	$(MKDIR_P) $(BUILD_DIR)/$@
	$(CXX) -shared -fpic -s -O2 $? -o $(BUILD_DIR)/$@/$(TARGET_LIB)

.PHONY: debug
debug: $(OBJS_MAIN)
	$(MKDIR_P) $(BUILD_DIR)/$@
	$(CXX) -shared -fpic -g $? -o $(BUILD_DIR)/$@/$(TARGET_LIB)

.PHONY: test
test: $(OBJS_MAIN) $(OBJS_TEST)
	$(MKDIR_P) $(BUILD_DIR)/$@
	$(CXX) $? -o $(BUILD_DIR)/$@/$(TARGET_EXEC)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

