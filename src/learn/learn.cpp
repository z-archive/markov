#include <exception>
#include <iostream>
#include <fstream>

#include <boost/format.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

#include "common/types.h"
#include "cli.h"

using boost::format;

template<typename S, typename F>
S& resolve_stream(S& default_stream, 
		  boost::optional<F>& file,
		  optional_string const& path)
{
  if (!path) {
    return default_stream;
  }
  file = boost::in_place(path.get());
  return file.get();
}
		    

int main(int argc, char* argv[])
{
  try {
    optional_string path_in;
    optional_string path_out;
    bool verbose;
    bool strict;
    chain_order_t order;

    int result = parse(argc, argv, path_in, path_out, verbose, strict, order);

    if (result != 0) {
      return result;
    }

    boost::optional<std::ifstream> fin;
    boost::optional<std::ofstream> fout;
    std::istream& in = resolve_stream(std::cin, fin, path_in);
    std::ostream& out = resolve_stream(std::cout, fout, path_out);
    
    if (result == 0) {
      std::cout << "result=" << result << std::endl;
      std::cout << "in=" << (path_in ? path_in.get() : "stdin") << std::endl;
      std::cout << "out=" << (path_out ? path_out.get() : "stdout") << std::endl;
      std::cout << "verbose=" << verbose << std::endl;
      std::cout << "strict=" << strict << std::endl;
      std::cout << "order=" << (int)order << std::endl;
    }
    return result;
  }
  catch (std::exception& e) {
    std::cerr << format("Error: %1%") % e.what() << std::endl;
    return 1;
  }
}
