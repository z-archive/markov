#ifndef __MARKOV_LEARN_CLI_H__
#define __MARKOV_LEARN_CLI_H__

#include <iostream>

#include "common/types.h"


int parse(int argc,
	  char* argv[],
	  OptionalString& in,
	  Timeout& timeout,
	  OptionalString& out,
	  ChainOrder& order,
	  bool& verbose,
	  bool& strict);
	   

#endif /* __MARKOV_LEARN_CLI_H__ */
