#ifndef __MARKOV_COMMON_FILE__
#define __MARKOV_COMMON_FILE__

#include <iostream>
#include <fstream>
#include "types.h"

std::istream& getInput(std::ifstream&,
                       OptionalString const&);
std::ostream& getOutput(std::ofstream&,
                        OptionalString const&);

#endif /* __MARKOV_COMMON_FILE__ */
