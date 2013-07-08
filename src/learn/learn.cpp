#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <boost/archive/text_oarchive.hpp>

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
    typedef typename Model::learn_converter_type converter_type;
    typedef typename Model::translator_type      translator_type;
    typedef typename Model::chain_type           chain_type;
    typedef Teacher<Model>                       this_type;
    typedef State<item_type>                     state_type;
    typedef boost::lock_guard<boost::mutex>      guard_type;

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
        guard_type lock(_mutex_merge);
        Model::save(archive,
                    _settings,
                    _converter,
                    _chain);
    }

private:
    bool get(Url& url)
    {
        guard_type lock(_mutex_in);
        BOOST_ASSERT(_in);
        return std::getline(*_in, url);
    }

    void work()
    {
        Url url;
        chain_type chain(_settings.order);
        converter_type converter;
        bool empty = true;
        while(get(url))
        {
            empty = false;
            process(chain, converter, url);
        }
        if (empty)
        {
            return;
        }
        if (_settings.verbose)
        {
            std::cerr << "[worker] merging chain to result..." << std::endl;
        }
        guard_type lock(_mutex_merge);
        translator_type translator(converter, _converter);
        _chain.merge(chain, translator);
        if (_settings.verbose)
        {
            std::cerr << "[worker] merging chain to result...done" << std::endl;
        }
    }

    void process(chain_type& chain, converter_type& converter, Url const& url)
    {
        Downloader d(url, _settings);
        Parser p(d, _settings);
        state_type state(_settings.order);
        bool delimeter;
        Word word;
        item_type item;

        while(p.next(word, delimeter))
        {
            if (delimeter)
            {
                state.clear();
            }
            else
            {
                item = converter(word);
                if (state.complete())
                {
                    chain.learn(state, item);
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
    mutable boost::mutex _mutex_in;
    mutable boost::mutex _mutex_merge;
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

void learn(std::istream &in,
           std::ostream &out,
           settings::Learn const &settings)
{
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
