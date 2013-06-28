#include <iostream>

#include "common/types.h"
#include "cli.h"

int main(int argc, char* argv[])
{
  try {
    optional_string in;
    optional_string out;
    bool verbose;
    bool strict;
    chain_order_t order=5;
    int result = parse(argc, argv, std::cout, std::cerr, in, out, verbose, strict, order);
    if (result == 0) {
      std::cout << "result=" << result << std::endl;
      std::cout << "in=" << (in ? in.get() : "stdin") << std::endl;
      std::cout << "out=" << (out ? out.get() : "stdout") << std::endl;
      std::cout << "verbose=" << verbose << std::endl;
      std::cout << "strict=" << strict << std::endl;
      std::cout << "order=" << (int)order << std::endl;
    }
    return result;
  }
  catch (std::exception& e) {
    stderr << format("Error: %1%") % e.what() << std::endl;
    return 1;
  }
}
