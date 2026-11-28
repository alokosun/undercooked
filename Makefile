#ChatGPT generated and edited as needed

# ==========================================
# Compiler configuration
# ==========================================

HOST_CXX    ?= aarch64-linux-gnu-g++
LINUX_CXX   ?= g++
WINDOWS_CXX ?= x86_64-w64-mingw32-g++

# ==========================================
# Compiler flags
# ==========================================

CPPFLAGS = -Iinclude -Ishared

CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Windows requires Winsock library
WINDOWS_LIBS = -lws2_32 -static-libgcc -static-libstdc++

# ==========================================
# Source files
# ==========================================

# Files used by all three executables
COMMON_SRC = \
	shared/api.cpp \
	shared/GameState.cpp \
	shared/Item.cpp \
	shared/Map.cpp \
	shared/net-common.cpp \
	shared/net-platform.cpp \
	shared/Order.cpp \
	shared/packetmaker.cpp \
	shared/Player.cpp \
	shared/Station.cpp \
	shared/string_helpers.cpp

# Raspberry Pi host
HOST_SRC = \
    server/main.cpp

CLIENT_SRC = 

TEST_GEN_SRC = tests/test-main.cpp

# ==========================================
# Object files
# ==========================================

HOST_OBJ = $(patsubst %.cpp,build/host/%.o,$(COMMON_SRC) $(HOST_SRC))

LINUX_OBJ = $(patsubst %.cpp,build/linux/%.o,$(COMMON_SRC) $(CLIENT_SRC))

WINDOWS_OBJ = $(patsubst %.cpp,build/windows/%.o,$(COMMON_SRC) $(CLIENT_SRC))

TEST_GEN_OBJ = $(patsubst %.cpp,build/tests/%.o,$(COMMON_SRC) $(TEST_GEN_SRC))

# ==========================================
# Build targets
# ==========================================

.PHONY: all host linux windows clean

all: host linux windows

# Raspberry Pi host
host: build/host/host.out

build/host/host.out: $(HOST_OBJ)
	$(HOST_CXX) $^ -o $@

# Linux client
linux: build/linux/client.o

build/linux/client.o: $(LINUX_OBJ)
	$(LINUX_CXX) $^ -o $@

# Windows client
windows: build/windows/client.exe

build/windows/client.exe: $(WINDOWS_OBJ)
	$(WINDOWS_CXX) $^ -o $@ $(WINDOWS_LIBS)

test: build/tests/test-general.out
build/tests/test-general.out: $(TEST_GEN_OBJ)
	$(LINUX_CXX) $^ -o $@

# ==========================================
# Compilation rules
# ==========================================

build/host/%.o: %.cpp
	mkdir -p $(dir $@)
	$(HOST_CXX) $(CPPFLAGS) $(CXXFLAGS) -DHOST_BUILD -MMD -MP -c $< -o $@

build/linux/%.o: %.cpp
	mkdir -p $(dir $@)
	$(LINUX_CXX) $(CPPFLAGS) $(CXXFLAGS) -DCLIENT_BUILD -MMD -MP -c $< -o $@

build/windows/%.o: %.cpp
	mkdir -p $(dir $@)
	$(WINDOWS_CXX) $(CPPFLAGS) $(CXXFLAGS) -DCLIENT_BUILD -MMD -MP -c $< -o $@

build/tests/%.o: %.cpp
	mkdir -p $(dir $@)
	$(LINUX_CXX) $(CPPFLAGS) $(CXXFLAGS) -DTEST_BUILD -MMD -MP -c $< -o $@

# Automatically include generated header dependencies
-include $(HOST_OBJ:.o=.d)
-include $(CLIENT_OBJ:.o=.d)
-include $(TEST_GEN_OBJ:.o=.d)

# ==========================================
# Cleanup
# ==========================================

clean:
	rm -rf build