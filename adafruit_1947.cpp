#include <adafruit_1947.h>


// **********************************************************
//								maskableILI9341
//	Adafruit_ILI9341 with the ability to have a mask added.	
// **********************************************************


maskableILI9341::maskableILI9341(int cs, int rst)
	: Adafruit_ILI9341(cs,LC_DC,rst) { }

	
maskableILI9341::~maskableILI9341(void) {  }
	
	
void maskableILI9341::drawPixel(int16_t x, int16_t y, uint16_t color) {

	if (gMask) {
		if (gMask->checkPixel(x,y)) {
			Adafruit_ILI9341::drawPixel(x,y,color);
		}
	} else {
		Adafruit_ILI9341::drawPixel(x,y,color);
	}
}


void maskableILI9341::writePixel(int16_t x, int16_t y, uint16_t color) {

	if (gMask) {
		if (gMask->checkPixel(x,y)) {
			Adafruit_ILI9341::writePixel(x,y,color);
		}
	} else {
		Adafruit_ILI9341::writePixel(x,y,color);
	}
}


void maskableILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

	int	lastX;
	int	lastY;
	
	if (gMask) {
		switch(gMask->checkRect(x,y,w,h)) {
			case unMasked			: Adafruit_ILI9341::fillRect(x,y,w,h,color); break;
			case totalMasked		: return;
			case partialMasked	:
				lastX = x + w;
				lastY = y + h;
				for(int i=y;i<lastY;i++) {
					for(int j=x;j<=lastX;j++) {
						drawPixel(j,i,color);
					}
				}
			break;
		}
	} else {
		Adafruit_ILI9341::fillRect(x,y,w,h,color);
	}
}

// **********************************************************
//								adafruit_1947	
// **********************************************************



adafruit_1947::adafruit_1947(void)
	:displayObj(true,true,true,true,false) {
    
	cTS				= NULL;
	theTFT			= NULL;
	cs					= ADA_1947_SHIELD_CS;
	rst				= ADA_1947_SHIELD_RST;
	lastTouch		= false;
	touchNum			= 0;
	lastTouchPt.x	= 0;
	lastTouchPt.y	= 0;
}


adafruit_1947::adafruit_1947(byte inCS,byte inRst)
  :displayObj(true,true,true,true,false) {

	cTS			= NULL;
	theTFT		= NULL;
	cs				= inCS;
	rst			= inRst;
	lastTouch	= false;
	touchNum		= 0;
}


adafruit_1947::~adafruit_1947(void) {

	if (theTFT!=NULL) {
		delete theTFT;
		theTFT = NULL;
	}
	if (cTS!=NULL) {
		delete cTS;
		cTS = NULL;
	}
}


bool adafruit_1947::begin(void) { 

  theTFT = new maskableILI9341(cs,rst);	// Have a go at allocating the display object.
  if (theTFT!=NULL) {							// If we got one?
    cTS = new Adafruit_FT6206();				// Have a go at allocating a touch object.
    if (cTS!=NULL) {								// We got both now?
    	theTFT->begin();							// Fire up the TFT. Doesn't have fail mode here.
    	if (cTS->begin(40)) {					// 40 comes from example code.
    		return true;							// If everything checks out? We exit with a success.
    	}												//
    }													//
  }													// At this point something went wrong. Recycle everything.
  if (theTFT!=NULL) {							// Have a TFT thing?
	 delete theTFT;								// Delete it.
	 theTFT = NULL;								// Note it.
  }													// 
  if (cTS!=NULL) {								// Have a touch object?
    delete cTS;									// Delete it.
    cTS = NULL;									// Note it.
  }													//
  return false;									// Return false, we failed.
}


int adafruit_1947::width(void)																							{ return theTFT->width(); }
int adafruit_1947::height(void)																							{ return theTFT->height(); }
void adafruit_1947::startWrite(void)																					{ theTFT->startWrite(); }
void adafruit_1947::endWrite(void)																						{ theTFT->endWrite(); }
void adafruit_1947::setRotation(byte inRotation)																	{ theTFT->setRotation(inRotation); }
void adafruit_1947::setTextColor(colorObj* inColor)																{ theTFT->setTextColor(inColor->getColor16()); }
void adafruit_1947::setTextColor(colorObj* tColor,colorObj* bColor) 											{ theTFT->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_1947::setTextSize(byte inSize)																			{ theTFT->setTextSize(inSize); }
void adafruit_1947::setTextWrap(boolean wrap)																		{ theTFT->setTextWrap(wrap); }
void adafruit_1947::setFont(const GFXfont* font)																	{ theTFT->setFont(font); }
void adafruit_1947::setCursor(int x,int y)																			{ theTFT->setCursor(gX(x),gY(y)); }
void adafruit_1947::drawText(const char* inText)																	{ theTFT->print(inText); }
void adafruit_1947::fillScreen(colorObj* inColor)																	{ theTFT->fillScreen(inColor->getColor16()); }
void adafruit_1947::fillRect(int x,int y,int width,int height,colorObj* inColor)							{ theTFT->fillRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void adafruit_1947::drawRect(int x,int y,int width,int height,colorObj* inColor)							{ theTFT->drawRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void adafruit_1947::fillRect(rect* inRect,colorObj* inColor)													{ theTFT->fillRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_1947::drawRect(rect* inRect,colorObj* inColor)													{ theTFT->drawRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_1947::fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_1947::drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_1947::drawCircle(int x,int y,int diam, colorObj* inColor)										{ drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_1947::drawCircleHelper(int x,int y,int rad,byte corner,colorObj* inColor)				{ theTFT->drawCircleHelper(gX(x),gY(y),rad,corner,inColor->getColor16()); }
void adafruit_1947::fillCircle(int x,int y,int diam, colorObj* inColor)										{ fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_1947::drawTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor) 					{ theTFT->drawTriangle(gP(pt0).x,gP(pt0).y,gP(pt1).x,gP(pt1).y,gP(pt2).x,gP(pt2).y,inColor->getColor16()); }
void adafruit_1947::fillTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor)					{ theTFT->fillTriangle(gP(pt0).x,gP(pt0).y,gP(pt1).x,gP(pt1).y,gP(pt2).x,gP(pt2).y,inColor->getColor16()); }
void adafruit_1947::drawVLine(int x,int y,int height,colorObj* inColor)										{ theTFT->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void adafruit_1947::drawHLine(int x,int y,int width,colorObj* inColor)										{ theTFT->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void adafruit_1947::drawLine(int x,int y,int x2,int y2,colorObj* inColor)									{ theTFT->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
void adafruit_1947::drawPixel(int x,int y,colorObj* inColor)													{ theTFT->drawPixel(gX(x),gY(y),inColor->getColor16()); }


point adafruit_1947::getPoint(void) {

	TS_Point	adaPoint;
	point		lcPoint;

	adaPoint = cTS->getPoint();								// Grab a point from the hardware.
	if (!adaPoint.z) {											// If the z value is zero..
		return lastTouchPt;										// Its a bogus touch! Return the last point.
	}																	//
	switch (theTFT->getRotation()) {							// For each type of rotation..
		case INV_PORTRAIT:										// INV_PORTRAIT is what we get by default.
			lcPoint.x = adaPoint.x;								// Load the x value into our lcPoint.		
			lcPoint.y = adaPoint.y;								// Load the y value into our lcPoint.
		break;														// And we waltz off.
		case INV_LANDSCAPE:										// INV_LANDSCAPE..
			lcPoint.x = adaPoint.y;								// x gets old y.
			lcPoint.y = theTFT->height() - adaPoint.x;	// Load the new y flipped around the y axis.
		break;														// Time to go. Better test this later, looks odd to me.
		case PORTRAIT:												// PORTRAIT I know this one works. I use it every damn day.
			lcPoint.x = theTFT->width() - adaPoint.x;		// Flip around the x axis.
			lcPoint.y = theTFT->height() - adaPoint.y;	// Flip around the y axis.
		break;														// And go!
		case LANDSCAPE:											// LANDSCAPE Another one I need to recheck. Its got some weird stuff in there too!
			lcPoint.x = theTFT->width() - adaPoint.y;		// Flip around the new x axis.
			lcPoint.y = adaPoint.x;								// y gets old x.
		break;														// Pack your bags and move on!
	}																	//
	lastTouchPt = lcPoint;										// Save it for later. (We cover bogus values with this.) I say "we" like anyone else is ever here..
	return lcPoint;												// And return our result!
}


bool adafruit_1947::touched(void) { return cTS->touched(); }

