#include <exception>
#include <boost/format.hpp>
#include "dictionary.h"

WordDict::WordDict() : _count()
{
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

TokenDict::TokenDict()
{
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

TokenDict::state_type TokenDict::operator()(Sentence const& sentence) const
{
    state_type result;
    auto n = _data.size();
    for(auto word: sentence)
    {
        for(auto i = 0; i < n; ++i)
        {
            if (_data[i] == word)
            {
                result.push_back(i);
                break;
            }
            if (i == n - 1)
            {
                auto message = boost::format("Word '%1%' is not found in model")
                    % word;
                throw std::runtime_error(message.str());
            }
        }
    }
    return result;
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
