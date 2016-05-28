

/** Request a resource from a server */
module.exports.get = function(url) {
	
	// Do a GET
	return module.exports.do("GET", url, null, "text");
	
}

/** Request a resource from a server as JSON */
module.exports.getJSON = function(url) {
	
	// Do a GET
	return module.exports.do("GET", url, null, "json");
	
}

/** Do a request with the specified method and data */
module.exports.do = function(method, url, payload, responseType) {
	
	// Return a promise
	console.log("Starting request to " + url);
	return new Promise(function(onSuccess, onFail) {
		
		// Create xhr
		var xhr = new XMLHttpRequest();
		xhr.open(method, url);
		xhr.send();
		
		// Listen for result
		xhr.onreadystatechange = function() {
			
			// Check state
			if (xhr.readyState != 4)
				return;
			
			// Check for error
			if (xhr.status < 200 || xhr.status >= 300) {
				console.warn("Failed to get data from " + url);
				onFail("XMLHttpRequest error");
				return;
			}
			
			// Success, process value
			var value = xhr.responseText;
			if (responseType == "json")
				value = JSON.parse(value);
			
			// Pass to promise
			console.log("Got data from " + url);
			onSuccess(value);
			
		}
		
	});
	
}