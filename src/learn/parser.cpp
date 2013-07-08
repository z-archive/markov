#include <cstring>
#include <cctype>

#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "downloader.h"
#include "parser.h"

using boost::format;

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

inline char Parser::Buffer::head() const
{
    return _data[_pos];
}

inline const char* Parser::Buffer::data() const
{
    return &_data[_pos];
}

inline bool Parser::Buffer::done() const
{
    return available() == 0;
}

inline BufferSize Parser::Buffer::available() const
{
    return _left;
}

inline char* Parser::Buffer::end()
{
    return &_data[_pos + _left];
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

Downloader const& Parser::Buffer::downloader() const
{
    return _downloader;
}

Parser::Parser(Downloader& downloader,
               settings::Learn const &settings) :
    _buffer(downloader,
            settings.parser_buffer_length,
            settings.max_word_length*2),
    _verbose(settings.verbose),
    _max_word_length(settings.max_word_length)
{
    if (_verbose)
    {
        std::cerr << format("[%1%] parsing...") % downloader.url() << std::endl;
    }
}

namespace
{

inline bool is_delimeter(char c)
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

inline bool is_word(char c)
{
    return isalnum(c);
}

inline bool for_ignore(char c)
{
    return !(is_word(c) || is_delimeter(c));
}

}

template<Parser::Predicate predicate>
BufferSize Parser::parse()
{
    BOOST_ASSERT(_buffer.available() > 0);
    BOOST_ASSERT(predicate(_buffer.head()));

    auto data = _buffer.data();
    auto available = _buffer.available();

    BufferSize length = 0;
    for(++length; length < available; ++length)
    {
        if (!predicate(data[length]))
        {
            break;
        }
    }
    return length;
}

template<Parser::Predicate predicate>
void Parser::skip()
{
    BOOST_ASSERT(_buffer.available() > 0);
    BOOST_ASSERT(predicate(_buffer.head()));

    do
    {
        auto count = parse<predicate>();
        BOOST_ASSERT(count > 0);
        _buffer.skip(count);
    }
    while(false == _buffer.done() && predicate(_buffer.head()));
}

bool Parser::next(Word &word, bool &delimeter)
{
    if(_buffer.done())
    {
        return false;
    }

    if (for_ignore(_buffer.head()))
    {
        // skip ignored data
        skip<for_ignore>();
    }

    if(_buffer.done())
    {
        if (_verbose)
        {
            std::cerr << format("[%1%] parsing...done")
                % _buffer.downloader().url() << std::endl;
        }
        return false;
    }

    if (is_word(_buffer.head()))
    {
        // parse word
        auto length = parse<is_word>();
        BOOST_ASSERT(length > 0);

        // save word
        word = std::string(_buffer.data(), length);
        boost::algorithm::to_lower(_word);

        // skip word
        _buffer.skip(length);

        if (is_word(_buffer.head()))
        {
            // too long word, truncated
            // skip tail of the word
            skip<is_word>();
        }

        delimeter = false;
        return true;
    }
    else
    {
        BOOST_ASSERT(is_delimeter(_buffer.head()));

        // parse delimeter
        skip<is_delimeter>();

        delimeter = true;
        return true;
    }
}
