var Weather = require("weather");
var EventStore = require("event-store");
var Settings = require("settings");
var vCal = require("vCal");
var Request = require("request");
var crc32 = require("crc32");


// Special event IDs
var EVENT_WEATHER 			= 0x03;
var EVENT_VCAL_ERROR		= 0x04;

// App startup
Pebble.addEventListener("ready", function() {
	
	// Refresh stuff
	refresh();
	
});

// When settings page is saved
Settings.onUpdated = function() {
	
	// Refresh stuff
	refresh();
	
};

/** Called when a message is received from the watch */
Pebble.addEventListener("appmessage", function(e) {
	
	// Ignore if no payload
	if (!e || !e.payload)
		return;
	
	// Check action
	if (e.payload.action == 2) {
		
		// Refresh
		refresh();
		
	}
	
});


/** Refresh event sources */
function refresh() {
	
	// Refresh weather
	checkWeather();
	
	// Check VCalendars
	checkvCal();
	
}


/** Checks the weather */
function checkWeather() {
	
	// Check if enabled
	if (!Settings.getSetting("weather-enabled")) {
		
		// Remove old weather event
		EventStore.removeID(EVENT_WEATHER);
		return;
		
	}
	
	// Get weather location setting
	var location = Settings.getSetting("weather-location");
	
	// Get weather
	Weather.get(location).then(function(weather) {
		
		// Remove old weather event
		EventStore.removeID(EVENT_WEATHER);
		
		// Get starting weather conditions
		var now = Date.now();
		var weatherStart = null;
		for (var i = 0 ; i < weather.predictions.length ; i++) {
			
			// Check if in the future now and we already have found a start time
			if (weather.predictions[i].time > now && weatherStart)
				break;
			
			// Check if serious
			if (weather.predictions[i].isNormal)
				continue;
			
			// Ignore rain if needed
			if (weather.predictions[i].isRain && !Settings.getSetting("weather-rain-enabled"))
				continue;
			
			// Use this item
			weatherStart = weather.predictions[i];
			
		}
		
		// Check if found
		if (!weatherStart)
			return;
		
		// Get ending weather conditions
		var weatherEnd = weather.predictions[weather.predictions.length-1];
		for (var x = 0 ; x < weather.predictions.length ; x++) {
			
			// Ignore if before start
			if (weather.predictions[x].time <= weatherStart.time)
				continue;
			
			// Check if changed
			if (!weather.predictions[x].isNormal)
				continue;
			
			// Use this one
			weatherEnd = weather.predictions[x];
			break;
			
		}
		
		// Check if got info
		if (!weatherEnd)
			return;
		
		// Create event
		var event = new EventStore.Event(EVENT_WEATHER);
		event.name = weatherStart.title;
		event.time = weatherStart.time;
		event.duration = weatherEnd.time - weatherStart.time;
		event.color = EventStore.Color.Red;
		event.type = EventStore.Type.Cloudy;
		event.hidden = (event.time < now);
		
		// Set appearance
		if (weatherStart.isRain) {
			event.color = EventStore.Color.Blue;
			event.type = EventStore.Type.Rain;
		} else if (weatherStart.isSnow) {
			event.color = EventStore.Color.White;
			event.type = EventStore.Type.Snow;
		} else if (weatherStart.isTornado || weatherStart.isHurricane || weatherStart.isTropicalStorm) {
			event.color = EventStore.Color.Brown;
			event.type = EventStore.Type.Wind;
		} else if (weatherStart.isHail) {
			event.color = EventStore.Color.Blue;
			event.type = EventStore.Type.Snow;
		}
		
		EventStore.add(event);
		
	}).catch(function(err) {
		
		// Failed
		console.warn("Unable to fetch weather! " + err);
		
	});
	
}


/** Checks vCal URLs for event updates */
function checkvCal() {

	// Remove old error events
	EventStore.removeID(EVENT_VCAL_ERROR);
	
	// Split URLs
	var urlStr = Settings.getSetting("vcal-urls") || "";
	var urls = urlStr.split(" ");
	
	// Check URLs
	for (var i = 0 ; i < urls.length ; i++)
		checkvCalURL(urls[i]);
	
}

/** Checks an individual vCal URL for updates */
function checkvCalURL(url) {
	
	// Fix webcal:// urls
	if (url.indexOf("webcal:") === 0)
		url = "http:" + url.substring(7);
	
	// Fetch data
	Request.get(url).then(function(vCalStr) {
		
		// Parse calendar
		var cal = vCal.parse(vCalStr);
		console.log(JSON.stringify(cal));
		
		// Go through events
		var now = Date.now();
		var numSent = 0;
		for (var i = 0 ; i < cal.events.length ; i++) {
			
			// Skip if in the past
			if (cal.events[i].endTime < now)
				continue;
			
			// Skip if too far in the future
			if (cal.events[i].time > now + 1000 * 60 * 60 * 24 * 7)
				continue;
			
			// Get short ID
			var id = crc32(cal.events[i].uid);
			
			// Create event
			var event = new EventStore.Event(id);
			event.name = cal.events[i].name;
			event.time = cal.events[i].time;
			event.duration = cal.events[i].duration;
			event.color = EventStore.Color.Green;
			event.type = EventStore.Type.Event;
			EventStore.add(event);
			
			// Only send max 20 items
			numSent++;
			if (numSent > 20)
				break;
			
		}
		
	}).catch(function(err) {
		
		// Get error
		var msg = err.message || "Unknown error";
		
		// Show error
		var event = new EventStore.Event(EVENT_VCAL_ERROR);
		event.name = "vCal error";
		event.subtitle = msg;
		event.time = Date.now();
		event.duration = 1000 * 60 * 2;
		event.color = EventStore.Color.Red;
		event.type = EventStore.Type.Warning;
		event.hidden = true;
		event.noSave = true;
		EventStore.add(event);
		
	});
	
}