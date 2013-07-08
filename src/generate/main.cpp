#include <exception>
#include <sstream>
#include <iostream>

#include <boost/format.hpp>

#include "cli.h"

using boost::format;


int main(int argc, char* argv[])
{
    try {
        settings::Generate s;
        int result = parse(argc, argv, s);

        if (result != 0) {
            return result;
        }

        if (s.verbose) {
            std::ostringstream s_begin;
            bool first = true;
            for(auto word: s.begin)
            {
                if (first)
                {
                    s_begin << " ";
                    first = false;
                }
                s_begin << word;
            }
            std::cerr << "Options:\n"
                      << format("  input from '%1%'\n") % (s.in ? s.in.get() : "stdin")
                      << format("  output to '%1%'\n") % (s.out ? s.out.get() : "stdout")
                      << format("  count is %1%\n") % s.count
                      << format("  seed is %1%\n") % s.seed
                      << format("  verbose? %1%\n") % (s.verbose ? "yes" : "no")
                      << format("  begin '%1%'\n") % (s_begin.str())
                      << std::flush;
        }

        //generate(s);
        return 0;
    }
    catch (std::exception& e) {
        std::cerr << format("Error: %1%") % e.what() << std::endl;
        return 1;
    }
}
