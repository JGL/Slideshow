/*
 *  ofMemoryImage.h
 *  An extension of ofImage to ease webscraping by construction from char * and size
 *
 *  Created by jesús gollonet bailén on 12/09/08.
 * used by Joel Gethin Lewis 3/11/2008
 *
 */
#ifndef _OF_MEMORY_IMAGE
#define _OF_MEMORY_IMAGE

#include "ofImage.h"

class   ofMemoryImage : public ofImage { 
	
	public:
		
		void loadFromData(const unsigned char * datasource, int len); 
	
	private :
	
		bool loadFromDataIntoPixels(const unsigned char * datasource, int len, ofPixels &pix); 

};

#endif
