#define BRIGHTNESS  40
#define RESCUEBRIGHTNESS  150
#define CHIPSET     WS2812
#define COLOR_ORDER GRB

#define PIN_HAUBE_LINKS  2
#define PIN_HAUBE_RECHTS 14
#define PIN_HECK_LINKS   7
#define PIN_HECK_RECHTS  8

#define PIN_HECK_UNTEN  6
//#define PIN_KUFE_LINKS 20
#define PIN_KUFE_RECHTS   21
#define PIN_KUFE_LINKS  5

#define NUM_LEDS_HAUBE_LINKS 77   // 38 unten, 39 oben
#define NUM_LEDS_HAUBE_LINKS_OBEN 39   
#define NUM_LEDS_HAUBE_LINKS_UNTEN 38   

#define NUM_LEDS_HAUBE_RECHTS 77  // 38 unten, 39 oben
#define NUM_LEDS_HAUBE_RECHTS_OBEN 39   
#define NUM_LEDS_HAUBE_RECHTS_UNTEN 38 

#define NUM_LEDS_HECK_LINKS 94    // 39 unten, 12 finne unten, 5 finne oben, 38 oben
#define NUM_LEDS_HECK_LINKS_OBEN 38   
#define NUM_LEDS_FINNE_LINKS_OBEN 5   
#define NUM_LEDS_FINNE_LINKS_UNTEN 12   
#define NUM_LEDS_HECK_LINKS_UNTEN 39   

#define NUM_LEDS_HECK_RECHTS 94   // 39 unten, 12 finne unten, 5 finne oben, 38 oben
#define NUM_LEDS_HECK_RECHTS_OBEN 38   
#define NUM_LEDS_FINNE_RECHTS_OBEN 5   
#define NUM_LEDS_FINNE_RECHTS_UNTEN 12   
#define NUM_LEDS_HECK_RECHTS_UNTEN 39  

#define NUM_LEDS_HECK_UNTEN 39    //

#define NUM_LEDS_KUFE_LINKS 28
#define NUM_LEDS_KUFE_RECHTS 28



CRGB ledsHaubeLinks[NUM_LEDS_HAUBE_LINKS];
CRGB ledsHaubeRechts[NUM_LEDS_HAUBE_RECHTS];
CRGB ledsHeckLinks[NUM_LEDS_HECK_LINKS];
CRGB ledsHeckRechts[NUM_LEDS_HECK_RECHTS];

CRGB ledsHeckUnten[NUM_LEDS_HECK_UNTEN];

CRGB ledsKufeLinks[NUM_LEDS_KUFE_LINKS];
CRGB ledsKufeRechts[NUM_LEDS_KUFE_RECHTS];


LEDStrip stripHaubeLinksOben(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, true, NUM_LEDS_HAUBE_LINKS_UNTEN, 0);
LEDStrip stripHaubeLinksUnten(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, false, 0, NUM_LEDS_HAUBE_LINKS_OBEN);
LEDStrip stripHeckLinksUnten(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, 0, NUM_LEDS_HECK_LINKS_OBEN + NUM_LEDS_FINNE_LINKS_UNTEN + NUM_LEDS_FINNE_LINKS_OBEN);
LEDStrip stripHeckLinksOben(ledsHeckLinks, NUM_LEDS_HECK_LINKS, false, NUM_LEDS_HECK_LINKS_UNTEN + NUM_LEDS_FINNE_LINKS_UNTEN + NUM_LEDS_FINNE_LINKS_OBEN, 0);
LEDStrip stripLinksFinne(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, NUM_LEDS_HECK_LINKS_UNTEN, NUM_LEDS_HECK_LINKS_OBEN);

LEDStrip stripHeckUnten(ledsHeckUnten, NUM_LEDS_HECK_UNTEN, true);
LEDStrip stripKufeLinks(ledsKufeLinks, NUM_LEDS_KUFE_LINKS, false);
LEDStrip stripKufeRechts(ledsKufeRechts, NUM_LEDS_KUFE_RECHTS, false);


LEDStrip stripHaubeRechtsOben(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, true, NUM_LEDS_HAUBE_RECHTS_UNTEN, 0);
LEDStrip stripHaubeRechtsOben2(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, false, NUM_LEDS_HAUBE_RECHTS_UNTEN, 0);
LEDStrip stripHaubeRechtsUnten(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, false, 0, NUM_LEDS_HAUBE_RECHTS_OBEN);
LEDStrip stripHeckRechtsUnten(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, true, 0, NUM_LEDS_HECK_RECHTS_OBEN + NUM_LEDS_FINNE_RECHTS_UNTEN + NUM_LEDS_FINNE_RECHTS_OBEN);
LEDStrip stripHeckRechtsOben(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, false, NUM_LEDS_HECK_RECHTS_UNTEN + NUM_LEDS_FINNE_RECHTS_UNTEN + NUM_LEDS_FINNE_RECHTS_OBEN, 0);
LEDStrip stripRechtsFinne(ledsHeckRechts, NUM_LEDS_HECK_RECHTS, true, NUM_LEDS_HECK_RECHTS_UNTEN, NUM_LEDS_HECK_RECHTS_OBEN);

LEDStrip stripHaubeLinksOben2(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, false, NUM_LEDS_HAUBE_LINKS_UNTEN, 0);
LEDStrip stripHeckLinksOben2(ledsHeckLinks, NUM_LEDS_HECK_LINKS, false, 0, NUM_LEDS_HECK_LINKS_OBEN + NUM_LEDS_FINNE_LINKS_UNTEN + NUM_LEDS_FINNE_LINKS_OBEN);

LEDStrip stripHaubeRechtsUnten2(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS, true, 0, NUM_LEDS_HAUBE_RECHTS_OBEN);
LEDStrip stripHaubeLinksUnten2(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS, true, 0, NUM_LEDS_HAUBE_LINKS_OBEN);
LEDStrip stripHeckLinksUnten2(ledsHeckLinks, NUM_LEDS_HECK_LINKS, true, NUM_LEDS_HECK_LINKS_UNTEN + NUM_LEDS_FINNE_LINKS_UNTEN + NUM_LEDS_FINNE_LINKS_OBEN, 0);



LEDVirtualStrip vStripLinksOben;
LEDVirtualStrip vStripLinksOben2;
LEDVirtualStrip vStripLinksUnten;
LEDVirtualStrip vStripLinksFinne;

LEDVirtualStrip vStripRechtsOben;
LEDVirtualStrip vStripRechtsOben2;
LEDVirtualStrip vStripRechtsUnten;
LEDVirtualStrip vStripRechtsFinne;

LEDVirtualStrip vStripObenRundrum;
LEDVirtualStrip vStripUntenRundrum;


LEDVirtualStrip vStripUntenLang;
LEDVirtualStrip vStripUntenKurz;
LEDVirtualStrip vStripUntenGesamt;


void setupIndividual()
{
  //set up the LED hardware
  FastLED.addLeds<CHIPSET, PIN_HAUBE_LINKS, COLOR_ORDER>(ledsHaubeLinks, NUM_LEDS_HAUBE_LINKS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HAUBE_RECHTS, COLOR_ORDER>(ledsHaubeRechts, NUM_LEDS_HAUBE_RECHTS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_LINKS, COLOR_ORDER>(ledsHeckLinks, NUM_LEDS_HECK_LINKS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_RECHTS, COLOR_ORDER>(ledsHeckRechts, NUM_LEDS_HECK_RECHTS).setCorrection( TypicalLEDStrip );
  
  FastLED.addLeds<CHIPSET, PIN_HECK_UNTEN, COLOR_ORDER>(ledsHeckUnten, NUM_LEDS_HECK_UNTEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_KUFE_LINKS, COLOR_ORDER>(ledsKufeLinks, NUM_LEDS_KUFE_LINKS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_KUFE_RECHTS, COLOR_ORDER>(ledsKufeRechts, NUM_LEDS_KUFE_RECHTS).setCorrection( TypicalLEDStrip );
  
  vStripLinksOben.addLEDStrip(stripHaubeLinksOben);
  vStripLinksOben.addLEDStrip(stripHeckLinksOben); 

  vStripLinksOben2.addLEDStrip(stripHaubeLinksOben2);
  vStripLinksOben2.addLEDStrip(stripHeckLinksOben); 

  vStripLinksFinne.addLEDStrip(stripLinksFinne);

  vStripLinksUnten.addLEDStrip(stripHaubeLinksUnten);
  vStripLinksUnten.addLEDStrip(stripHeckLinksUnten);

  vStripRechtsOben.addLEDStrip(stripHaubeRechtsOben);
  vStripRechtsOben.addLEDStrip(stripHeckRechtsOben);  

  vStripRechtsOben2.addLEDStrip(stripHaubeRechtsOben2);
  vStripRechtsOben2.addLEDStrip(stripHeckRechtsOben);  

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

  vStripUntenLang.addLEDStrip(stripKufeLinks);
  vStripUntenLang.addLEDStrip(stripHeckUnten);
  vStripUntenKurz.addLEDStrip(stripKufeRechts);

  vStripUntenGesamt.addLEDStrip(stripKufeLinks);
  vStripUntenGesamt.addLEDStrip(stripHeckUnten);
  vStripUntenGesamt.addLEDStrip(stripKufeRechts);



  set_max_power_in_volts_and_milliamps(5, 14000);
}

void setRcChannelValues()
{
  rcChannelSteps.setValue(map(Nightflight.currentChannelData, 750, 2250, 0, 12));
  rcChannelSteps2.setValue(Nightflight.currentChannelData2);
  rcChannelSteps3.setValue(Nightflight.currentChannelData3); //Pitch channel
}
