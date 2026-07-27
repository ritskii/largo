---
name: coding
description: "Use when: implementing, editing, or reviewing C++ code in this workspace. Follow the repo coding rules: no auto, C++17, no comments, no lambdas, no exceptions, and prefer unique_ptrs."
---

# Coding

Use this skill when working on C++ changes in this repository.

## Project rules

- Do not use `auto`
- Use C++17
- Do not add comments
- Do not use lambdas
- Do not use exceptions
- Prefer `std::unique_ptr`
- use camelCase for function
- use PascalCase for variable names and class names

## Working style

- Keep code explicit and straightforward
- Favor simple, direct control flow
- Prefer RAII and ownership patterns with `unique_ptr`
- Match the repository's CMake-based build setup
