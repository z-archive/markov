#include <cstring>
#include <cctype>

#include <boost/assert.hpp>
#include <boost/format.hpp>

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
    //return isalnum(c);
    return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9';
}

inline bool is_space(char c)
{
    switch(c)
    {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
        return true;
    default:
        return false;
    }
}

inline bool for_ignore(char c)
{
    if (is_word(c))
    {
        return false;
    }
    if (is_delimeter(c))
    {
        return false;
    }
    return true;
}

}

template<Parser::Predicate predicate>
BufferSize Parser::parse()
{
    auto data = _buffer.data();
    auto available = _buffer.available();

    BufferSize length = 0;
    for(; length < available; ++length)
    {
        if (!predicate(data[length]))
        {
            break;
        }
    }
    return length;
}

template<Parser::Predicate predicate>
bool Parser::skip()
{
    bool result = false;
    while (!_buffer.done())
    {
        auto count = parse<predicate>();
        if (count == 0)
        {
            break;
        }
        else
        {
            _buffer.skip(count);
            result = true;
        }
    }
    return result;
}

bool Parser::parseWord(Word& word, bool &delimeter)
{
    // parse word
    auto length = parse<is_word>();
    if (length == 0)
    {
        return false;
    }

    // save word
    word = std::string(_buffer.data(), length);
    // to lower case
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    // skip word
    _buffer.skip(length);

    if (!_buffer.done())
    {
        // truncate too long word
        skip<is_word>();
    }

    delimeter = false;
    return true;
}

bool Parser::parseDelimeter(bool &delimeter)
{
    // parse delimeter
    if (!skip<is_delimeter>())
    {
        return false;
    }

    delimeter = true;
    return true;
}

bool Parser::next(Word &word, bool &delimeter)
{
    do
    {
        skip<is_space>();

        if (parseWord(word, delimeter))
        {
            return true;
        }

        if (parseDelimeter(delimeter))
        {
            return true;
        }

        skip<for_ignore>();
    }
    while(!_buffer.done());

    if (_verbose)
    {
        std::cerr << format("[%1%] parsing...done")
            % _buffer.downloader().url() << std::endl;
    }
    return false;
}
