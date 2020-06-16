//
// Created by felix on 08.06.20.
//

#include "FileInput.h"
#include <iostream>
#include <fstream>

sp<std::vector<char>> geo::FileInput::read_file(std::filesystem::path url) {
    auto content = std::make_shared<std::vector<char>>();

    try {
        std::ifstream file(url.string(), std::ios::binary | std::ios::ate);

        if (file.is_open()) {

            content->resize((size_t)file.tellg());
            file.seekg(0);
            file.read(content->data(), content->capacity());

            file.close();

        } else {
            // TODO: insert Exception
            std::cerr << "open file " << url << " failed!" << std::endl;
        }

    } catch (std::exception& ex) {
        // TODO: insert Exception
        std::cerr << "Loading File failed due to exception: " << ex.what() << std::endl;
    }

    return content;
}
