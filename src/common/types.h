#ifndef __MARKOV_COMMON_TYPES_H__
#define __MARKOV_COMMON_TYPES_H__

#include <cstdint>
#include <string>
#include <vector>
#include <boost/optional.hpp>

typedef boost::optional<std::string> OptionalString;

typedef uint16_t ChainOrder;
typedef uint32_t BufferSize;

typedef std::string Url;
typedef uint16_t    Timeout;

typedef std::string Word;
typedef std::vector<Word> Sentence;

typedef uint32_t    Token;
typedef uint32_t    Frequency;

#endif /* __MARKOV_COMMON_TYPES_H__ */
