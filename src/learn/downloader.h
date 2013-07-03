#ifndef __MARKOV_LEARN_DOWNLOAD_H__
#define __MARKOV_LEARN_DOWNLOAD_H__

#include <boost/process.hpp>

#include "common/types.h"
#include "learn/learn.h"

class Downloader : boost::noncopyable
{
public:
  Downloader(Url const&, Timeout const&, bool verbose);
  virtual ~Downloader();

  std::istream& data();

  void join();

  Url const& url() const;
private:
  bool _verbose;
  Url const _url;
  bool _work;
  boost::process::child  _child;
};


#endif /* __MARKOV_LEARN_DOWNLOAD_H__ */
