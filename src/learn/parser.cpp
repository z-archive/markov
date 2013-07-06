#include <cstring>
#include <cctype>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

#include "downloader.h"
#include "parser.h"

Parser::Buffer::Buffer(Downloader &downloader,
                       BufferSize length,
                       BufferSize window_size) :
    _window_size(window_size),
    _downloader(downloader),
    _pos(0),
    _left(0)
{
    BOOST_ASSERT(length > window_size*2);
    _data.resize(length);
    skip(0);
}

const char* Parser::Buffer::data() const
{
    return &_data[0] + _pos;
}

bool Parser::Buffer::done() const
{
    return available() == 0;
}

BufferSize Parser::Buffer::available() const
{
    return _left;
}

char* Parser::Buffer::end()
{
    return &_data[0] + _pos + _left;
}

bool Parser::Buffer::enoughData() const
{
    return _left >= _window_size;
}

void Parser::Buffer::packDown()
{
    memmove(&_data[0], data(), available());
    _pos = 0;
}

void Parser::Buffer::fetch()
{
    auto count = &_data.back() - end();
    _left += _downloader.read(end(), count);
}

void Parser::Buffer::skip(BufferSize count)
{
    _pos  += count;
    _left -= count;

    if (!enoughData()) {
        packDown();
        fetch();
    }
}

Parser::Parser(Downloader& downloader,
               settings::Learn const &settings) :
    _buffer(downloader,
            settings.parser_buffer_length,
            settings.max_word_length*2),
    _max_word_length(settings.max_word_length),
    _delimeter(true)
{
}

namespace
{

bool is_delimeter(char c)
{
    switch(c)
    {
    case '.':
    case ',':
    case ';':
        return true;
    default:
        return false;
    }
}

bool is_word(char c)
{
    return isalnum(c);
}

bool for_ignore(char c)
{
    return !(is_word(c) || is_delimeter(c));
}

}

char Parser::head() const
{
    return _buffer.data()[0];
}

template<Parser::Predicate predicate>
BufferSize Parser::parse()
{
    BOOST_ASSERT(_buffer.available() > 0);
    BOOST_ASSERT(predicate(head()));

    auto data = _buffer.data();
    auto available = _buffer.available();

    BufferSize current = 0;
    do
    {
        ++current;
    }
    while(current < available && predicate(data[current]));

    return current;
}

template<Parser::Predicate predicate>
void Parser::skip()
{
    BOOST_ASSERT(_buffer.available() > 0);
    BOOST_ASSERT(predicate(head()));

    do
    {
        auto count = parse<predicate>();
        BOOST_ASSERT(count > 0);
        _buffer.skip(count);
    }
    while(false == _buffer.done() && predicate(head()));
}

bool Parser::next()
{
    if(_buffer.done())
    {
        return false;
    }

    if (for_ignore(head()))
    {
        // skip ignored data
        skip<for_ignore>();
    }

    if(_buffer.done())
    {
        return false;
    }

    if (is_word(head()))
    {
        // parse word
        auto length = parse<is_word>();
        BOOST_ASSERT(length > 0);

        // save word
        _word = std::string(_buffer.data(), length);
        boost::algorithm::to_lower(_word);

        // skip word
        _buffer.skip(length);

        if (is_word(head()))
        {
            // too long word, truncated
            // skip tail of the word
            skip<is_word>();
        }

        _delimeter = false;
        return true;
    }
    else
    {
        BOOST_ASSERT(is_delimeter(head()));

        // parse delimeter
        skip<is_delimeter>();

        _delimeter = true;
        return true;
    }
}

bool Parser::delimeter() const
{
    return _delimeter;
}

Word const& Parser::word() const
{
    BOOST_ASSERT(!_delimeter);
    return _word;
}
