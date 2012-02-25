#ifndef _pcrexx_Pattern_hpp__
#define _pcrexx_Pattern_hpp__

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
 * @file Pattern.hpp
 * @see http://www.pcre.org/pcre.txt
 */

#include <pcre.h>
#include "traits.hpp"
#include <vector>

namespace pcrexx {

    class Pattern
    {
        // Not copyable.
        Pattern ( const Pattern& );
        Pattern& operator= ( const Pattern& );

        /* nested types. */
    public:
        typedef wchar_t char_type;
        typedef /*typename*/ traits<char_type> traits_type;

        typedef /*typename*/ traits_type::handle Handle;
        typedef /*typename*/ traits_type::string string;

        /* data. */
    private:
        string myText;
        Handle myHandle;

        /* construction. */
    public:
        /*!
         * @brief Compile a regular expression.
         */
        Pattern ( const string& text );
        ~Pattern ();

        /* methods. */
    public:
        Handle handle () const;

        /*!
         * @brief Regular expression used to compile the pattern.
         */
        const string& text () const;

        /*!
         * @brief Obtain the number of capturing groups in the pattern.
         */
        int capturing_groups () const;

        /*!
         * @brief Obtain the index of a group using its name.
         */
        int group_index ( const string& name ) const;

        /*!
         * @brief Obtain the names of all capturing groups.
         *
         * The name of capturing groups are listed in alphabetical order.
         */
        std::vector<string> group_names () const;
    };

}

#endif /* _pcrexx_Pattern_hpp__ */
