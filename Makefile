APP := cancom
LIBNAME := lib$(APP).so
INSTALL_SCRIPT := libinstall.sh
UNINSTALL_SCRIPT := libuninstall.sh
START_CAN_SCRIPT := startcan.sh
ROOT:=/home/rarakers
NDK_PLATFORM_VER := 4
INSTALL_DIR := /data/local
 
ANDROID_NDK_ROOT:=$(ROOT)/android-ndk-r6b
ANDROID_NDK_HOST:=linux-x86
ANDROID_SDK_ROOT:=$(ROOT)/android-sdk-linux_x86
PREBUILD:=$(ROOT)/myandroid/prebuilt/$(ANDROID_NDK_HOST)/toolchain/arm-eabi-4.4.3
BIN := $(PREBUILD)/bin
 
CPP := $(BIN)/arm-eabi-g++
CC := $(BIN)/arm-eabi-gcc
CFLAGS := -I$(ANDROID_NDK_ROOT)/platforms/android-$(NDK_PLATFORM_VER)/arch-arm/usr/include -I$(ROOT)/myandroid/kernel_imx/include
LDFLAGS := -Wl,--entry=main,-rpath-link=$(ANDROID_NDK_ROOT)/platforms/android-$(NDK_PLATFORM_VER)/arch-arm/usr/lib,-dynamic-linker=/system/bin/linker -L$(ANDROID_NDK_ROOT)/platforms/android-$(NDK_PLATFORM_VER)/arch-arm/usr/lib
LDFLAGS += -nostdlib -lc -disable-multilib
 
all: $(APP)
 
OBJS += obj/$(APP).o

$(APP): $(OBJS)
	$(CPP) -shared $(LDFLAGS) -o out/$(LIBNAME) $^
 
obj/%.o: src/%.c
	$(CC) -c -fPIC $(INCLUDE) $(CFLAGS) $< -o $@

install: $(APP)
	$(ANDROID_SDK_ROOT)/platform-tools/adb push out/$(LIBNAME) $(INSTALL_DIR)/$(LIBNAME)
	$(ANDROID_SDK_ROOT)/platform-tools/adb push $(INSTALL_SCRIPT) $(INSTALL_DIR)/$(INSTALL_SCRIPT)
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/$(INSTALL_SCRIPT)
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell $(INSTALL_DIR)/$(INSTALL_SCRIPT)
	$(ANDROID_SDK_ROOT)/platform-tools/adb push $(START_CAN_SCRIPT) $(INSTALL_DIR)/$(START_CAN_SCRIPT)
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/$(START_CAN_SCRIPT)

uninstall: $(APP)
	$(ANDROID_SDK_ROOT)/platform-tools/adb push $(UNINSTALL_SCRIPT) $(INSTALL_DIR)/$(UNINSTALL_SCRIPT)
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/$(UNINSTALL_SCRIPT)
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell $(INSTALL_DIR)/$(UNINSTALL_SCRIPT)

test: testlib
	$(CC) -c $(INCLUDE) -rdynamic $(CFLAGS) testlib/main.c -o testlib/main.o
	$(CPP) -ldl $(LDFLAGS) -rdynamic -o testlib/testlib testlib/main.o

installtest: $(APP)
	$(ANDROID_SDK_ROOT)/platform-tools/adb push testlib/testlib $(INSTALL_DIR)/testlib
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/testlib
	
clean:
	@rm -f obj/*.o out/$(LIBNAME)
 

