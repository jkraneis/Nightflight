#define BRIGHTNESS  20
#define CHIPSET     WS2811
#define COLOR_ORDER GRB

#define PIN_HAUBE_RECHTS_OBEN  2
#define PIN_HAUBE_LINKS_UNTEN 14
#define PIN_HAUBE_LINKS_OBEN   7
#define PIN_HAUBE_RECHTS_UNTEN  8

#define PIN_HECK_RECHTS_OBEN  6
#define PIN_HECK_LINKS_UNTEN 20
#define PIN_HECK_LINKS_OBEN   21
#define PIN_HECK_RECHTS_UNTEN  5

#define NUM_LEDS_HAUBE_LINKS_OBEN 39
#define NUM_LEDS_HAUBE_LINKS_UNTEN 37
#define NUM_LEDS_HECK_LINKS_OBEN 35
#define NUM_LEDS_HECK_LINKS_UNTEN 47  //11 for the fin

#define NUM_LEDS_HAUBE_RECHTS_OBEN 39
#define NUM_LEDS_HAUBE_RECHTS_UNTEN 37
#define NUM_LEDS_HECK_RECHTS_OBEN 35
#define NUM_LEDS_HECK_RECHTS_UNTEN 47  //11 for the fin



CRGB ledsHaubeLinksOben[NUM_LEDS_HAUBE_LINKS_OBEN];
CRGB ledsHaubeLinksUnten[NUM_LEDS_HAUBE_LINKS_UNTEN];
CRGB ledsHeckLinksOben[NUM_LEDS_HECK_LINKS_OBEN];
CRGB ledsHeckLinksUnten[NUM_LEDS_HECK_LINKS_UNTEN];

CRGB ledsHaubeRechtsOben[NUM_LEDS_HAUBE_RECHTS_OBEN];
CRGB ledsHaubeRechtsUnten[NUM_LEDS_HAUBE_RECHTS_UNTEN];
CRGB ledsHeckRechtsOben[NUM_LEDS_HECK_RECHTS_OBEN];
CRGB ledsHeckRechtsUnten[NUM_LEDS_HECK_RECHTS_UNTEN];


LEDStrip stripHaubeLinksOben(ledsHaubeLinksOben, NUM_LEDS_HAUBE_LINKS_OBEN, false);
LEDStrip stripHaubeLinksUnten(ledsHaubeLinksUnten, NUM_LEDS_HAUBE_LINKS_UNTEN, false);
LEDStrip stripHeckLinksOben(ledsHeckLinksOben, NUM_LEDS_HECK_LINKS_OBEN, true);
LEDStrip stripHeckLinksUnten(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN, true, 0, 0);
LEDStrip stripLinksFinne(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN, false, 36, 0);

LEDStrip stripHaubeRechtsOben(ledsHaubeRechtsOben, NUM_LEDS_HAUBE_RECHTS_OBEN, false);
LEDStrip stripHaubeRechtsUnten(ledsHaubeRechtsUnten, NUM_LEDS_HAUBE_RECHTS_UNTEN, false);
LEDStrip stripHeckRechtsOben(ledsHeckRechtsOben, NUM_LEDS_HECK_RECHTS_OBEN, true);
LEDStrip stripHeckRechtsUnten(ledsHeckRechtsUnten, NUM_LEDS_HECK_RECHTS_UNTEN, true, 0, 0);
LEDStrip stripRechtsFinne(ledsHeckRechtsUnten, NUM_LEDS_HECK_RECHTS_UNTEN, false, 36, 0);

LEDStrip stripHaubeLinksOben2(ledsHaubeLinksOben, NUM_LEDS_HAUBE_LINKS_OBEN, true);
LEDStrip stripHeckLinksOben2(ledsHeckLinksOben, NUM_LEDS_HECK_LINKS_OBEN, false);

LEDStrip stripHaubeRechtsUnten2(ledsHaubeRechtsUnten, NUM_LEDS_HAUBE_RECHTS_UNTEN, false, 0, 11);
LEDStrip stripHaubeLinksUnten2(ledsHaubeLinksUnten, NUM_LEDS_HAUBE_LINKS_UNTEN, true);
LEDStrip stripHeckLinksUnten2(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN, false, 0, 11);



LEDVirtualStrip vStripLinksOben;
LEDVirtualStrip vStripLinksUnten;
LEDVirtualStrip vStripLinksFinne;

LEDVirtualStrip vStripRechtsOben;
LEDVirtualStrip vStripRechtsUnten;
LEDVirtualStrip vStripRechtsFinne;

LEDVirtualStrip vStripObenRundrum;
LEDVirtualStrip vStripUntenRundrum;

void setupIndividual()
{
  //set up the LED hardware
  FastLED.addLeds<CHIPSET, PIN_HAUBE_LINKS_OBEN, COLOR_ORDER>(ledsHaubeLinksOben, NUM_LEDS_HAUBE_LINKS_OBEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HAUBE_LINKS_UNTEN, COLOR_ORDER>(ledsHaubeLinksUnten, NUM_LEDS_HAUBE_LINKS_UNTEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_LINKS_OBEN, COLOR_ORDER>(ledsHeckLinksOben, NUM_LEDS_HECK_LINKS_OBEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_LINKS_UNTEN, COLOR_ORDER>(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN).setCorrection( TypicalLEDStrip );
  
  FastLED.addLeds<CHIPSET, PIN_HAUBE_RECHTS_OBEN, COLOR_ORDER>(ledsHaubeRechtsOben, NUM_LEDS_HAUBE_RECHTS_OBEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HAUBE_RECHTS_UNTEN, COLOR_ORDER>(ledsHaubeRechtsUnten, NUM_LEDS_HAUBE_RECHTS_UNTEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_RECHTS_OBEN, COLOR_ORDER>(ledsHeckRechtsOben, NUM_LEDS_HECK_RECHTS_OBEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_RECHTS_UNTEN, COLOR_ORDER>(ledsHeckRechtsUnten, NUM_LEDS_HECK_RECHTS_UNTEN).setCorrection( TypicalLEDStrip );
  
  vStripLinksOben.addLEDStrip(stripHaubeLinksOben);
  vStripLinksOben.addLEDStrip(stripHeckLinksOben);  
  vStripLinksFinne.addLEDStrip(stripLinksFinne);
  vStripLinksUnten.addLEDStrip(stripHaubeLinksUnten);
  vStripLinksUnten.addLEDStrip(stripHeckLinksUnten);

  vStripRechtsOben.addLEDStrip(stripHaubeRechtsOben);
  vStripRechtsOben.addLEDStrip(stripHeckRechtsOben);  
  vStripRechtsFinne.addLEDStrip(stripRechtsFinne);
  vStripRechtsUnten.addLEDStrip(stripHaubeRechtsUnten);
  vStripRechtsUnten.addLEDStrip(stripHeckRechtsUnten);

  vStripObenRundrum.addLEDStrip(stripHeckRechtsOben);
  vStripObenRundrum.addLEDStrip(stripHeckLinksOben2);
  vStripObenRundrum.addLEDStrip(stripHaubeLinksOben2);
  vStripObenRundrum.addLEDStrip(stripHaubeRechtsOben);

  vStripUntenRundrum.addLEDStrip(stripHeckRechtsUnten);
  vStripUntenRundrum.addLEDStrip(stripHeckLinksUnten2);
  vStripUntenRundrum.addLEDStrip(stripHaubeLinksUnten2);
  vStripUntenRundrum.addLEDStrip(stripHaubeRechtsUnten);


 
}

void setRcChannelValues()
{
  rcChannelSteps.setValue(map(Nightflight.currentChannelData, 955, 2090, 0, 3));
  rcChannelSteps2.setValue(map(Nightflight.currentChannelData2, 955, 2090, 0, 2));
  rcChannelSteps3.setValue(map(Nightflight.currentChannelData3, 955, 2090, 0, 2));
}
