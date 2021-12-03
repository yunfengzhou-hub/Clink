#include <jni.h>
#include "VectorAddOperator.h"
#include "org_apache_flink_ml_jni_operator_VectorAddOperator.h"
#include "Vector.h"
#include "org_apache_flink_ml_jni_core_Vector.h"

using namespace clink;

JNIEXPORT jlong JNICALL Java_org_apache_flink_ml_jni_operator_VectorAddOperator_add_1cpp
  (JNIEnv *env, jclass clazz, jlong addr1, jlong addr2) {
      Vector *vector1 = (Vector *)addr1;
      Vector *vector2 = (Vector *)addr2;
      return (jlong) add(vector1, vector2);
  }