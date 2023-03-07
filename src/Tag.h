//
// Created by felix on 3/7/23.
//

#ifndef PICTURE_POSSUM_TAG_H
#define PICTURE_POSSUM_TAG_H


#include <string>

class Color {
public:
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff);

private:
    unsigned char r,g,b,a;
};
class Tag {
public:
    std::string identifier;
    std::string emoji;
    std::string name;
    Color color;
    Tag(std::string identifier, std::string emoji, std::string name, const Color &color);
};


#endif //PICTURE_POSSUM_TAG_H
