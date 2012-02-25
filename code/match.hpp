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
 * @file Match.hpp
 * @see http://www.pcre.org/pcre.txt
 */

#include <pcre.h>
#include "traits.hpp"
#include "pattern.hpp"
#include <map>
#include <vector>

namespace pcrexx {

    template<typename T>
    class basic_match
    {
        // Not copyable.
        basic_match ( const basic_match& );
        basic_match& operator= ( const basic_match& );

        /* nested types. */
    public:
        typedef T char_type;
        typedef typename traits<char_type> traits_type;

        typedef typename traits_type::string string_type;

        typedef typename basic_pattern<char_type> pattern_type;

        /* data. */
    private:
        string_type myText;
        int myGroups;
        std::vector<int> myResults;

        /* construction. */
    public:
        /*!
         * @brief Match @a text using @a pattern.
         */
        basic_match ( const pattern_type& pattern, const string_type& text )
            : myText(text),
              myGroups(pattern.capturing_groups()),
              myResults((1+myGroups)*3, 0)
        {
            const int options = 0;
            const int status = traits_type::execute
                (pattern.handle(), 0, text.data(), text.size(),
                 0, options, &myResults[0], myResults.size());
            if (status < 0)
            {
                std::cerr
                    << "Failed to match string!"
                    << std::endl;
            }
            if (status == 0)
            {
                std::cerr
                    << "Results vector too small!"
                    << std::endl;
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
    };

    typedef basic_match<char> match;
    typedef basic_match<wchar_t> wmatch;

}

#endif /* _pcrexx_match_hpp__ */
