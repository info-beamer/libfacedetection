#!/bin/sh
rm -rf build overlay
mkdir -p build overlay/bin
cd build && \
  cmake -DCMAKE_INSTALL_PREFIX=install -DUSE_OPENMP=OFF -DENABLE_NEON=ON -DCMAKE_BUILD_TYPE=RELEASE -DDEMO=OFF -DENABLE_AVX2=OFF -DCMAKE_TOOLCHAIN_FILE=../pi.cmake .. && \
  cmake --build . --config Release && \
  cmake --build . --config Release --target install
cd ..
arm-linux-gnueabihf-g++ face_detect.cpp -Ibuild/install/include/facedetection -o overlay/bin/face_detect build/install/lib/libfacedetection.a --static -O3 -s
mksquashfs overlay ../overlay.squashfs -comp xz -all-root -noappend -no-xattrs -no-recovery -Xbcj arm -b 1048576
