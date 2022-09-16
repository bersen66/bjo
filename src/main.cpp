#include <fmt/color.h>
#include <fmt/core.h>

#include "adder/adder.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    fmt::print(fmt::fg(fmt::color::red), "Invalid usage!\n");
    fmt::print(fmt::fg(fmt::color::green), "Usage: ");
    fmt::print("<op1> <op2>\n");
    return EXIT_FAILURE;
  }
  fmt::print(fmt::fg(fmt::color::green), "Result: {}\n",
             adder::Add(std::atoi(argv[1]), std::atoi(argv[2])));
  return EXIT_SUCCESS;
}
