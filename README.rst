====================================
  pcrexx --- C++ wrappers for PCRE
====================================
:authors:
   Andre Caron
:contact: andre.l.caron@gmail.com

This project is a re-write of the C++ bindings for the `PCRE`_ (Perl-Compatible
Regular Expressions).  It aims to bring a cleaner, simpler interface to the
PCRE library.

.. warning::

   This library is in the experimental stage.  Do not use (yet) in production
   code.  In particular, interfaces are likely to change and not all possible
   error handling is included.

.. _`PCRE`: http://www.pcre.org/

Motivation
==========

This library is a replacement for ``pcrecpp``, the official C++ interface to
the `PCRE`_ regular expression library.  This replacement was born out of the
following requirements:

#. Must be BSD-licensed (or compatible)
#. Must support Unicode (UTF-16 is required, UTF-8 would be a bonus)
#. Must offer a separate abstractions for compiled patterns and search/match
   results.  Ideally, the interface should have a similar feel to Python's,
   .NET's or Java's API.
#. Must support proper error handling, ideally through exceptions, especially
   for incorrect use that cannot be dectected and/or prevented at compile-time
   (e.g. an invalid regular expression pattern).

Specifically, the original intent is to use this library with native Win32 API
functions, so direct UTF-16 support through a custom string type (for
compatibility purposes) is required.

Other Alternatives
------------------

Other C++ interfaces for PCRE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are other C++ interfaces for PCRE out there, including:

* `PCRE++`_
* `untropy::regex`_ (described in `Saying Goodbye to boost::regex`_)

``pcrecpp`` offers a rather delicate and underdocumented interface and seems not
to have real UTF-16 support.  ``PCRE++`` does not clearly define the separation
between regualar expression objects and search/match results.  Finally, the
``untropy::regex`` component only operates on ``std::string`` and does not
support UTF_16 either.

.. _`PCRE++`: http://www.daemon.de/PCRE
.. _`untropy::regex`: http://untropy.svn.sourceforge.net/viewvc/untropy/trunk/src/regex.hh?view=markup
.. _`Saying Goodbye to boost::regex`: http://mlangc.wordpress.com/2010/04/05/saying-goodbye-to-boostregex/

Other regular expression libraries for C++
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are other widely-used regular expression libraries for C++, most notably:

* `boost::regex`_
* C++11 ``std::regex``

The former is widely-used and well tested, but offers no Unicode support and
does not support custom string types.  The latter suffers from similar
limitations and is not widely available (yet).

.. _`boost::regex`: http://www.boost.org/doc/libs/1_49_0/libs/regex/doc/html/index.html

Synopsis
========

The library offers three main types:

#. ``pcrexx::exception``: runtime error reporting.
#. ``pcrexx::basic_pattern<>``: compiled regular expression object.
#. ``pcrexx::basic_match<>``: search/match results.

The library provides aliases ``pcrexx::pattern``, ``pcrexx::match`` for the
common case of manipulating ``std::string`` and their counter-parts
``pcrexx::wpattern`` and ``pcrexx::wmatch`` that operate on ``std::wstring``.

::

   namespace pcrexx {

       class exception :
           public std::exception
       {
       };

       template<typename C>
       class traits;

       template<> class traits<char>
       {
           typedef std::string string;
       };

       template<> class traits<wchar_t>
       {
           typedef std::wstring string;
       };

       class compile_options;

       template<typename C, typename S=typename traits<C>::string>
       class basic_pattern
       {
           // not copyable.
           basic_pattern ( const basic_pattern<C,S>& );
           basic_pattern& operator= ( const basic_pattern<C,S>& );

       public:
           basic_pattern ( const S& );
           basic_pattern ( const S&, compile_options );

           const S& text ();
       };

       typedef basic_pattern<char> pattern;
       typedef basic_pattern<wchar_t> wpattern;

       class runtime_options;

       template<typename C, typename S=typename traits<C>::string>
       class basic_match
       {
           // not copyable.
           basic_match ( const basic_match<C,S>& );
           basic_match& operator= ( const basic_match<C,S>& );

       public:
           basic_match ( const basic_pattern<C,S>&, const S& );
           basic_match ( const basic_pattern<C,S>&, const S&, runtime_options );

           const S& text ();

           operator bool () const;

           S group () const;
           S group ( int i ) const;

           std::vector<S> groups () const;

           std::map<S,S> named_groups ( const basic_patter<C,S>& ) const;
       };

       typedef basic_match<char> pattern;
       typedef basic_match<wchar_t> wmatch;

   }

If you would like to adapt the regular expression and match objects to custom
string types, you can use the second template parameter for
``pcrexx::basic_pattern<>`` and ``pcrexx::basic_match<>``.  Read the API
reference for further details.

License
=======

The code is distributed under the simplified 2-clause BSD licence.  It is
absolutely free to use in both open source and commercial applications,
provided you don't take credit for my work (see license terms below).

You don't need my consent or anything to use the software, but it would be nice
of you to tell me if you're using it.  It would allow me to keep a list of most
notable uses, giving credibility to the software and ensuring it is maintained
properly.  I also like to know that people are you using my software :-)

Here is a verbatim copy of the license:

::

   Copyright (c) 2012, Andre Caron (andre.l.caron@gmail.com)
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
