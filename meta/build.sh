#!/bin/bash

BUILD_DIRECTORY="build/cmake"
VCPKG_DIR="build/toolchain/vcpkg"

ensure_vcpkg_installation() {
    if [ ! -d "$VCPKG_DIR" ]; then
        echo "Installing vcpkg in $VCPKG_DIR..."
        git clone https://github.com/Microsoft/vcpkg.git "$VCPKG_DIR"
        (cd "$VCPKG_DIR" && ./bootstrap-vcpkg.sh -disableMetrics)
    fi
}

mkdir -p "$BUILD_DIRECTORY"

ensure_vcpkg_installation

cmake -B "$BUILD_DIRECTORY" -S . -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build "$BUILD_DIRECTORY"
