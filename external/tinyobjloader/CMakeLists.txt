cmake_minimum_required(VERSION 3.0)
project(tiny_obj_loader)

add_library(tiny_obj_loader src/tiny_obj_loader.cc)
target_include_directories(tiny_obj_loader PUBLIC include)