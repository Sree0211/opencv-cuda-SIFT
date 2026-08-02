# Day 02 — CUDA prototype plan

## Objective

Create a small, independently testable CUDA prototype for one SIFT stage before integrating anything into OpenCV.

## Scope for today

1. Set up a dedicated CUDA prototype area under the SIFT module.
2. Build a minimal Gaussian-buffer copy kernel as a smoke test.
3. Validate GPU launch, synchronization, host copy, and result checking.
4. Keep the CPU pipeline unchanged until the prototype is proven.

## Proposed structure

- sift/cuda/include/gaussian_cuda.hpp
- sift/cuda/src/gaussian_cuda.cu
- sift/cuda/tests/gaussian_cuda_smoke.cpp
- sift/cuda/CMakeLists.txt
