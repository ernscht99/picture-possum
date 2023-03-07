//
// Created by felix on 3/7/23.
//

#include "Tag.h"

#include <utility>

Tag::Tag(std::string identifier, std::string emoji, std::string name, const Color &color)
        : identifier(std::move(identifier)), emoji(std::move(emoji)), name(std::move(name)), color(color) {}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
