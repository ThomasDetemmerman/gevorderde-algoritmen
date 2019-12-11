//
// Created by Thomas on 11/12/2019.
//

#ifndef INC_10_4_1_ELIASCODE_ELIASCODE_H
#define INC_10_4_1_ELIASCODE_ELIASCODE_H

#include <string>

class EliasCode {
    virtual void encode(const char *bestandsnaam, const char *outname);
    virtual void decode(const char *bestandsnaam, const char *outname);
};


#endif //INC_10_4_1_ELIASCODE_ELIASCODE_H
