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

package org.apache.flink.ml.jna.feature.onehotencoder;

import com.sun.jna.Library;
import com.sun.jna.Native;

public interface OneHotEncoderNative extends Library {
    OneHotEncoderNative INSTANCE = (OneHotEncoderNative) 
        Native.loadLibrary("one_hot_encoder_jna", OneHotEncoderNative.class);

    long OneHotEncoderModel_load(String filename);

    org.apache.flink.ml.jna.linalg.SparseVector.ByValue OneHotEncoderModel_transform(
        long modelCppAddr, int x, int columnIndex);
}