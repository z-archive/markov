#include <exception>
#include <iostream>

#include <boost/format.hpp>

#include "cli.h"
#include "learn.h"

using boost::format;


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
                      << format("  worker count %1%\n") % s.worker_count
                      << format("  verbose? %1%\n") % (s.verbose ? "yes" : "no")
                      << format("  strict? %1%\n") % (s.strict ? "yes" : "no")
                      << std::flush;
        }

        learn(s);
        return 0;
    }
    catch (std::exception& e) {
        std::cerr << format("Error: %1%") % e.what() << std::endl;
        return 1;
    }
}
