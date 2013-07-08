#ifndef __MARKOV_LEARN_PARSER_H__
#define __MARKOV_LEARN_PARSER_H__

#include <boost/utility.hpp>

#include "common/types.h"
#include "common/config.h"

class Downloader;

class Parser : boost::noncopyable
{
private:
    /*
     * Read data from Downloader to internal buffer
     * Support at least window_size bytes in the buffer (if possible)
     */
    class Buffer
    {
    private:
        typedef std::vector<char> Data;

    public:
        Buffer(Downloader& downloader,
               BufferSize length,
               BufferSize window_size);

        char head() const;
        const char* data() const;
        bool done() const;
        BufferSize available() const;

        void skip(BufferSize count);

        Downloader const& downloader() const;

    private:
        char* end();
        bool enoughData() const;
        void packDown();
        void fetch();

    private:
        BufferSize const _window_size;
        Downloader& _downloader;
        BufferSize _pos;
        BufferSize _left;
        Data       _data;
    };

public:
    Parser(Downloader& downloader,
           settings::Learn const& settings);

    bool next(Word&, bool& delimeter);

private:
    typedef bool (*Predicate)(char);

    template<Predicate predicate>
    BufferSize parse();

    template<Predicate predicate>
    bool skip();

    bool parseWord(Word&, bool &delimeter);
    bool parseDelimeter(bool &delimeter);

private:
    Buffer _buffer;
    bool const _verbose;
    BufferSize const _max_word_length;

    bool _delimeter;
    Word _word;
};

#endif /* __MARKOV_LEARN_PARSER_H__ */
