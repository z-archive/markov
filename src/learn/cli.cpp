#include <boost/program_options.hpp>
#include <boost/format.hpp>

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

  add("input,i", po::value<std::string>(&input_var),
      "path to file with links for learning; stdin if omit");
  add("timeout,t", po::value<int>(&timeout_var)->default_value(defaults::timeout),
      "timeout for curl for every url");
  add("output,o", po::value<std::string>(&output_var),
      "file path for store model after learning; stdout if omit");
  add("max_word_length,w", po::value<int>(&mwl_var)->default_value(defaults::max_word_length));
  add("chain_order,c", po::value<int>(&order_var),
      (co_help % static_cast<int>(limits::order)).str().c_str());
  add("strict,s",
      "strict mode - begin of result sentense should be begin of some source sentence; same with end");
  add("help,h", 
      "print help message");
  add("verbose,v",
      "verbose output");
    
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    desc.print(std::cout);
    return 1;
  }
  
  if (vm.count("input")) {
    settings.in = input_var;
  }

  settings.timeout = static_cast<Timeout>(timeout_var);

  if (vm.count("output")) {
    settings.out = output_var;
  }

  if(vm.count("max_word_length")) {
    settings.max_word_length = mwl_var;
  }

  if (!vm.count("chain_order")) {
    auto message = "argument --chain_order=<integer>|-c <integer> must be specified";
    throw std::runtime_error(message);
  }

  if ((order_var < 1) || 
      (order_var > limits::order)) {
    auto string = "Error: invalid --chain_order=%1%, expected value from 1 to %2%";
    auto message = format(string) % settings.order % limits::order;
    throw std::out_of_range(message.str());
  } else {
    settings.order = order_var;
  }


  settings.strict = static_cast<bool>(vm.count("strict"));
  settings.verbose = static_cast<bool>(vm.count("verbose"));

  return 0;
}
