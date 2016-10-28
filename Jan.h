#define BRIGHTNESS  40
#define RESCUEBRIGHTNESS  150
#define CHIPSET     WS2811
#define COLOR_ORDER GRB

#define PIN_HAUBE_LINKS 2
#define PIN_HAUBE_RECHTS  14
#define PIN_HECK_LINKS  7
#define PIN_HECK_RECHTS   8

//#define PIN_KUFE_LINKS  6
//#define PIN_KUFE_RECHTS 20
//#define    21
//#define   5

#define NUM_LEDS_HAUBE_LINKS 78 //40 oben, 38 unten
#define NUM_LEDS_HAUBE_LINKS_OBEN 40   
#define NUM_LEDS_HAUBE_LINKS_UNTEN 38   


#define NUM_LEDS_HECK_LINKS 88  //8 for the fin
#define NUM_LEDS_HECK_LINKS_OBEN 36   
#define NUM_LEDS_FINNE_LINKS_OBEN 8   
#define NUM_LEDS_FINNE_LINKS_UNTEN 8   
#define NUM_LEDS_HECK_LINKS_UNTEN 36   

#define NUM_LEDS_HAUBE_RECHTS 78 //40 oben, 38 unten
#define NUM_LEDS_HAUBE_RECHTS_OBEN 40   
#define NUM_LEDS_HAUBE_RECHTS_UNTEN 38 

#define NUM_LEDS_HECK_RECHTS 88//124  //36+8+8+36 + 36
#define NUM_LEDS_HECK_RECHTS_OBEN 36   
#define NUM_LEDS_FINNE_RECHTS_OBEN 8   
#define NUM_LEDS_FINNE_RECHTS_UNTEN 8   
#define NUM_LEDS_HECK_RECHTS_UNTEN 36

/*#define NUM_LEDS_HECK_UNTEN 36    //


#define NUM_LEDS_KUFE_LINKS 19
#define NUM_LEDS_KUFE_RECHTS 19*/



CRGB ledsHaubeLinks[NUM_LEDS_HAUBE_LINKS];
CRGB ledsHeckLinks[NUM_LEDS_HECK_LINKS];

CRGB ledsHaubeRechts[NUM_LEDS_HAUBE_RECHTS];
CRGB ledsHeckRechts[NUM_LEDS_HECK_RECHTS];

/*CRGB ledsKufeLinks[NUM_LEDS_KUFE_LINKS];
CRGB ledsKufeRechts[NUM_LEDS_KUFE_RECHTS];
*/

LEDStrip stripHaubeLinksOben(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, true, 38, 0);
LEDStrip stripHaubeLinksUnten(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, false, 0, 40);
LEDStrip stripHeckLinksOben(ledsHeckLinks, NUM_LEDS_HECK_LINKS, false, 52, 0);
LEDStrip stripHeckLinksUnten(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, 0, 52);
LEDStrip stripLinksFinneUnten(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, 36, 44);
LEDStrip stripLinksFinneOben(ledsHeckLinks, NUM_LEDS_HECK_LINKS, false, 44, 36);


LEDStrip stripHaubeRechtsOben(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, true, 38, 0);
LEDStrip stripHaubeRechtsUnten(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, false, 0, 40);
LEDStrip stripHeckRechtsUnten(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, true, 0, 52);
LEDStrip stripRechtsFinneUnten(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, true, 36, 44);
LEDStrip stripRechtsFinneOben(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, false, 44, 36);
LEDStrip stripHeckRechtsOben(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, false, 52, 0);

/*LEDStrip stripHeckUnten(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, true, 88, 0);

LEDStrip stripKufeLinks(ledsKufeLinks, NUM_LEDS_KUFE_LINKS, false);
LEDStrip stripKufeRechts(ledsKufeRechts, NUM_LEDS_KUFE_RECHTS, false);*/

LEDStrip stripHaubeLinksOben2(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, false, 38, 0);
LEDStrip stripHeckLinksOben2(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, 52, 0);
LEDStrip stripHaubeRechtsOben2(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, true, 38, 0);
LEDStrip stripHeckRechtsOben2(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, true, 52, 0);

LEDStrip stripHaubeRechtsUnten2(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, false, 0, 40);
LEDStrip stripHaubeLinksUnten2(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, true, 0, 40);
LEDStrip stripHeckLinksUnten2(ledsHeckLinks, NUM_LEDS_HECK_LINKS, false, 0, 52);


LEDVirtualStrip vStripLinksOben;
LEDVirtualStrip vStripLinksUnten;
//LEDVirtualStrip vStripLinksFinne;

LEDVirtualStrip vStripRechtsOben;
LEDVirtualStrip vStripRechtsUnten;
//LEDVirtualStrip vStripRechtsFinne;

LEDVirtualStrip vStripObenRundrum;
LEDVirtualStrip vStripUntenRundrum;

/*LEDVirtualStrip vStripUntenLang;
LEDVirtualStrip vStripUntenKurz;
LEDVirtualStrip vStripUntenGesamt;*/

void setupIndividual()
{
  //set up the LED hardware
  FastLED.addLeds<CHIPSET, PIN_HAUBE_LINKS, COLOR_ORDER>(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_LINKS, COLOR_ORDER>(ledsHeckLinks, NUM_LEDS_HECK_LINKS).setCorrection( TypicalLEDStrip );

  FastLED.addLeds<CHIPSET, PIN_HAUBE_RECHTS, COLOR_ORDER>(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_RECHTS, COLOR_ORDER>(ledsHeckRechts, NUM_LEDS_HECK_RECHTS).setCorrection( TypicalLEDStrip );

 /* FastLED.addLeds<CHIPSET, PIN_KUFE_LINKS, COLOR_ORDER>(ledsKufeLinks, NUM_LEDS_KUFE_LINKS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_KUFE_RECHTS, COLOR_ORDER>(ledsKufeRechts, NUM_LEDS_KUFE_RECHTS).setCorrection( TypicalLEDStrip );
*/

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

  vStripObenRundrum.addLEDStrip(stripHeckRechtsOben);
  vStripObenRundrum.addLEDStrip(stripHeckLinksOben2);
  vStripObenRundrum.addLEDStrip(stripHaubeLinksOben2);
  vStripObenRundrum.addLEDStrip(stripHaubeRechtsOben);

  vStripUntenRundrum.addLEDStrip(stripHeckRechtsUnten);
  vStripUntenRundrum.addLEDStrip(stripHeckLinksUnten2);
  vStripUntenRundrum.addLEDStrip(stripHaubeLinksUnten2);
  vStripUntenRundrum.addLEDStrip(stripHaubeRechtsUnten);

  /*vStripUntenLang.addLEDStrip(stripKufeLinks);
  vStripUntenLang.addLEDStrip(stripHeckUnten);
  vStripUntenKurz.addLEDStrip(stripKufeRechts);

  vStripUntenGesamt.addLEDStrip(stripKufeLinks);
  vStripUntenGesamt.addLEDStrip(stripHeckUnten);
  vStripUntenGesamt.addLEDStrip(stripKufeRechts);*/
}


void setRcChannelValues()
{
  rcChannelSteps.setValue(Nightflight.currentChannelData);
  rcChannelSteps2.setValue(Nightflight.currentChannelData2);
  rcChannelSteps3.setValue(Nightflight.currentChannelData3);

}
