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

package org.apache.flink.ml.proto.feature.onehotencoder;

import org.apache.flink.table.api.Table;

/**
 * Wrapper class for Flink ML OneHotEncoder which generates models that save as protobuf file.
 */
public class OneHotEncoder extends org.apache.flink.ml.feature.onehotencoder.OneHotEncoder {
    @Override
    public OneHotEncoderModel fit(Table... inputs) {
        org.apache.flink.ml.feature.onehotencoder.OneHotEncoderModel oriModel = super.fit(inputs);
        OneHotEncoderModel jnaModel = new OneHotEncoderModel();

        jnaModel.setDropLast(oriModel.getDropLast());
        jnaModel.setInputCols(oriModel.getInputCols());
        jnaModel.setOutputCols(oriModel.getOutputCols());
        jnaModel.setModelData(oriModel.getModelData());
        return jnaModel;
    }
}