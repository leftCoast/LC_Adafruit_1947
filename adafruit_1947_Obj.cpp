#include "adafruit_1947_Obj.h"



// **********************************************************
//								maskableILI9341
//	Adafruit_ILI9341 with the ability to have a mask added.	
// **********************************************************

mask* gMask = NULL;

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
//								adafruit_1947_Obj	
// **********************************************************


void swap(int16_t* a, int16_t* b) {

	int16_t t;
	
	t = *a;
	*a = *b;
	*b = t;
}


adafruit_1947_Obj::adafruit_1947_Obj(void)
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


adafruit_1947_Obj::adafruit_1947_Obj(byte inCS,byte inRst)
  :displayObj(true,true,true,true,false) {

	cTS			= NULL;
	theTFT		= NULL;
	cs				= inCS;
	rst			= inRst;
	lastTouch	= false;
	touchNum		= 0;
}


adafruit_1947_Obj::~adafruit_1947_Obj(void) {

	if (theTFT!=NULL) {
		delete theTFT;
		theTFT = NULL;
	}
	if (cTS!=NULL) {
		delete cTS;
		cTS = NULL;
	}
}

//rect cRect;

bool adafruit_1947_Obj::dispObjBegin(void) { 

  //theTFT = new Adafruit_ILI9341(cs,LC_DC,rst);
  //theTFT = new ILI9341_t3(cs,LC_DC,rst);
  theTFT = new maskableILI9341(cs,rst);
  if (theTFT!=NULL) {
    cTS = new Adafruit_FT6206();
    if (cTS!=NULL) {
    	theTFT->begin();										// Doesn't have fail mode here.
    	if (cTS->begin(40)) {								// 40 comes from example code.
    		return true;
    	}
    }
  }
  if (theTFT!=NULL) {
	 delete theTFT;
	 theTFT = NULL;
  }
  if (cTS!=NULL) {
    delete cTS;
    cTS = NULL;
  }
  return false;
}

int adafruit_1947_Obj::width(void)																							{ return theTFT->width(); }
int adafruit_1947_Obj::height(void)																							{ return theTFT->height(); }
void adafruit_1947_Obj::startWrite(void)																					{ theTFT->startWrite(); }
void adafruit_1947_Obj::endWrite(void)																						{ theTFT->endWrite(); }
void adafruit_1947_Obj::setRotation(byte inRotation)																	{ theTFT->setRotation(inRotation); }
void adafruit_1947_Obj::setTextColor(colorObj* inColor)																{ theTFT->setTextColor(inColor->getColor16()); }
void adafruit_1947_Obj::setTextColor(colorObj* tColor,colorObj* bColor) 										{ theTFT->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_1947_Obj::setTextSize(byte inSize)																		{ theTFT->setTextSize(inSize); }
void adafruit_1947_Obj::setTextWrap(boolean wrap)																		{ theTFT->setTextWrap(wrap); }
void adafruit_1947_Obj::setFont(const GFXfont* font)																	{ theTFT->setFont(font); }
//void adafruit_1947_Obj::setFont(const GFXfont* font)																	{ theTFT->setFontAdafruit(); }
void adafruit_1947_Obj::setCursor(int x,int y)																			{ theTFT->setCursor(gX(x),gY(y)); }
void adafruit_1947_Obj::drawText(const char* inText)																	{ theTFT->print(inText); }
void adafruit_1947_Obj::fillScreen(colorObj* inColor)																	{ theTFT->fillScreen(inColor->getColor16()); }
void adafruit_1947_Obj::fillRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->fillRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void adafruit_1947_Obj::drawRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->drawRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void adafruit_1947_Obj::fillRect(rect* inRect,colorObj* inColor)													{ theTFT->fillRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_1947_Obj::drawRect(rect* inRect,colorObj* inColor)													{ theTFT->drawRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_1947_Obj::fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_1947_Obj::drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_1947_Obj::drawCircle(int x,int y,int diam, colorObj* inColor)									{ drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_1947_Obj::drawCircleHelper(int x,int y,int rad,byte corner,colorObj* inColor)				{ theTFT->drawCircleHelper(gX(x),gY(y),rad,corner,inColor->getColor16()); }
void adafruit_1947_Obj::fillCircle(int x,int y,int diam, colorObj* inColor)									{ fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_1947_Obj::drawTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor) 					{ theTFT->drawTriangle(gX(pt0->x),gX(pt0->y),gX(pt1->x),gX(pt1->y),gX(pt2->x),gX(pt2->y),inColor->getColor16()); }
void adafruit_1947_Obj::fillTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor)					{ theTFT->fillTriangle(gX(pt0->x),gX(pt0->y),gX(pt1->x),gX(pt1->y),gX(pt2->x),gX(pt2->y),inColor->getColor16()); }
void adafruit_1947_Obj::drawVLine(int x,int y,int height,colorObj* inColor)									{ theTFT->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void adafruit_1947_Obj::drawHLine(int x,int y,int width,colorObj* inColor)										{ theTFT->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void adafruit_1947_Obj::drawLine(int x,int y,int x2,int y2,colorObj* inColor)									{ theTFT->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
 
  
void adafruit_1947_Obj::drawPixel(int x,int y,colorObj* pColor) { theTFT->drawPixel(gX(x),gY(y),pColor->getColor16()); }
	 
point adafruit_1947_Obj::getPoint(void) {

	TS_Point	adaPoint;
	point		lcPoint;

	adaPoint = cTS->getPoint();								// Grab a point from the hardware.
	if (!adaPoint.z) {											// If the z value is zero..
		return lastTouchPt;										// Its a bogus touch! Return the last point.
	}																	//
	switch (theTFT->getRotation()) {							// For each type of rotation..
		case INV_PORTRAIT:										// INV_PORTRAIT is what we get by default.
		break;														// And we waltz off.
		case INV_LANDSCAPE:										// INV_LANDSCAPE..
			swap(&(adaPoint.x),&(adaPoint.y));				// Swap the x & y of the inputed point.
			adaPoint.x = adaPoint.x;							// What was I thinking here? replace value with itself?
			adaPoint.y = theTFT->height() - adaPoint.y;	// Looks like we swap around the y axis.
		break;														// Time to go. Better test this later, looks odd to me.
		case PORTRAIT:												// PORTRAIT I know this one works. I use it every damn day.
			adaPoint.x = theTFT->width() - adaPoint.x;	// Flip around the x axis.
			adaPoint.y = theTFT->height() - adaPoint.y;	// Flip around the y axis.
		break;														// And go!
		case LANDSCAPE:											// LANDSCAPE Another one I need to recheck. Its got some weird stuff in there too!
			swap(&(adaPoint.x),&(adaPoint.y));				// Swap the x & y of the inputed point.
			adaPoint.x = theTFT->width() - adaPoint.x;	// Flip around the new x axis.
			adaPoint.y = adaPoint.y;							// And I did it again here.. What was I thinking?
		break;														// Pack your bags and move on!
	}																	//
	lcPoint.x = adaPoint.x;										// Load the resulting x value into our lcPoint.		
	lcPoint.y = adaPoint.y;										// Load the resulting y value into our lcPoint.
	lastTouchPt = lcPoint;										// Save it for later. (We cover bogus values with this.) I say "we" like anyone else is ever here..
	return lcPoint;												// And return our result!
}


bool adafruit_1947_Obj::touched(void) { return cTS->touched(); }

