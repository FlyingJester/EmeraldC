import os

compiler_files = ["compiler.cpp", "error.cpp", "io.cpp", "term_type.cpp", "math_expression.cpp", 
  "boolean_expression.cpp", "relation.cpp", "asm.cpp", "program.cpp", "integral_types.cpp", 
  "declaration.cpp", "CPU.cpp"]

CPUs = ["MIPS", "MIPS_opt", "amd64", "amd64_opt"]
CPU_use = []

for cpu in CPUs:
    compiler_files += [os.path.join(cpu, "CPU.cpp"),
      os.path.join(cpu, "optimizer.cpp")]
    CPU_use += ["TARGET_"+cpu]
    

# Unused parameters are fine for keeping the interface consistent.
compiler = Program(compiler_files, 
    CXXFLAGS = " -std=c++11 ", 
    CCFLAGS = " -g -Os -Werror -Wall -Wextra -pedantic -Wno-unused-parameter ",
    CPPDEFINES = CPU_use)
