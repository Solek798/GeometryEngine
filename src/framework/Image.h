//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_IMAGE_H
#define GEOMETRYENGINE_IMAGE_H


namespace geo::framework {
    class Image {
    public:
        Image();
        Image(const char* path);
        virtual ~Image() = default;
    };
}


#endif //GEOMETRYENGINE_IMAGE_H
