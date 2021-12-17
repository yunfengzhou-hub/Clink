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

package org.clink.example;

import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.ml.api.AlgoOperator;
import org.apache.flink.ml.linalg.SparseVector;
import org.apache.flink.ml.param.Param;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.table.api.Table;
import org.apache.flink.table.api.bridge.java.StreamTableEnvironment;
import org.apache.flink.table.api.internal.TableImpl;
import org.apache.flink.types.Row;

import org.apache.flink.ml.proto.feature.onehotencoder.OneHotEncoderModel;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;

import java.io.IOException;
import java.util.Map;
import java.util.Properties;

/** Simple example of native library declaration and usage. */
public class Main {
    public interface ClinkKernels extends Library {
        ClinkKernels INSTANCE = (ClinkKernels) Native.loadLibrary("clink_kernels_jna", ClinkKernels.class);

        double SquareAdd(double x, double y);

        double Square(double x);
    }

    public static void main(String[] args) {
        System.out.println("Square result is " + ClinkKernels.INSTANCE.Square(3.0));
        System.out.println("SquareAdd result is " + ClinkKernels.INSTANCE.SquareAdd(1.0, 3.0));
    }
}
