APP_STL := gnustl_static
NDK_TOOLCHAIN_VERSION=4.8
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1
APP_CPPFLAGS += -std=c++11
LOCAL_C_INCLUDES += ${NDK_ROOT}/sources/cxx-stl/gnu-libstdc++/4.8/include