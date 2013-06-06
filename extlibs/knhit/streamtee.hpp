#ifndef KNIHT_INCLUDE_GUARD_A629F54A136C49C9938CB33EF8EDE676
#define KNIHT_INCLUDE_GUARD_A629F54A136C49C9938CB33EF8EDE676

// Copyright (c) 2009 Roger Pate. See LICENSE for details.

#include <cassert>
#include <cstring>
#include <streambuf>
#include <map>


namespace kniht {

template<class CharT, class Traits=std::char_traits<CharT> >
struct basic_streamtee : std::basic_streambuf<CharT, Traits> {
  typedef std::basic_ios<CharT, Traits> Stream;
  typedef std::basic_streambuf<CharT, Traits> StreamBuf;

  typedef typename StreamBuf::char_type char_type;
  typedef typename StreamBuf::traits_type traits_type;
  typedef typename StreamBuf::int_type int_type;
  typedef typename StreamBuf::pos_type pos_type;
  typedef typename StreamBuf::off_type off_type;

  basic_streamtee() : _key_buf(0) {}
  basic_streamtee(Stream& a, Stream& b) : _key_buf(0) {
    this->pubimbue(a.rdbuf()->getloc());
    _set_key_buf(a.rdbuf());
    insert(a);
    insert(b);
  }
  ~basic_streamtee() {
    sync();
    for (typename std::map<Stream*, StreamBuf*>::iterator i = _bufs.begin();
       i != _bufs.end();
       ++i)
    {
      StreamBuf* old = i->first->rdbuf(i->second);
      if (old != this) {
        old->pubsync();
      }
    }
  }

  // add this functionality?
  // streambufs would be unconnected with a stream
  // easy to do by changing _bufs to a multimap
  // and using null pointers for the keys
  //void insert(StreamBuf* buf);
  //void remove(StreamBuf* buf);

  void insert(Stream& s) {
    sync();
    if (!_bufs.count(&s)) {
      if (!_key_buf) {
        _set_key_buf(s.rdbuf());
      }
      _bufs[&s] = s.rdbuf(this);
    }
  }
  void remove(Stream& s) {
    sync();
    typename std::map<Stream*, StreamBuf*>::iterator i = _bufs.find(&s);
    if (i != _bufs.end()) {
      StreamBuf* old = i->second;
      i->first->rdbuf(i->second);
      _bufs.erase(i);

      if (old == _key_buf) {
        _set_key_buf(_bufs.empty() ? 0 : _bufs.begin()->second);
      }
    }
  }

private:
  basic_streamtee(basic_streamtee const&); // not defined
  basic_streamtee& operator=(basic_streamtee const&); // not defined

  StreamBuf* _key_buf;
  std::map<Stream*, StreamBuf*> _bufs;

  void _set_key_buf(StreamBuf* p) {
    //NOTE: does not sync, requires synced already
    _key_buf = p;
    _update_put_area();
  }
  void _update_put_area() {
    //NOTE: does not sync, requires synced already
    if (!_key_buf) {
      this->setp(0, 0);
    }
    else {
      this->setp((_key_buf->*&basic_streamtee::pbase)(),
             (_key_buf->*&basic_streamtee::epptr)());
    }
  }


#define FOREACH_BUF(var) \
for (typename std::map<Stream*, StreamBuf*>::iterator var = _bufs.begin(); \
var != _bufs.end(); ++var)


  // 27.5.2.4.1 Locales
  virtual void imbue(std::locale const& loc) {
    FOREACH_BUF(iter) {
      iter->second->pubimbue(loc);
    }
  }


  // 27.5.2.4.2 Buffer management and positioning
  //virtual StreamBuf* setbuf(char_type* s, std::streamsize n); // not required
  //virtual pos_type seekoff(off_type off, std::ios_base::seekdir way,
  //             std::ios_base::openmode which); // not required
  //virtual pos_type seekpos(pos_type sp, std::ios_base::openmode which); // not required
  virtual int sync() {
    if (!_key_buf) {
      return -1;
    }
    char_type* data = this->pbase();
    std::streamsize n = this->pptr() - data;
    (_key_buf->*&basic_streamtee::pbump)(n);
    FOREACH_BUF(iter) {
      StreamBuf* buf = iter->second;
      if (buf != _key_buf) {
        buf->sputn(data, n); //BUG: ignores put errors
        buf->pubsync(); //BUG: ignores errors
      }
    }
    _key_buf->pubsync(); //BUG: ignores errors
    _update_put_area();
    return 0;
  }


  // 27.5.2.4.3 Get area
  // ignore input completely, teeing doesn't make sense
  //virtual std::streamsize showmanyc();
  //virtual std::streamsize xsgetn(char_type* s, std::streamsize n);
  //virtual int_type underflow();
  //virtual int_type uflow();


  // 27.5.2.4.4 Putback
  // ignore input completely, teeing doesn't make sense
  //virtual int_type pbackfail(int_type c);


  // 27.5.2.4.5 Put area
  virtual std::streamsize xsputn(char_type const* s, std::streamsize n) {
    assert(n >= 0);
    if (!_key_buf) {
      return 0;
    }

    // available room in put area? delay sync if so
    if (this->epptr() - this->pptr() < n) {
      sync();
    }
    // enough room now?
    if (this->epptr() - this->pptr() >= n) {
      std::memcpy(this->pptr(), s, n);
      this->pbump(n);
    }
    else {
      FOREACH_BUF(iter) {
        iter->second->sputn(s, n);
        //BUG: ignores put errors
      }
      _update_put_area();
    }
    return n;
  }
  virtual int_type overflow(int_type c) {
    bool const c_is_eof = traits_type::eq_int_type(c, traits_type::eof());
    int_type const success = c_is_eof ? traits_type::not_eof(c) : c;
    sync();
    if (!c_is_eof) {
      char_type cc = traits_type::to_char_type(c);
      xsputn(&cc, 1);
      //BUG: ignores put errors
    }
    return success;
  }

#undef FOREACH_BUF
};

typedef basic_streamtee<char> streamtee;
typedef basic_streamtee<wchar_t> wstreamtee;

} // kniht::

#endif