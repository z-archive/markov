#include <iostream>

#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "common/file.h"
#include "common/dictionary.h"
#include "common/chain.h"
#include "common/model.h"
#include "learn/downloader.h"
#include "learn/parser.h"
#include "learn.h"

template<typename Model>
class Teacher : boost::noncopyable
{
private:
    typedef typename Model::item_type            item_type;
    typedef typename Model::item_return_type     item_return_type;
    typedef typename Model::learn_converter_type converter_type;
    typedef typename Model::chain_type           chain_type;
    typedef Teacher<Model>                       this_type;
    typedef boost::lock_guard<boost::mutex>      guard;

public:
    Teacher(settings::Learn const &settings) :
        _settings(settings),
        _chain(settings.order),
        _in(nullptr)
    {
    }

    void teach(std::istream& in)
    {
        _in = &in;
        std::vector<boost::thread> tv;
        do
        {
            boost::thread_group tg;

            for(auto i = 0; i < _settings.worker_count; ++i)
            {
                tv.push_back(boost::thread(&this_type::work, this));
            }
            for (auto &t: tv)
            {
                tg.add_thread(&t);
            }
            tg.join_all();
            for (auto &t: tv)
            {
                tg.remove_thread(&t);
            }
        }
        while(false);
    }

    void save(ArchiveOutput &archive) const
    {
        guard lock(_mutex);
        Model::save(archive,
                    _settings,
                    _converter,
                    _chain);
    }

private:
    bool get(Url& url)
    {
        guard lock(_mutex);
        BOOST_ASSERT(_in);
        return std::getline(*_in, url);
    }

    void work()
    {
        Url url;

        while(get(url))
        {
            parse(url);
        }
    }

    void parse(Url const& url)
    {
        Downloader d(url, _settings);
        Parser p(d, _settings);
        State<item_type> state(_settings.order);

        while(p.next())
        {
            if (p.delimeter())
            {
                state.clear();
            }
            else
            {
                item_return_type item = _converter(p.word());
                if (state.complete())
                {
                    guard lock(_mutex);
                    _chain.learn(state, item);
                }
                state.push(item);
            }
        }

        d.join();
    }

private:
    std::istream*    _in;
    settings::Learn  _settings;
    converter_type   _converter;
    chain_type       _chain;
    mutable boost::mutex _mutex;
};


template<typename Model>
void learn(settings::Learn const &settings,
           std::istream& in,
           ArchiveOutput& archive)
{
    Teacher<Model> teacher(settings);
    teacher.teach(in);
    teacher.save(archive);
}

void learn(settings::Learn const &settings)
{
    std::istream& in = getInput(settings.in);
    std::ostream& out = getOutput(settings.out);
    ArchiveOutput archive(out);

    if (settings.compress)
    {
        learn<TokenModel>(settings, in, archive);
    }
    else
    {
        learn<WordModel>(settings, in, archive);
    }
}
