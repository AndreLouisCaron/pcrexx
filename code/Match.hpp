#ifndef _pcrexx_Match_hpp__
#define _pcrexx_Match_hpp__

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
#include <map>
#include <vector>

namespace pcrexx {

    class Pattern;

    class Match
    {
        // Not copyable.
        Match ( const Match& );
        Match& operator= ( const Match& );

        /* nested types. */
    public:
        typedef wchar_t char_type;
        typedef /*typename*/ traits<char_type> traits_type;

        typedef /*typename*/ traits_type::string string;

        /* data. */
    private:
        string myText;
        int myGroups;
        std::vector<int> myResults;

        /* construction. */
    public:
        /*!
         * @brief Match @a text using @a pattern.
         */
        Match ( const Pattern& pattern, const string& text );

        /* methods. */
    public:
        /*!
         * @brief Subject text.
         */
        const string& text () const;

        /*!
         * @brief Get the offset of the entire match.
         */
        int group_base () const;

        /*!
         * @brief Get the offset of a specific group within the match.
         */
        int group_base ( int i ) const;

        /*!
         * @brief Get the size of the entire match.
         */
        int group_size () const;

        /*!
         * @brief Get the size a specific group within the match.
         */
        int group_size ( int i ) const;

        /*!
         * @brief Get the contents of the entire match.
         */
        string group () const;

        /*!
         * @brief Get the contents of a specific group within the match.
         */
        string group ( int i ) const;

        /*!
         * @brief Get the contents of all captured groups, in order.
         */
        std::vector<string> groups () const;

        /*!
         * @brief Get the contents of all captured groups, by name.
         *
         * @todo Keep an internal copy of the pattern to implement
         *  this without requiring the client to supply the pattern again.
         */
        std::map<string,string> named_groups ( const Pattern& pattern ) const;
    };

}

#endif /* _pcrexx_Match_hpp__ */
