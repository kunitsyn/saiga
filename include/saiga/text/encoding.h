/**
 * Copyright (c) 2017 Darius Rückert 
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#pragma once

#include "saiga/text/textParameters.h"
#include "saiga/rendering/object3d.h"
#include "saiga/geometry/triangle_mesh.h"

#include <cstdint>

namespace Saiga {

typedef std::vector<uint32_t> utf32string;

namespace Unicode{
//see https://en.wikipedia.org/wiki/Unicode_block
struct SAIGA_GLOBAL UnicodeBlock{
    uint32_t start;
    uint32_t end;
};


static const UnicodeBlock BasicLatin = {0,0x7F};
static const UnicodeBlock LatinSupplement = {0x80,0xFF};
static const UnicodeBlock LatinExtendedA = {0x100,0x17F};
static const UnicodeBlock LatinExtendedB = {0x180,0x24F};
static const UnicodeBlock Cyrillic = {0x400,0x4FF};
static const UnicodeBlock CyrillicSupplement = {0x500,0x52F};
}

class SAIGA_GLOBAL Encoding {
public:

    static uint32_t UTF8toUTF32(const std::vector<unsigned char> &utf8char);
    static utf32string UTF8toUTF32(const std::string& str);

    static std::vector<unsigned char> UTF32toUTF8(uint32_t utf32char);
    static std::string UTF32toUTF8(const utf32string &str);
};

}
