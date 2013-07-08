#include <exception>
#include <sstream>
#include <iostream>

#include <boost/format.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "common/config.h"
#include "common/file.h"
#include "common/model.h"
#include "cli.h"

std::string to_string(Sentence const& sentence)
{
    std::ostringstream result;
    bool first = true;
    for(auto word: sentence)
    {
        if (false == first)
        {
            result << " ";
        }
        first = false;
        result << word;
    }
    return result.str();
}

using boost::format;

template<typename Model>
void generate(ArchiveInput& archive,
              settings::Generate& settings,
              Sentence& result)
{
    typedef typename Model::item_type               item_type;
    typedef typename Model::generate_converter_type converter_type;
    typedef typename Model::chain_type              chain_type;
    typedef State<item_type>                        state_type;

    if (settings.verbose)
    {
        std::cerr << "reading model..." << std::endl;
    }

    converter_type converter;
    chain_type chain(settings.count);

    Model::load(archive, settings, converter, chain);

    if (settings.verbose)
    {
        std::cerr << "reading model...done" << std::endl;
    }

    state_type state(converter(settings.begin));
    ChainOrder left = settings.count;
    item_type current;
    while(left > 0)
    {
        if (!chain.generate(state, current))
        {
            break;
        }
        result.push_back(converter(current));
        --left;
    }
}

int generate(ArchiveInput &archive,
             settings::Generate &settings,
             std::ostream &out)
{
    Sentence sentence;
    if (settings.compress)
    {
        generate<TokenModel>(archive, settings, sentence);
    }
    else
    {
        generate<WordModel>(archive, settings, sentence);
    }
    if (sentence.size() < settings.count)
    {
        auto message = format("Built just %1% from %2%, partial result '%3%'")
            % sentence.size() % settings.count % to_string(sentence);
        std::cerr << message << std::endl;
        return 1;
    }
    Sentence result = settings.begin;
    for(auto word: sentence)
    {
        result.push_back(word);
    }
    out << to_string(result) << std::endl;
    return 0;
}


int main(int argc, char* argv[])
{
    try {
        settings::Generate s;
        int result = parse(argc, argv, s);

        if (result != 0) {
            return result;
        }

        if (false && s.verbose) {
            std::cerr << "Options:\n"
                      << format("  input from '%1%'\n") % (s.in ? s.in.get() : "stdin")
                      << format("  output to '%1%'\n") % (s.out ? s.out.get() : "stdout")
                      << format("  count is %1%\n") % s.count
                      << format("  seed is %1%\n") % s.seed
                      << format("  verbose? %1%\n") % (s.verbose ? "yes" : "no")
                      << format("  begin '%1%'\n") % to_string(s.begin)
                      << std::flush;
        }

        std::ifstream fin;
        std::ofstream fout;
        std::istream& in = getInput(fin, s.in);
        std::ostream& out = getOutput(fout, s.out);

        if (s.verbose)
        {
            std::cerr << "reading model header..." << std::endl;
        }

        ArchiveInput archive(in);

        load_header(archive, s);

        if (s.verbose)
        {
            std::cerr << "reading model header...done" << std::endl;
        }

        if (s.order != s.begin.size())
        {
            auto message = format("model chain order (=%1%) and begin size (=%2%) are different") % s.order % s.begin.size();
            throw std::runtime_error(message.str());
        }

        return generate(archive, s, out);

    }
    catch (std::exception& e) {
        std::cerr << format("Error: %1%") % e.what() << std::endl;
        return 1;
    }
}
