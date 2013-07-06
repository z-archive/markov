#include <exception>
#include <iostream>
#include <fstream>

#include <boost/format.hpp>
#include <boost/utility/typed_in_place_factory.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "cli.h"
#include "downloader.h"
#include "parser.h"

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
        settings::Learn s;
        int result = parse(argc, argv, s);

        if (result != 0) {
            return result;
        }

        if (s.verbose) {
            std::cerr << "Options:\n"
                      << format("  input from '%1%'\n") % (s.in ? s.in.get() : "stdin")
                      << format("  timeout is %1%\n") % s.timeout
                      << format("  output to '%1%'\n") % (s.out ? s.out.get() : "stdout")
                      << format("  chain order is %1%\n") % s.order
                      << format("  max word length %1%\n") % s.max_word_length
                      << format("  verbose? %1%\n") % (s.verbose ? "yes" : "no")
                      << format("  strict? %1%\n") % (s.strict ? "yes" : "no")
                      << std::flush;
        }

        boost::optional<std::ifstream> fin;
        boost::optional<std::ofstream> fout;
        std::istream& in = resolve_stream(std::cin, fin, s.in);
        std::ostream& out = resolve_stream(std::cout, fout, s.out);
        Url url;
        while(std::getline(in, url)) {
            Downloader d(url, s.timeout, s.verbose);
            Parser p(d, 100, 10);

            while(p.next())
            {
                if (p.delimeter())
                {
                    std::cout << "DELIMETER" << std::endl;
                }
                else
                {
                    std::cout << format("WORD '%s'") % p.word() << std::endl;
                }
            }

            d.join();
        }
        return 0;
    }
    catch (std::exception& e) {
        std::cerr << format("Error: %1%") % e.what() << std::endl;
        return 1;
    }
}
