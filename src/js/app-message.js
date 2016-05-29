
var queue = [];
var isBusy = false;
var delayBetweenMessages = 250;

// Sends a message to the watch
module.exports.send = function(data) {
	
	// Add to queue
	queue.push(data);
	
	// Start sending if not busy
	if (!isBusy)
		sendNext();
	
};

// Sends the next message to the watch
function sendNext(retryData, retryCount) {
	
	// We are busy now
	isBusy = true;
	
	// Stop if no more messages
	if (!retryData && queue.length === 0) {
		isBusy = false;
		return;
	}
	
	// Send it
	var data = retryData || queue.pop();
	console.log("Sending app message (try " + (retryCount || 0) + "): " + JSON.stringify(data));
	Pebble.sendAppMessage(data, function() {
		
		// Success, send next one
		setTimeout(sendNext, delayBetweenMessages);
		
	}, function(errData, errorMsg) {
		
		// Failed!
		console.log("Failed to send app message! " + errorMsg + " - " + JSON.stringify(errData));
		
		// Check retry count, if too high move on
		if (retryCount > 3)
			return setTimeout(sendNext, delayBetweenMessages);
		
		// Try again
		setTimeout(sendNext.bind(null, data, (retryCount || 0) + 1), delayBetweenMessages);
		
	});
	
}