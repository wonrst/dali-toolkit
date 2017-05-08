/** Copyright (c) 2017 Samsung Electronics Co., Ltd.
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
// This File has been auto-generated by SWIG and then modified using DALi Ruby Scripts
//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.10
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Dali
{

    using System;
    using System.Runtime.InteropServices;

    public class Animation : BaseHandle
    {
        private global::System.Runtime.InteropServices.HandleRef swigCPtr;

        internal Animation(global::System.IntPtr cPtr, bool cMemoryOwn) : base(NDalicPINVOKE.Animation_SWIGUpcast(cPtr), cMemoryOwn)
        {
            swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
        }

        internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Animation obj)
        {
            return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
        }

        ~Animation()
        {
            DisposeQueue.Instance.Add(this);
        }

        public override void Dispose()
        {
            if (!Window.IsInstalled())
            {
                DisposeQueue.Instance.Add(this);
                return;
            }

            lock (this)
            {
                if (swigCPtr.Handle != global::System.IntPtr.Zero)
                {
                    if (swigCMemOwn)
                    {
                        swigCMemOwn = false;
                        NDalicPINVOKE.delete_Animation(swigCPtr);
                    }
                    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
                }
                global::System.GC.SuppressFinalize(this);
                base.Dispose();
            }
        }




        private EventCallbackDelegateType1<IntPtr> _animationFinishedEventCallbackDelegate;

        private event EventHandler _animationFinishedEventHandler;

        /**
      * @brief Event for Finished signal which can be used to subscribe/unsubscribe the event handler
      * Finished signal is emitted when an Animation's animations have finished.
      */
        public event EventHandler Finished
        {
            add
            {
                lock (this)
                {
                    _animationFinishedEventHandler += value;
                    _animationFinishedEventCallbackDelegate = OnFinished;
                    this.FinishedSignal().Connect(_animationFinishedEventCallbackDelegate);
                }
            }
            remove
            {
                lock (this)
                {
                    if (_animationFinishedEventHandler != null)
                    {
                        this.FinishedSignal().Disconnect(_animationFinishedEventCallbackDelegate);
                    }
                    _animationFinishedEventHandler -= value;
                }
            }
        }

        // Callback for Animation FinishedSignal
        private void OnFinished(IntPtr data)
        {
            if (_animationFinishedEventHandler != null)
            {
                //here we send all data to user event handlers
                _animationFinishedEventHandler(this, null);
            }
        }

        public static Animation GetAnimationFromPtr(global::System.IntPtr cPtr)
        {
            Animation ret = new Animation(cPtr, false);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        private float MilliSecondsToSeconds(int millisec)
        {
            return (float)millisec / 1000.0f;
        }

        private int SecondsToMilliSeconds(float sec)
        {
            return (int)(sec * 1000);
        }

        public int Duration
        {
            set
            {
                SetDuration(MilliSecondsToSeconds(value));
            }
            get
            {
                return SecondsToMilliSeconds(GetDuration());
            }
        }

        public AlphaFunction DefaultAlphaFunction
        {
            set
            {
                SetDefaultAlphaFunction(value);
            }
            get
            {
                AlphaFunction ret = GetDefaultAlphaFunction();
                return ret;
            }
        }

        public Animation.State Status
        {
            get
            {
                return GetState();
            }
        }

        public int LoopCount
        {
            set
            {
                SetLoopCount(value);
            }
            get
            {
                int ret = GetLoopCount();
                return ret;
            }
        }

        public bool Looping
        {
            set
            {
                SetLooping(value);
            }
            get
            {
                bool ret = IsLooping();
                return ret;
            }
        }

        public Animation.EndActions EndAction
        {
            set
            {
                SetEndAction(value);
            }
            get
            {
                return GetEndAction();
            }
        }

        public void Stop(Animation.EndActions action)
        {
            SetEndAction(action);
            NDalicPINVOKE.Animation_Stop(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public int StartTime { set; get; }

        public int EndTime { set; get; }

        public string TargetProperty { set; get; }

        public object Destination { set; get; }

        public Dali.AlphaFunction AlphaFunction { set; get; }


        public void AnimateBy(View target)
        {
            string _str1 = TargetProperty.Substring(0, 1);
            string _str2 = TargetProperty.Substring(1);
            string _str = _str1.ToLower() + _str2;

            dynamic obj = (object)Destination;

            if (this.AlphaFunction != null)
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBy(new Property(target, _str), new Property.Value(obj), this.AlphaFunction);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBy(new Property(target, _str), new Property.Value(obj), this.AlphaFunction, time);
                }
            }
            else
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBy(new Property(target, _str), new Property.Value(obj));
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBy(new Property(target, _str), new Property.Value(obj), time);
                }
            }
        }

        public void AnimateBy(View target, string propertyIndex)
        {
            string _str1 = propertyIndex.Substring(0, 1);
            string _str2 = propertyIndex.Substring(1);
            string _str = _str1.ToLower() + _str2;

            dynamic obj = (object)Destination;

            if (this.AlphaFunction != null)
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBy(new Property(target, _str), new Property.Value(obj), this.AlphaFunction);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBy(new Property(target, _str), new Property.Value(obj), this.AlphaFunction, time);
                }
            }
            else
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBy(new Property(target, _str), new Property.Value(obj));
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBy(new Property(target, _str), new Property.Value(obj), time);
                }
            }
        }

        public void AnimateBy(View target, string propertyIndex, object relativeValue)
        {
            string _str1 = propertyIndex.Substring(0, 1);
            string _str2 = propertyIndex.Substring(1);
            string _str = _str1.ToLower() + _str2;

            dynamic obj = (object)relativeValue;

            if (this.AlphaFunction != null)
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBy(new Property(target, _str), new Property.Value(obj), this.AlphaFunction);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBy(new Property(target, _str), new Property.Value(obj), this.AlphaFunction, time);
                }
            }
            else
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBy(new Property(target, _str), new Property.Value(obj));
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBy(new Property(target, _str), new Property.Value(obj), time);
                }
            }
        }

        public void AnimateTo(View target)
        {
            string _str1 = TargetProperty.Substring(0, 1);
            string _str2 = TargetProperty.Substring(1);
            string _str = _str1.ToLower() + _str2;

            dynamic obj = (object)Destination;

            if (this.AlphaFunction != null)
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateTo(new Property(target, _str), new Property.Value(obj), this.AlphaFunction);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateTo(new Property(target, _str), new Property.Value(obj), this.AlphaFunction, time);
                }
            }
            else
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateTo(new Property(target, _str), new Property.Value(obj));
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateTo(new Property(target, _str), new Property.Value(obj), time);
                }
            }
        }

        public void AnimateTo(View target, string propertyIndex)
        {
            string _str1 = propertyIndex.Substring(0, 1);
            string _str2 = propertyIndex.Substring(1);
            string _str = _str1.ToLower() + _str2;

            dynamic obj = (object)Destination;

            if (this.AlphaFunction != null)
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateTo(new Property(target, _str), new Property.Value(obj), this.AlphaFunction);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateTo(new Property(target, _str), new Property.Value(obj), this.AlphaFunction, time);
                }
            }
            else
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateTo(new Property(target, _str), new Property.Value(obj));
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateTo(new Property(target, _str), new Property.Value(obj), time);
                }
            }
        }

        public void AnimateTo(View target, string propertyIndex, object destinationValue)
        {
            string _str1 = propertyIndex.Substring(0, 1);
            string _str2 = propertyIndex.Substring(1);
            string _str = _str1.ToLower() + _str2;

            dynamic obj = (object)destinationValue;

            if (this.AlphaFunction != null)
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateTo(new Property(target, _str), new Property.Value(obj), this.AlphaFunction);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateTo(new Property(target, _str), new Property.Value(obj), this.AlphaFunction, time);
                }
            }
            else
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateTo(new Property(target, _str), new Property.Value(obj));
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateTo(new Property(target, _str), new Property.Value(obj), time);
                }
            }
        }

        public void AnimateBetween(View target, KeyFrames keyFrames)
        {
            string _str1 = TargetProperty.Substring(0, 1);
            string _str2 = TargetProperty.Substring(1);
            string _str = _str1.ToLower() + _str2;


            if (this.AlphaFunction != null)
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBetween(new Property(target, _str), keyFrames, this.AlphaFunction);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBetween(new Property(target, _str), keyFrames, this.AlphaFunction, time);
                }
            }
            else
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBetween(new Property(target, _str), keyFrames);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBetween(new Property(target, _str), keyFrames, time);
                }
            }
        }

        public void AnimateBetween(View target, KeyFrames keyFrames, Animation.Interpolation interpolation)
        {
            string _str1 = TargetProperty.Substring(0, 1);
            string _str2 = TargetProperty.Substring(1);
            string _str = _str1.ToLower() + _str2;


            if (this.AlphaFunction != null)
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBetween(new Property(target, _str), keyFrames, this.AlphaFunction, interpolation);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBetween(new Property(target, _str), keyFrames, this.AlphaFunction, time, interpolation);
                }
            }
            else
            {
                if (this.StartTime == 0 && this.EndTime == 0)
                {
                    AnimateBetween(new Property(target, _str), keyFrames, interpolation);
                }
                else
                {
                    Dali.TimePeriod time = new Dali.TimePeriod(MilliSecondsToSeconds(this.StartTime), MilliSecondsToSeconds(this.EndTime - this.StartTime));
                    AnimateBetween(new Property(target, _str), keyFrames, time, interpolation);
                }
            }
        }


        /**
   * @brief Create an initialized Animation.
   *
   * The animation will not loop.
   * The default end action is "Bake".
   * The default Alpha function is linear.
   * @since 1.0.0
   * @param [in] durationmSeconds The duration in milli seconds (int).
   * @return A handle to a newly allocated Dali resource.
   * @pre DurationmSeconds must be greater than zero.
   */
        public Animation(int durationmSeconds) : this(NDalicPINVOKE.Animation_New((float)durationmSeconds / 1000.0f), true)
        {
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public Animation() : this(NDalicPINVOKE.Animation_New(0.0f), true)
        {
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }


        public Animation(float durationSeconds) : this(NDalicPINVOKE.Animation_New(durationSeconds), true)
        {
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();

        }

        public static Animation DownCast(BaseHandle handle)
        {
            Animation ret = new Animation(NDalicPINVOKE.Animation_DownCast(BaseHandle.getCPtr(handle)), true);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public Animation(Animation handle) : this(NDalicPINVOKE.new_Animation__SWIG_1(Animation.getCPtr(handle)), true)
        {
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public Animation Assign(Animation rhs)
        {
            Animation ret = new Animation(NDalicPINVOKE.Animation_Assign(swigCPtr, Animation.getCPtr(rhs)), false);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void SetDuration(float seconds)
        {
            NDalicPINVOKE.Animation_SetDuration(swigCPtr, seconds);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public float GetDuration()
        {
            float ret = NDalicPINVOKE.Animation_GetDuration(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void SetLooping(bool looping)
        {
            NDalicPINVOKE.Animation_SetLooping(swigCPtr, looping);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void SetLoopCount(int count)
        {
            NDalicPINVOKE.Animation_SetLoopCount(swigCPtr, count);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public int GetLoopCount()
        {
            int ret = NDalicPINVOKE.Animation_GetLoopCount(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public int GetCurrentLoop()
        {
            int ret = NDalicPINVOKE.Animation_GetCurrentLoop(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public bool IsLooping()
        {
            bool ret = NDalicPINVOKE.Animation_IsLooping(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void SetEndAction(Animation.EndActions action)
        {
            NDalicPINVOKE.Animation_SetEndAction(swigCPtr, (int)action);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public Animation.EndActions GetEndAction()
        {
            Animation.EndActions ret = (Animation.EndActions)NDalicPINVOKE.Animation_GetEndAction(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void SetDisconnectAction(Animation.EndActions disconnectAction)
        {
            NDalicPINVOKE.Animation_SetDisconnectAction(swigCPtr, (int)disconnectAction);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public Animation.EndActions GetDisconnectAction()
        {
            Animation.EndActions ret = (Animation.EndActions)NDalicPINVOKE.Animation_GetDisconnectAction(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void SetDefaultAlphaFunction(AlphaFunction alpha)
        {
            NDalicPINVOKE.Animation_SetDefaultAlphaFunction(swigCPtr, AlphaFunction.getCPtr(alpha));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public AlphaFunction GetDefaultAlphaFunction()
        {
            AlphaFunction ret = new AlphaFunction(NDalicPINVOKE.Animation_GetDefaultAlphaFunction(swigCPtr), true);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void SetCurrentProgress(float progress)
        {
            NDalicPINVOKE.Animation_SetCurrentProgress(swigCPtr, progress);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public float GetCurrentProgress()
        {
            float ret = NDalicPINVOKE.Animation_GetCurrentProgress(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void SetSpeedFactor(float factor)
        {
            NDalicPINVOKE.Animation_SetSpeedFactor(swigCPtr, factor);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public float GetSpeedFactor()
        {
            float ret = NDalicPINVOKE.Animation_GetSpeedFactor(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void SetPlayRange(Vector2 range)
        {
            NDalicPINVOKE.Animation_SetPlayRange(swigCPtr, Vector2.getCPtr(range));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public Vector2 GetPlayRange()
        {
            Vector2 ret = new Vector2(NDalicPINVOKE.Animation_GetPlayRange(swigCPtr), true);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void Play()
        {
            NDalicPINVOKE.Animation_Play(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void PlayFrom(float progress)
        {
            NDalicPINVOKE.Animation_PlayFrom(swigCPtr, progress);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void Pause()
        {
            NDalicPINVOKE.Animation_Pause(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public Animation.State GetState()
        {
            Animation.State ret = (Animation.State)NDalicPINVOKE.Animation_GetState(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void Stop()
        {
            NDalicPINVOKE.Animation_Stop(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void Clear()
        {
            NDalicPINVOKE.Animation_Clear(swigCPtr);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public AnimationSignal FinishedSignal()
        {
            AnimationSignal ret = new AnimationSignal(NDalicPINVOKE.Animation_FinishedSignal(swigCPtr), false);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
            return ret;
        }

        public void AnimateBy(Property target, Property.Value relativeValue)
        {
            NDalicPINVOKE.Animation_AnimateBy__SWIG_0(swigCPtr, Property.getCPtr(target), Property.Value.getCPtr(relativeValue));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBy(Property target, Property.Value relativeValue, AlphaFunction alpha)
        {
            NDalicPINVOKE.Animation_AnimateBy__SWIG_1(swigCPtr, Property.getCPtr(target), Property.Value.getCPtr(relativeValue), AlphaFunction.getCPtr(alpha));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBy(Property target, Property.Value relativeValue, TimePeriod period)
        {
            NDalicPINVOKE.Animation_AnimateBy__SWIG_2(swigCPtr, Property.getCPtr(target), Property.Value.getCPtr(relativeValue), TimePeriod.getCPtr(period));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBy(Property target, Property.Value relativeValue, AlphaFunction alpha, TimePeriod period)
        {
            NDalicPINVOKE.Animation_AnimateBy__SWIG_3(swigCPtr, Property.getCPtr(target), Property.Value.getCPtr(relativeValue), AlphaFunction.getCPtr(alpha), TimePeriod.getCPtr(period));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateTo(Property target, Property.Value destinationValue)
        {
            NDalicPINVOKE.Animation_AnimateTo__SWIG_0(swigCPtr, Property.getCPtr(target), Property.Value.getCPtr(destinationValue));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateTo(Property target, Property.Value destinationValue, AlphaFunction alpha)
        {
            NDalicPINVOKE.Animation_AnimateTo__SWIG_1(swigCPtr, Property.getCPtr(target), Property.Value.getCPtr(destinationValue), AlphaFunction.getCPtr(alpha));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateTo(Property target, Property.Value destinationValue, TimePeriod period)
        {
            NDalicPINVOKE.Animation_AnimateTo__SWIG_2(swigCPtr, Property.getCPtr(target), Property.Value.getCPtr(destinationValue), TimePeriod.getCPtr(period));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateTo(Property target, Property.Value destinationValue, AlphaFunction alpha, TimePeriod period)
        {
            NDalicPINVOKE.Animation_AnimateTo__SWIG_3(swigCPtr, Property.getCPtr(target), Property.Value.getCPtr(destinationValue), AlphaFunction.getCPtr(alpha), TimePeriod.getCPtr(period));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBetween(Property target, KeyFrames keyFrames)
        {
            NDalicPINVOKE.Animation_AnimateBetween__SWIG_0(swigCPtr, Property.getCPtr(target), KeyFrames.getCPtr(keyFrames));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBetween(Property target, KeyFrames keyFrames, Animation.Interpolation interpolation)
        {
            NDalicPINVOKE.Animation_AnimateBetween__SWIG_1(swigCPtr, Property.getCPtr(target), KeyFrames.getCPtr(keyFrames), (int)interpolation);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBetween(Property target, KeyFrames keyFrames, AlphaFunction alpha)
        {
            NDalicPINVOKE.Animation_AnimateBetween__SWIG_2(swigCPtr, Property.getCPtr(target), KeyFrames.getCPtr(keyFrames), AlphaFunction.getCPtr(alpha));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBetween(Property target, KeyFrames keyFrames, AlphaFunction alpha, Animation.Interpolation interpolation)
        {
            NDalicPINVOKE.Animation_AnimateBetween__SWIG_3(swigCPtr, Property.getCPtr(target), KeyFrames.getCPtr(keyFrames), AlphaFunction.getCPtr(alpha), (int)interpolation);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBetween(Property target, KeyFrames keyFrames, TimePeriod period)
        {
            NDalicPINVOKE.Animation_AnimateBetween__SWIG_4(swigCPtr, Property.getCPtr(target), KeyFrames.getCPtr(keyFrames), TimePeriod.getCPtr(period));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBetween(Property target, KeyFrames keyFrames, TimePeriod period, Animation.Interpolation interpolation)
        {
            NDalicPINVOKE.Animation_AnimateBetween__SWIG_5(swigCPtr, Property.getCPtr(target), KeyFrames.getCPtr(keyFrames), TimePeriod.getCPtr(period), (int)interpolation);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBetween(Property target, KeyFrames keyFrames, AlphaFunction alpha, TimePeriod period)
        {
            NDalicPINVOKE.Animation_AnimateBetween__SWIG_6(swigCPtr, Property.getCPtr(target), KeyFrames.getCPtr(keyFrames), AlphaFunction.getCPtr(alpha), TimePeriod.getCPtr(period));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void AnimateBetween(Property target, KeyFrames keyFrames, AlphaFunction alpha, TimePeriod period, Animation.Interpolation interpolation)
        {
            NDalicPINVOKE.Animation_AnimateBetween__SWIG_7(swigCPtr, Property.getCPtr(target), KeyFrames.getCPtr(keyFrames), AlphaFunction.getCPtr(alpha), TimePeriod.getCPtr(period), (int)interpolation);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void Animate(View view, Path path, Vector3 forward)
        {
            NDalicPINVOKE.Animation_Animate__SWIG_0(swigCPtr, View.getCPtr(view), Path.getCPtr(path), Vector3.getCPtr(forward));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void Animate(View view, Path path, Vector3 forward, AlphaFunction alpha)
        {
            NDalicPINVOKE.Animation_Animate__SWIG_1(swigCPtr, View.getCPtr(view), Path.getCPtr(path), Vector3.getCPtr(forward), AlphaFunction.getCPtr(alpha));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void Animate(View view, Path path, Vector3 forward, TimePeriod period)
        {
            NDalicPINVOKE.Animation_Animate__SWIG_2(swigCPtr, View.getCPtr(view), Path.getCPtr(path), Vector3.getCPtr(forward), TimePeriod.getCPtr(period));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void Animate(View view, Path path, Vector3 forward, AlphaFunction alpha, TimePeriod period)
        {
            NDalicPINVOKE.Animation_Animate__SWIG_3(swigCPtr, View.getCPtr(view), Path.getCPtr(path), Vector3.getCPtr(forward), AlphaFunction.getCPtr(alpha), TimePeriod.getCPtr(period));
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void Show(View view, float delaySeconds)
        {
            NDalicPINVOKE.Animation_Show(swigCPtr, View.getCPtr(view), delaySeconds);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public void Hide(View view, float delaySeconds)
        {
            NDalicPINVOKE.Animation_Hide(swigCPtr, View.getCPtr(view), delaySeconds);
            if (NDalicPINVOKE.SWIGPendingException.Pending)
                throw NDalicPINVOKE.SWIGPendingException.Retrieve();
        }

        public enum EndActions
        {
            Cancel,
            Discard,
            StopFinal
        }

        public enum Interpolation
        {
            Linear,
            Cubic
        }

        public enum State
        {
            Stopped,
            Playing,
            Paused
        }

    }

}