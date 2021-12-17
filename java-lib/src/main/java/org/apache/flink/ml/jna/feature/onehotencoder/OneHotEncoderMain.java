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

import org.apache.flink.ml.proto.feature.onehotencoder.OneHotEncoder;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.table.api.Table;
import org.apache.flink.table.api.bridge.java.StreamTableEnvironment;
import org.apache.flink.types.Row;

import java.nio.file.Files;
import java.nio.file.Path;

/** Example of making one hot encoder transformation with loaded C++ operator. */
public class OneHotEncoderMain {
    public static void main(String[] args) throws Exception {
        StreamExecutionEnvironment env = StreamExecutionEnvironment.createLocalEnvironment();
        StreamTableEnvironment tEnv = StreamTableEnvironment.create(env);

        OneHotEncoder estimator = new OneHotEncoder();
        estimator.setInputCols("a", "b");
        estimator.setOutputCols("c", "d");
        estimator.setDropLast(false);

        DataStream<Row> inputStream = env.fromElements(
                Row.of(0, 1),
                Row.of(2, 3)
        );
        Table inputTable = tEnv.fromDataStream(inputStream).as("a", "b");

        org.apache.flink.ml.proto.feature.onehotencoder.OneHotEncoderModel model = estimator.fit(inputTable);

        Path saveDir = Files.createTempDirectory("temp");
        String savePath = saveDir.toAbsolutePath().toString();

        model.save(savePath);

        org.apache.flink.ml.jna.feature.onehotencoder.OneHotEncoderModel model2 = 
            org.apache.flink.ml.jna.feature.onehotencoder.OneHotEncoderModel.load(env, savePath);

        Table outputTable = model2.transform(inputTable)[0];

        System.out.println(outputTable.execute().collect().next());
    }
}
