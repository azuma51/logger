
BUILD_DIR   ?= ./build
INCLUDE_DIR ?= ./include
SRC_DIR     ?= ./src
TESTS_DIR   ?= ./tests

VPATH = ./$(SRC_DIR)
INCLUDES = -I$(SRC_DIR) -I$(INCLUDE_DIR)

# utest objs
UTESTS := $(shell find $(TESTS_DIR) -name u*.cpp)
GTESTS := $(shell find $(TESTS_DIR) -name g*.cpp)
#tests/foo.cpp => build/tests/foo.cpp.o
UTEST_OBJS := $(UTESTS:%=$(BUILD_DIR)/%.o)
GTEST_OBJS := $(GTESTS:%=$(BUILD_DIR)/%.o)
DEPS += $(UTEST_OBJS:.o=.d)
DEPS += $(GTEST_OBJS:.o=.d)


# googletest (c++11)
GTEST = -std=c++11 -I ~/raspi/gtest/include
L_GTEST = ~/raspi/gtest/lib/libgtest.a

# gtk
GTK = $(shell pkg-config --cflags gtk+-2.0)
L_GTK = $(shell pkg-config --libs gtk+-2.0)

# c++
CXXFLAGS = -Wall -MMD -MP $(GTEST)
LDLIBS = -lrt -lpthread $(L_GTEST) #$(L_GTK)


.PHONY: vars check utest clean

vars:
	@echo UTESTS=$(UTESTS)
	@echo GTESTS=$(GTESTS)
	@echo UTEST_OBJS=$(UTEST_OBJS)
	@echo GTEST_OBJS=$(GTEST_OBJS)
	@echo DEPS=$(DEPS)
	@echo DEFS=$(DEFS)
	@echo CXXFLAGS=$(CXXFLAGS)
	@echo INCLUDES=$(INCLUDES)
	@echo LDFLAGS=$(LDFLAGS)
	@echo LDLIBS=$(LDLIBS)


check: utest
	$(BUILD_DIR)/utest.out

utest: $(BUILD_DIR)/utest.out
$(BUILD_DIR)/utest.out: $(UTEST_OBJS) $(GTEST_OBJS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(UTEST_OBJS):
$(GTEST_OBJS): CXXFLAGS += $(GTK)


$(BUILD_DIR)/%.cpp.o:%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

