#include "dictionary.h"

WordDict::WordDict() : _count(0)
{
    ++_count; // Token_Begin
    ++_count; // Token_End;
}

Token WordDict::operator()(Word const& word)
{
    guard lock(_mutex);

    // ir - insert result
    auto ir = _data.insert(std::make_pair(word, _count));
    if (ir.second) {
        return _count++;
    } else {
        return ir.first->second;
    }
}

TokenDict::TokenDict(WordDict const& dict) : _data(dict._count)
{
    WordDict::guard lock(dict._mutex);

    for(auto pair: dict._data) {
        auto word = pair.first;
        auto token = pair.second;
        _data[token] = word;
    }
}

Word const& TokenDict::operator()(Token token)
{
    return _data.at(token);
}
