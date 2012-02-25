#ifndef _pcrexx_traits_hpp__
#define _pcrexx_traits_hpp__

// Copyright (c) 2012, Andre Caron (andre.l.caron@gmail.com)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <pcre.h>
#include <string>

namespace pcrexx {

    template<typename T> struct traits;

    template<> struct traits<char>
    {
        typedef ::pcre* handle;
        typedef const ::pcre_extra* extra;

        typedef char* char_ptr;
        typedef const char* const_char_ptr;

        typedef std::string string;

        static handle compile ( const_char_ptr pattern, int options,
                                int * error, const char ** help, int * offset,
                                const unsigned char * table )
        {
            return (::pcre_compile2
                    (pattern, options, error, help, offset, table));
        }

        static void release ( handle pattern )
        {
            ::pcre_free(pattern);
        }

        static int query ( handle pattern, extra extra, int what, void * where )
        {
            return (::pcre_fullinfo(pattern, extra, what, where));
        }

        static int string_number ( handle pattern, const_char_ptr name )
        {
            return (::pcre_get_stringnumber(pattern, name));
        }

        static int execute ( handle pattern, extra extra,
                             const_char_ptr data, int size, int base,
                             int options, int * results, int count )
        {
            return (::pcre_exec(pattern, extra, data, size,
                                base, options, results, count));
        }

        static int table_offset ()
        {
            return (2);
        }
    };

    template<> struct traits<wchar_t>
    {
        typedef ::pcre16* handle;
        typedef const ::pcre16_extra* extra;

        typedef wchar_t * char_ptr;
        typedef const wchar_t * const_char_ptr;

        typedef std::wstring string;

        static handle compile ( const_char_ptr pattern, int options,
                                int * error, const char ** help, int * offset,
                                const unsigned char * table )
        {
            return (::pcre16_compile2
                    (to(pattern), options, error, help, offset, table));
        }

        static void release ( handle pattern )
        {
            ::pcre16_free(pattern);
        }

        static int query ( handle pattern, extra extra, int what, void * where )
        {
            return (::pcre16_fullinfo(pattern, extra, what, where));
        }

        static int string_number ( handle pattern, const_char_ptr name )
        {
            return (::pcre16_get_stringnumber(pattern, to(name)));
        }

        static int execute ( handle pattern, extra extra,
                             const_char_ptr data, int size, int base,
                             int options, int * results, int count )
        {
            return (::pcre16_exec(pattern, extra, to(data), size,
                                  base, options, results, count));
        }

        static int table_offset ()
        {
            return (1);
        }

    private:
        static const_char_ptr from ( PCRE_SPTR16 pointer )
        {
            return (reinterpret_cast<const_char_ptr>(pointer));
        }

        static PCRE_SPTR16 to ( const_char_ptr pointer )
        {
            return (reinterpret_cast<PCRE_SPTR16>(pointer));
        }
    };

}

#endif /* _pcrexx_traits_hpp__ */
