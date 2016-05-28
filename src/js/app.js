var Weather = require("weather.js");
var EventStore = require("event-store.js");

// App startup
Pebble.addEventListener("ready", function() {
	
	// Refresh weather
	checkWeather();
	
});


/** Checks the weather */
var EVENT_WEATHER = 0x03;
function checkWeather() {
	
	// Get weather
	Weather.get().then(function(weather) {
		
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