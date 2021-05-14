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

#ifndef ALEXA_CLIENT_SDK_AVSCOMMON_SDKINTERFACES_INCLUDE_AVSCOMMON_SDKINTERFACES_ENDPOINTS_ENDPOINTBUILDERINTERFACE_H_
#define ALEXA_CLIENT_SDK_AVSCOMMON_SDKINTERFACES_INCLUDE_AVSCOMMON_SDKINTERFACES_ENDPOINTS_ENDPOINTBUILDERINTERFACE_H_

#include <memory>
#include <string>

#include "AVSCommon/SDKInterfaces/CapabilityConfigurationInterface.h"
#include "AVSCommon/SDKInterfaces/DirectiveHandlerInterface.h"
#include "AVSCommon/SDKInterfaces/Endpoints/EndpointCapabilitiesBuilderInterface.h"
#include "AVSCommon/SDKInterfaces/Endpoints/EndpointCapabilitiesRegistrarInterface.h"
#include "AVSCommon/SDKInterfaces/Endpoints/EndpointInterface.h"
#include "AVSCommon/SDKInterfaces/ModeController/ModeControllerAttributes.h"
#include "AVSCommon/SDKInterfaces/ModeController/ModeControllerInterface.h"
#include "AVSCommon/SDKInterfaces/PowerController/PowerControllerInterface.h"
#include "AVSCommon/SDKInterfaces/RangeController/RangeControllerAttributes.h"
#include "AVSCommon/SDKInterfaces/RangeController/RangeControllerInterface.h"
#include "AVSCommon/SDKInterfaces/ToggleController/ToggleControllerAttributes.h"
#include "AVSCommon/SDKInterfaces/ToggleController/ToggleControllerInterface.h"

#include "AVSCommon/Utils/Optional.h"

namespace alexaClientSDK {
namespace avsCommon {
namespace sdkInterfaces {
namespace endpoints {

/**
 * Interface for an endpoint builder.
 *
 * The builder is responsible for building an endpoint object. Use the @c EndpointRegistrationManagerInterface
 * to register the endpoint and @c EndpointRegistrationObserverInterface to be notified about any changes to
 * the endpoint registration.
 *
 * @note The following attributes are mandatory and the build will fail if they are missing:
 *   - EndpointId
 *   - Manufacturer Name
 *   - Description
 *   - Friendly Name
 *   - Display Categories
 *   - At least one capability
 *
 * @note Endpoints that represent part of this AVS device must generate their identifier by calling @c
 * withDerivedEndpointId() with a suffix that is unique across other parts of this device. The final endpoint
 * identifier will be generated by combining the default endpoint identifier and the suffix provided.
 *
 * @note Endpoints that represent external devices connected to an AVS device must provide an identifier via @c
 * withEndpointId(). The identifier must be unique across endpoints registered to the user and consistent for every
 * build.
 *
 */
class EndpointBuilderInterface : public EndpointCapabilitiesRegistrarInterface {
public:
    /**
     * Destructor.
     */
    virtual ~EndpointBuilderInterface() = default;

    /**
     * Configures builder to use an endpointId that is generated using the default endpoint id and the given suffix.
     *
     * @note This will override any previous endpoint identifier configuration.
     * @note This function will use the suffix to generate a unique identifier for endpoints that represent a
     * component of the same device the client is running on. For example, if your device has a screen, you can
     * create an endpoint to control this screen and use "screen" as a suffix. The suffix must be unique for
     * this client and it must be consistent for every client execution.
     * @note The suffix can contain letters or numbers, spaces, and the following special characters: _ - = # ; : ? @ &
     * and it should not exceed 10 characters.
     *
     * @warning Do not use this function if endpoint being created can be controlled by different AVS clients. The
     * endpoint identifier must be consistent for every endpoint independently from the client that is controlling it.
     *
     * @param suffix A suffix that should be unique for the given client. The @c build() function fails if it exceeds 10
     * characters. If the suffix is not unique, the newly-built endpoint will replace the previous endpoint
     * which had the same suffix when registered.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withDerivedEndpointId(const std::string& suffix) = 0;

    /**
     * Configures builder to use the given identifier for the new endpoint.
     *
     * @note This will override any previous endpoint identifier configuration.
     * @note The identifier must be unique across all devices for the user. Registering an endpoint with the same
     * identifier as an existing endpoint will replace the original endpoint. In addition, the identifier must be
     * consistent for all discovery requests for the same device. An identifier can contain letters or numbers, spaces,
     * and the following special characters: _ - = # ; : ? @ &. The identifier cannot exceed 256 characters.
     * @note The builder will fail if the endpointId param is invalid.
     *
     * @param endpointId An endpointId that identifies this endpoint.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withEndpointId(const EndpointIdentifier& endpointId) = 0;

    /**
     * Configures builder to use the given friendly name.
     *
     * @note This will override any previous friendly name configuration.
     * @note This value can contain up to 128 characters.
     *
     * @param friendlyName The friendly name used to identify the endpoint.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withFriendlyName(const std::string& friendlyName) = 0;

    /**
     * Configures builder to use the given description.
     *
     * @note This will override any previous description configuration.
     * @note This value can contain up to 128 characters.
     * @note The builder will fail if the description param is invalid.
     *
     * @param description The description of the endpoint.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withDescription(const std::string& description) = 0;

    /**
     * Configures builder to use name of the device manufacturer representing the endpoint.
     *
     * @note This will override any previous manufacturer configuration.
     * @note This value can contain up to 128 characters.
     * @note The builder will fail if the manufacturer name param is invalid.
     *
     * @param description The manufacturer name of the device representing the endpoint.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withManufacturerName(const std::string& manufacturerName) = 0;

    /**
     * Configures builder to use the following display categories.
     *
     * @note This will override any previous display categories configuration.
     * @note The builder will fail if the displayCategories param is invalid.
     *
     * @param description The display category the device belongs to.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withDisplayCategory(const std::vector<std::string>& displayCategories) = 0;

    /**
     * Configures builder to set additional attributes that can be used to identify an endpoint.
     *
     * @note This will override any previous additional attributes configuration.
     * @note The builder will fail if any of the additional attributes params are invalid.
     *
     * @param manufacturer The name of the manufacturer of the device.
     * @param model The name of the model of the device.
     * @param serialNumber The serial number of the device.
     * @param firmwareVersion The firmware version of the device.
     * @param softwareVersion The software version of the device.
     * @param customIdentifier Your custom identifier for the device.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withAdditionalAttributes(
        const std::string& manufacturer,
        const std::string& model,
        const std::string& serialNumber,
        const std::string& firmwareVersion,
        const std::string& softwareVersion,
        const std::string& customIdentifier) = 0;

    /**
     * Configures builder to represent the methods that the endpoint uses to connect to the internet of smart home hub.
     *
     * @note This will override any previous connections configuration.
     * @note If the connections supplied is invalid, the builder will fail.
     *
     * @param connections The available connection methods and their attributes.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withConnections(
        const std::vector<std::map<std::string, std::string>>& connections) = 0;

    /**
     * Configures builder to store custom key value pair about the device.
     *
     * @note This will override any previous cookies configuration.
     * @note If the cookies supplied is invalid, the builder will fail.
     *
     * @param cookies Key value pairs for additional of the endpoint.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withCookies(const std::map<std::string, std::string>& cookies) = 0;

    /**
     * Configures builder to use a @c PowerControllerInterface
     *
     * @deprecated use the new @c withEndpointCapabilitiesBuilder() method instead.
     *
     * @param powerController An interface that provides the power operations.
     * @param isProactivelyReported Whether the property state change is proactively reported.
     * @param isRetrievable Whether the property state is retrievable.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withPowerController(
        std::shared_ptr<avsCommon::sdkInterfaces::powerController::PowerControllerInterface> powerController,
        bool isProactivelyReported,
        bool isRetrievable) = 0;

    /**
     * Configures builder to use a @c ToggleControllerInterface with @c instance identifier.
     *
     * @note The builder will fail if the instance name is already used in that endpoint.
     *
     * @deprecated use the new @c withEndpointCapabilitiesBuilder() method instead.
     *
     * @param toggleController An interface that performs the toggle operations.
     * @param instance A non-empty string identifying an instance of a toggle controller uniquely in a endpoint.
     * @param toggleControllerAttributes The attributes used in capability  discovery message.
     * @param isProactivelyReported Whether the property state change is proactively reported.
     * @param isRetrievable Whether the property state is retrievable.
     * @param isNonControllable Whether the property state can be controlled or not. This must be @c false for the
     * property state to be controllable. Default is @c false.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withToggleController(
        std::shared_ptr<avsCommon::sdkInterfaces::toggleController::ToggleControllerInterface> toggleController,
        const std::string& instance,
        const avsCommon::sdkInterfaces::toggleController::ToggleControllerAttributes& toggleControllerAttributes,
        bool isProactivelyReported,
        bool isRetrievable,
        bool isNonControllable = false) = 0;

    /**
     * Configures builder to use a @c ModeControllerInterface with @c instance identifier.
     *
     * @note The builder will fail if the instance name is already used in that endpoint.
     *
     * @deprecated use the new @c withEndpointCapabilitiesBuilder() method instead.
     *
     * @param modeController An interface that provides the mode operations.
     * @param instance A non-empty string identifying an instance of a mode controller uniquely in a endpoint.
     * @param modeControllerAttributes The attributes used in capability discovery message.
     * @param isProactivelyReported Whether the property state change is proactively reported.
     * @param isRetrievable Whether the property state is retrievable.
     * @param isNonControllable Whether the property state can be controlled or not. This must be @c false for the
     * property state to be controllable. Default is @c false.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withModeController(
        std::shared_ptr<avsCommon::sdkInterfaces::modeController::ModeControllerInterface> modeController,
        const std::string& instance,
        const avsCommon::sdkInterfaces::modeController::ModeControllerAttributes& modeControllerAttributes,
        bool isProactivelyReported,
        bool isRetrievable,
        bool isNonControllable = false) = 0;

    /**
     * Configures builder to use a @c RangeControllerInteface with @c instance identifier.
     *
     * @note The builder will fail if the instance name is already used in that endpoint.
     *
     * @deprecated use the new @c withEndpointCapabilitiesBuilder() method instead.
     *
     * @param rangeController An interface that provides the range operations.
     * @param instance A non-empty string identifying an instance of a range controller uniquely in a endpoint.
     * @param rangeControllerAttributes The attributes used in capability discovery message.
     * @param isProactivelyReported Whether the property state change is proactively reported.
     * @param isRetrievable Whether the property state is retrievable.
     * @param isNonControllable Whether the property state can be controlled or not. This must be @c false for the
     * property state to be controllable. Default is @c false.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withRangeController(
        std::shared_ptr<avsCommon::sdkInterfaces::rangeController::RangeControllerInterface> rangeController,
        const std::string& instance,
        const avsCommon::sdkInterfaces::rangeController::RangeControllerAttributes& rangeControllerAttributes,
        bool isProactivelyReported,
        bool isRetrievable,
        bool isNonControllable = false) = 0;

    /**
     * Configures builder to use a @c EndpointCapabilitiesBuilder object that can be used to build multiple capability
     * agents.
     *
     * @note The builder will fail if the endpointCapabilitiesBuilder fails in generating valid capability agents.
     *
     * @param endpointCapabilitiesBuilder A @c EndpointCapabilitiesBuilder object.
     * @return This builder which can be used to nest configuration function calls.
     */
    virtual EndpointBuilderInterface& withEndpointCapabilitiesBuilder(
        const std::shared_ptr<avsCommon::sdkInterfaces::endpoints::EndpointCapabilitiesBuilderInterface>&
            endpointCapabilitiesBuilder) = 0;

    /**
     * Builds an endpoint with the configured properties / components.
     *
     * Build will fail iff the format of any attribute is invalid or if a mandatory attribute is missing.
     *
     * @return A unique endpoint if the build succeeds; otherwise, nullptr.
     */
    virtual std::unique_ptr<EndpointInterface> build() = 0;
};

}  // namespace endpoints
}  // namespace sdkInterfaces
}  // namespace avsCommon
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_AVSCOMMON_SDKINTERFACES_INCLUDE_AVSCOMMON_SDKINTERFACES_ENDPOINTS_ENDPOINTBUILDERINTERFACE_H_
