
// The app's configuration info
module.exports = [
	
	{
		"type": "heading",
		"defaultValue": "InfoWatch"
	},
	
	{
		"type": "text",
		"defaultValue": "A minimalist watchface that shows you information as it happens."
	},
	
	{
		"type": "section",
		"items": [
			
			{
				"type": "heading",
				"defaultValue": "Weather"
			},
			
			{
				"type": "text",
				"defaultValue": "Weather data is provided by OpenWeatherMap."
			},
			
			{
				"type": "toggle",
				"label": "Enabled",
				"appKey": "weather-enabled",
				"defaultValue": true
			},
			
			{
				"type": "input",
				"label": "Location",
				"appKey": "weather-location",
				"description": "For automatic location using the GPS, leave this field blank. If you want to enter a city manually, use 'City, Country'. Eg: London, UK",
				"attributes": {
					"placeholder": "Automatic"
				}
			},
			
			{
				"type": "input",
				"label": "API key",
				"appKey": "openweathermap-api-key",
				"description": "If you would like to use your own OpenWeatherMap API key, you can set it here.",
				"attributes": {
					"placeholder": "Automatic"
				}
			}
			
		]
	},
	
	{
		"type": "submit",
		"defaultValue": "Save"
	}
	
];