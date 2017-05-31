//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.10
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Dali {

internal class ItemFactory : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ItemFactory(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ItemFactory obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~ItemFactory() {
    DisposeQueue.Instance.Add(this);
  }

  public virtual void Dispose() {
    if (!Window.IsInstalled()) {
      DisposeQueue.Instance.Add(this);
      return;
    }

    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NDalicPINVOKE.delete_ItemFactory(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }


  public virtual uint GetNumberOfItems() {
    uint ret = NDalicPINVOKE.ItemFactory_GetNumberOfItems(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual View NewItem(uint itemId) {
    View ret = new View(NDalicPINVOKE.ItemFactory_NewItem(swigCPtr, itemId), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual void ItemReleased(uint itemId, View view) {
    if (SwigDerivedClassHasMethod("ItemReleased", swigMethodTypes2)) NDalicPINVOKE.ItemFactory_ItemReleasedSwigExplicitItemFactory(swigCPtr, itemId, View.getCPtr(view)); else NDalicPINVOKE.ItemFactory_ItemReleased(swigCPtr, itemId, View.getCPtr(view));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public ItemFactory() : this(NDalicPINVOKE.new_ItemFactory(), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    SwigDirectorConnect();
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("GetNumberOfItems", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateItemFactory_0(SwigDirectorGetNumberOfItems);
    if (SwigDerivedClassHasMethod("NewItem", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateItemFactory_1(SwigDirectorNewItem);
    if (SwigDerivedClassHasMethod("ItemReleased", swigMethodTypes2))
      swigDelegate2 = new SwigDelegateItemFactory_2(SwigDirectorItemReleased);
    NDalicPINVOKE.ItemFactory_director_connect(swigCPtr, swigDelegate0, swigDelegate1, swigDelegate2);
  }

  private bool SwigDerivedClassHasMethod(string methodName, global::System.Type[] methodTypes) {
    global::System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, global::System.Reflection.BindingFlags.Public | global::System.Reflection.BindingFlags.NonPublic | global::System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(ItemFactory));
    return hasDerivedMethod;
  }

  private uint SwigDirectorGetNumberOfItems() {
    return GetNumberOfItems();
  }

  private global::System.IntPtr SwigDirectorNewItem(uint itemId) {
    return View.getCPtr(NewItem(itemId)).Handle;
  }

  private void SwigDirectorItemReleased(uint itemId, global::System.IntPtr actor) {
    ItemReleased(itemId, new View(actor, true));
  }

  public delegate uint SwigDelegateItemFactory_0();
  public delegate global::System.IntPtr SwigDelegateItemFactory_1(uint itemId);
  public delegate void SwigDelegateItemFactory_2(uint itemId, global::System.IntPtr actor);

  private SwigDelegateItemFactory_0 swigDelegate0;
  private SwigDelegateItemFactory_1 swigDelegate1;
  private SwigDelegateItemFactory_2 swigDelegate2;

  private static global::System.Type[] swigMethodTypes0 = new global::System.Type[] {  };
  private static global::System.Type[] swigMethodTypes1 = new global::System.Type[] { typeof(uint) };
  private static global::System.Type[] swigMethodTypes2 = new global::System.Type[] { typeof(uint), typeof(View) };
}

}
