#include "Vector.h"
#include "org_apache_flink_ml_jni_core_Vector.h"

JNIEXPORT jstring JNICALL Java_org_apache_flink_ml_jni_core_Vector_toString
  (JNIEnv *env, jclass, jlong addr) {
        clink::Vector *vector = (clink::Vector *)addr;
        return env->NewStringUTF(vector->to_string().c_str());
  }

JNIEXPORT jlong JNICALL Java_org_apache_flink_ml_jni_core_Vector_createNaiveObject
  (JNIEnv *env, jclass, jdoubleArray array) {
      jboolean isCopy;
      jdouble *elements = env->GetDoubleArrayElements(array, &isCopy);
      clink::Vector *vector = new clink::Vector(elements);
      return (jlong) vector;
  }