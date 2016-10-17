/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

using System;

namespace Dali {

public delegate IntPtr SwigDelegateKeyboardPreFocusChangeSignal(IntPtr current, IntPtr proposed, View.KeyboardFocus.Direction direction);

public class KeyboardPreFocusChangeSignal : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal KeyboardPreFocusChangeSignal(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(KeyboardPreFocusChangeSignal obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~KeyboardPreFocusChangeSignal() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NDalicManualPINVOKE.delete_KeyboardPreFocusChangeSignal(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public bool Empty() {
    bool ret = NDalicManualPINVOKE.KeyboardPreFocusChangeSignal_Empty(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint GetConnectionCount() {
    uint ret = NDalicManualPINVOKE.KeyboardPreFocusChangeSignal_GetConnectionCount(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Connect(KeyboardFocusManager.PreFocusChangeEventCallbackDelegate func) {
     NDalicManualPINVOKE.KeyboardPreFocusChangeSignal_Connect(swigCPtr, func);
     if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
 }

  public void Disconnect(System.Delegate func) {
    System.IntPtr ip = System.Runtime.InteropServices.Marshal.GetFunctionPointerForDelegate(func);
    {
      NDalicManualPINVOKE.KeyboardPreFocusChangeSignal_Disconnect(swigCPtr, new System.Runtime.InteropServices.HandleRef(this, ip));
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    }
  }

  public Actor Emit(Actor arg1, Actor arg2, View.KeyboardFocus.Direction arg3) {
    Actor ret = new Actor(NDalicManualPINVOKE.KeyboardPreFocusChangeSignal_Emit(swigCPtr, Actor.getCPtr(arg1), Actor.getCPtr(arg2), (int)arg3), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public KeyboardPreFocusChangeSignal() : this(NDalicManualPINVOKE.new_KeyboardPreFocusChangeSignal(), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
