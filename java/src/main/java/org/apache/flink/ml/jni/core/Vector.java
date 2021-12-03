package org.apache.flink.ml.jni.core;

import org.apache.flink.ml.jni.AbstractJNIClass;

/** A vector of double values. */
public class Vector extends AbstractJNIClass {
    public long addr;

    public Vector() {}

    public Vector(double[] values) {
        this.addr = createNaiveObject(values);
    }

    /**
     * Creates a {@link Vector} object from a corresponding C++ object.
     *
     * @param addr Address of the C++ object
     */
    public static Vector fromCppObj(long addr) {
        Vector vector = new Vector();
        vector.addr = addr;
        return vector;
    }

    @Override
    public String toString() {
        return toString(addr);
    }

    private static native String toString(long addr);

    private static native long createNaiveObject(double[] values);

    public static void main(String[] args) {
        Vector a = new Vector(new double[] {1.0, 2.0});
        System.out.println(a);
    }
}
