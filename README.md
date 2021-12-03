本文件夹展示了在已有C++文件的基础上编写java接口的代码示例。

编译代码生成jar包
```bash
$ make java
```

上述过程会运行java目录下的unit test做正确性验证。用户也可以通过直接运行jar包中的类以查看运行结果。

```bash
$ java -cp java/target/addoperator-0.0.1-SNAPSHOT-jar-with-dependencies.jar org.apache.flink.ml.jni.operator.VectorAddOperator
4.000000 6.000000 
```