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

        const char* data() const;
        bool done() const;
        BufferSize available() const;

        void skip(BufferSize count);

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

    bool next();

    // sentence delimeter
    bool delimeter() const;

    // current word (you must not call it on delimeter)
    Word const& word() const;

private:
    char head() const;

    typedef bool (*Predicate)(char);

    template<Predicate predicate>
    BufferSize parse();

    template<Predicate predicate>
    void skip();

    bool parseWord();
    bool parseDelimeter();

private:
    Buffer _buffer;
    BufferSize const _max_word_length;

    bool _delimeter;
    Word _word;
};

#endif /* __MARKOV_LEARN_PARSER_H__ */
