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
import java.net.URLClassLoader;
import java.util.Enumeration;
import java.util.Set;
import java.util.TreeSet;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/** Run all tests in org.clink. */
@RunWith(org.junit.runners.AllTests.class)
public class AllTestsRunner {
    public static TestSuite suite() throws Exception {
        TestSuite suite = new TestSuite();
        URLClassLoader classLoader =
                (URLClassLoader) Thread.currentThread().getContextClassLoader();
        File jarFile = new File(classLoader.getURLs()[0].getPath());
        ZipFile zipFile = new ZipFile(jarFile);
        Enumeration<? extends ZipEntry> entries = zipFile.entries();
        Set<String> classNames = new TreeSet<>();
        while (entries.hasMoreElements()) {
            String entryName = entries.nextElement().getName();
            if (isTestClass(entryName)) {
                int classNameEnd = entryName.length() - ".class".length();
                String className = entryName.substring(0, classNameEnd).replace('/', '.');
                classNames.add(className);
            }
        }
        for (String className : classNames) {
            Class<?> clazz = Class.forName(className);
            suite.addTest(new JUnit4TestAdapter(clazz));
        }
        return suite;
    }

    private static boolean isTestClass(String entryName) {
        return entryName.startsWith("org/clink")
                && !entryName.startsWith("org/clink/util")
                && entryName.endsWith(".class");
    }
}
