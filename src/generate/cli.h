#ifndef __MARKOV_GENERATE_CLI_H__
#define __MARKOV_GENERATE_CLI_H__

#include <iostream>

#include "common/types.h"
#include "common/config.h"

int parse(int argc,
          char* argv[],
          settings::Generate& settings);

#endif /* __MARKOV_GENERATE_CLI_H__ */
