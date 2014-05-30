/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include <dali-toolkit/internal/controls/text-input/text-input-popup-impl.h>

#include <dali-toolkit/public-api/controls/buttons/push-button.h>
#include <dali-toolkit/public-api/controls/default-controls/solid-color-actor.h>

#include <libintl.h>

using namespace std;
using namespace Dali;

#define GET_LOCALE_TEXT(string) dgettext("sys_string", string)

namespace {

// Default Colors

const Vector4 DEFAULT_POPUP_BACKGROUND( Vector4( 0.24f, 0.41f, 0.88f, 1.0f ) );
const Vector4 DEFAULT_POPUP_BUTTON_PRESSED( Vector4( 0.18f, 0.56f, 1.0f, 1.0f ) );
const Vector4 DEFAULT_BORDER_COLOR( Vector4( 0.2f, 0.2f, 0.2f, 1.0f ) );
const Vector3 POPUP_BORDER( Vector3(1.0f, 1.0f, 0.0f) );

// Popup: Divider
const char* DEFAULT_PANEL_BUTTON_DIVIDER = DALI_IMAGE_DIR "copypanelLine.png";

/* Functionality in place to have the end buttons using different images to inner button.
 * Supply a centre image and then a left and right image, the centre image can have straight ends while
 * the left image can be rounded on the left and straight on the right, the right image can be straight on the left and rounded on the right.
 */

// Popup: Tails
const char* DEFAULT_POPUP_TAIL_BOTTOM( DALI_IMAGE_DIR "00_popup_bubble_tail_bottom.png" );

// Popup: Vertical Constraint
// TODO: Remove - this should come from application - it is not possible to get the
// height of the indicator actor from Dali-Toolkit.
const Vector2 DEFAULT_POPUP_INDICATOR_OFFSET(0.0f, 60.0f);

const Vector3 POPUP_TEXT_OFFSET( 0.0f, 0.0f, 0.0f );
const Vector3 POPUP_TEXT_ENLARGE( 12.0f, 28.0f, 0.0f );
const Vector3 POPUP_MINIMUM_SIZE( 128.0f, 124.0f, 0.0f );

const Vector3 BUTTON_TEXT_ENLARGE( 32.0f, 0.0f, 0.0f );
const Vector3 BUTTON_TEXT_MINIMUM_SIZE( 128.0f, 126.0f, 0.0f );
const Vector3 BUTTON_TEXT_MAXIMUM_SIZE( 190.0f, 126.0f, 0.0f );
const Vector3 TEXT_LABEL_MAX_SIZE( 160.0f, 30.0f, 0.0f );

const float DIVIDER_WIDTH(2.0f);                                            ///< Width of each button divider
const float DIVIDER_MARGIN(0.0f);                                          ///< Top/Bottom Margin between divider and edge of popup.

const float DEFAULT_UI_FONT_SIZE(7.0f);                                     ///< Standard font size for Text-Input's UI

const float HIDE_POPUP_ANIMATION_DURATION(0.2f);                            ///< Duration of popup hide animation in seconds.
const float SHOW_POPUP_ANIMATION_DURATION(0.2f);                            ///< Duration of popup show animation in seconds.

const Vector2 DEFAULT_ICON_SIZE( 45.0f, 45.0f );                            ///< Default icon size for image in options
const float TEXT_POSITION_OFFSET( -19.0f );                                  ///< Default offset for text label
const float ICON_POSITION_OFFSET( 19.0f );                                  ///< Default offset for icon

const char* DEFAULT_ICON_CLIPBOARD( DALI_IMAGE_DIR "copy_paste_icon_clipboard.png" );
const char* DEFAULT_ICON_COPY( DALI_IMAGE_DIR "copy_paste_icon_copy.png" );
const char* DEFAULT_ICON_CUT( DALI_IMAGE_DIR "copy_paste_icon_cut.png" );
const char* DEFAULT_ICON_PASTE( DALI_IMAGE_DIR "copy_paste_icon_paste.png" );
const char* DEFAULT_ICON_SELECT( DALI_IMAGE_DIR "copy_paste_icon_select.png" );
const char* DEFAULT_ICON_SELECT_ALL( DALI_IMAGE_DIR "copy_paste_icon_select_all.png" );

// TODO: This should be based on the content for example:
// 1. For selection: should be above top of highlighted selection, or below bottom of highlighted selection + end handle.
// 2. For cursor: should be above top of cursor, or below bottom of cursor + grab handle.
const std::string POPUP_ALTERNATIVE_OFFSET("popup-alternative-offset");       ///< Alternative offset property for confinenment constraint.


/**
 * Confine Actor to boundaries of reference actor (e.g. Parent)
 * Actor bounds (top-left position + size) are confined to reference Actor's
 * bounds.
 */
struct ConfinementConstraint
{
  /**
   * Confinement constraint constructor.
   * @param[in] topLeftMargin (optional) Top-Left margins (defaults to 0.0f, 0.0f)
   * @param[in] bottomRightMargin (optional) Bottom-Right margins (defaults to 0.0f, 0.0f)
   * @param[in] flipVertical (optional) whether to flip Actor to the other side if near edge, and by
   * how much (defaults to 0.0f i.e. no flip)
   */
  ConfinementConstraint(Vector2 topLeftMargin = Vector2::ZERO, Vector2 bottomRightMargin = Vector2::ZERO, bool flipHorizontal = false, bool flipVertical = false)
  : mMinIndent(topLeftMargin),
    mMaxIndent(bottomRightMargin),
    mFlipHorizontal(flipHorizontal),
    mFlipVertical(flipVertical)
  {
  }

  Vector3 operator()(const Vector3&    constPosition,
                     const PropertyInput& sizeProperty,
                     const PropertyInput& parentOriginProperty,
                     const PropertyInput& anchorPointProperty,
                     const PropertyInput& referenceSizeProperty,
                     const PropertyInput& alternativeOffsetProperty)
  {
    const Vector3& size = sizeProperty.GetVector3();
    const Vector3& origin = parentOriginProperty.GetVector3();
    const Vector3& anchor = anchorPointProperty.GetVector3();
    const Vector3& referenceSize = referenceSizeProperty.GetVector3();
    const Vector2& alternativeOffset = alternativeOffsetProperty.GetVector2();

    Vector3 newPosition(constPosition);

    // Get actual position of Actor relative to parent's Top-Left.
    Vector3 position(constPosition + origin * referenceSize);

    // if top-left corner is outside of Top-Left bounds, then push back in screen.
    Vector3 corner(position - size * anchor - mMinIndent);

    if(mFlipHorizontal && corner.x < 0.0f)
    {
      corner.x = 0.0f;
      newPosition.x += size.width + alternativeOffset.width;
    }

    if(mFlipVertical && corner.y < 0.0f)
    {
      corner.y = 0.0f;
      newPosition.y += size.height + alternativeOffset.height;
    }

    newPosition.x -= std::min(corner.x, 0.0f);
    newPosition.y -= std::min(corner.y, 0.0f);

    // if bottom-right corner is outside of Bottom-Right bounds, then push back in screen.
    corner += size - referenceSize + mMinIndent + mMaxIndent;

    if(mFlipHorizontal && corner.x > 0.0f)
    {
      corner.x = 0.0f;
      newPosition.x -= size.width + alternativeOffset.width;
    }

    if(mFlipVertical && corner.y > 0.0f)
    {
      corner.y = 0.0f;
      newPosition.y -= size.height + alternativeOffset.height;
    }

    newPosition.x -= std::max(corner.x, 0.0f);
    newPosition.y -= std::max(corner.y, 0.0f);

    return newPosition;
  }

  Vector3 mMinIndent;                                   ///< Top-Left Margin
  Vector3 mMaxIndent;                                   ///< Bottom-Right Margin.
  bool mFlipHorizontal;                                 ///< Whether to flip actor's position if exceeds horizontal screen bounds
  bool mFlipVertical;                                   ///< Whether to flip actor's position if exceeds vertical screen bounds
};

} // unnamed namespace

namespace Dali
{

namespace Toolkit
{

namespace Internal
{

const char* const TextInputPopup::SIGNAL_PRESSED = "pressed";
const char* const TextInputPopup::SIGNAL_HIDE_FINISHED = "hide-finished";
const char* const TextInputPopup::SIGNAL_SHOW_FINISHED = "show-finished";

const char* const TextInputPopup::OPTION_SELECT_WORD = "select_word";                       // "Select Word" popup option.
const char* const TextInputPopup::OPTION_SELECT_ALL("select_all");                          // "Select All" popup option.
const char* const TextInputPopup::OPTION_CUT("cut");                                        // "Cut" popup option.
const char* const TextInputPopup::OPTION_COPY("copy");                                      // "Copy" popup option.
const char* const TextInputPopup::OPTION_PASTE("paste");                                    // "Paste" popup option.
const char* const TextInputPopup::OPTION_CLIPBOARD("clipboard");                            // "Clipboard" popup option.

TextInputPopup::TextInputPopup()
: mState(StateHidden),
  mRootActor(Layer::New()),
  mContentSize( Vector3::ZERO ),
  mCutPasteButtonsColor( DEFAULT_POPUP_BACKGROUND ),
  mCutPasteButtonsPressedColor( DEFAULT_POPUP_BUTTON_PRESSED ),
  mBorderColor( DEFAULT_BORDER_COLOR ),
  mSelectOptionPriority(1),
  mSelectAllOptionPriority(2),
  mCutOptionPriority(3),
  mCopyOptionPriority(4),
  mPasteOptionPriority(5),
  mClipboardOptionPriority(6),
  mPressedSignal(),
  mHideFinishedSignal(),
  mShowFinishedSignal()
{
  mAlternativeOffsetProperty = mRootActor.RegisterProperty( POPUP_ALTERNATIVE_OFFSET, Vector2::ZERO );
  mRootActor.SetParentOrigin( ParentOrigin::CENTER );
  mRootActor.SetAnchorPoint( AnchorPoint::BOTTOM_CENTER );
  // constrain popup to size of parent.
}

Actor TextInputPopup::Self()
{
  return mRootActor;
}

void TextInputPopup::AddToStage()
{
  // TODO: Confinement constraint borders should be defined by the application.
  // It should also not use the stage directly, instead it should add to parent container.
  Stage::GetCurrent().Add(mRootActor);

  ApplyConfinementConstraint();
}

void TextInputPopup::ApplyConfinementConstraint()
{
  mRootActor.RemoveConstraints();
  Constraint constraint = Constraint::New<Vector3>( Actor::POSITION,
                                                    LocalSource( Actor::SIZE ),
                                                    LocalSource( Actor::PARENT_ORIGIN ),
                                                    LocalSource( Actor::ANCHOR_POINT ),
                                                    ParentSource( Actor::SIZE ),
                                                    LocalSource( mAlternativeOffsetProperty ),
                                                    ConfinementConstraint( DEFAULT_POPUP_INDICATOR_OFFSET,
                                                                           Vector2::ZERO,
                                                                           false,
                                                                           true) );
  mRootActor.ApplyConstraint(constraint);
}

void TextInputPopup::RemoveFromStage()
{
  Actor rootActor = Self();
  Stage::GetCurrent().Remove( rootActor );
}

void TextInputPopup::Clear()
{
  if ( mBackground )
  {
    mRootActor.Remove( mBackground );
    mBackground.Reset();
    mButtonContainer.clear();
    mDividerContainer.clear();

    RemoveFromStage();
    mRootActor.RemoveConstraints();

    mState = StateHidden;
  }
}

Toolkit::TextView TextInputPopup::CreateOptionText( const MarkupProcessor::StyledTextArray& styledCaption )
{
  Toolkit::TextView label = Toolkit::TextView::New( styledCaption );
  label.SetSizePolicy( Toolkit::Control::Fixed, Toolkit::Control::Fixed );
  label.SetWidthExceedPolicy( Toolkit::TextView::Fade );
  label.SetParentOrigin( ParentOrigin::BOTTOM_CENTER );
  label.SetAnchorPoint( AnchorPoint::BOTTOM_CENTER );
  label.SetPosition( 0.0f, TEXT_POSITION_OFFSET );

  return label;
}

ImageActor TextInputPopup::CreateOptionIcon( Image iconImage )
{
  ImageActor icon = ImageActor::New( iconImage );

  icon.SetSize( DEFAULT_ICON_SIZE );
  icon.SetParentOrigin( ParentOrigin::TOP_CENTER );
  icon.SetAnchorPoint( AnchorPoint::TOP_CENTER );
  icon.SetPosition( 0.0f, ICON_POSITION_OFFSET );

  return icon;
}

void TextInputPopup::CreatePopUpBackground()
{
  // Create background-panel if not already created (required if we have at least one option)
  if ( !mBackground )
  {
    mBackground = Toolkit::CreateSolidColorActor( GetCutPastePopUpColor(), true, mBorderColor );

    Self().Add( mBackground );
    mContentSize = POPUP_TEXT_OFFSET;

    Hide(false);
    AddToStage();

    // Add Tail too.
    Image tailImage = Image::New( DEFAULT_POPUP_TAIL_BOTTOM );

    mTail = ImageActor::New( tailImage );
    mTail.SetParentOrigin( ParentOrigin::BOTTOM_CENTER );
    mTail.SetAnchorPoint( AnchorPoint::TOP_CENTER );
    mBackground.Add( mTail );
    // TODO: Make tail visible, and positioned in relation to original intended position of popup (i.e. before constrained effects)
    mTail.SetVisible(false);
  }
}

void TextInputPopup::CreateDivider()
{
  if(mButtonContainer.size() > 0)
  {
    Image dividerImage = Image::New( DEFAULT_PANEL_BUTTON_DIVIDER );
    ImageActor divider = ImageActor::New( dividerImage );
    divider.SetParentOrigin( ParentOrigin::TOP_LEFT );
    divider.SetAnchorPoint( AnchorPoint::TOP_LEFT );
    divider.SetPosition( Vector3( mContentSize.width, POPUP_TEXT_OFFSET.y, 0.0f ) );
    // Keep track of all the dividers. As their height's need to be updated to the max. of all
    // buttons currently added.
    mDividerContainer.push_back(divider);

    mBackground.Add( divider );
    mContentSize.width += DIVIDER_WIDTH;
  }
}

ImageActor TextInputPopup::CreatePressedBackground( const Vector3 requiredSize )
{
  std::string pressedImageFilename;
  Vector4 pressedImageBorder;
  Vector2 pressedImageSize;

  ImageActor pressedButtonBg = Toolkit::CreateSolidColorActor( GetCutPastePopUpPressedColor() );

  pressedButtonBg.SetSize ( requiredSize );
  pressedButtonBg.SetParentOrigin( ParentOrigin::CENTER );
  pressedButtonBg.SetAnchorPoint( AnchorPoint::CENTER );

  return pressedButtonBg;
}

TextInputPopup::ButtonRequirement TextInputPopup::CreateRequiredButton( TextInputPopup::Buttons buttonId, std::size_t orderOfPriority,
                                                                        const std::string& name, const std::string& caption, Image iconImage, bool enabled )
{
  TextInputPopup::ButtonRequirement currentButton;

  currentButton.buttonId = buttonId;
  currentButton.orderOfPriority = orderOfPriority;
  currentButton.name = name;
  currentButton.caption = caption;
  currentButton.iconImage = iconImage;
  currentButton.enabled = enabled;

  return currentButton;
}

void TextInputPopup::CreateOrderedListOfOptions()
{
  mOrderListOfButtons.clear();

  for ( std::size_t index= 0; index < ButtonsEnumEnd; index++ )
  {
    TextInputPopup::ButtonRequirement currentButton;

    // Create button for each possible option using Option priority
    switch ( index )
    {
      case ButtonsCut:
      {
        Image cutIcon = Image::New( DEFAULT_ICON_CUT );
        currentButton = CreateRequiredButton( ButtonsCut, mCutOptionPriority, OPTION_CUT, GET_LOCALE_TEXT("IDS_COM_BODY_CUT"), cutIcon, false );
        break;
      }
      case ButtonsCopy:
      {
        Image copyIcon = Image::New( DEFAULT_ICON_COPY );
        currentButton = CreateRequiredButton( ButtonsCopy, mCopyOptionPriority, OPTION_COPY, GET_LOCALE_TEXT("IDS_COM_BODY_COPY"), copyIcon, false );
        break;
      }
      case ButtonsPaste:
      {
        Image pasteIcon = Image::New( DEFAULT_ICON_PASTE );
        currentButton = CreateRequiredButton( ButtonsPaste, mPasteOptionPriority, OPTION_PASTE, GET_LOCALE_TEXT("IDS_COM_BODY_PASTE"), pasteIcon, false );
        break;
      }
      case ButtonsSelect:
      {
        Image selectIcon = Image::New( DEFAULT_ICON_SELECT );
        currentButton = CreateRequiredButton( ButtonsSelect, mSelectOptionPriority, OPTION_SELECT_WORD, GET_LOCALE_TEXT("IDS_COM_SK_SELECT"), selectIcon, false );
        break;
      }
      case ButtonsSelectAll:
      {
        Image selectAllIcon = Image::New( DEFAULT_ICON_SELECT_ALL );
        currentButton = CreateRequiredButton( ButtonsSelectAll, mSelectAllOptionPriority, OPTION_SELECT_ALL, GET_LOCALE_TEXT("IDS_COM_BODY_SELECT_ALL"), selectAllIcon, false );
        break;
      }
      case ButtonsClipboard:
      {
        Image clipboardIcon = Image::New( DEFAULT_ICON_CLIPBOARD );
        currentButton = CreateRequiredButton( ButtonsClipboard, mClipboardOptionPriority, OPTION_CLIPBOARD, GET_LOCALE_TEXT("IDS_COM_BODY_CLIPBOARD"), clipboardIcon, false );
        break;
      }
      case ButtonsEnumEnd:
      {
        DALI_ASSERT_DEBUG( "ButtonsEnumEnd used but an invalid choice");
        currentButton.orderOfPriority = 0;
        break;
      }
    }

    bool match = false;

    // Insert button in list of buttons in order of priority setting.
    for( std::vector<ButtonRequirement>::iterator it = mOrderListOfButtons.begin(), endIt = mOrderListOfButtons.end(); ( it != endIt && !match ); ++it )
    {
      const ButtonRequirement& button( *it );
      if ( currentButton.orderOfPriority < button.orderOfPriority )
      {
        if ( currentButton.orderOfPriority != 0 ) // If order priority 0 then do not add button as not required.
        {
          mOrderListOfButtons.insert( it, currentButton );
        }
        match = true;
      }
    }

    if ( !match)
    {
      mOrderListOfButtons.push_back( currentButton );
    }
  }
}

void TextInputPopup::AddOption(const std::string& name, const std::string& caption, const Image iconImage,  bool finalOption)
{
  CreatePopUpBackground();

  CreateDivider();

  // Create a Button with Text, Icon and highlight when pressed

  Toolkit::PushButton button = Toolkit::PushButton::New();
  button.SetSizePolicy( Toolkit::Control::Fixed, Toolkit::Control::Fixed );
  button.SetName( name );

  // Create container for text and icon when not pressed
  Actor iconTextContainer = Actor::New();
  iconTextContainer.SetParentOrigin( ParentOrigin::TOP_LEFT );
  iconTextContainer.SetAnchorPoint( AnchorPoint::TOP_LEFT );

  // 1. Add text.
  TextStyle style;
  style.SetFontPointSize( PointSize( DEFAULT_UI_FONT_SIZE ) );
  MarkupProcessor::StyledTextArray styledCaption;
  styledCaption.push_back( MarkupProcessor::StyledText( Text( caption ), style ) );
  Toolkit::TextView label = CreateOptionText( styledCaption );
  label.SetName( name );

  iconTextContainer.Add( label );

  // Get natural size of text and then constrain it to bounds.
  const Vector3 textSize = label.GetNaturalSize();
  const Vector3 constrainedTextSize = Min( textSize, TEXT_LABEL_MAX_SIZE );
  Vector3 buttonSize( Max(constrainedTextSize + BUTTON_TEXT_ENLARGE, BUTTON_TEXT_MINIMUM_SIZE) );
  buttonSize = ( Min(buttonSize, BUTTON_TEXT_MAXIMUM_SIZE) );
  label.SetSize( Min( buttonSize + BUTTON_TEXT_ENLARGE, constrainedTextSize ) );

  button.SetParentOrigin( ParentOrigin::TOP_LEFT );
  button.SetAnchorPoint( AnchorPoint::TOP_LEFT );
  button.SetSize( buttonSize );
  button.SetPosition( Vector3( mContentSize.width, POPUP_BORDER.y, 0.0f ) );

  // 2. Add icon
  ImageActor icon = CreateOptionIcon( iconImage );

  iconTextContainer.Add( icon );

  // 3. Add highlight - Pressed state in Pushbutton needs a new image which means creating the text and icon again but including a highlight this time.
  ImageActor pressedImageBg = CreatePressedBackground( buttonSize );

  Actor iconPressedTextContainer = Actor::New();
  iconPressedTextContainer.SetDrawMode( DrawMode::OVERLAY );

  Toolkit::TextView pressedLabel = CreateOptionText( styledCaption );
  pressedLabel.SetSize( Min( buttonSize, TEXT_LABEL_MAX_SIZE ) );
  ImageActor pressedIcon = CreateOptionIcon( iconImage );

  iconPressedTextContainer.Add( pressedImageBg );
  iconPressedTextContainer.Add( pressedLabel );
  iconPressedTextContainer.Add( pressedIcon );

  // Set Pressed button Image
  iconPressedTextContainer.SetSize( buttonSize );
  button.SetPressedImage( iconPressedTextContainer );

  // Set Normal button Image
  iconTextContainer.SetSize( buttonSize );
  button.SetButtonImage( iconTextContainer );
  mBackground.Add( button );

  // Update content size (represents size of all content i.e. from top-left of first button, to bottom-right of last button)
  mContentSize.width += buttonSize.width;
  mContentSize.height = std::max(mContentSize.height + ( POPUP_BORDER.y ), buttonSize.height);
  mButtonContainer.push_back(button);

  // resize all dividers based on the height content (i.e. max of all button heights)
  const float dividerHeight = mContentSize.height - DIVIDER_MARGIN;
  for(ActorIter i = mDividerContainer.begin(); i != mDividerContainer.end(); ++i)
  {
    i->SetSize( DIVIDER_WIDTH, dividerHeight );
  }

  Vector3 popupSize( Max(mContentSize, POPUP_MINIMUM_SIZE) );

  mBackground.SetSize( popupSize );
  // Make Root Actor reflect the size of its content
  mRootActor.SetSize( popupSize );
  mTail.SetPosition(Vector3(0.0f, -20.0f, 0.0f));

  button.ClickedSignal().Connect( this, &TextInputPopup::OnButtonPressed );
}

void TextInputPopup::Hide(bool animate)
{
  if(mBackground)
  {
    if(mAnimation)
    {
      mAnimation.Clear();
      mAnimation.Reset();
    }

    if(animate)
    {
      mAnimation = Animation::New( HIDE_POPUP_ANIMATION_DURATION );
      mAnimation.AnimateTo( Property(mBackground, Actor::SCALE), Vector3::ZERO, AlphaFunctions::EaseOut );
      mAnimation.AnimateTo( Property(mBackground, Actor::COLOR_ALPHA), 0.0f, AlphaFunctions::EaseOut );
      mAnimation.Play();

      mAnimation.FinishedSignal().Connect( this, &TextInputPopup::OnHideFinished );
      mState = StateHiding;
    }
    else
    {
      mBackground.SetProperty(Actor::SCALE, Vector3::ZERO);
      mBackground.SetProperty(Actor::COLOR_ALPHA, 0.0f);
      mState = StateHidden;
    }
  }
}

void TextInputPopup::Show(bool animate)
{
  if(mBackground)
  {
    if(mAnimation)
    {
      mAnimation.Clear();
      mAnimation.Reset();
    }

    if(animate)
    {
      mAnimation = Animation::New( SHOW_POPUP_ANIMATION_DURATION );
      mAnimation.AnimateTo( Property(mBackground, Actor::SCALE), Vector3::ONE, AlphaFunctions::EaseOut );
      mAnimation.AnimateTo( Property(mBackground, Actor::COLOR_ALPHA), 1.0f, AlphaFunctions::EaseOut );
      mAnimation.Play();

      mAnimation.FinishedSignal().Connect( this, &TextInputPopup::OnShowFinished );
      mState = StateShowing;
    }
    else
    {
      mBackground.SetProperty(Actor::SCALE, Vector3::ONE);
      mBackground.SetProperty(Actor::COLOR_ALPHA, 1.0f);
      mState = StateShown;
    }
  }
}

void TextInputPopup::SetAlternativeOffset(Vector2 offset)
{
  mRootActor.SetProperty( mAlternativeOffsetProperty, offset );
  ApplyConfinementConstraint();
}

TextInputPopup::State TextInputPopup::GetState(void) const
{
  return mState;
}

Actor TextInputPopup::GetRootActor() const
{
  return mRootActor;
}

// Styling

void TextInputPopup::SetCutPastePopUpColor( const Vector4& color )
{
  mCutPasteButtonsColor = color;
}

const Vector4& TextInputPopup::GetCutPastePopUpColor() const
{
  return mCutPasteButtonsColor;
}

void TextInputPopup::SetCutPastePopUpPressedColor( const Vector4& color )
{
  mCutPasteButtonsPressedColor = color;
}

const Vector4& TextInputPopup::GetCutPastePopUpPressedColor() const
{
  return mCutPasteButtonsPressedColor;
}

void TextInputPopup::TogglePopUpButtonOnOff( TextInputPopup::Buttons requiredButton, bool enable )
{
  bool match ( false );
  for( std::vector<ButtonRequirement>::iterator it = mOrderListOfButtons.begin(), endIt = mOrderListOfButtons.end(); ( it != endIt && !match ); ++it )
   {
     ButtonRequirement& button( *it );
     if ( requiredButton == button.buttonId )
     {
       button.enabled = enable;
       match = true;
     }
   }
}

void TextInputPopup::SetButtonPriorityPosition( TextInputPopup::Buttons button, unsigned int priority )
{
  switch ( button )
  {
    case ButtonsCut:
    {
      mCutOptionPriority = priority;
      break;
    }
    case ButtonsCopy:
    {
      mCopyOptionPriority = priority;
      break;
    }
    case ButtonsPaste:
    {
      mPasteOptionPriority = priority;
      break;
    }
    case ButtonsSelect:
    {
      mSelectOptionPriority = priority;
      break;
    }
    case ButtonsSelectAll:
    {
      mSelectAllOptionPriority = priority;
      break;
    }
    case ButtonsClipboard:
    {
      mClipboardOptionPriority = priority;
      break;
    }
    case ButtonsEnumEnd:
    {
      DALI_ASSERT_DEBUG( "ButtonsEnumEnd used but an invalid choice");
      break;
    }
  }
  CreateOrderedListOfOptions(); // Update list of options as priority changed.
}

unsigned int TextInputPopup::GetButtonPriorityPosition( TextInputPopup::Buttons button ) const
{
  unsigned int priority = 0;

  switch ( button )
  {
    case ButtonsCut:
    {
      priority = mCutOptionPriority;
      break;
    }
    case ButtonsCopy:
    {
      priority = mCopyOptionPriority;
      break;
    }
    case ButtonsPaste:
    {
      priority = mPasteOptionPriority;
      break;
    }
    case ButtonsSelect:
    {
      priority = mSelectOptionPriority;
      break;
    }
    case ButtonsSelectAll:
    {
      priority = mSelectAllOptionPriority;
      break;
    }
    case ButtonsClipboard:
    {
      priority = mClipboardOptionPriority;
      break;
    }
    case ButtonsEnumEnd:
    {
      DALI_ASSERT_DEBUG( "ButtonsEnumEnd used but an invalid choice");
      break;
    }
  }

  return priority;
}

void TextInputPopup::AddPopupOptions()
{
  for( std::vector<ButtonRequirement>::const_iterator it = mOrderListOfButtons.begin(), endIt = mOrderListOfButtons.end(); ( it != endIt ); ++it )
  {
    const ButtonRequirement& button( *it );
    if (  button.enabled )
    {
      AddOption( button.name, button.caption, button.iconImage, false );
    }
  }
}


bool TextInputPopup::OnButtonPressed( Toolkit::Button button )
{
  mPressedSignal.Emit( button );
  return false;
}

void TextInputPopup::OnHideFinished(Animation& source)
{
  source.FinishedSignal().Disconnect( this, &TextInputPopup::OnHideFinished );
  Clear();
  mState = StateHidden;
  mHideFinishedSignal.Emit( *this );
}

void TextInputPopup::OnShowFinished(Animation& source)
{
  source.FinishedSignal().Disconnect( this, &TextInputPopup::OnShowFinished );
  mState = StateShown;
  mShowFinishedSignal.Emit( *this );
}

TextInputPopup::PressedSignalV2& TextInputPopup::PressedSignal()
{
  return mPressedSignal;
}

TextInputPopup::HideFinishedSignalV2& TextInputPopup::HideFinishedSignal()
{
  return mHideFinishedSignal;
}

TextInputPopup::ShowFinishedSignalV2& TextInputPopup::ShowFinishedSignal()
{
  return mShowFinishedSignal;
}

} // namespace Internal

} // namespace Toolkit

} // namespace Dali
