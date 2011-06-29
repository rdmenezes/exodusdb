/*
Copyright (c) 2009 steve.bush@neosys.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef MVUTF_H
#define MVUTF_H 1

#include <string>
#include "ConvertUTF.h"

namespace exodus
{
// mvutf.cpp
std::string toUTF8(const std::wstring& wstr1);
std::wstring wstringfromUTF8(const UTF8* sourcestart, const size_t sourcelength);
std::string stringfromUTF16(const UTF16* sourcestart, const size_t sourcelength);
std::string stringfromUTF32(const UTF32* sourcestart, const size_t sourcelength);
std::wstring wstringfromchars(const char* sourcestart, const size_t sourcelength);

}//of namespace exodus

#endif //MYUTF_H
