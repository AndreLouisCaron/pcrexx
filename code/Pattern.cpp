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

#include "Pattern.hpp"
#include <iostream>

namespace {

    pcrexx::Pattern::traits_type::const_char_ptr
        name_table ( pcrexx::Pattern::Handle pattern )
    {
        typedef pcrexx::Pattern::traits_type traits_type;

        traits_type::const_char_ptr table = 0;
        const int status = traits_type::query
            (pattern, 0, PCRE_INFO_NAMETABLE, (void*)&table);
        if (status != 0)
        {
            std::cerr
                << "Failed to fetch group name table!"
                << std::endl;
        }
        return (table);
    }

    int name_table_stride ( pcrexx::Pattern::Handle pattern )
    {
        int stride = 0;
        const int status = pcrexx::Pattern::traits_type::query
            (pattern, 0, PCRE_INFO_NAMEENTRYSIZE, &stride);
        if (status != 0)
        {
            std::cerr
                << "Failed to fetch group name table stride!"
                << std::endl;
        }
        return (stride);
    }

}

namespace pcrexx {

    Pattern::Pattern ( const string& text )
        : myText(text), myHandle(0)
    {
        int error = 0;
        int offset = 0;
        int options = 0;
        const char * help = 0;
        myHandle = traits_type::compile
            (myText.c_str(), options, &error, &help, &offset, 0);
        if (myHandle == 0)
        {
            std::cerr
                << "Failed to compile pattern ("
                << error << ": '" << ((help==0)?"":help) << "')!"
                << std::endl;
        }
    }

    Pattern::~Pattern ()
    {
        traits_type::release(myHandle);
    }

    Pattern::Handle Pattern::handle () const
    {
        return (myHandle);
    }

    const Pattern::string& Pattern::text () const
    {
        return (myText);
    }

    int Pattern::capturing_groups () const
    {
        int groups = 0;
        const int status = traits_type::query
            (myHandle, 0, PCRE_INFO_CAPTURECOUNT, &groups);
        if (status != 0)
        {
            std::cerr
                << "Failed to count groups!"
                << std::endl;
        }
        return (groups);
    }

    int Pattern::group_index ( const string& name ) const
    {
        const int index =
            traits_type::string_number(myHandle, name.c_str());
        if (index == PCRE_ERROR_NOSUBSTRING)
        {
#if 0
            std::cerr
                << "No such group: '" << name << "'!"
                << std::endl;
#endif
        }
        return (index);
    }

    std::vector<Pattern::string> Pattern::group_names () const
    {
        // Access the name table.
        traits_type::const_char_ptr table = ::name_table(myHandle);
        const int stride = ::name_table_stride(myHandle);
        // Scan the table for names.
        std::vector<string> names;
        for (int i=0; (i < capturing_groups()); ++i) {
            names.push_back(table+(i*stride)+traits_type::table_offset());
        }
        return (names);
    }

}
