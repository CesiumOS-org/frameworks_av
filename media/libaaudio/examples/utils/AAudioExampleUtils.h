/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef AAUDIO_EXAMPLE_UTILS_H
#define AAUDIO_EXAMPLE_UTILS_H

#include <unistd.h>
#include <sched.h>
#include <aaudio/AAudio.h>

#define NANOS_PER_MICROSECOND ((int64_t)1000)
#define NANOS_PER_MILLISECOND (NANOS_PER_MICROSECOND * 1000)
#define NANOS_PER_SECOND      (NANOS_PER_MILLISECOND * 1000)

const char *getSharingModeText(aaudio_sharing_mode_t mode) {
    const char *modeText = "unknown";
    switch (mode) {
    case AAUDIO_SHARING_MODE_EXCLUSIVE:
        modeText = "EXCLUSIVE";
        break;
    case AAUDIO_SHARING_MODE_SHARED:
        modeText = "SHARED";
        break;
    default:
        break;
    }
    return modeText;
}

static int64_t getNanoseconds(clockid_t clockId = CLOCK_MONOTONIC) {
    struct timespec time;
    int result = clock_gettime(clockId, &time);
    if (result < 0) {
        return -errno;
    }
    return (time.tv_sec * NANOS_PER_SECOND) + time.tv_nsec;
}

static void displayPeakLevel(float peakLevel) {
    printf("%5.3f ", peakLevel);
    const int maxStars = 50; // arbitrary, fits on one line
    int numStars = (int) (peakLevel * maxStars);
    for (int i = 0; i < numStars; i++) {
        printf("*");
    }
    printf("\n");
}

/**
 * @param position1 position of hardware frame
 * @param nanoseconds1
 * @param position2 position of client read/write
 * @param nanoseconds2
 * @param sampleRate
 * @return latency in milliseconds
 */
static double calculateLatencyMillis(int64_t position1, int64_t nanoseconds1,
                              int64_t position2, int64_t nanoseconds2,
                              int64_t sampleRate) {
    int64_t deltaFrames = position2 - position1;
    int64_t deltaTime =
            (NANOS_PER_SECOND * deltaFrames / sampleRate);
    int64_t timeCurrentFramePlayed = nanoseconds1 + deltaTime;
    int64_t latencyNanos = timeCurrentFramePlayed - nanoseconds2;
    double latencyMillis = latencyNanos / 1000000.0;
    return latencyMillis;
}

#endif // AAUDIO_EXAMPLE_UTILS_H
