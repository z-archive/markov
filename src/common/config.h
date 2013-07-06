#ifndef __MARKOV_COMMON_CONFIG_H__
#define __MARKOV_COMMON_CONFIG_H__

#include "types.h"

namespace settings
{
struct Common
{
    OptionalString in;
    OptionalString out;
    bool           verbose;
};

struct Learn : public Common
{
    Timeout    timeout;
    ChainOrder order;
    BufferSize max_word_length;
    bool       strict;
};

namespace defaults
{
static Timeout   const timeout = 10;
static BufferSize const max_word_length = 64;
} // namespace defaults

namespace limits
{
static ChainOrder const order = 64;
} // namespace defauls
} // namespace settings

static Token Token_Begin = 0;
static Token Token_End = 1;

#endif /* __MARKOV_COMMON_CONFIG_H__ */
