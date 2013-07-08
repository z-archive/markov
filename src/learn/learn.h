#ifndef __MARKOV_TEACHER_H__
#define __MARKOV_TEACH_H__

#include <iostream>

#include "common/types.h"
#include "common/config.h"

void learn(std::istream&,
           std::ostream&,
           settings::Learn const &settings);

#endif /* __MARKOV_TEACHER_H__ */
