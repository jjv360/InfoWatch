
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
				"defaultValue": "General"
			},
			
			{
				"type": "select",
				"appKey": "hourHandMode",
				"label": "Hour hand",
				"defaultValue": 0,
				"description": "If set to Automatic, the hour hand will only be visible if there is an event in the next 12 hours.",
				"options": [
					{"value": "auto", "label": "Automatic"},
					{"value": "on", "label": "Visible"},
					{"value": "off", "label": "Hidden"}
				]
			},
			
			{
				"type": "toggle",
				"appKey": "showEventDots",
				"label": "Event dots",
				"defaultValue": true,
				"description": "Display event dots for the next 12 hours around the edge of the screen."
			},
			
			{
				"type": "toggle",
				"appKey": "vibrateOnNewEvent",
				"label": "Vibrate on new event",
				"defaultValue": false,
				"description": "If enabled, will vibrate when the upcoming event changes."
			}
			
		]
		
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
	
	/*{
		"type": "section",
		"items": [
			
			{
				"type": "heading",
				"defaultValue": "iCloud Calendar"
			},
			
			{
				"type": "toggle",
				"label": "Enabled",
				"appKey": "icloud-enabled",
				"defaultValue": false
			},
			
			{
				"type": "input",
				"label": "Email",
				"appKey": "icloud-email",
				"attributes": {
					"type": "email"
				}
			},
			
			{
				"type": "input",
				"label": "Password",
				"appKey": "icloud-password",
				"description": "If you use two-factor authentication, you will need to enter your App Specific Password here.",
				"attributes": {
					"type": "password"
				}
			}
			
		]
	},*/
	
	/*{
		"type": "section",
		"items": [
			
			{
				"type": "heading",
				"defaultValue": "Google Calendar"
			},
			
			{
				"type": "toggle",
				"label": "Enabled",
				"appKey": "gcal-enabled",
				"defaultValue": false
			},
			
			{
				"type": "input",
				"label": "Email",
				"appKey": "gcal-email",
				"attributes": {
					"type": "email"
				}
			},
			
			{
				"type": "input",
				"label": "Password",
				"appKey": "gcal-password",
				"description": "If you use two-factor authentication, you will need to enter your App Specific Password here.",
				"attributes": {
					"type": "password"
				}
			}
			
		]
	},*/
	
	{
		"type": "section",
		"items": [
			
			{
				"type": "heading",
				"defaultValue": "vCalendar"
			},
			
			{
				"type": "toggle",
				"label": "Enabled",
				"appKey": "vcal-enabled",
				"defaultValue": false
			},
			
			{
				"type": "input",
				"label": "URLs",
				"appKey": "vcal-urls",
				"description": "Here you can enter a space-separated list of vCalendar URLs."
			},
			
			{
				"type": "text",
				"defaultValue": "<h4>iCloud:</h4>To connect to an iCloud calendar, go to the Calendar app, open the calendar info (i), make it a Public Calendar, press Share Link and then copy the link into here.<br/><br/><h4>Google Calendar:</h4>To connect to a Google Calendar, open calendar.google.com on your PC, go to settings, go to the Calendars tab, click a calendar and then copy the Private Address into here."
			}
			
		]
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
	},
	
	{
		"type": "section",
		"items": [
			
			{
				"type": "heading",
				"defaultValue": "FAQs"
			},
			
			{
				"type": "text",
				"defaultValue": "<h5>What is the red line?</h5><h6 style='color: #a4a4a4; '>The red line is the current hour hand. It only appears when there is an upcoming event in the next 12 hours.</h6>",
			},
			
			{
				"type": "text",
				"defaultValue": "<h5>What are the dots around the edge of the screen?</h5><h6 style='color: #a4a4a4; '>The dots represent events in the next 12 hours. When the hour hand reaches a dot, that event starts.</h6>",
			},
			
		]
	},
	
];