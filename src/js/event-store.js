var AppMessage = require("app-message");

// Colors
module.exports.Color = {
	White: 1,
	Green: 2,
	Red: 3,
	Blue: 4,
	Brown: 5,
	Yellow: 6
};

// Types
module.exports.Type = {
	Generic: 0,
	Warning: 1,
	Failed: 2,
	Confirm: 3,
	Pin: 4,
	Battery: 5,
	BatteryCharging: 6,
	Sent: 7,
	Settings: 8,
	Event: 9,
	Rain: 10,
	Snow: 11,
	Cloudy: 12,
	Wind: 13
};

// Action codes
var ACTION_DELETE_ID 	= 0;
var ACTION_ADD_EVENT 	= 1;

/** Remove an event with the specified ID */
module.exports.removeID = function(id) {
	
	// Send msg to watch
	AppMessage.send({
		action: ACTION_DELETE_ID,
		eventID: id
	});
	
};

/** Add an event */
module.exports.add = function(event) {
	
	// Notify
	console.log("Adding new event: " + event.name + " at " + event.time + " with duration " + event.duration);
	
	// Send msg to watch
	AppMessage.send({
		action: ACTION_ADD_EVENT,
		eventID: event.id,
		name: event.name,
		subtitle: event.subtitle,
		time: Math.floor(event.time / 1000),
		duration: Math.floor(event.duration / 1000),
		color: event.color,
		type: event.type,
		hidden: event.hidden,
		noSave: event.noSave
	});
	
};

/** An event */
module.exports.Event = function(id) {
	
	// Save vars
	this.id				= id || Math.floor(Math.random() * 2147483647);
	this.name			= "Untitled Event";
	this.subtitle		= "";
	this.time			= Date.now();
	this.duration		= 0;
	this.color			= 0;
	this.type			= 0;
	this.hidden			= false;
	this.noSave			= false;
	
};