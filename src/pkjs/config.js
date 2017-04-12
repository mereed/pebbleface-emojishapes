module.exports = [
  {
    "type": "heading",
    "defaultValue": "Configuration"
  },
     {
        "type": "text",
        "defaultValue": "<h6>A white button = OFF, an orange button = ON</h6>",
      },
	{
    "type": "section",
    "items": [
 	{
        "type": "heading",
        "defaultValue": "<h5>Display</h5>"
	},
     {
        "type": "color",
        "messageKey": "hrcol",
        "defaultValue": "0xFFFFFF",
        "label": "Hour Color"
      },
      {
        "type": "color",
        "messageKey": "mincol",
        "defaultValue": "0xFFFFFF",
        "label": "Minute Colour"
      },
      {
        "type": "color",
        "messageKey": "seccol",
        "defaultValue": "0xFFFFFF",
        "label": "Second Colour"
	  },
		{
        "type": "color",
        "messageKey": "dcol",
        "defaultValue": "0xFFFFFF",
        "label": "Date Circle Colour"
	  },
		{
        "type": "color",
        "messageKey": "dtext",
        "defaultValue": "0x000000",
        "label": "Date Text Colour"
	  },
		{
        "type": "color",
        "messageKey": "bgcol",
        "defaultValue": "0x000000",
        "label": "Main bg color IF blank is selected below"
	  },
		{
        "type": "toggle",
        "messageKey": "date",
        "label": "Show date",
        "defaultValue": false
      },
	  {
        "type": "toggle",
        "messageKey": "secs",
        "label": "Show seconds",
        "defaultValue": false
      },
	  {
        "type": "toggle",
        "messageKey": "batt",
        "label": "Show battery icon",
        "defaultValue": true
      },
	
	
			{
  "type": "select",
  "messageKey": "background",
  "defaultValue": 0,
  "label": "Face style",
  "options": [
    { 
      "label": "Inner hour numbers",
      "value": 0
    },
    { 
      "label": "Outer minute numbers",
      "value": 1
    },
	  { 
      "label": "Hours 'n minute numbers",
      "value": 2
    },
    { 
      "label": "Blank (select background color above)",
      "value": 3
    },
    { 
      "label": "Hour 'n minute dots",
      "value": 4
    },
    { 
      "label": "Minute dots",
      "value": 5
    },
	{ 
      "label": "Radial Lines",
      "value": 6
    },
    { 
      "label": "Rings",
      "value": 7
    },
    { 
      "label": "Lines 'n minute numbers",
      "value": 8
    }
  ]
},

				{
  "type": "select",
  "messageKey": "topmarker",
  "defaultValue": 0,
  "label": "Marker at 12 position",
  "options": [
    { 
      "label": "Pebble",
      "value": 0
    },
    { 
      "label": "P",
      "value": 1
    },
	  { 
      "label": "00",
      "value": 2
    },
    { 
      "label": "White square",
      "value": 3
    },
    { 
      "label": "blank",
      "value": 4
    }
  ]
}
		]
	},
  {
        "type": "text",
        "defaultValue": "<h6>If you find this watchface useful, please consider making a <a href='https://www.paypal.me/markchopsreed'>small donation here</a>. Thankyou.</h6>",
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];