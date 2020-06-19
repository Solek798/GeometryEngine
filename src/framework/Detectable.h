//
// Created by felix on 18.06.20.
//

#ifndef GEOMETRYENGINE_DETECTABLE_H
#define GEOMETRYENGINE_DETECTABLE_H


namespace geo::framework {
    class Detectable {
    protected:
        Detectable();
        virtual ~Detectable();

        void markAsUpdated();

        bool registered;

    private:
        friend class Framework;

        bool wasUpdated;
    };
}


#endif //GEOMETRYENGINE_DETECTABLE_H
