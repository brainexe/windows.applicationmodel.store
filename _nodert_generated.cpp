// Copyright (c) The NodeRT Contributors
// All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the ""License""); you may
// not use this file except in compliance with the License. You may obtain a
// copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS
// OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY
// IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABLITY OR NON-INFRINGEMENT.
//
// See the Apache Version 2.0 License for specific language governing permissions
// and limitations under the License.

// TODO: Verify that this is is still needed..
#define NTDDI_VERSION 0x06010000

#include <v8.h>
#include "nan.h"
#include <string>
#include <ppltasks.h>
#include "CollectionsConverter.h"
#include "CollectionsWrap.h"
#include "node-async.h"
#include "NodeRtUtils.h"
#include "OpaqueWrapper.h"
#include "WrapperBase.h"

#using <Windows.WinMD>

// this undefs fixes the issues of compiling Windows.Data.Json, Windows.Storag.FileProperties, and Windows.Stroage.Search
// Some of the node header files brings windows definitions with the same names as some of the WinRT methods
#undef DocumentProperties
#undef GetObject
#undef CreateEvent
#undef FindText
#undef SendMessage

const char* REGISTRATION_TOKEN_MAP_PROPERTY_NAME = "__registrationTokenMap__";

using v8::Array;
using v8::String;
using v8::Value;
using v8::Boolean;
using v8::Integer;
using v8::FunctionTemplate;
using v8::Object;
using v8::Local;
using v8::Function;
using v8::Date;
using v8::Number;
using v8::PropertyAttribute;
using v8::Primitive;
using Nan::HandleScope;
using Nan::Persistent;
using Nan::Undefined;
using Nan::True;
using Nan::False;
using Nan::Null;
using Nan::MaybeLocal;
using Nan::EscapableHandleScope;
using Nan::HandleScope;
using Nan::TryCatch;
using namespace concurrency;

namespace NodeRT { namespace Windows { namespace ApplicationModel { namespace Store { 
  v8::Local<v8::Value> WrapLicenseInformation(::Windows::ApplicationModel::Store::LicenseInformation^ wintRtInstance);
  ::Windows::ApplicationModel::Store::LicenseInformation^ UnwrapLicenseInformation(Local<Value> value);
  
  v8::Local<v8::Value> WrapListingInformation(::Windows::ApplicationModel::Store::ListingInformation^ wintRtInstance);
  ::Windows::ApplicationModel::Store::ListingInformation^ UnwrapListingInformation(Local<Value> value);
  
  v8::Local<v8::Value> WrapPurchaseResults(::Windows::ApplicationModel::Store::PurchaseResults^ wintRtInstance);
  ::Windows::ApplicationModel::Store::PurchaseResults^ UnwrapPurchaseResults(Local<Value> value);
  
  v8::Local<v8::Value> WrapProductPurchaseDisplayProperties(::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ wintRtInstance);
  ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ UnwrapProductPurchaseDisplayProperties(Local<Value> value);
  
  v8::Local<v8::Value> WrapUnfulfilledConsumable(::Windows::ApplicationModel::Store::UnfulfilledConsumable^ wintRtInstance);
  ::Windows::ApplicationModel::Store::UnfulfilledConsumable^ UnwrapUnfulfilledConsumable(Local<Value> value);
  
  v8::Local<v8::Value> WrapProductLicense(::Windows::ApplicationModel::Store::ProductLicense^ wintRtInstance);
  ::Windows::ApplicationModel::Store::ProductLicense^ UnwrapProductLicense(Local<Value> value);
  
  v8::Local<v8::Value> WrapProductListing(::Windows::ApplicationModel::Store::ProductListing^ wintRtInstance);
  ::Windows::ApplicationModel::Store::ProductListing^ UnwrapProductListing(Local<Value> value);
  
  v8::Local<v8::Value> WrapCurrentApp(::Windows::ApplicationModel::Store::CurrentApp^ wintRtInstance);
  ::Windows::ApplicationModel::Store::CurrentApp^ UnwrapCurrentApp(Local<Value> value);
  
  v8::Local<v8::Value> WrapCurrentAppSimulator(::Windows::ApplicationModel::Store::CurrentAppSimulator^ wintRtInstance);
  ::Windows::ApplicationModel::Store::CurrentAppSimulator^ UnwrapCurrentAppSimulator(Local<Value> value);
  



  static void InitProductPurchaseStatusEnum(const Local<Object> exports) {
    HandleScope scope;

    Local<Object> enumObject = Nan::New<Object>();

    Nan::Set(exports, Nan::New<String>("ProductPurchaseStatus").ToLocalChecked(), enumObject);
    Nan::Set(enumObject, Nan::New<String>("succeeded").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::ProductPurchaseStatus::Succeeded)));
    Nan::Set(enumObject, Nan::New<String>("alreadyPurchased").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::ProductPurchaseStatus::AlreadyPurchased)));
    Nan::Set(enumObject, Nan::New<String>("notFulfilled").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::ProductPurchaseStatus::NotFulfilled)));
    Nan::Set(enumObject, Nan::New<String>("notPurchased").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::ProductPurchaseStatus::NotPurchased)));
  }

  static void InitProductTypeEnum(const Local<Object> exports) {
    HandleScope scope;

    Local<Object> enumObject = Nan::New<Object>();

    Nan::Set(exports, Nan::New<String>("ProductType").ToLocalChecked(), enumObject);
    Nan::Set(enumObject, Nan::New<String>("unknown").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::ProductType::Unknown)));
    Nan::Set(enumObject, Nan::New<String>("durable").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::ProductType::Durable)));
    Nan::Set(enumObject, Nan::New<String>("consumable").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::ProductType::Consumable)));
  }

  static void InitFulfillmentResultEnum(const Local<Object> exports) {
    HandleScope scope;

    Local<Object> enumObject = Nan::New<Object>();

    Nan::Set(exports, Nan::New<String>("FulfillmentResult").ToLocalChecked(), enumObject);
    Nan::Set(enumObject, Nan::New<String>("succeeded").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::FulfillmentResult::Succeeded)));
    Nan::Set(enumObject, Nan::New<String>("nothingToFulfill").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::FulfillmentResult::NothingToFulfill)));
    Nan::Set(enumObject, Nan::New<String>("purchasePending").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::FulfillmentResult::PurchasePending)));
    Nan::Set(enumObject, Nan::New<String>("purchaseReverted").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::FulfillmentResult::PurchaseReverted)));
    Nan::Set(enumObject, Nan::New<String>("serverError").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::ApplicationModel::Store::FulfillmentResult::ServerError)));
  }



  class LicenseInformation : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("LicenseInformation").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);




          
          Nan::SetPrototypeMethod(localRef,"addListener", AddListener);
          Nan::SetPrototypeMethod(localRef,"on", AddListener);
          Nan::SetPrototypeMethod(localRef,"removeListener", RemoveListener);
          Nan::SetPrototypeMethod(localRef, "off", RemoveListener);

          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("expirationDate").ToLocalChecked(), ExpirationDateGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("isActive").ToLocalChecked(), IsActiveGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("isTrial").ToLocalChecked(), IsTrialGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("productLicenses").ToLocalChecked(), ProductLicensesGetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("LicenseInformation").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      LicenseInformation(::Windows::ApplicationModel::Store::LicenseInformation^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::LicenseInformation^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::LicenseInformation^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::LicenseInformation^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      LicenseInformation *wrapperInstance = new LicenseInformation(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::LicenseInformation^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::LicenseInformation^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::LicenseInformation^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapLicenseInformation(winRtInstance));
    }





    static void ExpirationDateGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::LicenseInformation^>(info.This())) {
        return;
      }

      LicenseInformation *wrapper = LicenseInformation::Unwrap<LicenseInformation>(info.This());

      try  {
        ::Windows::Foundation::DateTime result = wrapper->_instance->ExpirationDate;
        info.GetReturnValue().Set(NodeRT::Utils::DateTimeToJS(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void IsActiveGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::LicenseInformation^>(info.This())) {
        return;
      }

      LicenseInformation *wrapper = LicenseInformation::Unwrap<LicenseInformation>(info.This());

      try  {
        bool result = wrapper->_instance->IsActive;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void IsTrialGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::LicenseInformation^>(info.This())) {
        return;
      }

      LicenseInformation *wrapper = LicenseInformation::Unwrap<LicenseInformation>(info.This());

      try  {
        bool result = wrapper->_instance->IsTrial;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ProductLicensesGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::LicenseInformation^>(info.This())) {
        return;
      }

      LicenseInformation *wrapper = LicenseInformation::Unwrap<LicenseInformation>(info.This());

      try  {
        ::Windows::Foundation::Collections::IMapView<::Platform::String^, ::Windows::ApplicationModel::Store::ProductLicense^>^ result = wrapper->_instance->ProductLicenses;
        info.GetReturnValue().Set(NodeRT::Collections::MapViewWrapper<::Platform::String^,::Windows::ApplicationModel::Store::ProductLicense^>::CreateMapViewWrapper(result, 
            [](::Platform::String^ val) -> Local<Value> {
              return NodeRT::Utils::NewString(val->Data());
            },
            [](Local<Value> value) -> bool {
              return value->IsString();
            },
            [](Local<Value> value) -> ::Platform::String^ {
              return ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));
            },
            [](::Windows::ApplicationModel::Store::ProductLicense^ val) -> Local<Value> {
              return WrapProductLicense(val);
            }
          ));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      


    static void AddListener(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"wrong arguments, expected arguments are eventName(string),callback(function)")));
        return;
      }

      String::Value eventName(v8::Isolate::GetCurrent(), info[0]);
      auto str = *eventName;

      Local<Function> callback = info[1].As<Function>();

      ::Windows::Foundation::EventRegistrationToken registrationToken;
      if (NodeRT::Utils::CaseInsenstiveEquals(L"licenseChanged", str))
      {
        if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::LicenseInformation^>(info.This()))
        {
          Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"The caller of this method isn't of the expected type or internal WinRt object was disposed")));
      return;
        }
        LicenseInformation *wrapper = LicenseInformation::Unwrap<LicenseInformation>(info.This());
      
        try {
          Persistent<Object>* perstPtr = new Persistent<Object>();
          perstPtr->Reset(NodeRT::Utils::CreateCallbackObjectInDomain(callback));
          std::shared_ptr<Persistent<Object>> callbackObjPtr(perstPtr,
            [] (Persistent<Object> *ptr ) {
              NodeUtils::Async::RunOnMain([ptr]() {
                ptr->Reset();
                delete ptr;
            });
          });

          registrationToken = wrapper->_instance->LicenseChanged::add(
            ref new ::Windows::ApplicationModel::Store::LicenseChangedEventHandler(
            [callbackObjPtr]() {
              NodeUtils::Async::RunOnMain([callbackObjPtr ]() {
                HandleScope scope;



                {
                  TryCatch tryCatch;




                }

                Local<Object> callbackObjLocalRef = Nan::New<Object>(*callbackObjPtr);
                NodeRT::Utils::CallCallbackInDomain(callbackObjLocalRef,0, nullptr);
              });
            })
          );
        }
        catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }

      }
 else  {
        Nan::ThrowError(Nan::Error(String::Concat(v8::Isolate::GetCurrent(), NodeRT::Utils::NewString(L"given event name isn't supported: "), info[0].As<String>())));
        return;
      }

      Local<Value> tokenMapVal = NodeRT::Utils::GetHiddenValue(callback, Nan::New<String>(REGISTRATION_TOKEN_MAP_PROPERTY_NAME).ToLocalChecked());
      Local<Object> tokenMap;

      if (tokenMapVal.IsEmpty() || Nan::Equals(tokenMapVal, Undefined()).FromMaybe(false)) {
        tokenMap = Nan::New<Object>();
        NodeRT::Utils::SetHiddenValueWithObject(callback, Nan::New<String>(REGISTRATION_TOKEN_MAP_PROPERTY_NAME).ToLocalChecked(), tokenMap);
      } else {
        tokenMap = Nan::To<Object>(tokenMapVal).ToLocalChecked();
      }

      Nan::Set(tokenMap, info[0], CreateOpaqueWrapper(::Windows::Foundation::PropertyValue::CreateInt64(registrationToken.Value)));
    }

    static void RemoveListener(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"wrong arguments, expected a string and a callback")));
        return;
      }

      String::Value eventName(v8::Isolate::GetCurrent(), info[0]);
      auto str = *eventName;

      if ((!NodeRT::Utils::CaseInsenstiveEquals(L"licenseChanged", str))) {
        Nan::ThrowError(Nan::Error(String::Concat(v8::Isolate::GetCurrent(), NodeRT::Utils::NewString(L"given event name isn't supported: "), info[0].As<String>())));
        return;
      }

      Local<Function> callback = info[1].As<Function>();
      Local<Value> tokenMap = NodeRT::Utils::GetHiddenValue(callback, Nan::New<String>(REGISTRATION_TOKEN_MAP_PROPERTY_NAME).ToLocalChecked());

      if (tokenMap.IsEmpty() || Nan::Equals(tokenMap, Undefined()).FromMaybe(false)) {
        return;
      }

      Local<Value> opaqueWrapperObj =  Nan::Get(Nan::To<Object>(tokenMap).ToLocalChecked(), info[0]).ToLocalChecked();

      if (opaqueWrapperObj.IsEmpty() || Nan::Equals(opaqueWrapperObj,Undefined()).FromMaybe(false)) {
        return;
      }

      OpaqueWrapper *opaqueWrapper = OpaqueWrapper::Unwrap<OpaqueWrapper>(opaqueWrapperObj.As<Object>());

      long long tokenValue = (long long) opaqueWrapper->GetObjectInstance();
      ::Windows::Foundation::EventRegistrationToken registrationToken;
      registrationToken.Value = tokenValue;

      try  {
        if (NodeRT::Utils::CaseInsenstiveEquals(L"licenseChanged", str)) {
          if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::LicenseInformation^>(info.This()))
          {
            Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"The caller of this method isn't of the expected type or internal WinRt object was disposed")));
            return;
          }
          LicenseInformation *wrapper = LicenseInformation::Unwrap<LicenseInformation>(info.This());
          wrapper->_instance->LicenseChanged::remove(registrationToken);
        }
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }

      Nan::Delete(Nan::To<Object>(tokenMap).ToLocalChecked(), Nan::To<String>(info[0]).ToLocalChecked());
    }
    private:
      ::Windows::ApplicationModel::Store::LicenseInformation^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapLicenseInformation(::Windows::ApplicationModel::Store::LicenseInformation^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::LicenseInformation^ UnwrapLicenseInformation(Local<Value> value);
  };

  Persistent<FunctionTemplate> LicenseInformation::s_constructorTemplate;

  v8::Local<v8::Value> WrapLicenseInformation(::Windows::ApplicationModel::Store::LicenseInformation^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(LicenseInformation::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::LicenseInformation^ UnwrapLicenseInformation(Local<Value> value) {
     return LicenseInformation::Unwrap<LicenseInformation>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitLicenseInformation(Local<Object> exports) {
    LicenseInformation::Init(exports);
  }

  class ListingInformation : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("ListingInformation").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);





          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("ageRating").ToLocalChecked(), AgeRatingGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("currentMarket").ToLocalChecked(), CurrentMarketGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("description").ToLocalChecked(), DescriptionGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("formattedPrice").ToLocalChecked(), FormattedPriceGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("name").ToLocalChecked(), NameGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("productListings").ToLocalChecked(), ProductListingsGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("currencyCode").ToLocalChecked(), CurrencyCodeGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("formattedBasePrice").ToLocalChecked(), FormattedBasePriceGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("isOnSale").ToLocalChecked(), IsOnSaleGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("saleEndDate").ToLocalChecked(), SaleEndDateGetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("ListingInformation").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      ListingInformation(::Windows::ApplicationModel::Store::ListingInformation^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::ListingInformation^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::ListingInformation^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      ListingInformation *wrapperInstance = new ListingInformation(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::ListingInformation^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::ListingInformation^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapListingInformation(winRtInstance));
    }





    static void AgeRatingGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        unsigned int result = wrapper->_instance->AgeRating;
        info.GetReturnValue().Set(Nan::New<Integer>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CurrentMarketGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->CurrentMarket;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void DescriptionGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Description;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void FormattedPriceGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->FormattedPrice;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void NameGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Name;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ProductListingsGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        ::Windows::Foundation::Collections::IMapView<::Platform::String^, ::Windows::ApplicationModel::Store::ProductListing^>^ result = wrapper->_instance->ProductListings;
        info.GetReturnValue().Set(NodeRT::Collections::MapViewWrapper<::Platform::String^,::Windows::ApplicationModel::Store::ProductListing^>::CreateMapViewWrapper(result, 
            [](::Platform::String^ val) -> Local<Value> {
              return NodeRT::Utils::NewString(val->Data());
            },
            [](Local<Value> value) -> bool {
              return value->IsString();
            },
            [](Local<Value> value) -> ::Platform::String^ {
              return ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));
            },
            [](::Windows::ApplicationModel::Store::ProductListing^ val) -> Local<Value> {
              return WrapProductListing(val);
            }
          ));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CurrencyCodeGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->CurrencyCode;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void FormattedBasePriceGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->FormattedBasePrice;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void IsOnSaleGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        bool result = wrapper->_instance->IsOnSale;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void SaleEndDateGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ListingInformation^>(info.This())) {
        return;
      }

      ListingInformation *wrapper = ListingInformation::Unwrap<ListingInformation>(info.This());

      try  {
        ::Windows::Foundation::DateTime result = wrapper->_instance->SaleEndDate;
        info.GetReturnValue().Set(NodeRT::Utils::DateTimeToJS(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      


    private:
      ::Windows::ApplicationModel::Store::ListingInformation^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapListingInformation(::Windows::ApplicationModel::Store::ListingInformation^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::ListingInformation^ UnwrapListingInformation(Local<Value> value);
  };

  Persistent<FunctionTemplate> ListingInformation::s_constructorTemplate;

  v8::Local<v8::Value> WrapListingInformation(::Windows::ApplicationModel::Store::ListingInformation^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(ListingInformation::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::ListingInformation^ UnwrapListingInformation(Local<Value> value) {
     return ListingInformation::Unwrap<ListingInformation>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitListingInformation(Local<Object> exports) {
    ListingInformation::Init(exports);
  }

  class PurchaseResults : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("PurchaseResults").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);





          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("offerId").ToLocalChecked(), OfferIdGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("receiptXml").ToLocalChecked(), ReceiptXmlGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("status").ToLocalChecked(), StatusGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("transactionId").ToLocalChecked(), TransactionIdGetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("PurchaseResults").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      PurchaseResults(::Windows::ApplicationModel::Store::PurchaseResults^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::PurchaseResults^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::PurchaseResults^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::PurchaseResults^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      PurchaseResults *wrapperInstance = new PurchaseResults(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::PurchaseResults^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::PurchaseResults^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::PurchaseResults^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapPurchaseResults(winRtInstance));
    }





    static void OfferIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::PurchaseResults^>(info.This())) {
        return;
      }

      PurchaseResults *wrapper = PurchaseResults::Unwrap<PurchaseResults>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->OfferId;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ReceiptXmlGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::PurchaseResults^>(info.This())) {
        return;
      }

      PurchaseResults *wrapper = PurchaseResults::Unwrap<PurchaseResults>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->ReceiptXml;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void StatusGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::PurchaseResults^>(info.This())) {
        return;
      }

      PurchaseResults *wrapper = PurchaseResults::Unwrap<PurchaseResults>(info.This());

      try  {
        ::Windows::ApplicationModel::Store::ProductPurchaseStatus result = wrapper->_instance->Status;
        info.GetReturnValue().Set(Nan::New<Integer>(static_cast<int>(result)));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void TransactionIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::PurchaseResults^>(info.This())) {
        return;
      }

      PurchaseResults *wrapper = PurchaseResults::Unwrap<PurchaseResults>(info.This());

      try  {
        ::Platform::Guid result = wrapper->_instance->TransactionId;
        info.GetReturnValue().Set(NodeRT::Utils::GuidToJs(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      


    private:
      ::Windows::ApplicationModel::Store::PurchaseResults^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapPurchaseResults(::Windows::ApplicationModel::Store::PurchaseResults^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::PurchaseResults^ UnwrapPurchaseResults(Local<Value> value);
  };

  Persistent<FunctionTemplate> PurchaseResults::s_constructorTemplate;

  v8::Local<v8::Value> WrapPurchaseResults(::Windows::ApplicationModel::Store::PurchaseResults^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(PurchaseResults::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::PurchaseResults^ UnwrapPurchaseResults(Local<Value> value) {
     return PurchaseResults::Unwrap<PurchaseResults>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitPurchaseResults(Local<Object> exports) {
    PurchaseResults::Init(exports);
  }

  class ProductPurchaseDisplayProperties : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("ProductPurchaseDisplayProperties").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);





          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("name").ToLocalChecked(), NameGetter, NameSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("image").ToLocalChecked(), ImageGetter, ImageSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("description").ToLocalChecked(), DescriptionGetter, DescriptionSetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("ProductPurchaseDisplayProperties").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      ProductPurchaseDisplayProperties(::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
      else if (info.Length() == 1
        && info[0]->IsString())
      {
        try {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          
          winRtInstance = ref new ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties(arg0);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
      else if (info.Length() == 0)
      {
        try {
          winRtInstance = ref new ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties();
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      ProductPurchaseDisplayProperties *wrapperInstance = new ProductPurchaseDisplayProperties(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapProductPurchaseDisplayProperties(winRtInstance));
    }





    static void NameGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info.This())) {
        return;
      }

      ProductPurchaseDisplayProperties *wrapper = ProductPurchaseDisplayProperties::Unwrap<ProductPurchaseDisplayProperties>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Name;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void NameSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsString()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info.This())) {
        return;
      }

      ProductPurchaseDisplayProperties *wrapper = ProductPurchaseDisplayProperties::Unwrap<ProductPurchaseDisplayProperties>(info.This());

      try {

        Platform::String^ winRtValue = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));

        wrapper->_instance->Name = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void ImageGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info.This())) {
        return;
      }

      ProductPurchaseDisplayProperties *wrapper = ProductPurchaseDisplayProperties::Unwrap<ProductPurchaseDisplayProperties>(info.This());

      try  {
        ::Windows::Foundation::Uri^ result = wrapper->_instance->Image;
        info.GetReturnValue().Set(NodeRT::Utils::CreateExternalWinRTObject("Windows.Foundation", "Uri", result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ImageSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Foundation::Uri^>(value)) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info.This())) {
        return;
      }

      ProductPurchaseDisplayProperties *wrapper = ProductPurchaseDisplayProperties::Unwrap<ProductPurchaseDisplayProperties>(info.This());

      try {

        ::Windows::Foundation::Uri^ winRtValue = dynamic_cast<::Windows::Foundation::Uri^>(NodeRT::Utils::GetObjectInstance(value));

        wrapper->_instance->Image = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void DescriptionGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info.This())) {
        return;
      }

      ProductPurchaseDisplayProperties *wrapper = ProductPurchaseDisplayProperties::Unwrap<ProductPurchaseDisplayProperties>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Description;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void DescriptionSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsString()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info.This())) {
        return;
      }

      ProductPurchaseDisplayProperties *wrapper = ProductPurchaseDisplayProperties::Unwrap<ProductPurchaseDisplayProperties>(info.This());

      try {

        Platform::String^ winRtValue = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));

        wrapper->_instance->Description = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      


    private:
      ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapProductPurchaseDisplayProperties(::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ UnwrapProductPurchaseDisplayProperties(Local<Value> value);
  };

  Persistent<FunctionTemplate> ProductPurchaseDisplayProperties::s_constructorTemplate;

  v8::Local<v8::Value> WrapProductPurchaseDisplayProperties(::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(ProductPurchaseDisplayProperties::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ UnwrapProductPurchaseDisplayProperties(Local<Value> value) {
     return ProductPurchaseDisplayProperties::Unwrap<ProductPurchaseDisplayProperties>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitProductPurchaseDisplayProperties(Local<Object> exports) {
    ProductPurchaseDisplayProperties::Init(exports);
  }

  class UnfulfilledConsumable : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("UnfulfilledConsumable").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);





          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("offerId").ToLocalChecked(), OfferIdGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("productId").ToLocalChecked(), ProductIdGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("transactionId").ToLocalChecked(), TransactionIdGetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("UnfulfilledConsumable").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      UnfulfilledConsumable(::Windows::ApplicationModel::Store::UnfulfilledConsumable^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::UnfulfilledConsumable^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::UnfulfilledConsumable^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      UnfulfilledConsumable *wrapperInstance = new UnfulfilledConsumable(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::UnfulfilledConsumable^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::UnfulfilledConsumable^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapUnfulfilledConsumable(winRtInstance));
    }





    static void OfferIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>(info.This())) {
        return;
      }

      UnfulfilledConsumable *wrapper = UnfulfilledConsumable::Unwrap<UnfulfilledConsumable>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->OfferId;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ProductIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>(info.This())) {
        return;
      }

      UnfulfilledConsumable *wrapper = UnfulfilledConsumable::Unwrap<UnfulfilledConsumable>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->ProductId;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void TransactionIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>(info.This())) {
        return;
      }

      UnfulfilledConsumable *wrapper = UnfulfilledConsumable::Unwrap<UnfulfilledConsumable>(info.This());

      try  {
        ::Platform::Guid result = wrapper->_instance->TransactionId;
        info.GetReturnValue().Set(NodeRT::Utils::GuidToJs(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      


    private:
      ::Windows::ApplicationModel::Store::UnfulfilledConsumable^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapUnfulfilledConsumable(::Windows::ApplicationModel::Store::UnfulfilledConsumable^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::UnfulfilledConsumable^ UnwrapUnfulfilledConsumable(Local<Value> value);
  };

  Persistent<FunctionTemplate> UnfulfilledConsumable::s_constructorTemplate;

  v8::Local<v8::Value> WrapUnfulfilledConsumable(::Windows::ApplicationModel::Store::UnfulfilledConsumable^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(UnfulfilledConsumable::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::UnfulfilledConsumable^ UnwrapUnfulfilledConsumable(Local<Value> value) {
     return UnfulfilledConsumable::Unwrap<UnfulfilledConsumable>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitUnfulfilledConsumable(Local<Object> exports) {
    UnfulfilledConsumable::Init(exports);
  }

  class ProductLicense : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("ProductLicense").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);





          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("expirationDate").ToLocalChecked(), ExpirationDateGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("isActive").ToLocalChecked(), IsActiveGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("productId").ToLocalChecked(), ProductIdGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("isConsumable").ToLocalChecked(), IsConsumableGetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("ProductLicense").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      ProductLicense(::Windows::ApplicationModel::Store::ProductLicense^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::ProductLicense^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductLicense^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::ProductLicense^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      ProductLicense *wrapperInstance = new ProductLicense(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductLicense^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::ProductLicense^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::ProductLicense^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapProductLicense(winRtInstance));
    }





    static void ExpirationDateGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductLicense^>(info.This())) {
        return;
      }

      ProductLicense *wrapper = ProductLicense::Unwrap<ProductLicense>(info.This());

      try  {
        ::Windows::Foundation::DateTime result = wrapper->_instance->ExpirationDate;
        info.GetReturnValue().Set(NodeRT::Utils::DateTimeToJS(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void IsActiveGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductLicense^>(info.This())) {
        return;
      }

      ProductLicense *wrapper = ProductLicense::Unwrap<ProductLicense>(info.This());

      try  {
        bool result = wrapper->_instance->IsActive;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ProductIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductLicense^>(info.This())) {
        return;
      }

      ProductLicense *wrapper = ProductLicense::Unwrap<ProductLicense>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->ProductId;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void IsConsumableGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductLicense^>(info.This())) {
        return;
      }

      ProductLicense *wrapper = ProductLicense::Unwrap<ProductLicense>(info.This());

      try  {
        bool result = wrapper->_instance->IsConsumable;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      


    private:
      ::Windows::ApplicationModel::Store::ProductLicense^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapProductLicense(::Windows::ApplicationModel::Store::ProductLicense^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::ProductLicense^ UnwrapProductLicense(Local<Value> value);
  };

  Persistent<FunctionTemplate> ProductLicense::s_constructorTemplate;

  v8::Local<v8::Value> WrapProductLicense(::Windows::ApplicationModel::Store::ProductLicense^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(ProductLicense::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::ProductLicense^ UnwrapProductLicense(Local<Value> value) {
     return ProductLicense::Unwrap<ProductLicense>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitProductLicense(Local<Object> exports) {
    ProductLicense::Init(exports);
  }

  class ProductListing : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("ProductListing").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);





          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("formattedPrice").ToLocalChecked(), FormattedPriceGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("name").ToLocalChecked(), NameGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("productId").ToLocalChecked(), ProductIdGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("formattedBasePrice").ToLocalChecked(), FormattedBasePriceGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("isOnSale").ToLocalChecked(), IsOnSaleGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("saleEndDate").ToLocalChecked(), SaleEndDateGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("currencyCode").ToLocalChecked(), CurrencyCodeGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("description").ToLocalChecked(), DescriptionGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("imageUri").ToLocalChecked(), ImageUriGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("keywords").ToLocalChecked(), KeywordsGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("tag").ToLocalChecked(), TagGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("productType").ToLocalChecked(), ProductTypeGetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("ProductListing").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      ProductListing(::Windows::ApplicationModel::Store::ProductListing^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::ProductListing^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::ProductListing^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      ProductListing *wrapperInstance = new ProductListing(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::ProductListing^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::ProductListing^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapProductListing(winRtInstance));
    }





    static void FormattedPriceGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->FormattedPrice;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void NameGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Name;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ProductIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->ProductId;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void FormattedBasePriceGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->FormattedBasePrice;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void IsOnSaleGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        bool result = wrapper->_instance->IsOnSale;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void SaleEndDateGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        ::Windows::Foundation::DateTime result = wrapper->_instance->SaleEndDate;
        info.GetReturnValue().Set(NodeRT::Utils::DateTimeToJS(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CurrencyCodeGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->CurrencyCode;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void DescriptionGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Description;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ImageUriGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        ::Windows::Foundation::Uri^ result = wrapper->_instance->ImageUri;
        info.GetReturnValue().Set(NodeRT::Utils::CreateExternalWinRTObject("Windows.Foundation", "Uri", result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void KeywordsGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        ::Windows::Foundation::Collections::IIterable<::Platform::String^>^ result = wrapper->_instance->Keywords;
        info.GetReturnValue().Set(NodeRT::Collections::IterableWrapper<::Platform::String^>::CreateIterableWrapper(result, 
            [](::Platform::String^ val) -> Local<Value> {
              return NodeRT::Utils::NewString(val->Data());
            }
          ));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void TagGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Tag;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ProductTypeGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductListing^>(info.This())) {
        return;
      }

      ProductListing *wrapper = ProductListing::Unwrap<ProductListing>(info.This());

      try  {
        ::Windows::ApplicationModel::Store::ProductType result = wrapper->_instance->ProductType;
        info.GetReturnValue().Set(Nan::New<Integer>(static_cast<int>(result)));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      


    private:
      ::Windows::ApplicationModel::Store::ProductListing^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapProductListing(::Windows::ApplicationModel::Store::ProductListing^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::ProductListing^ UnwrapProductListing(Local<Value> value);
  };

  Persistent<FunctionTemplate> ProductListing::s_constructorTemplate;

  v8::Local<v8::Value> WrapProductListing(::Windows::ApplicationModel::Store::ProductListing^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(ProductListing::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::ProductListing^ UnwrapProductListing(Local<Value> value) {
     return ProductListing::Unwrap<ProductListing>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitProductListing(Local<Object> exports) {
    ProductListing::Init(exports);
  }

  class CurrentApp : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("CurrentApp").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);

        Local<Function> func;
        Local<FunctionTemplate> funcTemplate;





        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);

        Nan::SetMethod(constructor, "reportProductFulfillment", ReportProductFulfillment);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetCustomerPurchaseIdAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getCustomerPurchaseIdAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetCustomerCollectionsIdAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getCustomerCollectionsIdAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetAppPurchaseCampaignIdAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getAppPurchaseCampaignIdAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(LoadListingInformationByProductIdsAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("loadListingInformationByProductIdsAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(LoadListingInformationByKeywordsAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("loadListingInformationByKeywordsAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(ReportConsumableFulfillmentAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("reportConsumableFulfillmentAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(RequestProductPurchaseAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("requestProductPurchaseAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetUnfulfilledConsumablesAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getUnfulfilledConsumablesAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(RequestAppPurchaseAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("requestAppPurchaseAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(LoadListingInformationAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("loadListingInformationAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetAppReceiptAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getAppReceiptAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetProductReceiptAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getProductReceiptAsync").ToLocalChecked(), func);
        Nan::SetAccessor(constructor, Nan::New<String>("appId").ToLocalChecked(), AppIdGetter);
        Nan::SetAccessor(constructor, Nan::New<String>("licenseInformation").ToLocalChecked(), LicenseInformationGetter);
        Nan::SetAccessor(constructor, Nan::New<String>("linkUri").ToLocalChecked(), LinkUriGetter);


        Nan::Set(exports, Nan::New<String>("CurrentApp").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      CurrentApp(::Windows::ApplicationModel::Store::CurrentApp^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::CurrentApp^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::CurrentApp^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::CurrentApp^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      CurrentApp *wrapperInstance = new CurrentApp(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::CurrentApp^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::CurrentApp^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::CurrentApp^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapCurrentApp(winRtInstance));
    }




    static void GetCustomerPurchaseIdAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 3
          && info[0]->IsString()
          && info[1]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          Platform::String^ arg1 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[1])));
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::GetCustomerPurchaseIdAsync(arg0,arg1);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void GetCustomerCollectionsIdAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 3
          && info[0]->IsString()
          && info[1]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          Platform::String^ arg1 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[1])));
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::GetCustomerCollectionsIdAsync(arg0,arg1);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void GetAppPurchaseCampaignIdAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::ApplicationModel::Store::CurrentApp::GetAppPurchaseCampaignIdAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void LoadListingInformationByProductIdsAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::ListingInformation^>^ op;


      if (info.Length() == 2
          && (NodeRT::Utils::IsWinRtWrapperOf<::Windows::Foundation::Collections::IIterable<::Platform::String^>^>(info[0]) || info[0]->IsArray()))
      {
        try
        {
          ::Windows::Foundation::Collections::IIterable<::Platform::String^>^ arg0 = 
            [] (v8::Local<v8::Value> value) -> ::Windows::Foundation::Collections::IIterable<::Platform::String^>^
            {
              if (value->IsArray())
              {
                return NodeRT::Collections::JsArrayToWinrtVector<::Platform::String^>(value.As<Array>(), 
                 [](Local<Value> value) -> bool {
                   return (!NodeRT::Utils::IsWinRtWrapper(value));
                 },
                 [](Local<Value> value) -> ::Platform::String^ {
                   return ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));
                 }
                );
              }
              else
              {
                return dynamic_cast<::Windows::Foundation::Collections::IIterable<::Platform::String^>^>(NodeRT::Utils::GetObjectInstance(value));
              }
            } (info[0]);
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::LoadListingInformationByProductIdsAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::ListingInformation^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapListingInformation(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void LoadListingInformationByKeywordsAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::ListingInformation^>^ op;


      if (info.Length() == 2
          && (NodeRT::Utils::IsWinRtWrapperOf<::Windows::Foundation::Collections::IIterable<::Platform::String^>^>(info[0]) || info[0]->IsArray()))
      {
        try
        {
          ::Windows::Foundation::Collections::IIterable<::Platform::String^>^ arg0 = 
            [] (v8::Local<v8::Value> value) -> ::Windows::Foundation::Collections::IIterable<::Platform::String^>^
            {
              if (value->IsArray())
              {
                return NodeRT::Collections::JsArrayToWinrtVector<::Platform::String^>(value.As<Array>(), 
                 [](Local<Value> value) -> bool {
                   return (!NodeRT::Utils::IsWinRtWrapper(value));
                 },
                 [](Local<Value> value) -> ::Platform::String^ {
                   return ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));
                 }
                );
              }
              else
              {
                return dynamic_cast<::Windows::Foundation::Collections::IIterable<::Platform::String^>^>(NodeRT::Utils::GetObjectInstance(value));
              }
            } (info[0]);
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::LoadListingInformationByKeywordsAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::ListingInformation^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapListingInformation(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void ReportConsumableFulfillmentAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::FulfillmentResult>^ op;


      if (info.Length() == 3
          && info[0]->IsString()
          && NodeRT::Utils::IsGuid(info[1]))
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          ::Platform::Guid arg1 = NodeRT::Utils::GuidFromJs(info[1]);
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::ReportConsumableFulfillmentAsync(arg0,arg1);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::FulfillmentResult> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = Nan::New<Integer>(static_cast<int>(result));
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void RequestProductPurchaseAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::PurchaseResults^>^ op;


      if (info.Length() == 2
          && info[0]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::RequestProductPurchaseAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
      else if (info.Length() == 4
          && info[0]->IsString()
          && info[1]->IsString()
          && NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info[2]))
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          Platform::String^ arg1 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[1])));
          ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ arg2 = UnwrapProductPurchaseDisplayProperties(info[2]);
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::RequestProductPurchaseAsync(arg0,arg1,arg2);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
      else if (info.Length() == 3
          && info[0]->IsString()
          && info[1]->IsBoolean())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          bool arg1 = Nan::To<bool>(info[1]).FromMaybe(false);
            
          //op = ::Windows::ApplicationModel::Store::CurrentApp::RequestProductPurchaseAsync(arg0,arg1);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::PurchaseResults^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapPurchaseResults(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void GetUnfulfilledConsumablesAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::Foundation::Collections::IVectorView<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>^>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::ApplicationModel::Store::CurrentApp::GetUnfulfilledConsumablesAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::Foundation::Collections::IVectorView<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Collections::VectorViewWrapper<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>::CreateVectorViewWrapper(result, 
            [](::Windows::ApplicationModel::Store::UnfulfilledConsumable^ val) -> Local<Value> {
              return WrapUnfulfilledConsumable(val);
            },
            [](Local<Value> value) -> bool {
              return NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>(value);
            },
            [](Local<Value> value) -> ::Windows::ApplicationModel::Store::UnfulfilledConsumable^ {
              return UnwrapUnfulfilledConsumable(value);
            }
          );
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void RequestAppPurchaseAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 2
          && info[0]->IsBoolean())
      {
        try
        {
          bool arg0 = Nan::To<bool>(info[0]).FromMaybe(false);
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::RequestAppPurchaseAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void LoadListingInformationAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::ListingInformation^>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::ApplicationModel::Store::CurrentApp::LoadListingInformationAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::ListingInformation^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapListingInformation(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void GetAppReceiptAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::ApplicationModel::Store::CurrentApp::GetAppReceiptAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void GetProductReceiptAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 2
          && info[0]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
            
          op = ::Windows::ApplicationModel::Store::CurrentApp::GetProductReceiptAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }


    static void ReportProductFulfillment(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 1
        && info[0]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          
          ::Windows::ApplicationModel::Store::CurrentApp::ReportProductFulfillment(arg0);
          return;
        }
        catch (Platform::Exception ^exception)
        {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else  {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }
    }



    static void AppIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      try
      {
        ::Platform::Guid result = ::Windows::ApplicationModel::Store::CurrentApp::AppId;
        info.GetReturnValue().Set(NodeRT::Utils::GuidToJs(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      

    static void LicenseInformationGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      try
      {
        ::Windows::ApplicationModel::Store::LicenseInformation^ result = ::Windows::ApplicationModel::Store::CurrentApp::LicenseInformation;
        info.GetReturnValue().Set(WrapLicenseInformation(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      

    static void LinkUriGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      try
      {
        ::Windows::Foundation::Uri^ result = ::Windows::ApplicationModel::Store::CurrentApp::LinkUri;
        info.GetReturnValue().Set(NodeRT::Utils::CreateExternalWinRTObject("Windows.Foundation", "Uri", result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      

    private:
      ::Windows::ApplicationModel::Store::CurrentApp^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapCurrentApp(::Windows::ApplicationModel::Store::CurrentApp^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::CurrentApp^ UnwrapCurrentApp(Local<Value> value);
  };

  Persistent<FunctionTemplate> CurrentApp::s_constructorTemplate;

  v8::Local<v8::Value> WrapCurrentApp(::Windows::ApplicationModel::Store::CurrentApp^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(CurrentApp::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::CurrentApp^ UnwrapCurrentApp(Local<Value> value) {
     return CurrentApp::Unwrap<CurrentApp>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitCurrentApp(Local<Object> exports) {
    CurrentApp::Init(exports);
  }

  class CurrentAppSimulator : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("CurrentAppSimulator").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);

        Local<Function> func;
        Local<FunctionTemplate> funcTemplate;





        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);

        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetAppPurchaseCampaignIdAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getAppPurchaseCampaignIdAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(LoadListingInformationByProductIdsAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("loadListingInformationByProductIdsAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(LoadListingInformationByKeywordsAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("loadListingInformationByKeywordsAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(ReportConsumableFulfillmentAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("reportConsumableFulfillmentAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(RequestProductPurchaseAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("requestProductPurchaseAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetUnfulfilledConsumablesAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getUnfulfilledConsumablesAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(RequestAppPurchaseAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("requestAppPurchaseAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(LoadListingInformationAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("loadListingInformationAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetAppReceiptAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getAppReceiptAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(GetProductReceiptAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("getProductReceiptAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(ReloadSimulatorAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("reloadSimulatorAsync").ToLocalChecked(), func);
        Nan::SetAccessor(constructor, Nan::New<String>("appId").ToLocalChecked(), AppIdGetter);
        Nan::SetAccessor(constructor, Nan::New<String>("licenseInformation").ToLocalChecked(), LicenseInformationGetter);
        Nan::SetAccessor(constructor, Nan::New<String>("linkUri").ToLocalChecked(), LinkUriGetter);


        Nan::Set(exports, Nan::New<String>("CurrentAppSimulator").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      CurrentAppSimulator(::Windows::ApplicationModel::Store::CurrentAppSimulator^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::ApplicationModel::Store::CurrentAppSimulator^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::CurrentAppSimulator^>(info[0])) {
        try {
          winRtInstance = (::Windows::ApplicationModel::Store::CurrentAppSimulator^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      CurrentAppSimulator *wrapperInstance = new CurrentAppSimulator(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::CurrentAppSimulator^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::ApplicationModel::Store::CurrentAppSimulator^ winRtInstance;
      try {
        winRtInstance = (::Windows::ApplicationModel::Store::CurrentAppSimulator^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapCurrentAppSimulator(winRtInstance));
    }




    static void GetAppPurchaseCampaignIdAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::GetAppPurchaseCampaignIdAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void LoadListingInformationByProductIdsAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::ListingInformation^>^ op;


      if (info.Length() == 2
          && (NodeRT::Utils::IsWinRtWrapperOf<::Windows::Foundation::Collections::IIterable<::Platform::String^>^>(info[0]) || info[0]->IsArray()))
      {
        try
        {
          ::Windows::Foundation::Collections::IIterable<::Platform::String^>^ arg0 = 
            [] (v8::Local<v8::Value> value) -> ::Windows::Foundation::Collections::IIterable<::Platform::String^>^
            {
              if (value->IsArray())
              {
                return NodeRT::Collections::JsArrayToWinrtVector<::Platform::String^>(value.As<Array>(), 
                 [](Local<Value> value) -> bool {
                   return (!NodeRT::Utils::IsWinRtWrapper(value));
                 },
                 [](Local<Value> value) -> ::Platform::String^ {
                   return ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));
                 }
                );
              }
              else
              {
                return dynamic_cast<::Windows::Foundation::Collections::IIterable<::Platform::String^>^>(NodeRT::Utils::GetObjectInstance(value));
              }
            } (info[0]);
            
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::LoadListingInformationByProductIdsAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::ListingInformation^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapListingInformation(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void LoadListingInformationByKeywordsAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::ListingInformation^>^ op;


      if (info.Length() == 2
          && (NodeRT::Utils::IsWinRtWrapperOf<::Windows::Foundation::Collections::IIterable<::Platform::String^>^>(info[0]) || info[0]->IsArray()))
      {
        try
        {
          ::Windows::Foundation::Collections::IIterable<::Platform::String^>^ arg0 = 
            [] (v8::Local<v8::Value> value) -> ::Windows::Foundation::Collections::IIterable<::Platform::String^>^
            {
              if (value->IsArray())
              {
                return NodeRT::Collections::JsArrayToWinrtVector<::Platform::String^>(value.As<Array>(), 
                 [](Local<Value> value) -> bool {
                   return (!NodeRT::Utils::IsWinRtWrapper(value));
                 },
                 [](Local<Value> value) -> ::Platform::String^ {
                   return ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));
                 }
                );
              }
              else
              {
                return dynamic_cast<::Windows::Foundation::Collections::IIterable<::Platform::String^>^>(NodeRT::Utils::GetObjectInstance(value));
              }
            } (info[0]);
            
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::LoadListingInformationByKeywordsAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::ListingInformation^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapListingInformation(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void ReportConsumableFulfillmentAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::FulfillmentResult>^ op;


      if (info.Length() == 3
          && info[0]->IsString()
          && NodeRT::Utils::IsGuid(info[1]))
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          ::Platform::Guid arg1 = NodeRT::Utils::GuidFromJs(info[1]);
            
          //op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::ReportConsumableFulfillmentAsync(arg0,arg1);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::FulfillmentResult> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = Nan::New<Integer>(static_cast<int>(result));
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void RequestProductPurchaseAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::PurchaseResults^>^ op;


      if (info.Length() == 2
          && info[0]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
            
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::RequestProductPurchaseAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
      else if (info.Length() == 4
          && info[0]->IsString()
          && info[1]->IsString()
          && NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^>(info[2]))
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          Platform::String^ arg1 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[1])));
          ::Windows::ApplicationModel::Store::ProductPurchaseDisplayProperties^ arg2 = UnwrapProductPurchaseDisplayProperties(info[2]);
            
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::RequestProductPurchaseAsync(arg0,arg1,arg2);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
      else if (info.Length() == 3
          && info[0]->IsString()
          && info[1]->IsBoolean())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          bool arg1 = Nan::To<bool>(info[1]).FromMaybe(false);
            
          //op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::RequestProductPurchaseAsync(arg0,arg1);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::PurchaseResults^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapPurchaseResults(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void GetUnfulfilledConsumablesAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::Foundation::Collections::IVectorView<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>^>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::GetUnfulfilledConsumablesAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::Foundation::Collections::IVectorView<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Collections::VectorViewWrapper<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>::CreateVectorViewWrapper(result, 
            [](::Windows::ApplicationModel::Store::UnfulfilledConsumable^ val) -> Local<Value> {
              return WrapUnfulfilledConsumable(val);
            },
            [](Local<Value> value) -> bool {
              return NodeRT::Utils::IsWinRtWrapperOf<::Windows::ApplicationModel::Store::UnfulfilledConsumable^>(value);
            },
            [](Local<Value> value) -> ::Windows::ApplicationModel::Store::UnfulfilledConsumable^ {
              return UnwrapUnfulfilledConsumable(value);
            }
          );
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void RequestAppPurchaseAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 2
          && info[0]->IsBoolean())
      {
        try
        {
          bool arg0 = Nan::To<bool>(info[0]).FromMaybe(false);
            
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::RequestAppPurchaseAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void LoadListingInformationAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::ApplicationModel::Store::ListingInformation^>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::LoadListingInformationAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::ApplicationModel::Store::ListingInformation^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapListingInformation(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void GetAppReceiptAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::GetAppReceiptAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void GetProductReceiptAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Platform::String^>^ op;


      if (info.Length() == 2
          && info[0]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
            
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::GetProductReceiptAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Platform::String^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = NodeRT::Utils::NewString(result->Data());
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void ReloadSimulatorAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncAction^ op;


      if (info.Length() == 2
          && NodeRT::Utils::IsWinRtWrapperOf<::Windows::Storage::StorageFile^>(info[0]))
      {
        try
        {
          ::Windows::Storage::StorageFile^ arg0 = dynamic_cast<::Windows::Storage::StorageFile^>(NodeRT::Utils::GetObjectInstance(info[0]));
            
          op = ::Windows::ApplicationModel::Store::CurrentAppSimulator::ReloadSimulatorAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<void> t)
      {
        try {
          t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [](NodeUtils::InvokeCallbackDelegate invokeCallback) {


                Local<Value> args[] = {Undefined()};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }




    static void AppIdGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      try
      {
        ::Platform::Guid result = ::Windows::ApplicationModel::Store::CurrentAppSimulator::AppId;
        info.GetReturnValue().Set(NodeRT::Utils::GuidToJs(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      

    static void LicenseInformationGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      try
      {
        ::Windows::ApplicationModel::Store::LicenseInformation^ result = ::Windows::ApplicationModel::Store::CurrentAppSimulator::LicenseInformation;
        info.GetReturnValue().Set(WrapLicenseInformation(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      

    static void LinkUriGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      try
      {
        ::Windows::Foundation::Uri^ result = ::Windows::ApplicationModel::Store::CurrentAppSimulator::LinkUri;
        info.GetReturnValue().Set(NodeRT::Utils::CreateExternalWinRTObject("Windows.Foundation", "Uri", result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      

    private:
      ::Windows::ApplicationModel::Store::CurrentAppSimulator^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapCurrentAppSimulator(::Windows::ApplicationModel::Store::CurrentAppSimulator^ wintRtInstance);
      friend ::Windows::ApplicationModel::Store::CurrentAppSimulator^ UnwrapCurrentAppSimulator(Local<Value> value);
  };

  Persistent<FunctionTemplate> CurrentAppSimulator::s_constructorTemplate;

  v8::Local<v8::Value> WrapCurrentAppSimulator(::Windows::ApplicationModel::Store::CurrentAppSimulator^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(CurrentAppSimulator::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::ApplicationModel::Store::CurrentAppSimulator^ UnwrapCurrentAppSimulator(Local<Value> value) {
     return CurrentAppSimulator::Unwrap<CurrentAppSimulator>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitCurrentAppSimulator(Local<Object> exports) {
    CurrentAppSimulator::Init(exports);
  }


} } } } 

NAN_MODULE_INIT(init) {
  // We ignore failures for now since it probably means that
  // the initialization already happened for STA, and that's cool

  CoInitializeEx(nullptr, COINIT_MULTITHREADED);



      NodeRT::Windows::ApplicationModel::Store::InitProductPurchaseStatusEnum(target);
      NodeRT::Windows::ApplicationModel::Store::InitProductTypeEnum(target);
      NodeRT::Windows::ApplicationModel::Store::InitFulfillmentResultEnum(target);
      NodeRT::Windows::ApplicationModel::Store::InitLicenseInformation(target);
      NodeRT::Windows::ApplicationModel::Store::InitListingInformation(target);
      NodeRT::Windows::ApplicationModel::Store::InitPurchaseResults(target);
      NodeRT::Windows::ApplicationModel::Store::InitProductPurchaseDisplayProperties(target);
      NodeRT::Windows::ApplicationModel::Store::InitUnfulfilledConsumable(target);
      NodeRT::Windows::ApplicationModel::Store::InitProductLicense(target);
      NodeRT::Windows::ApplicationModel::Store::InitProductListing(target);
      NodeRT::Windows::ApplicationModel::Store::InitCurrentApp(target);
      NodeRT::Windows::ApplicationModel::Store::InitCurrentAppSimulator(target);


  NodeRT::Utils::RegisterNameSpace("Windows.ApplicationModel.Store", target);
}



NODE_MODULE(binding, init)
