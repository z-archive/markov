#ifndef __MARKOV_COMMON_FILE__
#define __MARKOV_COMMON_FILE__

#include <iostream>
#include <fstream>
#include "types.h"

// return opened file or std::cin (if path not specified)
std::istream& getInput(std::ifstream&,
                       OptionalString const&);
// return opened file or std::cout (if path not specified)
std::ostream& getOutput(std::ofstream&,
                        OptionalString const&);

#endif /* __MARKOV_COMMON_FILE__ */
