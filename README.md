# EngineV2

A custom, high-performance 3D graphics engine written in C++ and OpenGL.

## Core Features

- **3D Graphics Pipeline**: Fully programmable vertex and fragment shaders executing perspective and view matrix transformations in real-time.
- **Hardware-Accelerated Asset Loading**: Seamlessly parses `.obj` 3D models (via `tinyobjloader`) and loads `.png`/`.jpg` textures (via `stb_image`) directly into GPU Vertex/Element Array Buffers and `sampler2D` textures.
- **Developer HUD**: Integrated `Dear ImGui` providing real-time engine controls, system metrics, and state monitoring without pausing execution.
- **Professional Performance Metrics**: Tracks frame distribution over a 1,000-frame rolling window, actively displaying the 99th and 99.9th percentile frame times (1% and 0.1% lows) alongside 100ms average FPS to diagnose thermal throttling or GC stutters.
- **First-Person Flight Camera**: Unlocked 6DOF camera (W/A/S/D/Space/Shift) supporting `GLFW_RAW_MOUSE_MOTION` hardware overrides to bypass OS cursor acceleration for zero-latency aiming.
- **Dynamic Viewports**: Real-time toggles to swap between dynamic window FOVs and forced cinematic 16:9 aspect ratios. 
- **Thermal Safety**: Dynamic V-Sync and swap interval toggles to prevent uncapped frame loops from triggering OS-level thermal throttling on modern silicon.
- **Data-Oriented Design (DOD)**: Strict adherence to DOD principles. Object-Oriented Programming (OOP) paradigms (like classes, inheritance, and encapsulation) are explicitly forbidden in favor of flat arrays, POD structs, and cache-friendly data transformation pipelines.
