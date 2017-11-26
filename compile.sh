export STAGING_DIR=/home/julian/build/LEDE/source/staging_dir
export BUILD_DIR=$STAGING_DIR
export TOOLCHAIN_DIR=$BUILD_DIR/toolchain-mipsel_24kc_gcc-5.5.0_musl
export LDFLAGS=$TOOLCHAIN_DIR/lib
export LD_LIBRARY_PATH=$TOOLCHAIN_DIR/lib
export PATH=$TOOLCHAIN_DIR/bin:$PATH
export CPATH=$TOOLCHAIN_DIR/include:$CPATH

mipsel-openwrt-linux-g++ sensorRead.cpp functionLogging.cpp sensorLogging.cpp state.cpp -o sensorLogging.o -std=c++11
mipsel-openwrt-linux-g++ aggregateTime.cpp functionLogging.cpp state.cpp -o aggregateTime.o -std=c++11
mipsel-openwrt-linux-g++ buttonRead.cpp sensorRead.cpp functionLogging.cpp state.cpp -o buttonRead.o -std=c++11
