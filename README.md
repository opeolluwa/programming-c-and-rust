# 300-Day Plan to Become a Rust/C++ Software Engineer

**Goal**: Become job-ready as a systems programmer using **Rust** and **C++**, with a focus on backend, performance, and low-level systems.

**Your Background**: You know **Rust**. This plan accelerates **C++ mastery** while deepening Rust and building dual-language projects.

---

## Daily Commitment
- **Weekdays**: 1–2 hours (focused coding/learning)
- **Weekends**: 3–4 hours (projects, review)
- **Tools**: VS Code / CLion, `g++`, `rustc`, GitHub, LeetCode
- **Track Progress**: Notion, Trello, or a simple journal

---

## Resources
| Language | Books & References |
|--------|-------------------|
| **C++** | *C++ Primer* (Lippman), *Effective Modern C++* (Meyers), [cppreference.com](https://cppreference.com) |
| **Rust** | *The Rust Book* (free), *Rust for Rustaceans* |
| **Practice** | LeetCode, HackerRank, Codewars (solve in both languages) |

---

## 10-Phase Plan (30 Days Each)

| Phase | Days | Focus | Key Tasks & Mini-Project |
|-------|------|-------|--------------------------|
| **1: C++ Fundamentals** | 1–30 | Syntax, pointers, memory | Todo list CLI |
| **2: C++ OOP** | 31–60 | Classes, inheritance, polymorphism | Bank account system |
| **3: STL Mastery** | 61–90 | Containers, algorithms | Graph with adjacency list |
| **4: Advanced C++** | 91–120 | Smart pointers, templates, RAII | Custom smart pointer |
| **5: Advanced Rust + Interop** | 121–150 | Concurrency, FFI, unsafe | Port C++ project to Rust |
| **6: DSA in Both Languages** | 151–180 | Arrays → Graphs | Pathfinding (Dijkstra) |
| **7: Advanced DSA** | 181–210 | DP, greedy, optimization | LeetCode hard solves |
| **8: Real Projects** | 211–240 | CLI tool, web server, game | 3 polished GitHub repos |
| **9: System Design & Soft Skills** | 241–270 | Scalability, resume, open-source | Contribute to OSS |
| **10: Interview Prep** | 271–300 | Mock interviews, 200+ LC | Apply to jobs |

> **Milestone**: By Day 300 → 3–5 portfolio projects, 300+ LeetCode solves, interview-ready.

---

# C++ Backend Project Ideas (for Systems Engineers)

You have **backend experience** → use C++ to build **high-performance, low-latency, production-grade services**.

---

## 1. High-Performance HTTP / gRPC Server
- **Libs**: `Boost.Beast`, `cpp-httplib`, `gRPC`
- **Features**: Async I/O, TLS, HTTP/2, streaming RPC
- **Benchmark**: `wrk`, `hey` vs Node.js/nginx

---

## 2. Distributed Key-Value Store (etcd/Redis-like)
- **Consensus**: Raft (use `cpp-raft` or implement)
- **Storage**: WAL, memory-mapped files
- **API**: gRPC + CLI client

---

## 3. Message Queue Broker (Kafka-style)
- **Zero-copy**: Ring buffers, `mmap`
- **Lock-free**: `std::atomic`, `moodycamel::ConcurrentQueue`
- **Transport**: ZeroMQ or raw AF_XDP

---

## 4. Real-Time Stream Processor (Flink mini)
- **Pipeline**: Source → Window → Aggregate → Sink
- **Parallelism**: `cppcoro`, `std::jthread`, SIMD

---

## 5. Custom Load Balancer / Reverse Proxy
- **I/O**: `io_uring`, `epoll`, `SO_REUSEPORT`
- **L7 Routing**: Host/path, rate-limiting

---

## 6. Database Proxy (MySQL/PostgreSQL Router)
- **Protocol**: MySQL wire parsing
- **Routing**: Read/write split, connection pooling

---

## 7. IoT / Edge Gateway
- **Protocols**: MQTT, CoAP, HTTP
- **Security**: Mbed TLS, OTA updates

---

## 8. HFT Matching Engine
- **Order Book**: Lock-free, cache-aligned
- **Protocol**: FIX/ITCH binary
- **Timing**: `__rdtsc`, nanosecond precision

---

## 9. Authoritative Game Server
- **Net**: ENet, yojimbo (reliable UDP)
- **Sync**: Snapshot interpolation, anti-cheat

---

## 10. Systems Tools (`curl`, `grep`, `tar`)
- **CLI**: `Boost.Program_options`, `fmt`
- **SIMD**: `std::simd`, parallel regex

---

# Quick-Start C++20 HTTP Server (Boost.Beast)

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(BackendCpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
find_package(Threads REQUIRED)
find_package(Boost 1.82 REQUIRED COMPONENTS system beast)
add_executable(server src/main.cpp)
target_link_libraries(server PRIVATE Threads::Threads Boost::beast)
