package org.apache.flink.ml.jni.core;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

/** Tests vector. */
public class VectorTest {
    @Test
    public void testVector() {
        Vector a = new Vector(new double[] {1.0, 2.0});
        assertEquals(a.toString(), "1.000000 2.000000 ");
    }
}
