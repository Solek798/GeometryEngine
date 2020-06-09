//
// Created by felix on 09.06.20.
//

#ifndef GEOMETRYENGINE_COMMAND_H
#define GEOMETRYENGINE_COMMAND_H


#include "Manageable.h"

namespace geo {
    class Command : public Manageable {
    public:
        Command();
        virtual ~Command() = default;

        void setup() override;
        void shutdown() override;

    private:

    };
}


#endif //GEOMETRYENGINE_COMMAND_H
