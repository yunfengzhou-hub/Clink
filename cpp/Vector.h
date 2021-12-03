#ifndef Vector_native
#define Vector_native
#include <string>

namespace clink {
    class Vector {
        public:
            double * values;
            Vector(double *);
            std::string to_string();
    };
}

#endif