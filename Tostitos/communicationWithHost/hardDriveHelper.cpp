#include "hardDriveHelper.h"

#include <assert.h>

#include <fstream>

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;

using Common::ByteArray;
using std::ios;

ByteArray CommunicationWithHost::ReadFromHost(string path)
{
    ifstream file(path, ios::binary | ios::ate);
    assert(file.is_open());
    
    auto size = file.tellg();

    file.seekg(0, ios::beg);
    
    ByteArray data(size);
    file.read((char*) &data[0], size);

    return data;
}

void CommunicationWithHost::WtiteToHost(string path, ByteArray data)
{
    ofstream file(path, ios::binary);
    file.write((char*) &data[0], data.size());
}