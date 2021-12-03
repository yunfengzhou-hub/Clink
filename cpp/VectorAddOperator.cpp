#include "VectorAddOperator.h"

namespace clink {
    Vector* add(Vector* vector1, Vector* vector2) {
      double *values = new double[2];
      for (int i = 0; i < 2; i++) {
          values[i] = vector1->values[i] + vector2->values[i];
      }
      return new Vector(values);
    }
}
