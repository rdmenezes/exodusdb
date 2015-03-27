# MAIN WIKI IS AT http://devwiki.neosys.com #

Join the email group at http://groups.google.com/group/exodus-users?pli=1

Exodus: The Next Revolution in Multivalue?

News: Exodus now has alpha version bindings for nearly all major application languages.  Specifically php, perl, python, java and C#. Server-side Javascript may be added later. This means that Exodus no longer requires a C++ compiler. Functions are identical on all platforms but numerical and logical operators are not.

Many people have dreamed of migrating their classic multivalue database applications and/or skills into mainstream programming environments. Until now however, it has been impossible. Where migration has actually been done, it has always meant that applications and skills have had to be redeveloped. The usual result is that multivalue applications are simply kept in operation and interfaces added to other systems. This isn't migration though, it is co-existence.

Exodus is freely licenced implementation of a unique concept that completely reincarnates multivalue database technology in a mainstream environment with:

  * no legacy database
  * no legacy language/compiler
  * no runtime engine

For those who know the history of the multivalue database world, Exodus offers its next revolution. First, multivalue stopped providing an operating system, next its data store became replaceable with mainstream databases. Now, with Exodus, even the Basic language/compiler has been eliminated.

Using only widely accepted tools should ease adoption of Exodus (and therefore multivalue based applications) by the vast majority of companies and individuals who are not familiar with multivalue technology and are more comfortable investing in mainstream products.

Exodus faithfully mimics classic multivalue Basic behaviour, down to the last detail, by implementing a simple and practical kind of "Javascript version of DataBasic" that can be compiled by a standard C++ compiler. Exodus provides an exact one-for-one translation for every classic multivalue Basic operator, statement and function. It does this by providing C++ library and a Postgresql database plugin.

Only a basic knowledge of the near universal "curly bracket syntax" used by C/C++/C#/Java/Javascript/PHP is required to program in Exodus. The move into the new syntax provides many practical and pleasurable ways to write code. Exodus code also be seamlessly integrated with C/C++ by including their libraries.

To enable a smooth migration of an application, all the underlying principles of multivalue databases must be retained and the code must be as readable and as writable as is was before. This is what Exodus provides. A change of syntax is unavoidable since Exodus uses a mainstream compiler, but it does not take long to learn and large bodies of existing code can be converted reliably and routinely.

Exodus uses PostgreSQL for all data storage while making it appear just like a completely traditional multivalue database - and without requiring any knowledge of SQL. All data definition is done in what appear to programmer/dba to be classic multivalue dictionary files.

Exodus has a tiny footprint, is thread-safe and can produce standalone programs or be linked into utilities, libraries or embedded projects - with or without database functionality. The possibilities are very broad. The Exodus library can be maintained by any average C++ programmer, there is nothing particularly difficult about its implementation. The best things are always simple, like the original Pick database - which looks like living on in yet another incarnation in Exodus.

LIST/ENGLISH/ACCESS:

Exodus supports various dictionary formats, all the common iconv/oconv functions and has a complete LIST/ENGLISH/ACCESS replacement that can output either text or html.

SQL:

In the very near future all multivalue data will also be readable via Postgresql's standard SQL interfaces. This is without any additional work on behalf of the programmer/dba beyond the maintenance of classic multivalue dictionary files.

Unicode:

All Exodus strings are Unicode and ordering is done using operating system collators. Internally, Exodus is UCS-2 on Windows 32bit and UTF-32 on Unix/Linux/Mac/Win64. File i/o can be in UTF-8 or codepage based.

Installation:

Exodus has a binary installer for Windows and OSX. Unix/Linux/Mac can be installed using configure/make/make install.

Exodus has no dependencies other than the obvious. If you want to write programs in Exodus, you need a C++ compiler. If you want to access or update multivalue data, you need Postgresql 8.1+, 9.0+.

Developing in Exodus/IDE:

To ease the life of Exodus programmers on all platforms, Exodus provides edic and other commands to edit/compile/catalog programs from any standard operating system command console. edic wraps the ubiquitous nano editor with color coding of exodus/c++ syntax and "jump to first error" after compilation errors. You can configure your own editor where desired. The Windows installer includes nano.

For traditional multivalue programmers, Exodus remains in the familiar world of "compiling" source code and executing "object code". Modern C++ compilers and debuggers are very sophisticated these days. When compiled in the default debug mode, Exodus provides a source level backtrace for run-time errors like "variable not assigned", "non-numeric data" etc. All exodus programs can be interrupted/resumed.

The massive range of commercial and open source tools is available to assist development in C++.

Deploying Exodus Applications:

In the standards-based world that Exodus inhabits, there is a huge variety of tools to deploy applications.

Source code:

If you want to build Exodus from source, you need Boost binaries 1.32 or later. Portability to other platforms is forecast to be high. The code base is quite small and comprehensible being mostly implementation of a single class called "var".

Exodus's command line utilities are all written in Exodus/C++ providing both portability and samples of real programming using Exodus.

History:

Exodus has been developed to date by NEOSYS to serve as the new back-end platform for the next generation of their vertical market package. It is nominally 90% complete and the input of multivalue professionals is sought before the API is frozen and put into production.

Roadmap:

Remaining development (none require major work)

  * mv-programmer-friendly way of doing labelled common
  * Sorting and indexing on multivalues
  * Completion of Exodus Programmer's Manual

Possible future development

  * Complete data visibility via SQL (automatic from multivalue dictionary)
  * Binary build for main Linux distros
  * Move everything onto sourceforge
  * Move unit tests into subversion
  * mv programmer friendly db management utils file/account-create/save/restore/
  * Ease to use dictionary editor instead of plain edit?
  * F correlatives
  * Port the Postgres plugin to other databases