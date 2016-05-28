var Location = require("location.js");
var Request = require("request.js");

// Vars
var OpenWeatherMapAppID = "5e0f44fd8666d5fc8c2ad33651664696";

/** Get the weather */
module.exports.get = function() {
	
	// Get GPS location
	return Location.get().then(function(location) {
		
		// Get the current weather
		console.log("Getting weather...");
		return Request.getJSON("http://api.openweathermap.org/data/2.5/weather?units=metric&lat=" + location.coords.latitude + "&lon=" + location.coords.longitude + "&appid=" + OpenWeatherMapAppID).then(function(currentWeather) {
			
			// Get weather data for these coordinates
			return Request.getJSON("http://api.openweathermap.org/data/2.5/forecast?units=metric&lat=" + location.coords.latitude + "&lon=" + location.coords.longitude + "&appid=" + OpenWeatherMapAppID).then(function(weather) {

				// Create info
				var info = {};
				info.coords = location.coords;
				info.city = weather.city;
				info.predictions = [];
				
				// Add first prediction, the current weather
				if (currentWeather)
					weather.list.unshift(currentWeather);

				// Make weather results into a nice format
				weather.list.forEach(function(p) {

					// Create prediction
					var pred = new WeatherPrediction();
					pred.time = p.dt * 1000;
					pred.weatherID = p.weather.id;
					pred.temperature = p.main.temp;
					pred.temperatureMin = p.main.temp_min;
					pred.temperatureMax = p.main.temp_max;
					pred.name = p.weather.main;
					pred.description = p.weather.description;
					pred.iconURL = "https://openweathermap.org/img/w/" + p.weather.icon + ".png";
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
			
	});
	
};


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