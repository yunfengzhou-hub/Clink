package org.apache.flink.ml.jni.operator;

import org.apache.flink.ml.jni.AbstractJNIClass;
import org.apache.flink.ml.jni.core.Vector;

/** Operator that adds two vectors. */
public class VectorAddOperator extends AbstractJNIClass {
    /** Add two vectors. */
    public static Vector add(Vector a, Vector b) {
        long addr = add_cpp(a.addr, b.addr);
        return Vector.fromCppObj(addr);
    }

    private static native long add_cpp(long addr1, long addr2);

    public static void main(String[] args) {
        Vector a = new Vector(new double[] {1.0, 2.0});
        Vector b = new Vector(new double[] {3.0, 4.0});
        VectorAddOperator op = new VectorAddOperator();
        Vector c = op.add(a, b);
        System.out.println(c);
    }
}
