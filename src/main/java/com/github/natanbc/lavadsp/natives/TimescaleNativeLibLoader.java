/*
 * Copyright 2018 natanbc
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.github.natanbc.lavadsp.natives;

import com.github.natanbc.nativeloader.NativeLibLoader;
import com.github.natanbc.nativeloader.feature.X86Feature;
import com.github.natanbc.nativeloader.system.DefaultArchitectureTypes;
import com.github.natanbc.nativeloader.system.ArchitectureType;

import java.util.Set;

public class TimescaleNativeLibLoader {
    private static final NativeLibLoader LOADER = NativeLibLoader.create(TimescaleNativeLibLoader.class, "timescale");
    private static final NativeLibLoader LOADER_AVX2 = NativeLibLoader.create(TimescaleNativeLibLoader.class, "timescale").withFeature(X86Feature.AVX2);
    private static final Set<ArchitectureType> EXCLUDE_AVX2 = Set.of(DefaultArchitectureTypes.ARM, DefaultArchitectureTypes.ARM_HF, DefaultArchitectureTypes.ARMv8_32, DefaultArchitectureTypes.ARMv8_64);
    private static volatile boolean loaded = false;
    private static volatile String soundTouchVersion;
    private static volatile int soundTouchVersionID;
    private static volatile boolean criticalNativesAvailable;

    public static void loadTimescaleLibrary() {
        if(loaded) return;

        // override for arm, armhf, aarch32, aarch64 to load the non-avx2 version
        if(EXCLUDE_AVX2.contains(DefaultArchitectureTypes.detect())) {
            LOADER.load();
        } else {
            LOADER_AVX2.load();
        }
        soundTouchVersion = TimescaleLibrary.soundTouchVersion();
        soundTouchVersionID = TimescaleLibrary.soundTouchVersionID();
        criticalNativesAvailable = TimescaleLibrary.criticalMethodsAvailable();
        loaded = true;
    }

    public static boolean isLoaded() {
        return loaded;
    }

    public static String getSoundTouchVersion() {
        loadTimescaleLibrary();
        return soundTouchVersion;
    }

    public static int getSoundTouchVersionID() {
        loadTimescaleLibrary();
        return soundTouchVersionID;
    }

    public static boolean areCriticalNativesAvailable() {
        loadTimescaleLibrary();
        return criticalNativesAvailable;
    }
}