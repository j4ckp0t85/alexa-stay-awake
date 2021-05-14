/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <acsdkManufactory/ComponentAccumulator.h>
#include <AVSCommon/SDKInterfaces/ChannelVolumeFactoryInterface.h>

#include "SpeakerManager/SpeakerManager.h"
#include "SpeakerManager/SpeakerManagerComponent.h"

namespace alexaClientSDK {
namespace capabilityAgents {
namespace speakerManager {

using namespace acsdkManufactory;
using namespace capabilityAgents::speakerManager;

SpeakerManagerComponent getComponent() {
    return ComponentAccumulator<>().addRequiredFactory(SpeakerManager::createSpeakerManagerCapabilityAgent);
}

}  // namespace speakerManager
}  // namespace capabilityAgents
}  // namespace alexaClientSDK
