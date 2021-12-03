.PHONY: cpp

.PHONY: java

default_target: cpp

cpp:
	cd cpp && g++ Vector.cpp VectorAddOperator.cpp -o libC_native.so -fPIC -shared -I ${JAVA_HOME}/include/darwin -I ${JAVA_HOME}/include

java:
	mkdir -p java/src/main/resources/org/apache/flink/ml/jni
	g++ cpp/Vector.cpp cpp/org_apache_flink_ml_jni_core_Vector.cpp cpp/VectorAddOperator.cpp cpp/org_apache_flink_ml_jni_operator_VectorAddOperator.cpp -o java/src/main/resources/org/apache/flink/ml/jni/libC.so -fPIC -shared -I ${JAVA_HOME}/include/darwin -I ${JAVA_HOME}/include
	cd java && mvn package

clean:
	rm *.o *.so *.class