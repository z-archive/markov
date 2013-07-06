#ifndef __MARKOV_LEARN_DOWNLOADER_H__
#define __MARKOV_LEARN_DOWNLOADER_H__

#include <iostream>

#include <boost/utility.hpp>
#include <boost/process.hpp>

#include "common/types.h"
#include "common/config.h"


class Downloader : boost::noncopyable
{
public:
    Downloader(Url const&, settings::Learn const&);
    virtual ~Downloader();

    BufferSize read(char *buffer, BufferSize max);
    void join();

private:
    bool _verbose;
    Url const _url;
    bool _work;
    boost::process::child     _child;
    boost::process::pistream& _stdout;
};

#endif /* __MARKOV_LEARN_DOWNLOADER_H__ */
