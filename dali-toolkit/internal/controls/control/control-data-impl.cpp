/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

// CLASS HEADER
#include "control-data-impl.h"

// EXTERNAL INCLUDES
#include <dali/public-api/common/dali-common.h>
#include <dali/integration-api/debug.h>
#include <dali/devel-api/object/handle-devel.h>
#include <dali/devel-api/scripting/enum-helper.h>
#include <dali/devel-api/scripting/scripting.h>
#include <dali/integration-api/debug.h>
#include <dali/public-api/object/type-registry-helper.h>
#include <cstring>
#include <limits>

// INTERNAL INCLUDES
#include <dali-toolkit/internal/visuals/visual-base-impl.h>
#include <dali-toolkit/devel-api/controls/control-depth-index-ranges.h>
#include <dali-toolkit/internal/styling/style-manager-impl.h>
#include <dali-toolkit/public-api/visuals/image-visual-properties.h>
#include <dali-toolkit/devel-api/visuals/visual-properties-devel.h>
#include <dali-toolkit/internal/visuals/visual-string-constants.h>
#include <dali-toolkit/devel-api/controls/control-devel.h>

namespace Dali
{

namespace Toolkit
{

namespace Internal
{

extern const Dali::Scripting::StringEnum ControlStateTable[];
extern const unsigned int ControlStateTableCount;


// Not static or anonymous - shared with other translation units
const Scripting::StringEnum ControlStateTable[] = {
  { "NORMAL",   Toolkit::DevelControl::NORMAL   },
  { "FOCUSED",  Toolkit::DevelControl::FOCUSED  },
  { "DISABLED", Toolkit::DevelControl::DISABLED },
};
const unsigned int ControlStateTableCount = sizeof( ControlStateTable ) / sizeof( ControlStateTable[0] );



namespace
{

#if defined(DEBUG_ENABLED)
Debug::Filter* gLogFilter = Debug::Filter::New( Debug::NoLogging, false, "LOG_CONTROL_VISUALS");
#endif


template<typename T>
void Remove( Dictionary<T>& keyValues, const std::string& name )
{
  keyValues.Remove(name);
}

void Remove( DictionaryKeys& keys, const std::string& name )
{
  DictionaryKeys::iterator iter = std::find( keys.begin(), keys.end(), name );
  if( iter != keys.end())
  {
    keys.erase(iter);
  }
}

Toolkit::DevelVisual::Type GetVisualTypeFromMap( const Property::Map& map )
{
  Property::Value* typeValue = map.Find( Toolkit::DevelVisual::Property::TYPE, VISUAL_TYPE  );
  Toolkit::DevelVisual::Type type = Toolkit::DevelVisual::IMAGE;
  if( typeValue )
  {
    Scripting::GetEnumerationProperty( *typeValue, VISUAL_TYPE_TABLE, VISUAL_TYPE_TABLE_COUNT, type );
  }
  return type;
}

/**
 *  Finds visual in given array, returning true if found along with the iterator for that visual as a out parameter
 */
bool FindVisual( Property::Index targetIndex, const RegisteredVisualContainer& visuals, RegisteredVisualContainer::Iterator& iter )
{
  for ( iter = visuals.Begin(); iter != visuals.End(); iter++ )
  {
    if ( (*iter)->index ==  targetIndex )
    {
      return true;
    }
  }
  return false;
}

void FindChangableVisuals( Dictionary<Property::Map>& stateVisualsToAdd,
                           Dictionary<Property::Map>& stateVisualsToChange,
                           DictionaryKeys& stateVisualsToRemove)
{
  DictionaryKeys copyOfStateVisualsToRemove = stateVisualsToRemove;

  for( DictionaryKeys::iterator iter = copyOfStateVisualsToRemove.begin();
       iter != copyOfStateVisualsToRemove.end(); ++iter )
  {
    const std::string& visualName = (*iter);
    Property::Map* toMap = stateVisualsToAdd.Find( visualName );
    if( toMap )
    {
      stateVisualsToChange.Add( visualName, *toMap );
      stateVisualsToAdd.Remove( visualName );
      Remove( stateVisualsToRemove, visualName );
    }
  }
}

Toolkit::Visual::Base GetVisualByName(
  const RegisteredVisualContainer& visuals,
  const std::string& visualName )
{
  Toolkit::Visual::Base visualHandle;

  RegisteredVisualContainer::Iterator iter;
  for ( iter = visuals.Begin(); iter != visuals.End(); iter++ )
  {
    Toolkit::Visual::Base visual = (*iter)->visual;
    if( visual && visual.GetName() == visualName )
    {
      visualHandle = visual;
      break;
    }
  }
  return visualHandle;
}

/**
 * Performs actions as requested using the action name.
 * @param[in] object The object on which to perform the action.
 * @param[in] actionName The action to perform.
 * @param[in] attributes The attributes with which to perfrom this action.
 * @return true if action has been accepted by this control
 */
const char* ACTION_ACCESSIBILITY_ACTIVATED = "accessibilityActivated";
static bool DoAction( BaseObject* object, const std::string& actionName, const Property::Map& attributes )
{
  bool ret = false;

  if( object && ( 0 == strcmp( actionName.c_str(), ACTION_ACCESSIBILITY_ACTIVATED ) ) )
  {
    Toolkit::Control control = Toolkit::Control::DownCast( BaseHandle( object ) );
    if( control )
    {
      // if cast succeeds there is an implementation so no need to check
      ret = Internal::GetImplementation( control ).OnAccessibilityActivated();
    }
  }

  return ret;
}

/**
 * Connects a callback function with the object's signals.
 * @param[in] object The object providing the signal.
 * @param[in] tracker Used to disconnect the signal.
 * @param[in] signalName The signal to connect to.
 * @param[in] functor A newly allocated FunctorDelegate.
 * @return True if the signal was connected.
 * @post If a signal was connected, ownership of functor was passed to CallbackBase. Otherwise the caller is responsible for deleting the unused functor.
 */
const char* SIGNAL_KEY_EVENT = "keyEvent";
const char* SIGNAL_KEY_INPUT_FOCUS_GAINED = "keyInputFocusGained";
const char* SIGNAL_KEY_INPUT_FOCUS_LOST = "keyInputFocusLost";
const char* SIGNAL_TAPPED = "tapped";
const char* SIGNAL_PANNED = "panned";
const char* SIGNAL_PINCHED = "pinched";
const char* SIGNAL_LONG_PRESSED = "longPressed";
static bool DoConnectSignal( BaseObject* object, ConnectionTrackerInterface* tracker, const std::string& signalName, FunctorDelegate* functor )
{
  Dali::BaseHandle handle( object );

  bool connected( false );
  Toolkit::Control control = Toolkit::Control::DownCast( handle );
  if ( control )
  {
    Internal::Control& controlImpl( Internal::GetImplementation( control ) );
    connected = true;

    if ( 0 == strcmp( signalName.c_str(), SIGNAL_KEY_EVENT ) )
    {
      controlImpl.KeyEventSignal().Connect( tracker, functor );
    }
    else if( 0 == strcmp( signalName.c_str(), SIGNAL_KEY_INPUT_FOCUS_GAINED ) )
    {
      controlImpl.KeyInputFocusGainedSignal().Connect( tracker, functor );
    }
    else if( 0 == strcmp( signalName.c_str(), SIGNAL_KEY_INPUT_FOCUS_LOST ) )
    {
      controlImpl.KeyInputFocusLostSignal().Connect( tracker, functor );
    }
    else if( 0 == strcmp( signalName.c_str(), SIGNAL_TAPPED ) )
    {
      controlImpl.EnableGestureDetection( Gesture::Tap );
      controlImpl.GetTapGestureDetector().DetectedSignal().Connect( tracker, functor );
    }
    else if( 0 == strcmp( signalName.c_str(), SIGNAL_PANNED ) )
    {
      controlImpl.EnableGestureDetection( Gesture::Pan );
      controlImpl.GetPanGestureDetector().DetectedSignal().Connect( tracker, functor );
    }
    else if( 0 == strcmp( signalName.c_str(), SIGNAL_PINCHED ) )
    {
      controlImpl.EnableGestureDetection( Gesture::Pinch );
      controlImpl.GetPinchGestureDetector().DetectedSignal().Connect( tracker, functor );
    }
    else if( 0 == strcmp( signalName.c_str(), SIGNAL_LONG_PRESSED ) )
    {
      controlImpl.EnableGestureDetection( Gesture::LongPress );
      controlImpl.GetLongPressGestureDetector().DetectedSignal().Connect( tracker, functor );
    }
  }
  return connected;
}

/**
 * Creates control through type registry
 */
BaseHandle Create()
{
  return Internal::Control::New();
}
// Setup signals and actions using the type-registry.
DALI_TYPE_REGISTRATION_BEGIN( Control, CustomActor, Create );

// Note: Properties are registered separately below.

SignalConnectorType registerSignal1( typeRegistration, SIGNAL_KEY_EVENT, &DoConnectSignal );
SignalConnectorType registerSignal2( typeRegistration, SIGNAL_KEY_INPUT_FOCUS_GAINED, &DoConnectSignal );
SignalConnectorType registerSignal3( typeRegistration, SIGNAL_KEY_INPUT_FOCUS_LOST, &DoConnectSignal );
SignalConnectorType registerSignal4( typeRegistration, SIGNAL_TAPPED, &DoConnectSignal );
SignalConnectorType registerSignal5( typeRegistration, SIGNAL_PANNED, &DoConnectSignal );
SignalConnectorType registerSignal6( typeRegistration, SIGNAL_PINCHED, &DoConnectSignal );
SignalConnectorType registerSignal7( typeRegistration, SIGNAL_LONG_PRESSED, &DoConnectSignal );

TypeAction registerAction( typeRegistration, ACTION_ACCESSIBILITY_ACTIVATED, &DoAction );

DALI_TYPE_REGISTRATION_END()

} // unnamed namespace


// Properties registered without macro to use specific member variables.
const PropertyRegistration Control::Impl::PROPERTY_1( typeRegistration, "styleName",              Toolkit::Control::Property::STYLE_NAME,                   Property::STRING,  &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_2( typeRegistration, "backgroundColor",        Toolkit::Control::Property::BACKGROUND_COLOR,             Property::VECTOR4, &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_3( typeRegistration, "backgroundImage",        Toolkit::Control::Property::BACKGROUND_IMAGE,             Property::MAP,     &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_4( typeRegistration, "keyInputFocus",          Toolkit::Control::Property::KEY_INPUT_FOCUS,              Property::BOOLEAN, &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_5( typeRegistration, "background",             Toolkit::Control::Property::BACKGROUND,                   Property::MAP,     &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_6( typeRegistration, "tooltip",                Toolkit::DevelControl::Property::TOOLTIP,                 Property::MAP,     &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_7( typeRegistration, "state",                  Toolkit::DevelControl::Property::STATE,                   Property::STRING,  &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_8( typeRegistration, "subState",               Toolkit::DevelControl::Property::SUB_STATE,               Property::STRING,  &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_9( typeRegistration, "leftFocusableActorId",   Toolkit::DevelControl::Property::LEFT_FOCUSABLE_ACTOR_ID, Property::INTEGER, &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_10( typeRegistration, "rightFocusableActorId", Toolkit::DevelControl::Property::RIGHT_FOCUSABLE_ACTOR_ID,Property::INTEGER, &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_11( typeRegistration, "upFocusableActorId",    Toolkit::DevelControl::Property::UP_FOCUSABLE_ACTOR_ID,   Property::INTEGER, &Control::Impl::SetProperty, &Control::Impl::GetProperty );
const PropertyRegistration Control::Impl::PROPERTY_12( typeRegistration, "downFocusableActorId",  Toolkit::DevelControl::Property::DOWN_FOCUSABLE_ACTOR_ID, Property::INTEGER, &Control::Impl::SetProperty, &Control::Impl::GetProperty );



Control::Impl::Impl( Control& controlImpl )
: mControlImpl( controlImpl ),
  mState( Toolkit::DevelControl::NORMAL ),
  mSubStateName(""),
  mLeftFocusableActorId( -1 ),
  mRightFocusableActorId( -1 ),
  mUpFocusableActorId( -1 ),
  mDownFocusableActorId( -1 ),
  mStyleName(""),
  mBackgroundColor(Color::TRANSPARENT),
  mStartingPinchScale( NULL ),
  mKeyEventSignal(),
  mPinchGestureDetector(),
  mPanGestureDetector(),
  mTapGestureDetector(),
  mLongPressGestureDetector(),
  mFlags( Control::ControlBehaviour( CONTROL_BEHAVIOUR_DEFAULT ) ),
  mIsKeyboardNavigationSupported( false ),
  mIsKeyboardFocusGroup( false )
{

}

Control::Impl::~Impl()
{
  // All gesture detectors will be destroyed so no need to disconnect.
  delete mStartingPinchScale;
}

Control::Impl& Control::Impl::Get( Internal::Control& internalControl )
{
  return *internalControl.mImpl;
}

const Control::Impl& Control::Impl::Get( const Internal::Control& internalControl )
{
  return *internalControl.mImpl;
}

// Gesture Detection Methods
void Control::Impl::PinchDetected(Actor actor, const PinchGesture& pinch)
{
  mControlImpl.OnPinch(pinch);
}

void Control::Impl::PanDetected(Actor actor, const PanGesture& pan)
{
  mControlImpl.OnPan(pan);
}

void Control::Impl::TapDetected(Actor actor, const TapGesture& tap)
{
  mControlImpl.OnTap(tap);
}

void Control::Impl::LongPressDetected(Actor actor, const LongPressGesture& longPress)
{
  mControlImpl.OnLongPress(longPress);
}

// Called by a Visual when it's resource is ready
void Control::Impl::ResourceReady( Visual::Base& object)
{

  // go through and check if all the visuals are ready, if they are emit a signal
  for ( RegisteredVisualContainer::ConstIterator visualIter = mVisuals.Begin();
        visualIter != mVisuals.End(); ++visualIter )
  {
    const Toolkit::Visual::Base visual = (*visualIter)->visual;
    const Internal::Visual::Base& visualImpl = Toolkit::GetImplementation( visual );

    // one of the visuals is not ready
    if( !visualImpl.IsResourceReady() )
    {
      return;
    }
  }

  // all the visuals are ready
  Dali::Toolkit::Control handle( mControlImpl.GetOwner() );
  mResourceReadySignal.Emit( handle );

}

bool Control::Impl::IsResourceReady() const
{
  // go through and check all the visuals are ready
  for ( RegisteredVisualContainer::ConstIterator visualIter = mVisuals.Begin();
         visualIter != mVisuals.End(); ++visualIter )
   {
     const Toolkit::Visual::Base visual = (*visualIter)->visual;
     const Internal::Visual::Base& visualImpl = Toolkit::GetImplementation( visual );

     // one of the visuals is not ready
     if( !visualImpl.IsResourceReady()  )
     {
       return false;
     }
   }
  return true;
}

void Control::Impl::RegisterVisual( Property::Index index, Toolkit::Visual::Base& visual )
{
  RegisterVisual( index, visual, VisualState::ENABLED, DepthIndexValue::NOT_SET );
}

void Control::Impl::RegisterVisual( Property::Index index, Toolkit::Visual::Base& visual, float depthIndex )
{
  RegisterVisual( index, visual, VisualState::ENABLED, DepthIndexValue::SET, depthIndex );
}

void Control::Impl::RegisterVisual( Property::Index index, Toolkit::Visual::Base& visual, bool enabled )
{
  RegisterVisual( index, visual, ( enabled ? VisualState::ENABLED : VisualState::DISABLED ), DepthIndexValue::NOT_SET );
}

void Control::Impl::RegisterVisual( Property::Index index, Toolkit::Visual::Base& visual, bool enabled, float depthIndex )
{
  RegisterVisual( index, visual, ( enabled ? VisualState::ENABLED : VisualState::DISABLED ), DepthIndexValue::SET, depthIndex );
}

void Control::Impl::RegisterVisual( Property::Index index, Toolkit::Visual::Base& visual, VisualState::Type enabled, DepthIndexValue::Type depthIndexValueSet, float depthIndex )
{
  bool visualReplaced ( false );
  Actor self = mControlImpl.Self();

  if( !mVisuals.Empty() )
  {
    RegisteredVisualContainer::Iterator iter;
    // Check if visual (index) is already registered.  Replace if so.
    if ( FindVisual( index, mVisuals, iter ) )
    {
      if( (*iter)->visual && self.OnStage() )
      {
        Toolkit::GetImplementation((*iter)->visual).SetOffStage( self );
      }

      // If we've not set the depth-index value and the new visual does not have a depth index applied to it, then use the previously set depth-index for this index
      if( ( depthIndexValueSet == DepthIndexValue::NOT_SET ) &&
          EqualsZero( visual.GetDepthIndex() ) )
      {
        const float currentDepthIndex = (*iter)->visual.GetDepthIndex();
        visual.SetDepthIndex( currentDepthIndex );
      }

      StopObservingVisual( (*iter)->visual );
      StartObservingVisual( visual );

      (*iter)->visual = visual;
      (*iter)->enabled = ( enabled == VisualState::ENABLED ) ? true : false;
      visualReplaced = true;
    }
  }

  // If not set, set the name of the visual to the same name as the control's property.
  // ( If the control has been type registered )
  if( visual.GetName().empty() )
  {
    // Check if the control has been type registered:
    TypeInfo typeInfo = TypeRegistry::Get().GetTypeInfo( typeid( mControlImpl ) );
    if( typeInfo )
    {
      // Check if the property index has been registered:
      Property::IndexContainer indices;
      typeInfo.GetPropertyIndices( indices );
      Property::IndexContainer::Iterator iter = std::find( indices.Begin(), indices.End(), index );
      if( iter != indices.End() )
      {
        // If it has, then get it's name and use that for the visual
        std::string visualName = typeInfo.GetPropertyName( index );
        visual.SetName( visualName );
      }
    }
  }

  if( !visualReplaced ) // New registration entry
  {
    mVisuals.PushBack( new RegisteredVisual( index, visual, ( enabled == VisualState::ENABLED ? true : false ) ) );

    // monitor when the visuals resources are ready
    StartObservingVisual( visual );

    // If we've not set the depth-index value, we have more than one visual and the visual does not have a depth index, then set it to be the highest
    if( ( depthIndexValueSet == DepthIndexValue::NOT_SET ) &&
        ( mVisuals.Size() > 1 ) &&
        EqualsZero( visual.GetDepthIndex() ) )
    {
      float maxDepthIndex = std::numeric_limits< float >::min();

      RegisteredVisualContainer::ConstIterator iter;
      const RegisteredVisualContainer::ConstIterator endIter = mVisuals.End();
      for ( iter = mVisuals.Begin(); iter != endIter; iter++ )
      {
        const float visualDepthIndex = (*iter)->visual.GetDepthIndex();
        if ( visualDepthIndex > maxDepthIndex )
        {
          maxDepthIndex = visualDepthIndex;
        }
      }

      ++maxDepthIndex; // Add one to the current maximum depth index so that our added visual appears on top
      visual.SetDepthIndex( maxDepthIndex );
    }
  }

  if( visual )
  {
    // If the caller has set the depth-index, then set it here
    if( depthIndexValueSet == DepthIndexValue::SET )
    {
      visual.SetDepthIndex( depthIndex );
    }

    // Put on stage if enabled and the control is already on the stage
    if( ( enabled == VisualState::ENABLED ) && self.OnStage() )
    {
      Toolkit::GetImplementation(visual).SetOnStage( self );
    }
  }

  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "Control::RegisterVisual() Registered %s(%d), enabled:%s\n",  visual.GetName().c_str(), index, enabled?"T":"F" );
}

void Control::Impl::UnregisterVisual( Property::Index index )
{
   RegisteredVisualContainer::Iterator iter;
   if ( FindVisual( index, mVisuals, iter ) )
   {
     // stop observing visual
     StopObservingVisual( (*iter)->visual );

     Actor self( mControlImpl.Self() );
     Toolkit::GetImplementation((*iter)->visual).SetOffStage( self );
     (*iter)->visual.Reset();
     mVisuals.Erase( iter );
   }
}

Toolkit::Visual::Base Control::Impl::GetVisual( Property::Index index ) const
{
  RegisteredVisualContainer::Iterator iter;
  if ( FindVisual( index, mVisuals, iter ) )
  {
    return (*iter)->visual;
  }

  return Toolkit::Visual::Base();
}

void Control::Impl::EnableVisual( Property::Index index, bool enable )
{
  RegisteredVisualContainer::Iterator iter;
  if ( FindVisual( index, mVisuals, iter ) )
  {
    if (  (*iter)->enabled == enable )
    {
      DALI_LOG_INFO( gLogFilter, Debug::Verbose, "Control::EnableVisual Visual %s(%d) already %s\n", (*iter)->visual.GetName().c_str(), index, enable?"enabled":"disabled");
      return;
    }

    (*iter)->enabled = enable;
    Actor parentActor = mControlImpl.Self();
    if ( mControlImpl.Self().OnStage() ) // If control not on Stage then Visual will be added when StageConnection is called.
    {
      if ( enable )
      {
        DALI_LOG_INFO( gLogFilter, Debug::Verbose, "Control::EnableVisual Setting %s(%d) on stage \n", (*iter)->visual.GetName().c_str(), index );
        Toolkit::GetImplementation((*iter)->visual).SetOnStage( parentActor );
      }
      else
      {
        DALI_LOG_INFO( gLogFilter, Debug::Verbose, "Control::EnableVisual Setting %s(%d) off stage \n", (*iter)->visual.GetName().c_str(), index );
        Toolkit::GetImplementation((*iter)->visual).SetOffStage( parentActor );  // No need to call if control not staged.
      }
    }
  }
}

bool Control::Impl::IsVisualEnabled( Property::Index index ) const
{
  RegisteredVisualContainer::Iterator iter;
  if ( FindVisual( index, mVisuals, iter ) )
  {
    return (*iter)->enabled;
  }
  return false;
}

void Control::Impl::StopObservingVisual( Toolkit::Visual::Base& visual )
{
  Internal::Visual::Base& visualImpl = Toolkit::GetImplementation( visual );

  // Stop observing the visual
  visualImpl.RemoveResourceObserver( *this );
}

void Control::Impl::StartObservingVisual( Toolkit::Visual::Base& visual)
{
  Internal::Visual::Base& visualImpl = Toolkit::GetImplementation( visual );

  // start observing the visual for resource ready
  visualImpl.AddResourceObserver( *this );
}

Dali::Animation Control::Impl::CreateTransition( const Toolkit::TransitionData& handle )
{
  Dali::Animation transition;
  const Internal::TransitionData& transitionData = Toolkit::GetImplementation( handle );

  if( transitionData.Count() > 0 )
  {
    // Setup a Transition from TransitionData.
    TransitionData::Iterator end = transitionData.End();
    for( TransitionData::Iterator iter = transitionData.Begin() ;
         iter != end; ++iter )
    {
      TransitionData::Animator* animator = (*iter);

      Toolkit::Visual::Base visual = GetVisualByName( mVisuals, animator->objectName );

      if( visual )
      {
        Internal::Visual::Base& visualImpl = Toolkit::GetImplementation( visual );
        visualImpl.AnimateProperty( transition, *animator );
      }
      else
      {
        // Otherwise, try any actor children of control (Including the control)
        Actor child = mControlImpl.Self().FindChildByName( animator->objectName );
        if( child )
        {
          Property::Index propertyIndex = DevelHandle::GetPropertyIndex( child, animator->propertyKey );
          if( propertyIndex != Property::INVALID_INDEX )
          {
            if( animator->animate == false )
            {
              if( animator->targetValue.GetType() != Property::NONE )
              {
                child.SetProperty( propertyIndex, animator->targetValue );
              }
            }
            else // animate the property
            {
              if( animator->initialValue.GetType() != Property::NONE )
              {
                child.SetProperty( propertyIndex, animator->initialValue );
              }

              if( ! transition )
              {
                transition = Dali::Animation::New( 0.1f );
              }

              transition.AnimateTo( Property( child, propertyIndex ),
                                    animator->targetValue,
                                    animator->alphaFunction,
                                    TimePeriod( animator->timePeriodDelay,
                                                animator->timePeriodDuration ) );
            }
          }
        }
      }
    }
  }

  return transition;
}

void Control::Impl::SetProperty( BaseObject* object, Property::Index index, const Property::Value& value )
{
  Toolkit::Control control = Toolkit::Control::DownCast( BaseHandle( object ) );

  if ( control )
  {
    Control& controlImpl( GetImplementation( control ) );

    switch ( index )
    {
      case Toolkit::Control::Property::STYLE_NAME:
      {
        controlImpl.SetStyleName( value.Get< std::string >() );
        break;
      }

      case Toolkit::DevelControl::Property::STATE:
      {
        bool withTransitions=true;
        const Property::Value* valuePtr=&value;
        Property::Map* map = value.GetMap();
        if(map)
        {
          Property::Value* value2 = map->Find("withTransitions");
          if( value2 )
          {
            withTransitions = value2->Get<bool>();
          }

          valuePtr = map->Find("state");
        }

        if( valuePtr )
        {
          Toolkit::DevelControl::State state( controlImpl.mImpl->mState );
          if( Scripting::GetEnumerationProperty< Toolkit::DevelControl::State >( *valuePtr, ControlStateTable, ControlStateTableCount, state ) )
          {
            controlImpl.mImpl->SetState( state, withTransitions );
          }
        }
      }
      break;

      case Toolkit::DevelControl::Property::SUB_STATE:
      {
        std::string subState;
        if( value.Get( subState ) )
        {
          controlImpl.mImpl->SetSubState( subState );
        }
      }
      break;

      case Toolkit::DevelControl::Property::LEFT_FOCUSABLE_ACTOR_ID:
      {
        int focusId;
        if( value.Get( focusId ) )
        {
          controlImpl.mImpl->mLeftFocusableActorId = focusId;
        }
      }
      break;

      case Toolkit::DevelControl::Property::RIGHT_FOCUSABLE_ACTOR_ID:
      {
        int focusId;
        if( value.Get( focusId ) )
        {
          controlImpl.mImpl->mRightFocusableActorId = focusId;
        }
      }
      break;

      case Toolkit::DevelControl::Property::UP_FOCUSABLE_ACTOR_ID:
      {
        int focusId;
        if( value.Get( focusId ) )
        {
          controlImpl.mImpl->mUpFocusableActorId = focusId;
        }
      }
      break;

      case Toolkit::DevelControl::Property::DOWN_FOCUSABLE_ACTOR_ID:
      {
        int focusId;
        if( value.Get( focusId ) )
        {
          controlImpl.mImpl->mDownFocusableActorId = focusId;
        }
      }
      break;

      case Toolkit::Control::Property::BACKGROUND_COLOR:
      {
        DALI_LOG_WARNING( "BACKGROUND_COLOR property is deprecated. Use BACKGROUND property instead\n" );
        controlImpl.SetBackgroundColor( value.Get< Vector4 >() );
        break;
      }

      case Toolkit::Control::Property::BACKGROUND_IMAGE:
      {
        DALI_LOG_WARNING( "BACKGROUND_IMAGE property is deprecated. Use BACKGROUND property instead\n" );
        Image image = Scripting::NewImage( value );
        if ( image )
        {
          controlImpl.SetBackgroundImage( image );
        }
        else
        {
          // An empty image means the background is no longer required
          controlImpl.ClearBackground();
        }
        break;
      }

      case Toolkit::Control::Property::KEY_INPUT_FOCUS:
      {
        if ( value.Get< bool >() )
        {
          controlImpl.SetKeyInputFocus();
        }
        else
        {
          controlImpl.ClearKeyInputFocus();
        }
        break;
      }

      case Toolkit::Control::Property::BACKGROUND:
      {
        std::string url;
        Vector4 color;
        const Property::Map* map = value.GetMap();
        if( map && !map->Empty() )
        {
          controlImpl.SetBackground( *map );
        }
        else if( value.Get( url ) )
        {
          // don't know the size to load
          Toolkit::Visual::Base visual = Toolkit::VisualFactory::Get().CreateVisual( url, ImageDimensions() );
          if( visual )
          {
            controlImpl.mImpl->RegisterVisual( Toolkit::Control::Property::BACKGROUND, visual, float( DepthIndex::BACKGROUND ) );
          }
        }
        else if( value.Get( color ) )
        {
          controlImpl.SetBackgroundColor(color);
        }
        else
        {
          // The background is an empty property map, so we should clear the background
          controlImpl.ClearBackground();
        }
        break;
      }

      case Toolkit::DevelControl::Property::TOOLTIP:
      {
        TooltipPtr& tooltipPtr = controlImpl.mImpl->mTooltip;
        if( ! tooltipPtr )
        {
          tooltipPtr = Tooltip::New( control );
        }
        tooltipPtr->SetProperties( value );
      }
    }
  }
}

Property::Value Control::Impl::GetProperty( BaseObject* object, Property::Index index )
{
  Property::Value value;

  Toolkit::Control control = Toolkit::Control::DownCast( BaseHandle( object ) );

  if ( control )
  {
    Control& controlImpl( GetImplementation( control ) );

    switch ( index )
    {
      case Toolkit::Control::Property::STYLE_NAME:
      {
        value = controlImpl.GetStyleName();
        break;
      }

      case Toolkit::DevelControl::Property::STATE:
      {
        value = controlImpl.mImpl->mState;
        break;
      }

      case Toolkit::DevelControl::Property::SUB_STATE:
      {
        value = controlImpl.mImpl->mSubStateName;
        break;
      }

      case Toolkit::DevelControl::Property::LEFT_FOCUSABLE_ACTOR_ID:
      {
        value = controlImpl.mImpl->mLeftFocusableActorId;
        break;
      }

      case Toolkit::DevelControl::Property::RIGHT_FOCUSABLE_ACTOR_ID:
      {
        value = controlImpl.mImpl->mRightFocusableActorId;
        break;
      }

      case Toolkit::DevelControl::Property::UP_FOCUSABLE_ACTOR_ID:
      {
        value = controlImpl.mImpl->mUpFocusableActorId;
        break;
      }

      case Toolkit::DevelControl::Property::DOWN_FOCUSABLE_ACTOR_ID:
      {
        value = controlImpl.mImpl->mDownFocusableActorId;
        break;
      }

      case Toolkit::Control::Property::BACKGROUND_COLOR:
      {
        DALI_LOG_WARNING( "BACKGROUND_COLOR property is deprecated. Use BACKGROUND property instead\n" );
        value = controlImpl.GetBackgroundColor();
        break;
      }

      case Toolkit::Control::Property::BACKGROUND_IMAGE:
      {
        DALI_LOG_WARNING( "BACKGROUND_IMAGE property is deprecated. Use BACKGROUND property instead\n" );
        Property::Map map;
        Toolkit::Visual::Base visual = controlImpl.mImpl->GetVisual( Toolkit::Control::Property::BACKGROUND );
        if( visual )
        {
          visual.CreatePropertyMap( map );
        }
        value = map;
        break;
      }

      case Toolkit::Control::Property::KEY_INPUT_FOCUS:
      {
        value = controlImpl.HasKeyInputFocus();
        break;
      }

      case Toolkit::Control::Property::BACKGROUND:
      {
        Property::Map map;
        Toolkit::Visual::Base visual = controlImpl.mImpl->GetVisual( Toolkit::Control::Property::BACKGROUND );
        if( visual )
        {
          visual.CreatePropertyMap( map );
        }

        value = map;
        break;
      }

      case Toolkit::DevelControl::Property::TOOLTIP:
      {
        Property::Map map;
        if( controlImpl.mImpl->mTooltip )
        {
          controlImpl.mImpl->mTooltip->CreatePropertyMap( map );
        }
        value = map;
        break;
      }

    }
  }

  return value;
}


void  Control::Impl::CopyInstancedProperties( RegisteredVisualContainer& visuals, Dictionary<Property::Map>& instancedProperties )
{
  for(RegisteredVisualContainer::Iterator iter = visuals.Begin(); iter!= visuals.End(); iter++)
  {
    if( (*iter)->visual )
    {
      Property::Map instanceMap;
      Toolkit::GetImplementation((*iter)->visual).CreateInstancePropertyMap(instanceMap);
      instancedProperties.Add( (*iter)->visual.GetName(), instanceMap );
    }
  }
}


void Control::Impl::RemoveVisual( RegisteredVisualContainer& visuals, const std::string& visualName )
{
  Actor self( mControlImpl.Self() );

  for ( RegisteredVisualContainer::Iterator visualIter = visuals.Begin();
        visualIter != visuals.End(); ++visualIter )
  {
    Toolkit::Visual::Base visual = (*visualIter)->visual;
    if( visual && visual.GetName() == visualName )
    {
      Toolkit::GetImplementation(visual).SetOffStage( self );
      (*visualIter)->visual.Reset();
      visuals.Erase( visualIter );
      break;
    }
  }
}

void Control::Impl::RemoveVisuals( RegisteredVisualContainer& visuals, DictionaryKeys& removeVisuals )
{
  Actor self( mControlImpl.Self() );
  for( DictionaryKeys::iterator iter = removeVisuals.begin(); iter != removeVisuals.end(); ++iter )
  {
    const std::string visualName = *iter;
    RemoveVisual( visuals, visualName );
  }
}

void Control::Impl::RecreateChangedVisuals( Dictionary<Property::Map>& stateVisualsToChange,
                             Dictionary<Property::Map>& instancedProperties )
{
  Dali::CustomActor handle( mControlImpl.GetOwner() );
  for( Dictionary<Property::Map>::iterator iter = stateVisualsToChange.Begin();
       iter != stateVisualsToChange.End(); ++iter )
  {
    const std::string& visualName = (*iter).key;
    const Property::Map& toMap = (*iter).entry;

    // is it a candidate for re-creation?
    bool recreate = false;

    Toolkit::Visual::Base visual = GetVisualByName( mVisuals, visualName );
    if( visual )
    {
      Property::Map fromMap;
      visual.CreatePropertyMap( fromMap );

      Toolkit::DevelVisual::Type fromType = GetVisualTypeFromMap( fromMap );
      Toolkit::DevelVisual::Type toType = GetVisualTypeFromMap( toMap );

      if( fromType != toType )
      {
        recreate = true;
      }
      else
      {
        if( fromType == Toolkit::DevelVisual::IMAGE || fromType == Toolkit::DevelVisual::N_PATCH
            || fromType == Toolkit::DevelVisual::SVG || fromType == Toolkit::DevelVisual::ANIMATED_IMAGE )
        {
          Property::Value* fromUrl = fromMap.Find( Toolkit::ImageVisual::Property::URL, IMAGE_URL_NAME );
          Property::Value* toUrl = toMap.Find( Toolkit::ImageVisual::Property::URL, IMAGE_URL_NAME );

          if( fromUrl && toUrl )
          {
            std::string fromUrlString;
            std::string toUrlString;
            fromUrl->Get(fromUrlString);
            toUrl->Get(toUrlString);

            if( fromUrlString != toUrlString )
            {
              recreate = true;
            }
          }
        }
      }

      const Property::Map* instancedMap = instancedProperties.FindConst( visualName );
      if( recreate || instancedMap )
      {
        RemoveVisual( mVisuals, visualName );
        Style::ApplyVisual( handle, visualName, toMap, instancedMap );
      }
      else
      {
        // @todo check to see if we can apply toMap without recreating the visual
        // e.g. by setting only animatable properties
        // For now, recreate all visuals, but merge in instance data.
        RemoveVisual( mVisuals, visualName );
        Style::ApplyVisual( handle, visualName, toMap, instancedMap );
      }
    }
  }
}

void Control::Impl::ReplaceStateVisualsAndProperties( const StylePtr oldState, const StylePtr newState, const std::string& subState )
{
  // Collect all old visual names
  DictionaryKeys stateVisualsToRemove;
  if( oldState )
  {
    oldState->visuals.GetKeys( stateVisualsToRemove );
    if( ! subState.empty() )
    {
      const StylePtr* oldSubState = oldState->subStates.FindConst(subState);
      if( oldSubState )
      {
        DictionaryKeys subStateVisualsToRemove;
        (*oldSubState)->visuals.GetKeys( subStateVisualsToRemove );
        Merge( stateVisualsToRemove, subStateVisualsToRemove );
      }
    }
  }

  // Collect all new visual properties
  Dictionary<Property::Map> stateVisualsToAdd;
  if( newState )
  {
    stateVisualsToAdd = newState->visuals;
    if( ! subState.empty() )
    {
      const StylePtr* newSubState = newState->subStates.FindConst(subState);
      if( newSubState )
      {
        stateVisualsToAdd.Merge( (*newSubState)->visuals );
      }
    }
  }

  // If a name is in both add/remove, move it to change list.
  Dictionary<Property::Map> stateVisualsToChange;
  FindChangableVisuals( stateVisualsToAdd, stateVisualsToChange, stateVisualsToRemove);

  // Copy instanced properties (e.g. text label) of current visuals
  Dictionary<Property::Map> instancedProperties;
  CopyInstancedProperties( mVisuals, instancedProperties );

  // For each visual in remove list, remove from mVisuals
  RemoveVisuals( mVisuals, stateVisualsToRemove );

  // For each visual in add list, create and add to mVisuals
  Dali::CustomActor handle( mControlImpl.GetOwner() );
  Style::ApplyVisuals( handle, stateVisualsToAdd, instancedProperties );

  // For each visual in change list, if it requires a new visual,
  // remove old visual, create and add to mVisuals
  RecreateChangedVisuals( stateVisualsToChange, instancedProperties );
}

void Control::Impl::SetState( DevelControl::State newState, bool withTransitions )
{
  DevelControl::State oldState = mState;
  Dali::CustomActor handle( mControlImpl.GetOwner() );
  DALI_LOG_INFO(gLogFilter, Debug::Concise, "Control::Impl::SetState: %s\n",
                (mState == DevelControl::NORMAL ? "NORMAL" :(
                  mState == DevelControl::FOCUSED ?"FOCUSED" : (
                    mState == DevelControl::DISABLED?"DISABLED":"NONE" ))));

  if( mState != newState )
  {
    // If mState was Disabled, and new state is Focused, should probably
    // store that fact, e.g. in another property that FocusManager can access.
    mState = newState;

    // Trigger state change and transitions
    // Apply new style, if stylemanager is available
    Toolkit::StyleManager styleManager = Toolkit::StyleManager::Get();
    if( styleManager )
    {
      const StylePtr stylePtr = GetImpl( styleManager ).GetRecordedStyle( Toolkit::Control( mControlImpl.GetOwner() ) );

      if( stylePtr )
      {
        std::string oldStateName = Scripting::GetEnumerationName< Toolkit::DevelControl::State >( oldState, ControlStateTable, ControlStateTableCount );
        std::string newStateName = Scripting::GetEnumerationName< Toolkit::DevelControl::State >( newState, ControlStateTable, ControlStateTableCount );

        const StylePtr* newStateStyle = stylePtr->subStates.Find( newStateName );
        const StylePtr* oldStateStyle = stylePtr->subStates.Find( oldStateName );
        if( oldStateStyle && newStateStyle )
        {
          // Only change if both state styles exist
          ReplaceStateVisualsAndProperties( *oldStateStyle, *newStateStyle, mSubStateName );
        }
      }
    }
  }
}

void Control::Impl::SetSubState( const std::string& subStateName, bool withTransitions )
{
  if( mSubStateName != subStateName )
  {
    // Get existing sub-state visuals, and unregister them
    Dali::CustomActor handle( mControlImpl.GetOwner() );

    Toolkit::StyleManager styleManager = Toolkit::StyleManager::Get();
    if( styleManager )
    {
      const StylePtr stylePtr = GetImpl( styleManager ).GetRecordedStyle( Toolkit::Control( mControlImpl.GetOwner() ) );
      if( stylePtr )
      {
        // Stringify state
        std::string stateName = Scripting::GetEnumerationName< Toolkit::DevelControl::State >( mState, ControlStateTable, ControlStateTableCount );

        const StylePtr* state = stylePtr->subStates.Find( stateName );
        if( state )
        {
          StylePtr stateStyle(*state);

          const StylePtr* newStateStyle = stateStyle->subStates.Find( subStateName );
          const StylePtr* oldStateStyle = stateStyle->subStates.Find( mSubStateName );
          if( oldStateStyle && newStateStyle )
          {
            std::string empty;
            ReplaceStateVisualsAndProperties( *oldStateStyle, *newStateStyle, empty );
          }
        }
      }
    }

    mSubStateName = subStateName;
  }
}

} // namespace Internal

} // namespace Toolkit

} // namespace Dali
