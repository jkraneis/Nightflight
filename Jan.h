#define BRIGHTNESS  20
#define CHIPSET     WS2811
#define COLOR_ORDER GRB

#define PIN_HAUBE_RECHTS  2
#define PIN_HAUBE_LINKS 14
#define PIN_HECK_RECHTS   7
#define PIN_HECK_LINKS  8

#define NUM_LEDS_HAUBE_LINKS 78 //40 oben, 38 unten
#define NUM_LEDS_HECK_LINKS 88  //8 for the fin

#define NUM_LEDS_HAUBE_RECHTS 78 //40 oben, 38 unten
#define NUM_LEDS_HECK_RECHTS 88  //36+8+8+36


CRGB ledsHaubeLinks[NUM_LEDS_HAUBE_LINKS];
CRGB ledsHeckLinks[NUM_LEDS_HECK_LINKS];

CRGB ledsHaubeRechts[NUM_LEDS_HAUBE_RECHTS];
CRGB ledsHeckRechts[NUM_LEDS_HECK_RECHTS];


LEDStrip stripHaubeLinksOben(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, true, 38, 0);
LEDStrip stripHaubeLinksUnten(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, false, 0, 40);
LEDStrip stripHeckLinksOben(ledsHeckLinks, NUM_LEDS_HECK_LINKS, false, 52, 0);
LEDStrip stripHeckLinksUnten(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, 0, 52);
LEDStrip stripLinksFinneUnten(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, 36, 44);
LEDStrip stripLinksFinneOben(ledsHeckLinks, NUM_LEDS_HECK_LINKS, false, 44, 36);

LEDStrip stripHaubeRechtsOben(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, true, 38, 0);
LEDStrip stripHaubeRechtsUnten(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, false, 0, 40);
LEDStrip stripHeckRechtsOben(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, false, 52, 0);
LEDStrip stripHeckRechtsUnten(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, true, 0, 52);
LEDStrip stripRechtsFinneOben(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, false, 44, 36);
LEDStrip stripRechtsFinneUnten(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, true, 36, 44);

LEDStrip stripHaubeLinksOben2(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, false, 38, 0);
LEDStrip stripHeckLinksOben2(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, 52, 0);
LEDStrip stripHaubeRechtsOben2(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, true, 38, 0);
LEDStrip stripHeckRechtsOben2(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, false, 52, 0);

LEDStrip stripHaubeLinksUnten2(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, true, 0, 40);
LEDStrip stripHeckLinksUnten2(ledsHeckLinks, NUM_LEDS_HECK_LINKS, false, 0, 52);


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
  FastLED.addLeds<CHIPSET, PIN_HAUBE_LINKS, COLOR_ORDER>(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_LINKS, COLOR_ORDER>(ledsHeckLinks, NUM_LEDS_HECK_LINKS).setCorrection( TypicalLEDStrip );

  FastLED.addLeds<CHIPSET, PIN_HAUBE_RECHTS, COLOR_ORDER>(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_RECHTS, COLOR_ORDER>(ledsHeckRechts, NUM_LEDS_HECK_RECHTS).setCorrection( TypicalLEDStrip );

  vStripLinksOben.addLEDStrip(stripHaubeLinksOben);
  vStripLinksOben.addLEDStrip(stripHeckLinksOben);  
  vStripLinksOben.addLEDStrip(stripLinksFinneOben);  
  //vStripLinksFinne.addLEDStrip(stripLinksFinneUnten);
  vStripLinksUnten.addLEDStrip(stripHaubeLinksUnten);
  vStripLinksUnten.addLEDStrip(stripHeckLinksUnten);
  vStripLinksUnten.addLEDStrip(stripLinksFinneUnten);

  vStripRechtsOben.addLEDStrip(stripHaubeRechtsOben);
  vStripRechtsOben.addLEDStrip(stripHeckRechtsOben);  
  vStripRechtsOben.addLEDStrip(stripRechtsFinneOben);  
  //vStripRechtsFinne.addLEDStrip(stripRechtsFinneUnten);
  vStripRechtsUnten.addLEDStrip(stripHaubeRechtsUnten);
  vStripRechtsUnten.addLEDStrip(stripHeckRechtsUnten);
  vStripRechtsUnten.addLEDStrip(stripRechtsFinneUnten);

  vStripObenRundrum.addLEDStrip(stripHeckRechtsOben2);
  vStripObenRundrum.addLEDStrip(stripHeckLinksOben2);
  vStripObenRundrum.addLEDStrip(stripHaubeLinksOben2);
  vStripObenRundrum.addLEDStrip(stripHaubeRechtsOben2);

  vStripUntenRundrum.addLEDStrip(stripHeckRechtsUnten);
  vStripUntenRundrum.addLEDStrip(stripHeckLinksUnten2);
  vStripUntenRundrum.addLEDStrip(stripHaubeLinksUnten2);
  vStripUntenRundrum.addLEDStrip(stripHaubeRechtsUnten);


  /*Nightflight.addVirtualStrip(vStripLinksOben);
  Nightflight.addVirtualStrip(vStripLinksUnten);
  Nightflight.addVirtualStrip(vStripRechtsOben);
  Nightflight.addVirtualStrip(vStripRechtsUnten);*/
}


void setRcChannelValues()
{
  rcChannelSteps.setValue(map(Nightflight.currentChannelData, 955, 2090, 0, 10));
  rcChannelSteps2.setValue(map(Nightflight.currentChannelData2, 955, 2090, 0, 2));

}
