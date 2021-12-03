package org.apache.flink.ml.jni;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Abstract class that loads native methods from .so file. All classes that have native methods
 * should extend this class.
 */
public abstract class AbstractJNIClass {
    static {
        try {
            InputStream is =
                    ClassLoader.class.getResourceAsStream("/org/apache/flink/ml/jni/libC.so");
            File file = File.createTempFile("lib", ".so");
            OutputStream os = new FileOutputStream(file);
            byte[] buffer = new byte[1024];
            int length;
            while ((length = is.read(buffer)) != -1) {
                os.write(buffer, 0, length);
            }
            is.close();
            os.close();

            System.load(file.getAbsolutePath());
            file.deleteOnExit();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
