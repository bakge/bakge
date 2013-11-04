# Convenience makefile

DEBUG?=ON
TARGET=build
SDK=sdk
GDK=gdk

BUILD_DYNAMIC?=OFF
BUILD_TESTS?=OFF
BUILD_EXAMPLES?=OFF
BUILD_ENGINE?=OFF

export BUILD_DYNAMIC BUILD_TESTS BUILD_EXAMPLES

GLFW_OPTIONS=-DGLFW_USE_EGL=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF

BAKGE_OPTIONS=-DBAKGE_SDK_PATH=$(SDK) -DBAKGE_BUILD_TESTS=$(BUILD_TESTS) -DBAKGE_BUILD_EXAMPLES=$(BUILD_EXAMPLES) -DBUILD_SHARED_LIBS=$(BUILD_DYNAMIC) -DBAKGE_GDK_BUILD_ENGINE=$(BUILD_ENGINE)

BULLET_OPTIONS=-DBUILD_EXTRAS=OFF -DBUILD_DEMOS=OFF

# All options
OPTIONS=$(GLFW_OPTIONS) $(BAKGE_OPTIONS) $(BULLET_OPTIONS)

help:
	@echo ""
	@echo "Bakge Makefile targets"
	@echo "======================"
	@echo " - all: Builds Debug & Release"
	@echo " - debug: Build a debug configuration"
	@echo " - release: Build a release configuration"
	@echo " - sdk: Build the full SDK"
	@echo " - clean: Delete all of Bakge's generated files"
	@echo ""
	@echo "Bakge Makefile options"
	@echo "======================"
	@echo " - Usage: $$ make <OPTION>=ON/OFF <TARGET>"
	@echo " - BUILD_DYNAMIC: Build shared libraries. Default: OFF"
	@echo " - BUILD_TESTS: Build Bakge's test suite. Default: ON"
	@echo " - BUILD_ExAMPLES: Build Bakge's example programs. Default: ON"
	@echo ""

all:
	@$(MAKE) -s debug release;

debug:
	@mkdir -p $(TARGET) && cd $(TARGET) && cmake .. -G "Unix Makefiles" $(OPTIONS) -DCMAKE_BUILD_TYPE=Debug && make -s;

release:
	@mkdir -p $(TARGET) && cd $(TARGET) && cmake .. -G "Unix Makefiles" $(OPTIONS) -DCMAKE_BUILD_TYPE=Release && make -s;

sdk:
	@make -s BUILD_DYNAMIC=ON BUILD_TESTS=OFF all;
	@make -s BUILD_TESTS=OFF BUILD_EXAMPLES=OFF all;

codeblocks:
	@mkdir -p $(TARGET) && cd $(TARGET) && cmake .. -G "CodeBlocks - Unix Makefiles" $(OPTIONS);

clean:
	rm -rf $(TARGET) $(SDK) $(GDK);

