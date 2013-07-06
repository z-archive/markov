#ifndef __MARKOV_LEARN_CLI_H__
#define __MARKOV_LEARN_CLI_H__

#include <iostream>

#include "common/types.h"
#include "common/config.h"

int parse(int argc,
          char* argv[],
          settings::Learn& settings);

#endif /* __MARKOV_LEARN_CLI_H__ */
