var Promise = require('es6-promise.js').Promise;


/** Fetch location */
module.exports.get = function() {
	
	// Create promise
	console.log("Getting location...");
	return new Promise(function(onSuccess, onFail) {
		
		// Fetch position
		navigator.geolocation.getCurrentPosition(function(data) {
			
			// Got location
			console.log("Got coordinates: " + data.coords.latitude + ", " + data.coords.longitude);
			onSuccess(data);
			
		}, onFail, {
			timeout: 1000 * 60,
			maximumAge: 1000 * 60 * 60
		});
		
	});
	
};