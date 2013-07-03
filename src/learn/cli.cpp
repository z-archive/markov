#include <boost/program_options.hpp>
#include <boost/format.hpp>

#include "common/config.h"
#include "cli.h"

namespace po = boost::program_options;
using boost::format;

int parse(int argc,
	  char* argv[],
	  OptionalString& in,
	  Timeout& timeout,
	  OptionalString& out,
	  ChainOrder& order,
	  bool& verbose,
	  bool& strict)
{
  auto usage = "learn OPTIONS";
  auto co_help = format("markov's chain order - integer from 1 to %1%");

  po::options_description desc(usage);
  auto add = desc.add_options();

  std::string input_var;
  std::string output_var;
  int order_var;
  int timeout_var;

  add("input,i", po::value<std::string>(&input_var),
      "path to file with links for learning; stdin if omit");
  add("timeout,t", po::value<int>(&timeout_var)->default_value(default_timeout),
      "timeout for curl for every url");
  add("output,o", po::value<std::string>(&output_var),
      "file path for store model after learning; stdout if omit");
  add("chain_order,c", po::value<int>(&order_var),
      (co_help % static_cast<int>(max_chain_order)).str().c_str());
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
    in = input_var;
  }

  timeout = static_cast<Timeout>(timeout_var);

  if (vm.count("output")) {
    out = output_var;
  }

  if (!vm.count("chain_order")) {
    auto message = "argument --chain_order=<integer>|-c <integer> must be specified";
    throw std::runtime_error(message);
  }

  if ((order_var < 1) || 
      (order_var > max_chain_order)) {
    auto string = "Error: invalid --chain_order=%1%, expected value from 1 to %2%";
    auto message = format(string) % order % max_chain_order;
    throw std::out_of_range(message.str());
  } else {
    order = order_var;
  }


  strict = static_cast<bool>(vm.count("strict"));
  verbose = static_cast<bool>(vm.count("verbose"));
  return 0;
}
