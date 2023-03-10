#pragma once

#include <iostream>
#include <tuple>

#include "NX/Common/NXCommon_export.hpp"
#include "NX/Common/Types.hpp"

namespace NX::Common
{
/**
 * @brief The Rgba alias is used to specify the format for storing integer
 * color information. Values are stored as sequential unsigned char components
 * ranging from 0 to 255 describing red, green, blue, and alpha values.
 */
using Rgba = uint32;

/**
 * @brief The RgbColor namespace stores calculations on or for ARGB values.
 * Calculations range from extracting color components to creating new ARGB
 * values to printing values to an output stream.
 */
namespace RgbColor
{
/**
 * @brief RgbColor::Components are integer color values for a single color
 * component. Values range from 0 to 255.
 */
using Component = uint8;

/**
 * @brief Returns the integer representation of the red component.
 * @param rgb
 * @return uint8
 */
Component NXCOMMON_EXPORT dRed(NX::Common::Rgba rgb);

/**
 * @brief Returns the integer representation of the green component.
 * @param rgb
 * @return uint8
 */
Component NXCOMMON_EXPORT dGreen(NX::Common::Rgba rgb);

/**
 * @brief Returns the integer representation of the blue component.
 * @param rgb
 * @return uint8
 */
Component NXCOMMON_EXPORT dBlue(NX::Common::Rgba rgb);

/**
 * @brief Returns the integer representation of the alpha component.
 * @param rgb
 * @return uint8
 */
Component NXCOMMON_EXPORT dAlpha(NX::Common::Rgba rgb);

/**
 * @brief Returns the integer representation of the grayscale value.
 * @param rgb
 * @return uint8
 */
Component NXCOMMON_EXPORT dGray(NX::Common::Rgba rgb);

/**
 * @brief Returns the Rgba value from the individual component values.
 * @param r
 * @param g
 * @param b
 * @param a
 * @return NX::Common::Rgba
 */
NX::Common::Rgba NXCOMMON_EXPORT dRgb(Component r, Component g, Component b, Component a);

/**
 * @brief Prints the Rgba value to stream using the specified separator.
 * @param out
 * @param sep
 * @param rgb
 */
void NXCOMMON_EXPORT print(std::ostream& out, const char* sep, NX::Common::Rgba rgb);

/**
 * @brief Checks equality between two Rgba values.
 * @param lhs
 * @param rhs
 * @return bool
 */
bool NXCOMMON_EXPORT isEqual(NX::Common::Rgba lhs, NX::Common::Rgba rhs);

/**
 * @brief Converts an Rgba value to a tuple of floats
 * @param rgb
 * @return tuple<float32, float32, float32>
 */
std::tuple<float32, float32, float32> NXCOMMON_EXPORT fRgb(NX::Common::Rgba rgb);
} // namespace RgbColor
} // namespace NX::Common
