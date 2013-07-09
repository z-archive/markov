#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>

#include "cli.h"

namespace po = boost::program_options;
using namespace settings;
using boost::format;

int parse(int argc,
          char* argv[],
          settings::Learn &settings)
{
    auto usage = "learn OPTIONS";
    auto co_help = format("markov's chain order - integer from 1 to %1%");

    po::options_description desc(usage);
    auto add = desc.add_options();

    std::string input_var;
    std::string output_var;
    int order_var;
    int timeout_var;
    int mwl_var;
    int worker_count_var;

    // --input
    add("input,i",
        po::value<std::string>(&input_var),
        "path to file with links for learning; stdin if omit");

    // --output
    add("output,o",
        po::value<std::string>(&output_var),
        "file path for store model after learning; stdout if omit");

    // --chain_order
    add("chain_order,c", po::value<int>(&order_var),
        (co_help % static_cast<int>(limits::max_order)).str().c_str());

    // --max_word_length
    add("max_word_length,m",
        po::value<int>(&mwl_var)->default_value(defaults::max_word_length),
        "all words which longer max_word_length would truncated");

    // --compress
    add("compress,z",
        "compress word by dictionary or not");

    // --timeout
    add("timeout,t",
        po::value<int>(&timeout_var)->default_value(defaults::timeout),
        "timeout for curl for every url");

    // --worker
    add("worker_count,w",
        po::value<int>(&worker_count_var)->default_value(boost::thread::hardware_concurrency()),
        "downloader and parser thread count");

    // --strict
    //add("strict,s",
    //    "strict mode - begin of result sentense should be begin of some source sentence; same with end");

    // --help
    add("help,h",
        "print help message");

    // --verbose
    add("verbose,v",
        "verbose output");

    // parse
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // --help
    if (vm.count("help")) {
        desc.print(std::cout);
        return 1;
    }

    // --input
    if (vm.count("input")) {
        settings.in = input_var;
    }

    // --output
    if (vm.count("output")) {
        settings.out = output_var;
    }

    // --chain_order
    if (!vm.count("chain_order")) {
        auto message = "argument --chain_order=<integer>|-c <integer> must be specified";
        throw std::runtime_error(message);
    }

    if ((order_var < 1) ||
        (order_var > limits::max_order)) {
        auto string = "Error: invalid --chain_order=%1%, expected value from 1 to %2%";
        auto message = format(string) % settings.order % limits::max_order;
        throw std::out_of_range(message.str());
    } else {
        settings.order = order_var;
    }

    // --max_word_length
    settings.max_word_length = mwl_var;

    // --compress
    settings.compress = vm.count("compress");

    // --verbose
    settings.verbose = static_cast<bool>(vm.count("verbose"));

    // --timeout
    settings.timeout = static_cast<Timeout>(timeout_var);

    // parse_buffer_length
    settings.parser_buffer_length = settings.max_word_length * 1024;

    // --worker
    settings.worker_count = worker_count_var;


    // --strict
    //settings.strict = static_cast<bool>(vm.count("strict"));

    return 0;
}
