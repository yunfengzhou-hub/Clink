/*
 * Copyright 2021 The Clink Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.flink.ml.jna.linalg;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.Structure.FieldOrder;

@FieldOrder({"n", "indices", "indicesLen", "values", "valuesLen"})
public class SparseVector extends Structure {
    public static class ByValue extends SparseVector implements Structure.ByValue{}
    public int n;
    public Pointer indices;
    public int indicesLen;
    public Pointer values;
    public int valuesLen;

    public org.apache.flink.ml.linalg.SparseVector toSparseVector() {
        return new org.apache.flink.ml.linalg.SparseVector(
            n, indices.getIntArray(0, indicesLen), values.getDoubleArray(0, valuesLen));
    }
}