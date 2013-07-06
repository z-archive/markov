#ifndef __MARKOV_COMMON_DICTIONARY_H__
#define __MARKOV_COMMON_DICTIONARY_H__

#include <boost/pool/pool.hpp>
#include <boost/thread.hpp>
#include "common/types.h"

class WordDict : boost::noncopyable
{
private:
    typedef boost::lock_guard<boost::mutex> guard;
    typedef std::map<Word, Token> Data;
    friend class TokenDict;

public:
    WordDict();
    Token convert(Word const&);

private:
    Data  _data;
    Token _count;
    mutable boost::mutex _mutex;
};

class TokenDict : boost::noncopyable
{
private:
    typedef std::vector<Word> Data;

public:
    TokenDict(WordDict const& dict);
    Word const& convert(Token);

private:
    Data _data;
};

#endif /* __MARKOV_COMMON_DICTIONARY_H__ */
