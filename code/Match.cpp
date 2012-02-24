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

#include "Match.hpp"
#include "Pattern.hpp"
#include <iostream>

namespace pcrexx {

    Match::Match ( const Pattern& pattern, const std::string& text )
        : myText(text),
          myGroups(pattern.capturing_groups()),
          myResults((1+myGroups)*3, 0)
    {
        const int options = 0;
        const int status = ::pcre_exec
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

    const std::string& Match::text () const
    {
        return (myText);
    }

    int Match::group_base () const
    {
        return (myResults[0]);
    }

    int Match::group_base ( int i ) const
    {
        return (myResults[2*i]);
    }

    int Match::group_size () const
    {
        return (myResults[1]-myResults[0]);
    }

    int Match::group_size ( int i ) const
    {
        return (myResults[2*i+1]-myResults[2*i]);
    }

    std::string Match::group () const
    {
        return (myText.substr(group_base(), group_size()));
    }

    std::string Match::group ( int i ) const
    {
        return (myText.substr(group_base(i), group_size(i)));
    }

    std::map<std::string,std::string>
        Match::named_groups ( const Pattern& pattern ) const
    {
        std::vector<std::string> names = pattern.group_names();
        std::map<std::string,std::string> groups;
        for (std::size_t i=0; (i < names.size()); ++i)
        {
            const std::string& name = names[i];
            const std::string  data = group(pattern.group_index(name));
            groups.insert(std::make_pair(name, data));
        }
        return (groups);
    }

}
