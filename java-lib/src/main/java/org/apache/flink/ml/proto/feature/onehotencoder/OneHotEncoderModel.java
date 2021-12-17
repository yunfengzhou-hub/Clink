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
 * Wrapper class for Flink ML OneHotEncoderModel which saves as protobuf file.
 */
public class OneHotEncoderModel extends org.apache.flink.ml.feature.onehotencoder.OneHotEncoderModel {
    private long modelCppAddr;

    @Override
    public Table[] transform(Table... inputs) {
        throw new UnsupportedOperationException();
    }

    @Override
    public void save(String path) throws IOException {
        OneHotEncoderParams.Builder paramsBuilder = OneHotEncoderParams.newBuilder();
        paramsBuilder.setIsDropLast(getDropLast());
        for (String inputCol: getInputCols()) {
            paramsBuilder.addInputCols(inputCol);
        }
        for (String outputCol: getOutputCols()) {
            paramsBuilder.addOutputCols(outputCol);
        }
        OneHotEncoderParams params = paramsBuilder.build();

        ReadWriteUtils.saveMetadata(params, path);

        StreamTableEnvironment tEnv =
                (StreamTableEnvironment) ((TableImpl) getModelData()[0]).getTableEnvironment();

        OneHotEncoderModelData.Builder builder = OneHotEncoderModelData.newBuilder();
        try {
            List<Tuple2<Integer,Integer>> list = IteratorUtils.toList(
                tEnv.toDataStream(getModelData()[0])
                .map(
                        new MapFunction<Row, Tuple2<Integer, Integer>>() {
                            @Override
                            public Tuple2<Integer, Integer> map(Row row) {
                                return new Tuple2<>(
                                        (int) row.getField("f0"), (int) row.getField("f1"));
                            }
                        }).executeAndCollect());
            list.sort(Comparator.comparingInt(o -> o.f0));
            for(Tuple2<Integer,Integer> tuple: list) {
                builder.addFeatureSize(tuple.f1);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        OneHotEncoderModelData modelData = builder.build();
        
        ReadWriteUtils.saveModelData(modelData, path);
    }

    public static OneHotEncoderModel load(StreamExecutionEnvironment env, String path) throws IOException {
        throw new UnsupportedOperationException();
    }
}