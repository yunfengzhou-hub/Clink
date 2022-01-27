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

package org.flinkextended.clink.feature.onehotencoder;

import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.api.common.functions.MapPartitionFunction;
import org.apache.flink.api.common.functions.RichMapFunction;
import org.apache.flink.api.common.typeinfo.TypeInformation;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.api.java.typeutils.RowTypeInfo;
import org.apache.flink.configuration.Configuration;
import org.apache.flink.ml.api.Model;
import org.apache.flink.ml.common.broadcast.BroadcastUtils;
import org.apache.flink.ml.common.datastream.DataStreamUtils;
import org.apache.flink.ml.common.datastream.TableUtils;
import org.apache.flink.ml.common.param.HasHandleInvalid;
import org.apache.flink.ml.feature.onehotencoder.OneHotEncoderModelData;
import org.apache.flink.ml.feature.onehotencoder.OneHotEncoderParams;
import org.apache.flink.ml.linalg.SparseVector;
import org.apache.flink.ml.param.Param;
import org.apache.flink.ml.util.ReadWriteUtils;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.table.api.Table;
import org.apache.flink.table.api.bridge.java.StreamTableEnvironment;
import org.apache.flink.table.api.internal.TableImpl;
import org.apache.flink.table.runtime.typeutils.ExternalTypeInfo;
import org.apache.flink.types.Row;
import org.apache.flink.util.Collector;
import org.apache.flink.util.FileUtils;
import org.apache.flink.util.Preconditions;

import com.sun.jna.Pointer;
import org.apache.commons.lang3.ArrayUtils;
import org.flinkextended.clink.jna.ClinkJna;
import org.flinkextended.clink.jna.SparseVectorJna;
import org.flinkextended.clink.util.ByteArrayDecoder;
import org.flinkextended.clink.util.ByteArrayEncoder;
import org.flinkextended.clink.util.ClinkReadWriteUtils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.function.Function;

import static org.apache.flink.ml.util.ParamUtils.initializeMapWithDefaultValues;

/**
 * Wrapper class for Flink ML OneHotEncoderModel which calls equivalent C++ operator to transform.
 */
public class ClinkOneHotEncoderModel
        implements Model<ClinkOneHotEncoderModel>, OneHotEncoderParams<ClinkOneHotEncoderModel> {
    private final Map<Param<?>, Object> paramMap = new HashMap<>();
    private final String broadcastModelKey = "OneHotEncoderModelStream";
    private StreamExecutionEnvironment env;
    private String modelDataPath;
    private Table modelDataTable;

    public ClinkOneHotEncoderModel() {
        initializeMapWithDefaultValues(this.paramMap, this);
    }

    @Override
    public Table[] transform(Table... inputs) {
        final String[] inputCols = getInputCols();
        final String[] outputCols = getOutputCols();

        Preconditions.checkArgument(getHandleInvalid().equals(HasHandleInvalid.ERROR_INVALID));
        Preconditions.checkArgument(inputs.length == 1);
        Preconditions.checkArgument(inputCols.length == outputCols.length);

        RowTypeInfo inputTypeInfo = TableUtils.getRowTypeInfo(inputs[0].getResolvedSchema());
        RowTypeInfo outputTypeInfo =
                new RowTypeInfo(
                        ArrayUtils.addAll(
                                inputTypeInfo.getFieldTypes(),
                                Collections.nCopies(
                                                outputCols.length,
                                                ExternalTypeInfo.of(Vector.class))
                                        .toArray(new TypeInformation[0])),
                        ArrayUtils.addAll(inputTypeInfo.getFieldNames(), outputCols));

        GenerateOutputsFunction mapFunction =
                new GenerateOutputsFunction(
                        getParamMap(), broadcastModelKey, inputCols, modelDataPath);

        if (modelDataPath == null) {
            return transformNonPersistent(inputs, mapFunction, outputTypeInfo);
        } else {
            return transformPersistent(inputs, mapFunction, outputTypeInfo);
        }
    }

    /**
     * Performs transformation on the basis that params and model data are not in permanent storage.
     */
    private Table[] transformNonPersistent(
            Table[] inputs, GenerateOutputsFunction mapFunction, RowTypeInfo outputTypeInfo) {
        StreamTableEnvironment tEnv =
                (StreamTableEnvironment) ((TableImpl) modelDataTable).getTableEnvironment();
        DataStream<Row> input = tEnv.toDataStream(inputs[0]);

        DataStream<Tuple2<Integer, Integer>> modelStream =
                OneHotEncoderModelData.getModelDataStream(modelDataTable);

        Function<List<DataStream<?>>, DataStream<Row>> function =
                dataStreams -> {
                    DataStream stream = dataStreams.get(0);
                    return stream.map(mapFunction, outputTypeInfo);
                };

        DataStream<Row> output =
                BroadcastUtils.withBroadcastStream(
                        Collections.singletonList(input),
                        Collections.singletonMap(broadcastModelKey, modelStream),
                        function);

        Table outputTable = tEnv.fromDataStream(output);

        return new Table[] {outputTable};
    }

    /**
     * Performs transformation on the basis that params and model data live in permanent storage.
     */
    private Table[] transformPersistent(
            Table[] inputs, GenerateOutputsFunction mapFunction, RowTypeInfo outputTypeInfo) {
        StreamTableEnvironment tEnv = StreamTableEnvironment.create(env);
        DataStream<Row> input = tEnv.toDataStream(inputs[0]);

        DataStream<Row> output = input.map(mapFunction, outputTypeInfo);
        Table outputTable = tEnv.fromDataStream(output);
        return new Table[] {outputTable};
    }

    @Override
    public ClinkOneHotEncoderModel setModelData(Table... inputs) {
        Preconditions.checkArgument(inputs.length == 1);
        modelDataTable = inputs[0];
        return this;
    }

    @Override
    public Table[] getModelData() {
        // Materializes model data table if it has not been loaded from given path.
        if (modelDataTable == null && modelDataPath != null) {
            DataStream<byte[]> modelDataProtobuf =
                    ReadWriteUtils.loadModelData(env, modelDataPath, new ByteArrayDecoder());
            DataStream<Tuple2<Integer, Integer>> modelData =
                    modelDataProtobuf.flatMap(
                            new FlatMapFunction<byte[], Tuple2<Integer, Integer>>() {
                                @Override
                                public void flatMap(
                                        byte[] bytes, Collector<Tuple2<Integer, Integer>> collector)
                                        throws Exception {
                                    for (Tuple2<Integer, Integer> tup2 :
                                            OneHotEncoderProtobufUtils.getModelDataIterable(
                                                    bytes)) {
                                        collector.collect(tup2);
                                    }
                                }
                            });

            StreamTableEnvironment tEnv = StreamTableEnvironment.create(env);
            setModelData(tEnv.fromDataStream(modelData));
        }

        return new Table[] {modelDataTable};
    }

    @Override
    public Map<Param<?>, Object> getParamMap() {
        return paramMap;
    }

    private static class GenerateOutputsFunction extends RichMapFunction<Row, Row> {
        private final Map<Param<?>, Object> paramMap;
        private final String broadcastModelKey;
        private final String[] inputCols;
        private final String modelDataPath;
        private Pointer modelPointer = null;

        private GenerateOutputsFunction(
                Map<Param<?>, Object> paramMap,
                String broadcastModelKey,
                String[] inputCols,
                String modelDataPath) {
            this.paramMap = new HashMap<>(paramMap);
            this.inputCols = inputCols;
            this.broadcastModelKey = broadcastModelKey;
            this.modelDataPath = modelDataPath;
        }

        @Override
        public void open(Configuration parameters) throws Exception {
            super.open(parameters);
            if (modelDataPath != null) {
                modelPointer = ClinkJna.INSTANCE.OneHotEncoderModel_load(modelDataPath);
            }
        }

        @Override
        public Row map(Row row) throws IOException {
            if (modelPointer == null) {
                List<Tuple2<Integer, Integer>> modelDataList =
                        getRuntimeContext().getBroadcastVariable(broadcastModelKey);
                modelPointer = loadCppModel(paramMap, modelDataList);
            }

            Row resultRow = new Row(inputCols.length);
            for (int i = 0; i < inputCols.length; i++) {
                String inputCol = inputCols[i];
                int number = ((Number) row.getField(inputCol)).intValue();
                SparseVectorJna.ByReference jnaVector =
                        ClinkJna.INSTANCE.OneHotEncoderModel_transform(modelPointer, number, i);
                SparseVector vector = jnaVector.toSparseVector();
                ClinkJna.INSTANCE.SparseVector_delete(jnaVector);
                resultRow.setField(i, vector);
            }
            return Row.join(row, resultRow);
        }

        @Override
        public void close() throws Exception {
            super.close();
            if (modelPointer != null) {
                ClinkJna.INSTANCE.OneHotEncoderModel_delete(modelPointer);
                modelPointer = null;
            }
        }
    }

    private static Pointer loadCppModel(
            Map<Param<?>, Object> paramMap, List<Tuple2<Integer, Integer>> modelDataList)
            throws IOException {
        File tmpDir = Files.createTempDirectory("ClinkOneHotEncoderModel").toFile();
        String tmpDirStr = tmpDir.getAbsolutePath();

        ClinkReadWriteUtils.saveMetadata(paramMap, ClinkOneHotEncoderModel.class, tmpDirStr);

        byte[] modelDataBytes = OneHotEncoderProtobufUtils.getModelDataByteArray(modelDataList);

        new File(tmpDir, "data").mkdirs();
        OutputStream modelDataOutput =
                new FileOutputStream(Paths.get(tmpDirStr, "data", "modelData").toFile());

        new ByteArrayEncoder().encode(modelDataBytes, modelDataOutput);

        Pointer model = ClinkJna.INSTANCE.OneHotEncoderModel_load(tmpDirStr);
        FileUtils.deleteDirectory(tmpDir);
        return model;
    }

    @Override
    public void save(String path) throws IOException {
        ClinkReadWriteUtils.saveMetadata(getParamMap(), getClass(), path);

        DataStream<byte[]> modelDataProtoBuf =
                DataStreamUtils.mapPartition(
                        OneHotEncoderModelData.getModelDataStream(getModelData()[0]),
                        new GenerateProtobufModelDataByteArrayFunction());
        modelDataProtoBuf.getTransformation().setParallelism(1);

        ReadWriteUtils.saveModelData(modelDataProtoBuf, path, new ByteArrayEncoder());
    }

    private static class GenerateProtobufModelDataByteArrayFunction
            implements MapPartitionFunction<Tuple2<Integer, Integer>, byte[]> {
        @Override
        public void mapPartition(
                Iterable<Tuple2<Integer, Integer>> iterable, Collector<byte[]> collector) {
            collector.collect(OneHotEncoderProtobufUtils.getModelDataByteArray(iterable));
        }
    }

    public static ClinkOneHotEncoderModel load(StreamExecutionEnvironment env, String path)
            throws IOException {
        ClinkOneHotEncoderModel clinkModel =
                (ClinkOneHotEncoderModel) ReadWriteUtils.loadStageParam(path);

        // Load model data lazily to avoid IO operation if possible.
        clinkModel.modelDataPath = path;
        clinkModel.env = env;

        return clinkModel;
    }
}
