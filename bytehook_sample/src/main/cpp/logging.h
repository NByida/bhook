/*
2 * Copyright (C) 2011 The Android Open Source Project
3 *
4 * Licensed under the Apache License, Version 2.0 (the "License");
5 * you may not use this file except in compliance with the License.
6 * You may obtain a copy of the License at
7 *
8 *      http://www.apache.org/licenses/LICENSE-2.0
9 *
10 * Unless required by applicable law or agreed to in writing, software
11 * distributed under the License is distributed on an "AS IS" BASIS,
12 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
13 * See the License for the specific language governing permissions and
14 * limitations under the License.
15 */

#ifndef ANDROID_BASE_LOGGING_H
#define ANDROID_BASE_LOGGING_H

// NOTE: For Windows, you must include logging.h after windows.h to allow the
// following code to suppress the evil ERROR macro:
#ifdef _WIN32
// windows.h includes wingdi.h which defines an evil macro ERROR.
#ifdef ERROR
#undef ERROR
#endif
#endif

//#include <functional>
//#include <memory>
//#include <ostream>

//#include "android-base/macros.h"
//namespace android {
//        namespace base {
enum LogSeverity {
     VERBOSE,
     DEBUG,
     INFO,
     WARNING,
     ERROR,
     FATAL,
   };
//}
//}
