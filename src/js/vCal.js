var moment = require("moment");
var RRule = require("rrule").RRule;


/** Parse a vCalendar string */
module.exports.parse = function(str) {
	
	// Create calendar
	var cal = new Calendar();
	
	// Go through lines
	var lines = str.split("\n");
	var currentEvent = null;
	var context = [];
	for (var i = 0 ; i < lines.length ; i++) {
		
		// Get line
		var line = lines[i].trim();
		
		// Check type
		if (line.indexOf("BEGIN:") === 0) {
			
			// Start context
			var ctx = line.substring(6);
			context.push(ctx);
			
			// Check what's starting
			if (ctx == "VEVENT") {
			
				// Start event
				currentEvent = new Event();

			}
			
		} else if (line.indexOf("END:") === 0) {
			
			// Pop context
			var ctx2 = context.pop();
			
			// Check what's ending
			if (ctx2 == "VEVENT" && currentEvent) {
			
				// Add it
				cal.events.push(currentEvent);
				
				// Add next recurrence as well
				var recurrence = currentEvent.nextRecurrence();
				if (recurrence)
					cal.events.push(recurrence);

				currentEvent = null;
				
			}
			
		} else if (line.indexOf("SUMMARY:") === 0 && context[context.length-1] == "VEVENT") {
			
			// Store info
			if (currentEvent)
				currentEvent.name = line.substring(8).trim();
			
		} else if (line.indexOf("DESCRIPTION:") === 0 && context[context.length-1] == "VEVENT") {
			
			// Store info
			if (currentEvent)
				currentEvent.description = line.substring(12).trim();
			
		} else if (line.indexOf("LOCATION:") === 0 && context[context.length-1] == "VEVENT") {
			
			// Store info
			if (currentEvent)
				currentEvent.location = line.substring(9).trim();
			
		} else if (line.indexOf("DTSTART") === 0 && context[context.length-1] == "VEVENT") {
			
			// Get start time
			if (currentEvent)
				currentEvent.time = getTimestampFromLine(line);
			
		} else if (line.indexOf("DTEND") === 0 && context[context.length-1] == "VEVENT") {
			
			// Get end time
			if (currentEvent)
				currentEvent.endTime = getTimestampFromLine(line);
			
		} else if (line.indexOf("UID:") === 0 && context[context.length-1] == "VEVENT") {
			
			// Store info
			if (currentEvent)
				currentEvent.uid = line.substring(4).trim();
			
		} else if (line.indexOf("RRULE:") === 0 && context[context.length-1] == "VEVENT") {
			
			// Store info
			if (currentEvent)
				currentEvent.recurrenceRule = line.substring(6).trim();
			
		}
		
	}
		
	// Sort by date
	cal.events.sort(function(a, b) {
		return a.time - b.time;
	});
	
	// Done
	return cal;
	
};


/** @private Extract a timestamp from a line */
function getTimestampFromLine(line) {
	
	// Get timezone
	var timezone = "UTC";
	if (line.indexOf(";TZID=") != -1) 
		timezone = line.substring(line.indexOf(";TZID=")+6, line.indexOf(":"));
	
	// Get time string
	var timeStr = line.substring(line.indexOf(":")+1).trim();
	
	// Apply time zone offset
	var m = moment.tz(timeStr, timezone);
	return m.valueOf();
	
}


/** Calendar class */
function Calendar() {
	
	// Properties
	this.events			= [];
	
}


/** Event class */
function Event() {
	
	// Properties
	this.time			= 0;
	this.name			= "Untitled Event";
	this.description	= "";
	this.endTime		= 0;
	this.location		= "";
	this.uid			= "";
	this.recurrenceOf	= null;
	
}

/** Duration */
Object.defineProperty(Event.prototype, "duration", {
	get: function() {
		return this.endTime - this.time;
	}
});

/** Get next recurrence */
Event.prototype.nextRecurrence = function() {
	
	// Check for a recurrence rule
	if (!this.recurrenceRule)
		return;
	
	// Get rule
	var options = RRule.parseString(this.recurrenceRule);
	options.dtstart = new Date(this.time);
	var rule = new RRule(options);
	
	// Get next recurrence
	var next = rule.after(new Date());
	if (!next)
		return;
	
	// Create new event
	var event = new Event();
	event.time = next.getTime();
	event.name = this.name;
	event.description = this.description;
	event.endTime = event.time + this.duration;
	event.location = this.location;
	event.uid = this.uid;
	event.recurrenceOf = this;
	return event;
	
};