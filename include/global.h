#ifndef GLOBAL_H
#define GLOBAL_H

#include <engine/level.h>

class Global {
    public:
        static inline int width = 800;
        static inline int height = 600;

        static inline double last_frame_time = 0;

        static inline Level *level;
};

#endif
