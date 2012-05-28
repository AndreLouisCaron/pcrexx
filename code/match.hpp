#ifndef _pcrexx_match_hpp__
#define _pcrexx_match_hpp__

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
 * @file match.hpp
 * @see http://www.pcre.org/pcre.txt
 */

#include <pcre.h>
#include "exception.hpp"
#include "options.hpp"
#include "pattern.hpp"
#include "traits.hpp"
#include <map>
#include <vector>

namespace pcrexx {

    /*!
     * @brief Regular expression search/match result.
     * @tparam C Character type.  @c traits<C> must be defined.  By default,
     *  it is only @c traits<char> and @c traits<wchar_t> are provided.
     * @tparam S String type.  Must have a default constructor, copy
     *  constructor, copy-assignment operator and a constructor that accepts
     *  a C-style null-terminated string with @c C as character type.  It must
     *  also have a @c c_str() member function that returns a C-style
     *  null-terminated string.
     *
     * Seaach/match results are useful for extrating captured groups from a
     * matched subject string.
     *
     * @note Match objects are immutable and thread-safe.
     */
    template<class C, class S=typename traits<C>::string>
    class basic_match
    {
        /* nested types. */
    public:
        typedef C char_type;
        typedef typename traits<char_type> traits_type;

        typedef S string_type;

        typedef typename basic_pattern<char_type> pattern_type;

        /* data. */
    private:
        string_type myText;
        int myGroups;
        std::vector<int> myResults;

        /* construction. */
    public:
        basic_match ()
            : myText()
            , myGroups(0)
            , myResults()
        {}

        /*!
         * @brief Match @a text using @a pattern.
         */
        basic_match ( const pattern_type& pattern, const string_type& text,
                      runtime_options options=runtime_options() )
            : myText(text),
              myGroups(pattern.capturing_groups()),
              myResults((1+myGroups)*3, 0)
        {
            const int status = traits_type::execute
                (pattern.handle(), 0, text.data(), text.size(),
                 0, options, &myResults[0], myResults.size());
            if (status < 0)
            {
                if (status != PCRE_ERROR_NOMATCH) {
                    throw (exception(status, "name_table()"));
                }
                myGroups = 0, myResults.clear();
            }
        }

        /* methods. */
    public:
        /*!
         * @brief Subject text.
         */
        const string_type& text () const
        {
            return (myText);
        }

        /*!
         * @brief Get the offset of the entire match.
         */
        int group_base () const
        {
            return (myResults[0]);
        }

        /*!
         * @brief Get the offset of a specific group within the match.
         */
        int group_base ( int i ) const
        {
            return (myResults[2*i]);
        }

        /*!
         * @brief Get the size of the entire match.
         */
        int group_size () const
        {
            return (myResults[1]-myResults[0]);
        }

        /*!
         * @brief Get the size a specific group within the match.
         */
        int group_size ( int i ) const
        {
            return (myResults[2*i+1]-myResults[2*i]);
        }

        /*!
         * @brief Get the contents of the entire match.
         */
        string_type group () const
        {
            return (myText.substr(group_base(), group_size()));
        }

        /*!
         * @brief Get the contents of a specific group within the match.
         */
        string_type group ( int i ) const
        {
            return (myText.substr(group_base(i), group_size(i)));
        }

        /*!
         * @brief Get the contents of all captured groups, in order.
         */
        std::vector<string_type> groups () const;

        /*!
         * @brief Get the contents of all captured groups, by name.
         *
         * @todo Keep an internal copy of the pattern to implement
         *  this without requiring the client to supply the pattern again.
         */
        std::map<string_type,string_type>
            named_groups ( const pattern_type& pattern ) const
        {
            std::vector<string_type> names = pattern.group_names();
            std::map<string_type,string_type> groups;
            for (std::size_t i=0; (i < names.size()); ++i)
            {
                const string_type& name = names[i];
                const string_type  data = group(pattern.group_index(name));
                groups.insert(std::make_pair(name, data));
            }
            return (groups);
        }

        /* operators. */
    public:
        operator bool () const
        {
            return (myResults.size() > 0);
        }
    };

    /*!
     * @brief Search/match result for UTF-8 strings stored in @c std::string.
     */
    typedef basic_match<char> match;

    /*!
     * @brief Search/match result for UTF-16 strings stored in @c std::wstring.
     */
    typedef basic_match<wchar_t> wmatch;

    // pattern(text,options) -> match.
    template<class C, class S>
    basic_match<C,S> basic_pattern<C,S>::operator() (
        const S& text, runtime_options options=runtime_options()) const
    {
        return (basic_match<C,S>(*this, text, options));
    }

}

#endif /* _pcrexx_match_hpp__ */
