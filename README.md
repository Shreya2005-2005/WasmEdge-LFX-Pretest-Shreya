# WasmEdge-LFX-Pretest-Shreya
Pre-test submission for LFX Mentorship Spring 2026 - WasmEdge JIT project.
## Personal Details
- **Name:** Shreya Bhakat
- **College:** Polaris School of Technology, Bengaluru
- **Target Project:** Enable JIT mode support for per-function compilation (Issue #4516)

## Environment & Build Setup
- **OS:** Windows Subsystem for Linux (WSL2) - Ubuntu
- **LLVM Version:** 15
- **Build Command:** `cmake -DCMAKE_BUILD_TYPE=Release -DWASMEDGE_USE_LLVM=ON ..`

## Troubleshooting the Build
During the compilation, the build failed at 75% with a `fatal error: lld/Common/Driver.h: No such file or directory`. 
I identified that the LLD development headers were missing. I resolved this by installing the matching library version:
`sudo apt install liblld-15-dev`
After clearing the CMake cache, the build resumed and reached 100% successfully.

## Verification of JIT Mode
After building, I verified the engine by running a sample WebAssembly file with the JIT flag enabled:
`./tools/wasmedge/wasmedge --enable-jit hello.wasm`

**Results:**
- **Execution:** Successfully printed "Hello WasmEdge!"
- **Version:** WasmEdge version 0.13.5 (as shown in the uploaded screenshots)

## Next Steps
My goal is to complete the per-function compilation task  by analyzing the trigger points in `lib/executor/executor.cpp`.
