# Native Engine starter

A small C++20 engine foundation with a generational ECS registry, input state, a window interface, a deterministic headless window backend, a CPU framebuffer renderer, and a finite demo executable. The included backend is intentionally headless (no OS window or GPU API dependency); it provides a buildable base for adding SDL, GLFW, Vulkan, or another platform backend.

## Build and run

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/native_engine_demo
```

On Windows, run `build\\Debug\\native_engine_demo.exe` or the corresponding configuration directory. The demo runs for a few frames and reports a framebuffer checksum.
