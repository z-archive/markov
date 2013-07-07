#include <iostream>
#include <fstream>

#include <boost/utility/typed_in_place_factory.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

#include "common/dictionary.h"
#include "common/model.h"
#include "learn/downloader.h"
#include "learn/parser.h"
#include "learn.h"

template<typename S, typename F>
S& resolve_stream(S& default_stream,
                  boost::optional<F>& file,
                  OptionalString const& path)
{
    if (!path) {
        return default_stream;
    }
    file = boost::in_place(path.get());
    return file.get();
}

template<typename Converter>
class Learner : boost::noncopyable
{
private:
    typedef typename Converter::item_type item_type;
    typedef typename Converter::item_return_type item_return_type;
    typedef Learner<Converter> this_type;
    typedef boost::lock_guard<boost::mutex> guard;

public:
    Learner(settings::Learn const &settings) :
        _settings(settings),
        _model(settings.order),
        _in(nullptr)
    {
    }

    void learn(std::istream& in)
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

    void save(std::ostream& out) const
    {
        guard lock(_mutex);
        boost::archive::text_oarchive oa(out);
        oa & _converter;
        oa & _model;
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
                    _model.add(state, item);
                }
                state.add(item);
            }
        }

        d.join();
    }

private:
    std::istream*        _in;
    settings::Learn      _settings;
    Converter            _converter;
    Model<item_type>     _model;
    mutable boost::mutex _mutex;
};


void learn(settings::Learn const &settings)
{
    boost::optional<std::ifstream> fin;
    boost::optional<std::ofstream> fout;
    std::istream& in = resolve_stream(std::cin, fin, settings.in);
    std::ostream& out = resolve_stream(std::cout, fout, settings.out);

    Learner<WordDict> learner(settings);
    learner.learn(in);
    learner.save(out);
}
