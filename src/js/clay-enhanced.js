var Clay = require("clay");


// Extend standard Clay class
module.exports = function() {
	
	// Call super constructor
	Clay.apply(this, arguments);
	
	// Add updated event handler
	this.onUpdated 			= null;
	this.cachedSettings 	= null;
	
	// Get options
	var options = arguments[2] || {};
	
	// Check if handling events automatically
	if (options.autoHandleEvents !== false) {
		
		// Add window closed handler
		var self = this;
		Pebble.addEventListener('webviewclosed', function(e) {

			// Ignore if no data
      		if (!e || !e.response)
				return;
			
			// Clear cached data
			self.cachedSettings = null;
			
			// Notify of update
			if (self.onUpdated)
				self.onUpdated();
			
		});
		
	}
	
};

// Subclass
module.exports.prototype = Object.create(Clay.prototype);
module.exports.prototype.constructor = module.exports;

/** @private Loads cached settings from localStorage */
module.exports.prototype.loadCachedSettings = function() {
	
	// Check if got already
	if (this.cachedSettings)
		return;
	
	// Try fetch
	try {
		
		// Get settings
		var str = localStorage["clay-settings"];
		if (str)
			this.cachedSettings = JSON.parse(str);
		
	} catch (e) {
	}
	
	// If nothing, set to empty
	this.cachedSettings = this.cachedSettings || {};
	
	// Set default values
	this.loadDefaults(this.config);
	
};

/** @private Iteratively loads defaults into the cached settings object */
module.exports.prototype.loadDefaults = function(items) {
	
	// Go through each item
	for (var i = 0 ; i < items.length ; i++) {
		
		// Get default
		var appKey = items[i].appKey;
		var val = items[i].defaultValue;
		
		// Set it if no value already
		if (appKey && val && !this.cachedSettings[appKey] && this.cachedSettings[appKey] !== false)
			this.cachedSettings[appKey] = val;
		
		// Go through children
		if (items[i].items && items[i].items.length)
			this.loadDefaults(items[i].items);
		
	}
	
};

/** Fetch value for the given app key */
module.exports.prototype.getSetting = function(key) {
	
	// Load cached settings
	this.loadCachedSettings();
	
	// Return value
	return this.cachedSettings[key];
	
};