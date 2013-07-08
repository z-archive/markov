#include "dictionary.h"

WordDict::WordDict() : _count()
{
    ++_count; // Token_Begin
    ++_count; // Token_End;
}

Token WordDict::operator()(Word const& word)
{
    // ir - insert result
    auto &result = _data[word];
    if (result == 0)
    {
        result = _count++;
    }
    return result;
}

TokenDict::TokenDict(WordDict const& dict) : _data(dict._count)
{
    for(auto pair: dict._data) {
        auto word = pair.first;
        auto token = pair.second;
        _data[token] = word;
    }
}

Word const& TokenDict::operator()(Token token) const
{
    return _data.at(token);
}

Translator::Translator(WordDict const &from,
                       WordDict &to) :
    _from(from),
    _to(to)
{
}

Token Translator::operator()(Token const &token)
{
    return _to(_from(token));
}

std::vector<Token> Translator::operator()(std::vector<Token> const &state)
{
    std::vector<Token> result(state);
    for(Token &token: result)
    {
        token = (*this)(token);
    }
    return result;
}
