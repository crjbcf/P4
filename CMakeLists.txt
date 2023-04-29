cmake_minimum_required(VERSION 3.25)
project(P4)

set(CMAKE_CXX_STANDARD 14)

add_executable(P4 main.cpp parser.cpp parser.h token.h scanner.cpp scanner.h staticSem.cpp staticSem.h node.h treePrint.cpp treePrint.h codeGen.cpp codeGen.h)
