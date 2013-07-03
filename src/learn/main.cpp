#include <exception>
#include <iostream>
#include <fstream>

#include <boost/format.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

#include "common/types.h"
#include "cli.h"
#include "downloader.h"

using boost::format;

template<typename S, typename F>
S& resolve_stream(S& default_stream, 
		  boost::optional<F>& file,
		  OptionalString const& path)
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
    // input path
    OptionalString pin;
    Timeout timeout;
    // output path
    OptionalString pout;
    // chain order
    ChainOrder order;
    bool verbose;
    bool strict;

    int result = parse(argc, argv, pin, timeout, pout, order, verbose, strict);

    if (result != 0) {
      return result;
    }

    if (verbose) {
      std::cerr << "Options:\n"
		<< format("  input from '%1%'\n") % (pin ? pin.get() : "stdin")
		<< format("  timeout is %1%\n") % timeout
		<< format("  output to '%1%'\n") % (pout ? pout.get() : "stdout")
		<< format("  chain order is %1%\n") % static_cast<int>(order)
		<< format("  verbose? %1%\n") % (verbose ? "yes" : "no")
		<< format("  strict? %1%\n") % (strict ? "yes" : "no")
		<< std::flush;
    }

    boost::optional<std::ifstream> fin;
    boost::optional<std::ofstream> fout;
    std::istream& in = resolve_stream(std::cin, fin, pin);
    std::ostream& out = resolve_stream(std::cout, fout, pout);
    Url url;
    while(std::getline(in, url)) {
      std::cout << boost::format("Url is %1%") % url << std::endl;
      Downloader d(url, timeout, verbose);
      std::cout << "Count result" << std::endl;
      std::string dummy;
      int count = 0;
      std::istream& s = d.data();
      while(std::getline(s, dummy)) {
	++count;
      }
      std::cout << format("Total %1% lines\n") % count  << std::flush;
      d.join();
    }

    return 0;
  }
  catch (std::exception& e) {
    std::cerr << format("Error: %1%") % e.what() << std::endl;
    return 1;
  }
}
