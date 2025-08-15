#!/bin/bash

# Simple verification script to test the fixes
# This script verifies that hardcoded paths are fixed and basic build setup works

echo "🔍 Verifying SDL2 Roguelike Survivor fixes..."

# Test 1: Check for hardcoded paths
echo "1. Checking for hardcoded D:/ paths..."
HARDCODED_PATHS=$(grep -r "D:/" . --include="*.cpp" --include="*.h" | grep -v "comment\|#" | wc -l)
if [ "$HARDCODED_PATHS" -eq 0 ]; then
    echo "✅ No hardcoded D:/ paths found in source code"
else
    echo "❌ Found $HARDCODED_PATHS hardcoded D:/ paths"
    grep -r "D:/" . --include="*.cpp" --include="*.h" | grep -v "comment\|#"
fi

# Test 2: Check that assets exist
echo "2. Checking asset files..."
MISSING_ASSETS=""
for asset in "Asset/font/your_font.ttf" "Asset/player.png" "Asset/chaser.png" "Asset/brute.png"; do
    if [ ! -f "$asset" ]; then
        MISSING_ASSETS="$MISSING_ASSETS $asset"
    fi
done

# Check for arrow.PNG (case sensitive)
if [ ! -f "Asset/arrow.PNG" ] && [ ! -f "Asset/arrow.png" ]; then
    MISSING_ASSETS="$MISSING_ASSETS Asset/arrow.png"
fi

if [ -z "$MISSING_ASSETS" ]; then
    echo "✅ All essential assets found"
else
    echo "❌ Missing assets:$MISSING_ASSETS"
fi

# Test 3: Check CMake configuration
echo "3. Testing CMake configuration..."
mkdir -p test_build_verify
cd test_build_verify
if cmake .. >/dev/null 2>&1; then
    echo "✅ CMake configuration successful"
else
    echo "❌ CMake configuration failed"
fi
cd ..
rm -rf test_build_verify

# Test 4: Check build files exist
echo "4. Checking build configuration files..."
BUILD_FILES="CMakeLists.txt build_web.sh web_template.html README.md"
for file in $BUILD_FILES; do
    if [ ! -f "$file" ]; then
        echo "❌ Missing build file: $file"
    else
        echo "✅ Found: $file"
    fi
done

# Summary
echo ""
echo "🎮 Verification complete!"
echo ""
echo "The game should now:"
echo "  - Load assets using relative paths"
echo "  - Build for desktop with CMake"
echo "  - Build for web with Emscripten"
echo "  - Run in web browsers when served properly"