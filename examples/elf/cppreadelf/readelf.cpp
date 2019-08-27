// readelf.cpp
// Demonstrates reading an ELF format file

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include "elf.h"

using namespace std;

int main(int argc, char **argv)
{
    ifstream strm;
    Elf32_Ehdr elfHeader;

    if (argc != 2) {
        cerr << "Usage: readelf <elf executable>" << endl;
        return 0;
    }

    strm.open(argv[1]); // prog.exe should be in ELF format

    if (!strm) {
        cerr << "Unable to open input file.";
        return 0;
    }

    // read ELF header
    strm.read((char *)&elfHeader, sizeof(elfHeader));

    if (!strm) {
        cerr << "Error reading ELF header.";
        return 0;
    }

    if (!(elfHeader.e_ident[0] == '\x7f' &&
          elfHeader.e_ident[1] == 'E' &&
          elfHeader.e_ident[2] == 'L' &&
          elfHeader.e_ident[3] == 'F')) {
        cerr << "Input file is not an ELF file.";
        return 0;
    }

    cout << "Entry point:" << elfHeader.e_entry << endl;

    Elf32_Phdr progHeader;
    strm.seekg(elfHeader.e_phoff); // seek to first program header entry

    char *data = new char[elfHeader.e_phentsize];

    // read first program header entry
    strm.read(data, elfHeader.e_phentsize);
    memcpy(&progHeader, data, min(sizeof(progHeader), (unsigned int) elfHeader.e_phentsize));

    cout << "Segment offset: " << progHeader.p_offset << endl;
    cout << "Segment length: " << progHeader.p_filesz << endl;
    cout << "Segment load address: " << progHeader.p_paddr << endl;
    strm.close();

    return 0;
}

