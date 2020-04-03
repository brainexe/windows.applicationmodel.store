_ProductPurchaseStatus = function () {
  this.succeeded = 0;
  this.alreadyPurchased = 1;
  this.notFulfilled = 2;
  this.notPurchased = 3;
}
exports.ProductPurchaseStatus = new _ProductPurchaseStatus();

_ProductType = function () {
  this.unknown = 0;
  this.durable = 1;
  this.consumable = 2;
}
exports.ProductType = new _ProductType();

_FulfillmentResult = function () {
  this.succeeded = 0;
  this.nothingToFulfill = 1;
  this.purchasePending = 2;
  this.purchaseReverted = 3;
  this.serverError = 4;
}
exports.FulfillmentResult = new _FulfillmentResult();

LicenseInformation = (function () {
  var cls = function LicenseInformation() {
    this.expirationDate = new Date();
    this.isActive = new Boolean();
    this.isTrial = new Boolean();
    this.productLicenses = new Object();
  };
  

    cls.prototype.addListener = function addListener(eventName, callback){}
    cls.prototype.removeListener = function removeListener(eventName, callback){}
    cls.prototype.on = function on(eventName, callback){}
    cls.prototype.off = function off(eventName, callback){}
  return cls;
}) ();
exports.LicenseInformation = LicenseInformation;

ListingInformation = (function () {
  var cls = function ListingInformation() {
    this.ageRating = new Number();
    this.currentMarket = new String();
    this.description = new String();
    this.formattedPrice = new String();
    this.name = new String();
    this.productListings = new Object();
    this.currencyCode = new String();
    this.formattedBasePrice = new String();
    this.isOnSale = new Boolean();
    this.saleEndDate = new Date();
  };
  

  return cls;
}) ();
exports.ListingInformation = ListingInformation;

PurchaseResults = (function () {
  var cls = function PurchaseResults() {
    this.offerId = new String();
    this.receiptXml = new String();
    this.status = new ProductPurchaseStatus();
    this.transactionId = new String();
  };
  

  return cls;
}) ();
exports.PurchaseResults = PurchaseResults;

ProductPurchaseDisplayProperties = (function () {
  var cls = function ProductPurchaseDisplayProperties() {
    this.name = new String();
    this.image = new Object();
    this.description = new String();
  };
  
var cls = function ProductPurchaseDisplayProperties(name) {
      this.name = new String();
      this.image = new Object();
      this.description = new String();
};


  return cls;
}) ();
exports.ProductPurchaseDisplayProperties = ProductPurchaseDisplayProperties;

UnfulfilledConsumable = (function () {
  var cls = function UnfulfilledConsumable() {
    this.offerId = new String();
    this.productId = new String();
    this.transactionId = new String();
  };
  

  return cls;
}) ();
exports.UnfulfilledConsumable = UnfulfilledConsumable;

ProductLicense = (function () {
  var cls = function ProductLicense() {
    this.expirationDate = new Date();
    this.isActive = new Boolean();
    this.productId = new String();
    this.isConsumable = new Boolean();
  };
  

  return cls;
}) ();
exports.ProductLicense = ProductLicense;

ProductListing = (function () {
  var cls = function ProductListing() {
    this.formattedPrice = new String();
    this.name = new String();
    this.productId = new String();
    this.formattedBasePrice = new String();
    this.isOnSale = new Boolean();
    this.saleEndDate = new Date();
    this.currencyCode = new String();
    this.description = new String();
    this.imageUri = new Object();
    this.keywords = new Object();
    this.tag = new String();
    this.productType = new ProductType();
  };
  

  return cls;
}) ();
exports.ProductListing = ProductListing;

CurrentApp = (function () {
  var cls = function CurrentApp() {
  };
  

  cls.getCustomerPurchaseIdAsync = function getCustomerPurchaseIdAsync(serviceTicket, publisherUserId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="serviceTicket" type="String">A param.</param>
    /// <param name="publisherUserId" type="String">A param.</param>
    /// </signature>
  }



  cls.getCustomerCollectionsIdAsync = function getCustomerCollectionsIdAsync(serviceTicket, publisherUserId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="serviceTicket" type="String">A param.</param>
    /// <param name="publisherUserId" type="String">A param.</param>
    /// </signature>
  }



  cls.getAppPurchaseCampaignIdAsync = function getAppPurchaseCampaignIdAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }



  cls.loadListingInformationByProductIdsAsync = function loadListingInformationByProductIdsAsync(productIds, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productIds" type="Object">A param.</param>
    /// </signature>
  }



  cls.loadListingInformationByKeywordsAsync = function loadListingInformationByKeywordsAsync(keywords, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="keywords" type="Object">A param.</param>
    /// </signature>
  }



  cls.reportConsumableFulfillmentAsync = function reportConsumableFulfillmentAsync(productId, transactionId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// <param name="transactionId" type="String">A param.</param>
    /// </signature>
  }



  cls.requestProductPurchaseAsync = function requestProductPurchaseAsync(productId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// </signature>
  }

cls.requestProductPurchaseAsync = function requestProductPurchaseAsync(productId, offerId, displayProperties, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// <param name="offerId" type="String">A param.</param>
    /// <param name="displayProperties" type="ProductPurchaseDisplayProperties">A param.</param>
    /// </signature>
  }

cls.requestProductPurchaseAsync = function requestProductPurchaseAsync(productId, includeReceipt, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// <param name="includeReceipt" type="Boolean">A param.</param>
    /// </signature>
  }



  cls.getUnfulfilledConsumablesAsync = function getUnfulfilledConsumablesAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }



  cls.requestAppPurchaseAsync = function requestAppPurchaseAsync(includeReceipt, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="includeReceipt" type="Boolean">A param.</param>
    /// </signature>
  }



  cls.loadListingInformationAsync = function loadListingInformationAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }



  cls.getAppReceiptAsync = function getAppReceiptAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }



  cls.getProductReceiptAsync = function getProductReceiptAsync(productId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// </signature>
  }



  cls.reportProductFulfillment = function reportProductFulfillment(productId) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// </signature>
  }


  cls.appId = new String();
  cls.licenseInformation = new LicenseInformation();
  cls.linkUri = new Object();
  return cls;
}) ();
exports.CurrentApp = CurrentApp;

CurrentAppSimulator = (function () {
  var cls = function CurrentAppSimulator() {
  };
  

  cls.getAppPurchaseCampaignIdAsync = function getAppPurchaseCampaignIdAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }



  cls.loadListingInformationByProductIdsAsync = function loadListingInformationByProductIdsAsync(productIds, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productIds" type="Object">A param.</param>
    /// </signature>
  }



  cls.loadListingInformationByKeywordsAsync = function loadListingInformationByKeywordsAsync(keywords, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="keywords" type="Object">A param.</param>
    /// </signature>
  }



  cls.reportConsumableFulfillmentAsync = function reportConsumableFulfillmentAsync(productId, transactionId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// <param name="transactionId" type="String">A param.</param>
    /// </signature>
  }



  cls.requestProductPurchaseAsync = function requestProductPurchaseAsync(productId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// </signature>
  }

cls.requestProductPurchaseAsync = function requestProductPurchaseAsync(productId, offerId, displayProperties, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// <param name="offerId" type="String">A param.</param>
    /// <param name="displayProperties" type="ProductPurchaseDisplayProperties">A param.</param>
    /// </signature>
  }

cls.requestProductPurchaseAsync = function requestProductPurchaseAsync(productId, includeReceipt, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// <param name="includeReceipt" type="Boolean">A param.</param>
    /// </signature>
  }



  cls.getUnfulfilledConsumablesAsync = function getUnfulfilledConsumablesAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }



  cls.requestAppPurchaseAsync = function requestAppPurchaseAsync(includeReceipt, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="includeReceipt" type="Boolean">A param.</param>
    /// </signature>
  }



  cls.loadListingInformationAsync = function loadListingInformationAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }



  cls.getAppReceiptAsync = function getAppReceiptAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
  }



  cls.getProductReceiptAsync = function getProductReceiptAsync(productId, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="productId" type="String">A param.</param>
    /// </signature>
  }



  cls.reloadSimulatorAsync = function reloadSimulatorAsync(simulatorSettingsFile, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="simulatorSettingsFile" type="Object">A param.</param>
    /// </signature>
  }



  cls.appId = new String();
  cls.licenseInformation = new LicenseInformation();
  cls.linkUri = new Object();
  return cls;
}) ();
exports.CurrentAppSimulator = CurrentAppSimulator;

