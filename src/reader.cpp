#include "reader.hpp"

u1 Reader::read_u1(FILE *fp){
    unsigned char res;
    fread(&res, 1, 1, fp);
    return res;
}

u2 Reader::read_u2(FILE* fp){
    unsigned short res = 0;
    unsigned char temp;

    fread(&res, 1, 1, fp);
    fread(&temp, 1, 1, fp);

    res = res << 8;
    res |= temp;
    return res;
}

u4 Reader::read_u4(FILE* fp){
    unsigned int res = 0;
    unsigned char temp;

    for (int i = 0; i < 4; i++){
        res = res << 8;
        fread(&temp, 1, 1, fp);
        res |= temp;  
    }

    return res;
}