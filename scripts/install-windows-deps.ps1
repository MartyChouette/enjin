# PowerShell script to check and install Windows dependencies
# Run: powershell -ExecutionPolicy Bypass -File .\scripts\install-windows-deps.ps1

Write-Host "=== Enjin Engine Windows Dependency Checker ===" -ForegroundColor Cyan
Write-Host ""

# Check if running as Administrator
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Host "⚠️  Note: Some installations may require Administrator privileges" -ForegroundColor Yellow
    Write-Host ""
}

# Function to check if command exists
function Test-Command {
    param($Command)
    $null = Get-Command $Command -ErrorAction SilentlyContinue
    return $?
}

# Function to check version
function Get-Version {
    param($Command, $VersionArg)
    try {
        $version = & $Command $VersionArg 2>&1 | Select-Object -First 1
        return $version
    } catch {
        return $null
    }
}

# 1. Check CMake
Write-Host "1. Checking CMake..." -ForegroundColor Yellow
if (Test-Command "cmake") {
    $cmakeVersion = Get-Version "cmake" "--version"
    Write-Host "   ✅ CMake found: $cmakeVersion" -ForegroundColor Green
    
    # Check if version is 3.20+
    $versionMatch = $cmakeVersion -match "version (\d+)\.(\d+)"
    if ($versionMatch) {
        $major = [int]$matches[1]
        $minor = [int]$matches[2]
        if ($major -gt 3 -or ($major -eq 3 -and $minor -ge 20)) {
            Write-Host "   ✅ Version 3.20+ detected" -ForegroundColor Green
        } else {
            Write-Host "   ⚠️  Version $major.$minor is below 3.20" -ForegroundColor Yellow
            Write-Host "   Install latest: winget install Kitware.CMake" -ForegroundColor Cyan
        }
    }
} else {
    Write-Host "   ❌ CMake not found" -ForegroundColor Red
    Write-Host ""
    Write-Host "   Installing CMake..." -ForegroundColor Cyan
    
    # Try winget first
    if (Test-Command "winget") {
        Write-Host "   Using winget..." -ForegroundColor Gray
        winget install Kitware.CMake --accept-package-agreements --accept-source-agreements
    }
    # Try chocolatey
    elseif (Test-Command "choco") {
        Write-Host "   Using Chocolatey..." -ForegroundColor Gray
        choco install cmake -y
    }
    # Manual download
    else {
        Write-Host "   ⚠️  winget or chocolatey not found" -ForegroundColor Yellow
        Write-Host "   Please install CMake manually:" -ForegroundColor Yellow
        Write-Host "   1. Download from: https://cmake.org/download/" -ForegroundColor Cyan
        Write-Host "   2. Run installer" -ForegroundColor Cyan
        Write-Host "   3. Check 'Add CMake to system PATH' during installation" -ForegroundColor Cyan
    }
}

Write-Host ""

# 2. Check Vulkan SDK
Write-Host "2. Checking Vulkan SDK..." -ForegroundColor Yellow
$vulkanFound = $false

# Check environment variable
if ($env:VULKAN_SDK) {
    Write-Host "   ✅ VULKAN_SDK environment variable: $env:VULKAN_SDK" -ForegroundColor Green
    $vulkanFound = $true
}

# Check if vulkaninfo exists
if (Test-Command "vulkaninfo") {
    Write-Host "   ✅ vulkaninfo found" -ForegroundColor Green
    try {
        $vulkanInfo = & vulkaninfo --summary 2>&1 | Select-Object -First 5
        Write-Host "   Vulkan Info:" -ForegroundColor Gray
        $vulkanInfo | ForEach-Object { Write-Host "      $_" -ForegroundColor Gray }
        $vulkanFound = $true
    } catch {
        Write-Host "   ⚠️  vulkaninfo exists but may not be working" -ForegroundColor Yellow
    }
}

if (-not $vulkanFound) {
    Write-Host "   ❌ Vulkan SDK not found" -ForegroundColor Red
    Write-Host ""
    Write-Host "   Installing Vulkan SDK..." -ForegroundColor Cyan
    
    # Try winget
    if (Test-Command "winget") {
        Write-Host "   Using winget..." -ForegroundColor Gray
        Write-Host "   ⚠️  Note: Vulkan SDK installer will run separately" -ForegroundColor Yellow
        winget install VulkanSDK --accept-package-agreements --accept-source-agreements
    }
    # Manual download
    else {
        Write-Host "   ⚠️  Please install Vulkan SDK manually:" -ForegroundColor Yellow
        Write-Host "   1. Download from: https://vulkan.lunarg.com/sdk/home" -ForegroundColor Cyan
        Write-Host "   2. Run installer (default location: C:\VulkanSDK\<version>)" -ForegroundColor Cyan
        Write-Host "   3. Restart PowerShell after installation" -ForegroundColor Cyan
    }
}

Write-Host ""

# 3. Check GLFW3 (via vcpkg)
Write-Host "3. Checking GLFW3..." -ForegroundColor Yellow

# Check if vcpkg exists
$vcpkgPath = $null
$possibleVcpkgPaths = @(
    "$env:USERPROFILE\vcpkg",
    "$env:USERPROFILE\source\vcpkg",
    "C:\vcpkg",
    "C:\tools\vcpkg"
)

foreach ($path in $possibleVcpkgPaths) {
    if (Test-Path "$path\vcpkg.exe") {
        $vcpkgPath = $path
        break
    }
}

if ($vcpkgPath) {
    Write-Host "   ✅ vcpkg found at: $vcpkgPath" -ForegroundColor Green
    
    # Check if GLFW3 is installed
    $glfwInstalled = & "$vcpkgPath\vcpkg.exe" list glfw3 2>&1
    if ($glfwInstalled -match "glfw3") {
        Write-Host "   ✅ GLFW3 is installed via vcpkg" -ForegroundColor Green
        Write-Host "      $glfwInstalled" -ForegroundColor Gray
    } else {
        Write-Host "   ⚠️  GLFW3 not installed via vcpkg" -ForegroundColor Yellow
        Write-Host ""
        Write-Host "   Installing GLFW3..." -ForegroundColor Cyan
        & "$vcpkgPath\vcpkg.exe" install glfw3:x64-windows
    }
} else {
    Write-Host "   ❌ vcpkg not found" -ForegroundColor Red
    Write-Host ""
    Write-Host "   Setting up vcpkg..." -ForegroundColor Cyan
    
    # Ask user where to install
    $installPath = "$env:USERPROFILE\vcpkg"
    Write-Host "   Installing vcpkg to: $installPath" -ForegroundColor Gray
    
    # Clone vcpkg
    if (Test-Command "git") {
        Write-Host "   Cloning vcpkg repository..." -ForegroundColor Gray
        git clone https://github.com/Microsoft/vcpkg.git $installPath
        
        # Bootstrap
        Write-Host "   Bootstrapping vcpkg..." -ForegroundColor Gray
        Push-Location $installPath
        .\bootstrap-vcpkg.bat
        Pop-Location
        
        # Install GLFW3
        Write-Host "   Installing GLFW3..." -ForegroundColor Gray
        & "$installPath\vcpkg.exe" install glfw3:x64-windows
        
        Write-Host "   ✅ vcpkg setup complete!" -ForegroundColor Green
        Write-Host "   Remember to use: -DCMAKE_TOOLCHAIN_FILE=$installPath\scripts\buildsystems\vcpkg.cmake" -ForegroundColor Cyan
    } else {
        Write-Host "   ❌ git not found" -ForegroundColor Red
        Write-Host "   Please install Git first:" -ForegroundColor Yellow
        Write-Host "   winget install Git.Git" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "   Or install vcpkg manually:" -ForegroundColor Yellow
        Write-Host "   1. Download from: https://github.com/Microsoft/vcpkg/archive/refs/heads/master.zip" -ForegroundColor Cyan
        Write-Host "   2. Extract to desired location" -ForegroundColor Cyan
        Write-Host "   3. Run bootstrap-vcpkg.bat" -ForegroundColor Cyan
        Write-Host "   4. Run: vcpkg install glfw3:x64-windows" -ForegroundColor Cyan
    }
}

Write-Host ""
Write-Host "=== Summary ===" -ForegroundColor Cyan

# Final checks
$allGood = $true

if (-not (Test-Command "cmake")) {
    Write-Host "❌ CMake: NOT INSTALLED" -ForegroundColor Red
    $allGood = $false
} else {
    Write-Host "✅ CMake: INSTALLED" -ForegroundColor Green
}

if (-not (Test-Command "vulkaninfo") -and -not $env:VULKAN_SDK) {
    Write-Host "❌ Vulkan SDK: NOT INSTALLED" -ForegroundColor Red
    $allGood = $false
} else {
    Write-Host "✅ Vulkan SDK: INSTALLED" -ForegroundColor Green
}

if (-not $vcpkgPath) {
    Write-Host "❌ vcpkg/GLFW3: NOT INSTALLED" -ForegroundColor Red
    $allGood = $false
} else {
    Write-Host "✅ vcpkg/GLFW3: INSTALLED" -ForegroundColor Green
}

Write-Host ""

if ($allGood) {
    Write-Host "✅ All dependencies are installed!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Cyan
    Write-Host "1. Open 'x64 Native Tools Command Prompt for VS 2019'" -ForegroundColor White
    Write-Host "2. cd to your project directory" -ForegroundColor White
    Write-Host "3. mkdir build && cd build" -ForegroundColor White
    if ($vcpkgPath) {
        Write-Host "4. cmake .. -G `"Visual Studio 16 2019`" -A x64 -DCMAKE_TOOLCHAIN_FILE=$vcpkgPath\scripts\buildsystems\vcpkg.cmake" -ForegroundColor White
    } else {
        Write-Host "4. cmake .. -G `"Visual Studio 16 2019`" -A x64" -ForegroundColor White
    }
    Write-Host "5. cmake --build . --config Release" -ForegroundColor White
} else {
    Write-Host "⚠️  Some dependencies are missing. Please install them above." -ForegroundColor Yellow
}

Write-Host ""
