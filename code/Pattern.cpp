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

    const char * name_table ( ::pcre * pattern )
    {
        const char * table = 0;
        const int status = ::pcre_fullinfo
            (pattern, 0, PCRE_INFO_NAMETABLE, (void*)&table);
        if (status != 0)
        {
            std::cerr
                << "Failed to fetch group name table!"
                << std::endl;
        }
        return (table);
    }

    int name_table_stride ( ::pcre * pattern )
    {
        int stride = 0;
        const int status = ::pcre_fullinfo
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

    Pattern::Pattern ( const std::string& text )
        : myText(text), myHandle(0)
    {
        int error = 0;
        int offset = 0;
        int options = 0;
        const char * help = 0;
        myHandle = ::pcre_compile2
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
        ::pcre_free(myHandle);
    }

    Pattern::Handle Pattern::handle () const
    {
        return (myHandle);
    }

    const std::string& Pattern::text () const
    {
        return (myText);
    }

    int Pattern::capturing_groups () const
    {
        int groups = 0;
        const int status = ::pcre_fullinfo
            (myHandle, 0, PCRE_INFO_CAPTURECOUNT, &groups);
        if (status != 0)
        {
            std::cerr
                << "Failed to count groups!"
                << std::endl;
        }
        return (groups);
    }

    int Pattern::group_index ( const std::string& name ) const
    {
        const int index = ::pcre_get_stringnumber(myHandle, name.c_str());
        if (index == PCRE_ERROR_NOSUBSTRING)
        {
            std::cerr
                << "No such group: '" << name << "'!"
                << std::endl;
        }
        return (index);
    }

    std::vector<std::string> Pattern::group_names () const
    {
        // Access the name table.
        const char *const table = ::name_table(myHandle);
        const int stride = ::name_table_stride(myHandle);
        // Scan the table for names.
        std::vector<std::string> names;
        for (int i=0; (i < capturing_groups()); ++i) {
            names.push_back(table+(i*stride)+2);
        }
        return (names);
    }

}
