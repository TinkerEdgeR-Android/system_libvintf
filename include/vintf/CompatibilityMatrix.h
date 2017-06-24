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

#ifndef ANDROID_VINTF_COMPATIBILITY_MATRIX_H
#define ANDROID_VINTF_COMPATIBILITY_MATRIX_H

#include <map>
#include <string>

#include <utils/Errors.h>

#include "HalGroup.h"
#include "MapValueIterator.h"
#include "MatrixHal.h"
#include "MatrixKernel.h"
#include "SchemaType.h"
#include "Sepolicy.h"
#include "Vndk.h"
#include "XmlFileGroup.h"

namespace android {
namespace vintf {

// Compatibility matrix defines what hardware does the framework requires.
struct CompatibilityMatrix : public HalGroup<MatrixHal>, public XmlFileGroup<MatrixXmlFile> {
    // Create a framework compatibility matrix.
    CompatibilityMatrix() : mType(SchemaType::FRAMEWORK) {};

    SchemaType type() const;

    constexpr static Version kVersion{1, 0};

private:
    bool add(MatrixHal &&hal);
    bool add(MatrixKernel &&kernel);

    // Find a MatrixKernel entry that has version v. nullptr if not found.
    const MatrixKernel *findKernel(const KernelVersion &v) const;

    status_t fetchAllInformation(const std::string &path);

    friend struct HalManifest;
    friend struct RuntimeInfo;
    friend struct CompatibilityMatrixConverter;
    friend struct LibVintfTest;
    friend class VintfObject;
    friend class AssembleVintf;
    friend bool operator==(const CompatibilityMatrix &, const CompatibilityMatrix &);

    SchemaType mType;

    // entries only for framework compatibility matrix.
    struct {
        std::vector<MatrixKernel> mKernels;
        Sepolicy mSepolicy;
        Version mAvbMetaVersion;
    } framework;

    // entries only for device compatibility matrix.
    struct {
        Vndk mVndk;
    } device;
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_COMPATIBILITY_MATRIX_H
