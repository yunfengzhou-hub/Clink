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

package org.clink.util;

import com.sun.jna.LastErrorException;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import org.clink.linalg.SparseVectorJNA;

/** Utility methods that have implementations in C++. */
public interface ClinkJNA extends Library {
    ClinkJNA INSTANCE = Native.load("clink_jna", ClinkJNA.class);

    double SquareAdd(double x, double y);

    double Square(double x);

    /**
     * Loads a {@link org.apache.flink.ml.feature.onehotencoder.OneHotEncoderModel} C++ operator
     * using parameters already stored in memory.
     *
     * @param paramsJson Json string representing the params of the Model
     * @param modelDataProtoBuf Pointer to a memory chunk containing model data converted to
     *     ProtoBuf byte array
     * @param modelDataProtoBufLen Length of the model data byte array
     * @return Pointer to the loaded C++ Operator
     */
    Pointer OneHotEncoderModel_loadFromMemory(
            String paramsJson, Pointer modelDataProtoBuf, int modelDataProtoBufLen)
            throws LastErrorException;

    /**
     * Converts an indexed integer to one-hot-encoded sparse vector, using the {@link
     * org.apache.flink.ml.feature.onehotencoder.OneHotEncoderModel} C++ operator.
     *
     * @param modelPointer Pointer to the OneHotEncoder C++ operator
     * @param value The indexed integer to be converted
     * @param columnIndex The column index which the indexed integer locates
     * @return A one-hot-encoded sparse vector
     */
    SparseVectorJNA.ByValue OneHotEncoderModel_transform(
            Pointer modelPointer, int value, int columnIndex) throws LastErrorException;

    /**
     * Deletes a {@link org.apache.flink.ml.feature.onehotencoder.OneHotEncoderModel} C++ operator
     * in order to avoid memory leak.
     *
     * @param modelPointer Pointer to the OneHotEncoder C++ operator
     */
    void OneHotEncoderModel_delete(Pointer modelPointer);
}
