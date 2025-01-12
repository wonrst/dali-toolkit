#ifndef DALI_TOOLKIT_TEXT_EDITABLE_CONTROL_INTERFACE_H
#define DALI_TOOLKIT_TEXT_EDITABLE_CONTROL_INTERFACE_H

/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
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

// INTERNAL INCLUDES
#include <dali-toolkit/internal/text/input-style.h>
#include <dali-toolkit/public-api/controls/text-controls/input-filter-properties.h>

namespace Dali
{
class Actor;

namespace Toolkit
{
namespace Text
{
/**
 * @brief An interface that the Text::Controller uses to notify about text changes and add decoration to the text control.
 */
class EditableControlInterface
{
public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~EditableControlInterface()
  {
  }

  /**
   * @brief Called to signal that text has been inserted.
   */
  virtual void TextInserted(unsigned int position, unsigned int length, const std::string& content) = 0;

  /**
   * @brief Called to signal that text has been deleted.
   */
  virtual void TextDeleted(unsigned int position, unsigned int length, const std::string& content) = 0;

  /**
   * @brief Called to signal that caret (cursor position) has been moved.
   */
  virtual void CursorPositionChanged(unsigned int oldPosition, unsigned int newPosition) = 0;

  /**
   * @brief Called to signal that text has been inserted or deleted.
   *
   * @param[in] immediate If true, it immediately emits the signal, if false, only emits once the signal when OnRelayout() is called next time.
   */
  virtual void TextChanged(bool immediate) = 0;

  /**
   * @brief Called when the number of characters to be inserted exceeds the maximum limit
   */
  virtual void MaxLengthReached() = 0;

  /**
   * @brief Called to signal that input style has been changed.
   *
   * @param[in] inputStyleMask Mask with the bits of the input style that has changed.
   */
  virtual void InputStyleChanged(InputStyle::Mask inputStyleMask) = 0;

  /**
   * @brief Called when the character to be inserted is filtered by the input filter.
   *
   * @param[in] type The filter type is ACCEPTED or REJECTED.
   */
  virtual void InputFiltered(Toolkit::InputFilter::Property::Type type) = 0;

  /**
   * @brief Add a decoration.
   *
   * @param[in] decoration The actor displaying a decoration.
   * @param[in] needsClipping Whether the actor needs clipping.
   */
  virtual void AddDecoration(Actor& actor, bool needsClipping) = 0;

  /**
   * @brief Gets the color of the control.
   *
   * @param[out] The color of the control.
   */
  virtual void GetControlBackgroundColor(Vector4& color) const = 0;

  /**
   * @brief Editable status (on/off).
   *
   * @return true if it can be edit, else false.
   */
  virtual bool IsEditable() const = 0;

  /**
   * @brief Change the editable status (on/off) .
   *
   * @param[in] editable The editable status.
   */
  virtual void SetEditable(bool editable) = 0;

  /**
   * @brief Called to copy the selected text.
   * @return The copied text.
   */
  virtual string CopyText() = 0;

  /**
   * @brief Called to cut the selected text.
   * @return The cut text.
   */
  virtual string CutText() = 0;

  /**
   * @brief Called to paste the most recent clipboard text item into the control.
   */
  virtual void PasteText() = 0;
};

} // namespace Text

} // namespace Toolkit

} // namespace Dali

#endif // DALI_TOOLKIT_TEXT_EDITABLE_CONTROL_INTERFACE_H
