#ifndef HARD_DRIVE_HELPER_H__TOSTITOS
#define HARD_DRIVE_HELPER_H__TOSTITOS

#include "../common/typeDescriptions.h"

#include <string>


namespace CommunicationWithHost
{
    // read from host to virtual drive
    Common::ByteArray ReadFromHost(std::string path);

    // write to host from virtual disk
    void WtiteToHost(std::string path, Common::ByteArray data);
};

#endif
