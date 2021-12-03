package org.apache.flink.ml.jni.operator;

import org.apache.flink.ml.jni.core.Vector;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

/** Tests VectorAddOperator. */
public class VectorAddOperatorTest {
    @Test
    public void testAddVectors() {
        Vector a = new Vector(new double[] {1.0, 2.0});
        Vector b = new Vector(new double[] {3.0, 4.0});
        Vector c = VectorAddOperator.add(a, b);
        assertEquals(c.toString(), "4.000000 6.000000 ");
    }
}
