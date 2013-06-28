#ifndef __MARKOV_LEARN_CLI_H__
#define __MARKOV_LEARN_CLI_H__

#include <iostream>

#include "common/types.h"


int parse(int argc,
	  char* argv[],
	  optional_string& in,
	  optional_string& out,
	  bool& verbose,
	  bool& strict,
	  chain_order_t& order);
	   

#endif /* __MARKOV_LEARN_CLI_H__ */
