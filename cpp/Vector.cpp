#include <iostream>
#include "Vector.h"

namespace clink {
    Vector::Vector(double *array) {
        this->values = new double[2];
        memcpy(values, (double *) array, 2 * sizeof(double));
    }

    std::string Vector::to_string() {
        std::string result = "";
        for (int i = 0; i < 2; i++) {
            result += std::to_string(this->values[i]);
            result += " ";
        }
        return result;
    }
}
