#ifndef _pcrexx_pattern_hpp__
#define _pcrexx_pattern_hpp__

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

/*!
 * @file pattern.hpp
 * @see http://www.pcre.org/pcre.txt
 */

#include <pcre.h>
#include "exception.hpp"
#include "options.hpp"
#include "traits.hpp"
#include <vector>

namespace pcrexx {

    template<class C, class S> class basic_match;

    /*!
     * @brief Compiled regular expression object.
     * @tparam C Character type.  @c traits<C> must be defined.  By default,
     *  it is only @c traits<char> and @c traits<wchar_t> are provided.
     * @tparam S String type.  Must have a default constructor, copy
     *  constructor, copy-assignment operator and a constructor that accepts
     *  a C-style null-terminated string with @c C as character type.  It must
     *  also have a @c c_str() member function that returns a C-style
     *  null-terminated string.
     *
     * @note Pattern objects are immutable and thread-safe.
     */
    template<class C, class S=typename traits<C>::string>
    class basic_pattern
    {
        // Not copyable.
        basic_pattern ( const basic_pattern& );
        basic_pattern& operator= ( const basic_pattern& );

        /* nested types. */
    public:
        typedef C char_type;
        typedef typename traits<char_type> traits_type;

        typedef typename traits_type::handle handle_type;

        typedef S string_type;

    private:
        typedef basic_pattern<C,S> self_type;
        typedef typename traits_type::const_char_ptr const_char_ptr;

        /* class methods. */
    private:
        static const_char_ptr name_table ( handle_type pattern )
        {
            const_char_ptr table = 0;
            const int status = traits_type::query
                (pattern, 0, PCRE_INFO_NAMETABLE, (void*)&table);
            if (status != 0) {
                throw (exception(status, "name_table()"));
            }
            return (table);
        }

        static int name_table_stride ( handle_type pattern )
        {
            int stride = 0;
            const int status = traits_type::query
                (pattern, 0, PCRE_INFO_NAMEENTRYSIZE, &stride);
            if (status != 0) {
                throw (exception(status, "name_table_stride()"));
            }
            return (stride);
        }

        /* data. */
    private:
        string_type myText;
        handle_type myHandle;

        /* construction. */
    public:
        /*!
         * @brief Compile a regular expression.
         */
        basic_pattern ( const string_type& text,
                        compile_options options=compile_options() )
            : myText(text), myHandle(0)
        {
            int error = 0;
            int offset = 0;
            const char * help = 0;
            myHandle = traits_type::compile
                (myText.c_str(), options, &error, &help, &offset, 0);
            if (myHandle == 0) {
                throw (exception(error, help));
            }
        }

        ~basic_pattern ()
        {
            traits_type::release(myHandle);
        }

        /* methods. */
    public:
        handle_type handle () const
        {
            return (myHandle);
        }

        /*!
         * @brief Regular expression used to compile the pattern.
         */
        const string_type& text () const
        {
            return (myText);
        }

        /*!
         * @brief Obtain the number of capturing groups in the pattern.
         */
        int capturing_groups () const
        {
            int groups = 0;
            const int status = traits_type::query
                (myHandle, 0, PCRE_INFO_CAPTURECOUNT, &groups);
            if (status != 0) {
                throw (exception(status, "capturing_groups()"));
            }
            return (groups);
        }

        /*!
         * @brief Obtain the index of a group using its name.
         */
        int group_index ( const string_type& name ) const
        {
            const int index =
                traits_type::string_number(myHandle, name.c_str());
            if (index == PCRE_ERROR_NOSUBSTRING) {
                return (-1);
            }
            if (index < 0) {
                throw (exception(index, "capturing_groups()"));
            }
            return (index);
        }

        /*!
         * @brief Obtain the names of all capturing groups.
         *
         * The name of capturing groups are listed in alphabetical order.
         */
        std::vector<string_type> group_names () const
        {
            // Access the name table.
            const_char_ptr table = name_table(myHandle);
            const int stride = name_table_stride(myHandle);
            // Scan the table for names.
            std::vector<string_type> names;
            for (int i=0; (i < capturing_groups()); ++i) {
                names.push_back(table+(i*stride)+traits_type::table_offset());
            }
            return (names);
        }

        /* operators. */
    public:
        // See "match.hpp".
        basic_match<C,S> operator() (
            const string_type& text,
            runtime_options options=runtime_options()) const;
    };

    /*!
     * @brief Regular expression for UTF-8 strings stored in @c std::string.
     */
    typedef basic_pattern<char> pattern;

    /*!
     * @brief Regular expression for UTF-16 strings stored in @c std::wstring.
     */
    typedef basic_pattern<wchar_t> wpattern;

}

#endif /* _pcrexx_pattern_hpp__ */
