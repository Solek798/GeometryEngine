//
// Created by felix on 08.06.20.
//

#ifndef GEOMETRYENGINE_FILEINPUT_H
#define GEOMETRYENGINE_FILEINPUT_H


#include <vector>
#include <filesystem>
#include "../GlobaleScope.h"

namespace geo {
    class FileInput {
    public:

        static sp<std::vector<char>> read_file(std::filesystem::path);
    };
}


#endif //GEOMETRYENGINE_FILEINPUT_H
