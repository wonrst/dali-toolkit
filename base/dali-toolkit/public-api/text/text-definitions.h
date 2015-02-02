#ifndef __DALI_TEXT_ABSTRACTION_TEXT_TYPE_DEFINITIONS_H__
#define __DALI_TEXT_ABSTRACTION_TEXT_TYPE_DEFINITIONS_H__

/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// EXTERNAL INCLUDES
#include <dali/public-api/text-abstraction/text-abstraction.h>

namespace Dali
{

namespace Toolkit
{

namespace Text
{

typedef TextAbstraction::FontId          FontId;          ///< The unique identifier for a font face (generated by FontClient)
typedef TextAbstraction::FontMetrics     FontMetrics;     ///< The metrics for a Font expressed in 26.6 fractional pixel format
typedef TextAbstraction::PointSize26Dot6 PointSize26Dot6; ///< The point size in 26.6 fractional points
typedef TextAbstraction::FaceIndex       FaceIndex;       ///< Used with fonts which allow several font faces
typedef TextAbstraction::GlyphIndex      GlyphIndex;      ///< Uniquely identifies a glyph within a particular font
typedef TextAbstraction::Character       Character;       ///< A UTF-32 representation of a character
typedef TextAbstraction::GlyphInfo       GlyphInfo;       ///< The information describing a glyph (font ID, index, metrics)
typedef TextAbstraction::CharacterIndex  CharacterIndex;  ///< An index into an array of characters
typedef TextAbstraction::Length          Length;          ///< The length of an array
typedef TextAbstraction::BidiInfoIndex   BidiInfoIndex;   ///< Index to the bidirectional info for a paragraph.

typedef uint32_t GlyphIndex;     ///< An index into an array of glyphs

} // namespace Text

} // namespace Toolkit

} // namespace Dali

#endif // __DALI_TEXT_ABSTRACTION_TEXT_TYPE_DEFINITIONS_H__
