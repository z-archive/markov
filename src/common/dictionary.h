#ifndef __MARKOV_COMMON_DICTIONARY_H__
#define __MARKOV_COMMON_DICTIONARY_H__

#include <map>
#include <boost/utility.hpp>

#include "common/types.h"

class TokenDict;

class WordDict : boost::noncopyable
{
private:
    typedef std::map<Word, Token> data_type;
    friend class TokenDict;
    friend class Translator;

public:
    WordDict();
    Token operator()(Word const&);

private:
    Token _count;
    data_type  _data;
};

class DummyWordDict : boost::noncopyable
{
public:
    Word const& operator()(Word const &word)
    {
        return word;
    }

    Sentence initial(Sentence const& sentence) const
    {
        return sentence;
    }

    template<typename Archive>
    void serialize(Archive &, unsigned int const)
    {
    }
};

class TokenDict : boost::noncopyable
{
private:
    typedef std::vector<Word> data_type;
    typedef std::vector<Token>      state_type;
    friend class Translator;

public:
    TokenDict();
    TokenDict(WordDict const& dict);
    Word const& operator()(Token) const;

    state_type initial(Sentence const&) const;

    template<typename Archive>
    void serialize(Archive &ar, unsigned int const /*version*/)
    {
        ar & _data;
    }

private:
    data_type _data;
};

class Translator : boost::noncopyable
{
public:
    Translator(WordDict const &from,
               WordDict &to);

    Token operator()(Token const&);
    std::vector<Token> operator()(std::vector<Token> const&);

private:
    TokenDict _from;
    WordDict  &_to;
};

class DummyTranslator : boost::noncopyable
{
public:
    DummyTranslator(DummyWordDict const&,
                    DummyWordDict&)
    {
    }

    Word const& operator()(Word const &word)
    {
        return word;
    }

    std::vector<Word> const& operator()(std::vector<Word> const &state)
    {
        return state;
    }
};

#endif /* __MARKOV_COMMON_DICTIONARY_H__ */
