#ifndef __MARKOV_COMMON_DICTIONARY_H__
#define __MARKOV_COMMON_DICTIONARY_H__

#include <boost/thread.hpp>

#include "common/types.h"

namespace boost
{
namespace archive
{
    class text_iarchive;
} // namespace archive
} // namespace boost

class TokenDict;

class WordDict : boost::noncopyable
{
private:
    typedef boost::lock_guard<boost::mutex> guard;
    typedef std::map<Word, Token> Data;
    friend class TokenDict;

public:
    typedef Token item_type;
    typedef Token item_return_type;

    WordDict();
    Token operator()(Word const&);

    template<typename Archive>
    void serialize(Archive &ar, unsigned int const version)
    {
        TokenDict tdict(*this);
        bool compressed = true;
        ar & compressed;
        ar & tdict;
    }

private:
    Data  _data;
    Token _count;
    mutable boost::mutex _mutex;
};

class DummyWordDict : boost::noncopyable
{
public:
    typedef Word        item_type;
    typedef Word const &item_return_type;

    Word const& operator()(Word const &word)
    {
        return word;
    }

    template<typename Archive>
    void serialize(Archive &ar, unsigned int const /*version*/)
    {
        bool compressed = false;
        ar & compressed;
    }
};

class TokenDict : boost::noncopyable
{
private:
    typedef std::vector<Word> Data;

public:
    TokenDict(WordDict const& dict);
    TokenDict(boost::archive::text_iarchive&);
    Word const& operator()(Token);

    template<typename Archive>
    void serialize(Archive &ar, unsigned int const /*version*/)
    {
        ar & _data;
    }

private:
    Data _data;
};

#endif /* __MARKOV_COMMON_DICTIONARY_H__ */
