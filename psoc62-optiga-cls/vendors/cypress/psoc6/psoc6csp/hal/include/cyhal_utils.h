/***************************************************************************//**
* \file cyhal_utils.h
*
* \brief
* Provides utility functions for working with the PSoC 6 HAL implementation.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

/**
* \addtogroup group_hal_psoc6 PSoC 6 Implementation Specific
* \{
* This section provides details about the PSoC 6 implementation of the Cypress HAL.
* All information within this section is platform specific and is provided for reference.
* Portable application code should depend only on the APIs and types which are documented
* in the @ref group_hal section.
*/

#pragma once

#include "cyhal_hw_types.h"
#include "cy_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif


#define CYHAL_IRQN_OFFSET          16 /**< Offset for implementation-defined ISR type numbers (IRQ0 = 16) */
#define CYHAL_GET_CURRENT_IRQN()   ((IRQn_Type) (__get_IPSR() - CYHAL_IRQN_OFFSET)) /**< Macro to get the IRQn of the current ISR */

/**
* \addtogroup group_hal_psoc6_pin_package
* \{
*/

#define CYHAL_GET_PIN(pin)         ((uint8_t)(pin & 0xFFFFUL))  /**< Macro to extract the pin number */
#define CYHAL_GET_PORT(pin)        ((uint8_t)((uint32_t)(pin >> 16) & 0xFFUL)) /**< Macro to extract the port number */

/** Looks up the resource block that connects to the specified pins from the provided resource pin mapping table.
 * This is a convinience utility for cyhal_utils_get_resource() if the mappings is an array of known size.
 *
 * @param[in] pin        The pin to lookup the hardware block for
 * @param[in] mappings   The mappings of pin to hardware block
 * @return The entry for the specified pin if it exists, or null if it doesn't.
 */
#define CY_UTILS_GET_RESOURCE(pin, mappings) cyhal_utils_get_resource(pin, mappings, sizeof(mappings)/sizeof(cyhal_resource_pin_mapping_t))

/** Converts the provided gpio pin to a resource instance object
 *
 * @param[in] pin  The pin to get a resource object for
 * @return The equivilant resource instance object for the provided pin.
 */
static inline cyhal_resource_inst_t cyhal_utils_get_gpio_resource(cyhal_gpio_t pin)
{
    cyhal_resource_inst_t rsc = { CYHAL_RSC_GPIO, CYHAL_GET_PORT(pin), CYHAL_GET_PIN(pin) };
    return rsc;
}

/** Looks up the resource block that connects to the specified pins from the provided resource pin mapping table.
 *
 * @param[in] pin        The pin to lookup the hardware block for
 * @param[in] mappings   The mappings of pin to hardware block
 * @param[in] count      The number of items in the mappings table
 * @return The entry for the specified pin if it exists, or null if it doesn't.
 */
const cyhal_resource_pin_mapping_t *cyhal_utils_get_resource(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t* mappings, size_t count);

/** Disconnects any routing for the pin from the interconnect driver and then free's the pin from the hwmgr.
 *
 * @param[in] pin       The pin to disconnect and free
 */
void cyhal_utils_disconnect_and_free(cyhal_gpio_t pin);


/** Converts a set of flags from one representation to a equivalent set of flags
 * in another representation.
 * For example usage see cyhal_syspm.c or cyhal_sdhc.c
 *
 * @param[in] map           New flag values. Each index corresponds to a bit position
 *                          from the source_flags. Index 0 is a default value if nothing
 *                          matches.
 * @param[in] count         Count of the total flags in the list.
 * @param[in] source_flags  Source flags to be converted.
 * @return Converted flags.
 */
uint32_t cyhal_utils_convert_flags(const uint32_t map[], uint32_t count, uint32_t source_flags);
/** \} group_hal_psoc6_pin_package */

/** Calculate the peri clock divider value that need to be set to reach frequency closest to the input frequency
 * 
 * @param[in] frequency The desired frequency
 * @param[in] frac_bits The number of fractional bits that the divider has
 * @return The calculate divider value to set, NOTE a divider value of x divide the frequency by (x+1)
 */
static inline uint32_t cyhal_divider_value(uint32_t frequency, uint32_t frac_bits)
{
    return ((Cy_SysClk_ClkPeriGetFrequency() * (1 << frac_bits)) + (frequency / 2)) / frequency - 1;
}

#if defined(__cplusplus)
}
#endif

/** \} group_hal_psoc6_utils */
/** \} group_hal_psoc6 */
