var Location = require("location");
var Request = require("request");
var Settings = require("settings");

/** Get the weather */
module.exports.get = function(location) {
	
	// Check if got location
	if (location) {
		
		// Use this location
		return getWeatherIn(location);
		
	}
	
	// Get GPS location
	return Location.get().then(function(location) {
		
		// Get weather in these coordinates
		return getWeatherIn(null, location.coords.latitude, location.coords.longitude);
			
	});
	
};

/** @private Get weather in a location or a latitude+longitude */
function getWeatherIn(location, latitude, longitude) {
	
	// Get API key
	var APIKey = Settings.getSetting("openweathermap-api-key") || "5e0f44fd8666d5fc8c2ad33651664696";
	
	// Choose URL
	var currentURL, forecastURL;
	if (location) {
		currentURL = "http://api.openweathermap.org/data/2.5/weather?units=metric&q=" + encodeURIComponent(location) + "&appid=" + APIKey;
		forecastURL = "http://api.openweathermap.org/data/2.5/forecast?units=metric&q=" + encodeURIComponent(location) + "&appid=" + APIKey;
	} else {
		currentURL = "http://api.openweathermap.org/data/2.5/weather?units=metric&lat=" + latitude + "&lon=" + longitude + "&appid=" + APIKey;
		forecastURL = "http://api.openweathermap.org/data/2.5/forecast?units=metric&lat=" + latitude + "&lon=" + longitude + "&appid=" + APIKey;
	}
	
		
	// Get the current weather
	console.log("Getting weather...");
	return Request.getJSON(currentURL).then(function(currentWeather) {

		// Get weather data for these coordinates
		return Request.getJSON(forecastURL).then(function(weather) {

			// Create info
			var info = {};
			info.coords = location && location.coords;
			info.city = weather.city;
			info.predictions = [];

			// Add first prediction, the current weather
			if (currentWeather)
				weather.list.unshift(currentWeather);

			// Make weather results into a nice format
			weather.list.forEach(function(p) {
				
				// Skip if not enough weather info
				if (!p.weather || !p.weather.length)
					return;

				// Create prediction
				var pred = new WeatherPrediction();
				pred.time = p.dt * 1000;
				pred.weatherID = p.weather[0].id;
				pred.temperature = p.main.temp;
				pred.temperatureMin = p.main.temp_min;
				pred.temperatureMax = p.main.temp_max;
				pred.name = p.weather[0].main;
				pred.description = p.weather[0].description;
				pred.iconURL = "https://openweathermap.org/img/w/" + p.weather[0].icon + ".png";
				info.predictions.push(pred);

			});

			// Make sure predictions are sorted by date
			info.predictions.sort(function(a, b) {
				return a.time - b.time;
			});

			// Done
			console.log("Got weather");
			return info;

		});

	});
	
}


/** Weather predictions */
function WeatherPrediction() {
}

/** Check if rain */
Object.defineProperty(WeatherPrediction.prototype, "isRain", {
	get: function() {
		return (this.weatherID >= 200 && this.weatherID <= 399) || (this.weatherID >= 500 && this.weatherID <= 599);
	}
});

/** Check if snow */
Object.defineProperty(WeatherPrediction.prototype, "isSnow", {
	get: function() {
		return (this.weatherID >= 600 && this.weatherID <= 699);
	}
});

/** Check if tornado */
Object.defineProperty(WeatherPrediction.prototype, "isTornado", {
	get: function() {
		return (this.weatherID == 900);
	}
});

/** Check if tropical storm */
Object.defineProperty(WeatherPrediction.prototype, "isTropicalStorm", {
	get: function() {
		return (this.weatherID == 901);
	}
});

/** Check if hail */
Object.defineProperty(WeatherPrediction.prototype, "isHail", {
	get: function() {
		return (this.weatherID == 906);
	}
});

/** Check if tornado */
Object.defineProperty(WeatherPrediction.prototype, "isHurricane", {
	get: function() {
		return (this.weatherID == 902) || (this.weatherID >= 961 && this.weatherID <= 962);
	}
});

/** Check if normal */
Object.defineProperty(WeatherPrediction.prototype, "isNormal", {
	get: function() {
		return !this.isHurricane && !this.isHail && !this.isTropicalStorm && !this.isTornado && !this.isSnow && !this.isRain;
	}
});

/** Get title */
Object.defineProperty(WeatherPrediction.prototype, "title", {
	get: function() {
		
		// Check for name
		if (this.name)
			return this.name;
		
		// Check conditions
		if (this.isRain)
			return "Rain";
		
		// Check conditions
		if (this.isSnow)
			return "Snow";
		
		// Check conditions
		if (this.isTornado)
			return "Tornado";
		
		// Check conditions
		if (this.isTropicalStorm)
			return "Tropical Storm";
		
		// Check conditions
		if (this.isHail)
			return "Hail";
		
		// Check conditions
		if (this.isHurricane)
			return "Hurricane";
		
		// Unknown weather title
		return "Normal";
		
	}
});