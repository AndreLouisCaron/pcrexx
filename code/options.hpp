#ifndef _pcrexx_options_hpp__
#define _pcrexx_options_hpp__
 
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
 * @file options.hpp
 * @see http://www.pcre.org/pcre.txt
 */

#include <exception>

namespace pcrexx {

    class compile_options
    {
        /* data. */
    private:
        int myMask;

        /* construction. */
    public:
        compile_options ()
            : myMask(0)
        {}

        /* methods. */
    public:
        compile_options& case_insensitive () {
            myMask |= PCRE_CASELESS; return(*this);
        }

        compile_options& multiline () {
            myMask |= PCRE_MULTILINE; return(*this);
        }

        compile_options& match_newlines () {
            myMask |= PCRE_DOTALL; return(*this);
        }

        compile_options& ignore_whitespace () {
            myMask |= PCRE_EXTENDED; return(*this);
        }

        compile_options& anchored () {
            myMask |= PCRE_ANCHORED; return(*this);
        }

        compile_options& anchor_at_end () {
            myMask |= PCRE_DOLLAR_ENDONLY; return(*this);
        }

        compile_options& strict_escape () {
            myMask |= PCRE_EXTRA; return(*this);
        }

        compile_options& not_greedy () {
            myMask |= PCRE_UNGREEDY; return(*this);
        }

        compile_options& unicode_aware () {
            // Note: PCRE_UTF8==PCRE_UTF16.
            myMask |= PCRE_UTF8; return(*this);
        }

        compile_options& no_capture () {
            myMask |= PCRE_NO_AUTO_CAPTURE; return(*this);
        }

        compile_options& unicode_no_check () {
            // Note: PCRE_NO_UTF8_CHECK==PCRE_NO_UTF16_CHECK.
            myMask |= PCRE_NO_UTF8_CHECK; return(*this);
        }

        compile_options& first_line () {
            myMask |= PCRE_FIRSTLINE; return(*this);
        }

        compile_options& newline_cr () {
            myMask |= PCRE_NEWLINE_CR; return(*this);
        }

        compile_options& newline_lf () {
            myMask |= PCRE_NEWLINE_LF; return(*this);
        }

        compile_options& newline_crlf () {
            myMask |= PCRE_NEWLINE_CRLF; return(*this);
        }

        compile_options& newline_any () {
            myMask |= PCRE_NEWLINE_ANY; return(*this);
        }

        compile_options& newline_any_crlf () {
            myMask |= PCRE_NEWLINE_ANYCRLF; return(*this);
        }

        compile_options& javascript () {
            myMask |= PCRE_JAVASCRIPT_COMPAT; return(*this);
        }

        /* operators. */
    public:
        operator int () const
        {
            return (myMask);
        }
    };

    class runtime_options
    {
        /* data. */
    private:
        int myMask;

        /* construction. */
    public:
        runtime_options ()
            : myMask(0)
        {}

        /* methods. */
    public:
        runtime_options& anchored () {
            myMask |= PCRE_ANCHORED; return(*this);
        }

        runtime_options& anchor_at_end () {
            myMask |= PCRE_DOLLAR_ENDONLY; return(*this);
        }

        runtime_options& not_start_of_line () {
            myMask |= PCRE_NOTBOL; return(*this);
        }

        runtime_options& not_end_of_line () {
            myMask |= PCRE_NOTEOL; return(*this);
        }

        runtime_options& not_empty () {
            myMask |= PCRE_NOTEMPTY; return(*this);
        }

        runtime_options& accept_partial_soft () {
            myMask |= PCRE_PARTIAL_SOFT; return(*this);
        }

        runtime_options& accept_partial_hard () {
            myMask |= PCRE_PARTIAL_HARD; return(*this);
        }

        runtime_options& first_line () {
            myMask |= PCRE_FIRSTLINE; return(*this);
        }

        runtime_options& newline_cr () {
            myMask |= PCRE_NEWLINE_CR; return(*this);
        }

        runtime_options& newline_lf () {
            myMask |= PCRE_NEWLINE_LF; return(*this);
        }

        runtime_options& newline_crlf () {
            myMask |= PCRE_NEWLINE_CRLF; return(*this);
        }

        runtime_options& newline_any () {
            myMask |= PCRE_NEWLINE_ANY; return(*this);
        }

        runtime_options& newline_any_crlf () {
            myMask |= PCRE_NEWLINE_ANYCRLF; return(*this);
        }

        runtime_options& javascript () {
            myMask |= PCRE_JAVASCRIPT_COMPAT; return(*this);
        }

        runtime_options& not_empty_at_start () {
            myMask |= PCRE_NOTEMPTY_ATSTART; return(*this);
        }

        /* operators. */
    public:
        operator int () const
        {
            return (myMask);
        }
    };

}

#endif /*  _pcrexx_options_hpp__ */
