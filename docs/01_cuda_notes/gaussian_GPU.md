# Gaussian Blur Performance Benchmark

This document tracks the scalability and processing throughput of a custom 1D separable Gaussian Blur filter, comparing a GPU CUDA implementation against a baseline CPU host implementation.

## 1. Environment Profiles

### GPU Environment (Google Colab)
* **Compute Device:** NVIDIA Tesla T4 (Turing Architecture)
* **VRAM:** 16 GB GDDR6

### CPU Environment (MacBook Pro)
* **Processor:** 2 GHz Quad-Core Intel Core i5
* **System Memory:** 16 GB 3733 MHz LPDDR4X
* **Operating System:** macOS

### Algorithmic Parameters
* **Kernel Execution Size ($K$):** 32
* **Data Format:** Single-Channel 32-bit Floating Point

---

## 2. Benchmark Metrics

### GPU CUDA Performance (NVIDIA T4)

| Image Dimensions | Total Pixels | Latency (ms) | Throughput (MPix/s) | Scaling Factor |
| :--- | :--- | :--- | :--- | :--- |
| **8 × 8** | 64 | 0.246 | 0.26 | Baseline (Overhead Bound) |
| **16 × 16** | 256 | 0.259 | 0.99 | 1.05x |
| **32 × 32** | 1,024 | 0.238 | 4.31 | 0.96x |
| **128 × 128** | 16,384 | 0.297 | 55.11 | 1.20x |
| **256 × 256** | 65,536 | 0.578 | 113.32 | 1.94x |
| **512 × 512** | 262,144 | 1.226 | 213.89 | 2.12x |
| **1024 × 1024** | 1,048,576 | 3.743 | 280.16 | 3.05x |
| **2048 × 2048** | 4,194,304 | 13.725 | 305.61 | 3.66x (Peak Throughput) |
| **4096 × 4096** | 16,777,216 | 84.021 | 199.68 | 6.12x (Efficiency Drop) |

### CPU Performance (Intel Core i5)

| Image Dimensions | Total Pixels | Latency (ms) | Throughput (MPix/s) | Scaling Factor |
| :--- | :--- | :--- | :--- | :--- |
| **8 × 8** | 64 | 0.009 | 7.20 | Baseline |
| **16 × 16** | 256 | 0.019 | 13.25 | 2.11x |
| **32 × 32** | 1,024 | 0.056 | 18.43 | 6.22x |
| **128 × 128** | 16,384 | 0.842 | 19.47 | 93.55x |
| **256 × 256** | 65,536 | 3.372 | 19.43 | 374.66x |
| **512 × 512** | 262,144 | 13.031 | 20.12 | 1,447.88x |
| **1024 × 1024** | 1,048,576 | 52.018 | 20.16 | 5,779.77x |
| **2048 × 2048** | 4,194,304 | 212.290 | 19.76 | 23,587.77x |
| **4096 × 4096** | 16,777,216 | 935.893 | 17.93 | 103,988.11x |

---

## 3. Core Insights & Engineering Inferences

* **Kernel Launch Overhead:** Below $128 \times 128$, GPU processing latency is completely flat (`~0.25 ms`). This shows that the API driver and hardware configuration overhead limits. For small frame structures, the CPU remains orders of magnitude more efficient.
* **CPU Saturation Boundary:** The CPU hits an absolute physical wall at `~20 MPix/s` starting from an image size of $32 \times 32$. This indicates a single-threaded execution context that is entirely bound by host cache lines or system memory bus performance.
* **GPU Saturation and Memory Degradation:** The GPU reaches maximum scaling optimization at $2048 \times 2048$, processing at **305.61 MPix/s** (a **15.4x** performance gain over the host CPU). However, at $4096 \times 4096$, processing efficiency falls by **34.6%**. This performance degradation indicates memory bottlenecks or hardware multi-processor sub-optimization at massive block allocations.

## 4. Next-Step Optimizations

To look into the performance at $4096 \times 4096$, the following steps could be performed:
1. **Eliminate Shared Memory Bank Conflicts:** Adjust the inner padding matrix indices to align with 32-bank warp allocations.
2. **Utilize Constant Memory (`__constant__`):** Transfer the explicit global filter vector pointer to the chip's internal L1-speed constant memory cache. At the moment gaussian kernel filter value is being used through the kernel instantiation, it could be made constant.
3. **Incorporate Asynchronous Streams:** Enable non-blocking, pipelined data copy transactions across the PCIe bus. This could be done by moving away from serialized cudaMemcpy to cudaMemcpyAsync and non-default cudastream_t instances.