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

import junit.framework.JUnit4TestAdapter;
import junit.framework.TestSuite;
import org.junit.runner.RunWith;

import java.io.File;
import java.io.IOException;
import java.net.URLClassLoader;
import java.util.Enumeration;
import java.util.Set;
import java.util.TreeSet;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Run all tests in org.clink.
 *
 * <p>Code below is referenced from https://stackoverflow.com/a/46368961.
 */
@RunWith(org.junit.runners.AllTests.class)
public class AllTestsRunner {
    private static final String entryNamePrefix = "org/clink";

    public static TestSuite suite() throws IOException {
        TestSuite suite = new TestSuite();
        URLClassLoader classLoader =
                (URLClassLoader) Thread.currentThread().getContextClassLoader();
        // The first entry on the classpath contains the srcs from java_test
        findClassesInJar(new File(classLoader.getURLs()[0].getPath())).stream()
                .map(
                        c -> {
                            try {
                                return Class.forName(c);
                            } catch (ClassNotFoundException e) {
                                throw new RuntimeException(e);
                            }
                        })
                .filter(AllTestsRunner::isTestClass)
                .map(JUnit4TestAdapter::new)
                .forEach(suite::addTest);
        return suite;
    }

    private static boolean isTestClass(Class<?> clazz) {
        return !clazz.getName().startsWith("org.clink.util");
    }

    private static Set<String> findClassesInJar(File jarFile) {
        Set<String> classNames = new TreeSet<>();
        try {
            try (ZipFile zipFile = new ZipFile(jarFile)) {
                Enumeration<? extends ZipEntry> entries = zipFile.entries();
                while (entries.hasMoreElements()) {
                    String entryName = entries.nextElement().getName();
                    if (entryName.startsWith(entryNamePrefix) && entryName.endsWith(".class")) {
                        int classNameEnd = entryName.length() - ".class".length();
                        classNames.add(entryName.substring(0, classNameEnd).replace('/', '.'));
                    }
                }
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return classNames;
    }
}
