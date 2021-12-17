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

import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.ml.api.AlgoOperator;
import org.apache.flink.ml.linalg.SparseVector;
import org.apache.flink.ml.param.Param;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.table.api.Table;
import org.apache.flink.table.api.bridge.java.StreamTableEnvironment;
import org.apache.flink.table.api.internal.TableImpl;
import org.apache.flink.types.Row;

import org.apache.flink.ml.proto.feature.onehotencoder.OneHotEncoderParams;
import org.apache.flink.ml.proto.feature.onehotencoder.OneHotEncoderModelData;

import org.apache.flink.ml.jna.util.ReadWriteUtils;
import org.apache.flink.ml.jna.util.JNAUtils;

import org.apache.commons.collections.IteratorUtils;

import java.io.IOException;
import java.io.FileInputStream;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Properties;

import java.lang.UnsupportedOperationException;

/**
 * Wrapper class for Flink ML OneHotEncoderModel which calls equivalent C++ operator to transform.
 */
public class OneHotEncoderModel extends org.apache.flink.ml.feature.onehotencoder.OneHotEncoderModel {
    private long modelCppAddr;

    @Override
    public Table[] transform(Table... inputs) {
        String[] inputCols = getInputCols();
        StreamTableEnvironment tEnv =
                (StreamTableEnvironment) ((TableImpl) inputs[0]).getTableEnvironment();

        DataStream<Row> output = tEnv.toDataStream(inputs[0])
                .map(new MapFunction<Row, Row>() {
                    @Override
                    public Row map(Row row) throws Exception {
                        Row resultRow = new Row(inputCols.length);
                        for (int i = 0; i < inputCols.length; i++) {
                            String inputCol = inputCols[i];
                            int number = ((Number) row.getField(inputCol)).intValue();
                            org.apache.flink.ml.jna.linalg.SparseVector.ByValue vectorCpp = 
                                OneHotEncoderNative.INSTANCE.OneHotEncoderModel_transform(modelCppAddr, number, i);
                            SparseVector vector = vectorCpp.toSparseVector();
                            resultRow.setField(i, vector);
                        }
                        return Row.join(row, resultRow);
                    }
                });
        
        return new Table[]{tEnv.fromDataStream(output)};
    }

    @Override
    public void save(String path) throws IOException {
        throw new UnsupportedOperationException();
    }

    public static OneHotEncoderModel load(StreamExecutionEnvironment env, String path) throws IOException {
        OneHotEncoderModel model = new OneHotEncoderModel();
        model.modelCppAddr = OneHotEncoderNative.INSTANCE.OneHotEncoderModel_load(path);

        OneHotEncoderParams params = OneHotEncoderParams.parseFrom(
            new FileInputStream(path + "/metadata")
        );

        model.setInputCols(params.getInputColsList().toArray(new String[0]));
        return model;
    }

    @Override
    protected void finalize() {
        JNAUtils.deleteObject(modelCppAddr);
    }
}