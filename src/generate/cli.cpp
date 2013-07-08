#include <ctime>
#include <cctype>
#include <exception>
//#include <algorithm>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/format.hpp>

#include "cli.h"

namespace po = boost::program_options;
using namespace settings;
using boost::format;

int parse(int argc,
          char* argv[],
          settings::Generate &settings)
{
    auto usage = "generate OPTIONS BEGIN";

    po::options_description desc(usage);
    auto add = desc.add_options();

    std::string input_var;
    std::string output_var;
    int count_var;
    int seed_var = std::time(NULL);

    // --input
    add("input,i",
        po::value<std::string>(&input_var),
        "path to file with model; stdin if omit");

    // --output
    add("output,o",
        po::value<std::string>(&output_var),
        "file path for store result sentence; stdout if omit");

    // --count
    add("count,c", po::value<int>(&count_var),
        "words count which generate should build");

    // --seed
    add("seed,s", po::value<int>(&seed_var),
        "random seed (usable for repeat generation");

    // BEGIN
    add("begin", po::value<Sentence>(&settings.begin),
        "begin of the sentence for continue; this value and model order should be same");

    // --help
    add("help,h",
        "print help message");

    // --verbose
    add("verbose,v",
        "verbose output");

    // BEGIN
    po::positional_options_description p;
    p.add("begin", -1);

    // parse
    po::variables_map vm;
    auto parser = po::command_line_parser(argc, argv);
    po::store(parser.options(desc).positional(p).run(), vm);
    po::notify(vm);

    // --help
    if (vm.count("help"))
    {
        desc.print(std::cout);
        return 1;
    }

    // --input
    if (vm.count("input"))
    {
        settings.in = input_var;
    }

    // --output
    if (vm.count("output"))
    {
        settings.out = output_var;
    }

    // --count
    if (!vm.count("count"))
    {
        auto message = "argument --count=<integer>|-c <integer> must be specified";
        throw std::runtime_error(message);
    }
    settings.count = count_var;

    // --seed
    settings.seed = seed_var;

    // BEGIN
    if (settings.begin.empty())
    {
        auto message = "position argument begin=<word list> must contains at least single word";
        throw std::runtime_error(message);
    }

    // validate BEGIN
    for(auto const &word: settings.begin)
    {
        /*
          does not work yet in gcc (C++11 <algorithm>):
            std::all_off(std::begin(word), std::end(word), isalnum)
        */
        bool valid=true;
        for(auto const& c: word)
        {
            if (!isalnum(c))
            {
                valid=false;
                break;
            }
        }
        if (!valid)
        {
            auto message = format("word '%1%' in begin sentence contain invalid symbol; your word must be letters or numbers") % word;
            throw std::runtime_error(message.str());
        }
    }

    // --verbose
    settings.verbose = static_cast<bool>(vm.count("verbose"));

    return 0;
}
