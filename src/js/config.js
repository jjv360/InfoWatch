
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
	
	/*{
		"type": "section",
		"items": [
			
			{
				"type": "heading",
				"defaultValue": "RSS Feed"
			},
			
			{
				"type": "color",
				"label": "Feed Color 1",
				"appKey": "rss-feed-color-1",
				"defaultValue": "ff0000",
				"layout": [
					["ff0000", "00ff00", "0000ff"],
					["ffffff", "aaaa55", "ffff00"]
				]
			},
			
			{
				"type": "input",
				"label": "Feed URL 1",
				"appKey": "rss-feed-url-1",
				"attributes": {
					"type": "url"
				}
			},
			
			{
				"type": "color",
				"label": "Feed Color 2",
				"appKey": "rss-feed-color-2",
				"defaultValue": "00ff00",
				"layout": [
					["ff0000", "00ff00", "0000ff"],
					["ffffff", "aaaa55", "ffff00"]
				]
			},
			
			{
				"type": "input",
				"label": "Feed URL 2",
				"appKey": "rss-feed-url-2",
				"attributes": {
					"type": "url"
				}
			},
			
			{
				"type": "color",
				"label": "Feed Color 3",
				"appKey": "rss-feed-color-3",
				"defaultValue": "0000ff",
				"layout": [
					["ff0000", "00ff00", "0000ff"],
					["ffffff", "aaaa55", "ffff00"]
				]
			},
			
			{
				"type": "input",
				"label": "Feed URL 3",
				"appKey": "rss-feed-url-3",
				"attributes": {
					"type": "url"
				}
			},
			
			{
				"type": "color",
				"label": "Feed Color 4",
				"appKey": "rss-feed-color-4",
				"defaultValue": "aaaa55",
				"layout": [
					["ff0000", "00ff00", "0000ff"],
					["ffffff", "aaaa55", "ffff00"]
				]
			},
			
			{
				"type": "input",
				"label": "Feed URL 4",
				"appKey": "rss-feed-url-4",
				"attributes": {
					"type": "url"
				}
			}
			
		]
	},*/
	
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
				"type": "toggle",
				"label": "Rain Events",
				"appKey": "weather-rain-enabled",
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