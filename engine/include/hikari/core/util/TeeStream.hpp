#ifndef HIKARI_CORE_UTIL_TEESTREAM
#define HIKARI_CORE_UTIL_TEESTREAM

#include "hikari/core/Platform.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>
 
namespace hikari {
 
    template <class charT, class traits = std::char_traits<charT>>
    class HIKARI_API BasicTeeBuffer : public std::basic_streambuf<charT, traits> {
    public:
        typedef charT char_type;
        typedef typename traits::int_type int_type;
        typedef typename traits::pos_type pos_type;
        typedef typename traits::off_type off_type;
        typedef traits traits_type;
        typedef std::basic_streambuf<charT, traits> streambuf_type;
 
    private:
        streambuf_type *m_sbuf1;
        streambuf_type *m_sbuf2;
        char_type *m_buffer;
 
        enum {BUFFER_SIZE = 4096 / sizeof(char_type)};
 
    public:
        BasicTeeBuffer(streambuf_type *sbuf1, streambuf_type *sbuf2)
            : m_sbuf1(sbuf1), m_sbuf2(sbuf2), m_buffer(new char_type[BUFFER_SIZE])
        {
            setp(m_buffer, m_buffer + BUFFER_SIZE);
        }
 
        ~BasicTeeBuffer()
        {
            pubsync();
            delete[] m_buffer;
        }
 
    protected:
        virtual int_type overflow(int_type c = traits_type::eof())
        {
            // empty our buffer into m_sbuf1 and m_sbuf2
            std::streamsize n = static_cast<std::streamsize>(pptr() - pbase());
            std::streamsize size1 = m_sbuf1->sputn(pbase(), n);
            std::streamsize size2 = m_sbuf2->sputn(pbase(), n);
            if (size1 != n || size2 != n)
                return traits_type::eof();
 
            // reset our buffer
            setp(m_buffer, m_buffer + BUFFER_SIZE);
 
            // write the passed character if necessary
            if (!traits_type::eq_int_type(c, traits_type::eof()))
            {
                traits_type::assign(*pptr(), traits_type::to_char_type(c));
                pbump(1);
            }//if
 
            return traits_type::not_eof(c);
        }//overflow
 
        virtual int sync()
        {
            // flush our buffer into m_sbuf1 and m_sbuf2
            int_type c = overflow(traits_type::eof());
 
            // checking return for eof.
            if (traits_type::eq_int_type(c, traits_type::eof()))
                return -1;
 
            // flush m_sbuf1 and m_sbuf2
            if (m_sbuf1->pubsync() == -1 || m_sbuf2->pubsync() == -1)
                return -1;
 
            return 0;
        }//sync
    };//BasicTeeBuffer
 
    typedef BasicTeeBuffer<char>    TeeStream;
    typedef BasicTeeBuffer<wchar_t> WideTeeStream;

    template <class charT, class traits = std::char_traits<charT> >
    struct scoped_basic_streambuf_assignment
    {
        typedef std::basic_ostream<charT, traits> ostream_type;
        typedef std::basic_streambuf<charT, traits> streambuf_type;
 
        ostream_type &m_s;
        streambuf_type *m_orig_sb;
 
        scoped_basic_streambuf_assignment(ostream_type &s, streambuf_type *new_sb) 
            : m_s(s)
        {
            m_orig_sb = m_s.rdbuf(new_sb);
        }//constructor
 
        ~scoped_basic_streambuf_assignment()
        {
            m_s.rdbuf(m_orig_sb);
        }//destructor
    };//scoped_basic_streambuf_replacement
 
    typedef scoped_basic_streambuf_assignment<char>    scoped_streambuf_assignment;
    typedef scoped_basic_streambuf_assignment<wchar_t> scoped_wstreambuf_assignment;

} // hikari

#endif // HIKARI_CORE_UTIL_TEESTREAM