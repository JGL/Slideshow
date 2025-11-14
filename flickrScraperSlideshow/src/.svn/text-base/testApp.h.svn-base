#ifndef _TEST_APP
#define _TEST_APP

// flickrScraperSlideShow
// An oF app that scrapes flickr.com for a certain keyword, and then displays up to 100 of them in a slideshow style.
// System design and assembly by Joel Gethin Lewis
// http://www.joelgethinlewis.com

// XML query and manipulation of Flickr API by bbalantine, jesús gollonet bailén
// http://www.openframeworks.cc/forum/viewtopic.php?p=3578#3578

// slideshow logic based upon work by prisonerjohn, memo akten, theo watson, zachary lieberman, otherside, arturo
// http://www.openframeworks.cc/forum/viewtopic.php?t=816

// thread and scraping, SimpleWebScraper by jesús gollonet bailén, zachary lieberman, arturo
// http://www.openframeworks.cc/forum/viewtopic.php?p=6715

// based in turn on work on webImageLoader by bballantine, theo watson, zachary lieberman, mantissa, Matt Parker
// http://www.openframeworks.cc/forum/viewtopic.php?t=562

// all thanks to openFrameworks
// http://www.openFrameworks.cc
// and especially Zachary Lieberman and Theo Watson

#include "ofMain.h"
#include "ofAddons.h"

#include "SimpleWebScraper.h"
#include "ofMemoryImage.h"

#include "Poco/URIStreamOpener.h" 
#include "Poco/StreamCopier.h" 
#include "Poco/Path.h" 
#include "Poco/URI.h" 
#include "Poco/Exception.h" 
#include "Poco/Net/HTTPStreamFactory.h" 
#include "Poco/XML/XMLString.h" 
#include "Poco/DOM/DOMParser.h" 
#include "Poco/DOM/Document.h" 
#include "Poco/DOM/NodeIterator.h" 
#include "Poco/DOM/NodeFilter.h" 
#include "Poco/DOM/NamedNodeMap.h" 
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>

using Poco::URIStreamOpener; 
using Poco::StreamCopier; 
using Poco::Path; 
using Poco::URI; 
using Poco::Exception; 
using Poco::Net::HTTPStreamFactory; 
using Poco::XML::XMLString; 
using Poco::XML::DOMParser; 
using Poco::XML::Document; 
using Poco::XML::NodeIterator; 
using Poco::XML::NodeFilter; 
using Poco::XML::Node; 
using Poco::XML::NamedNodeMap; 

static const std::string API_KEY = "GETONEYOURSELF";
//http://www.flickr.com/services/api/flickr.photos.search.html
// is the spec for searching, and how to get the API key
//static const std::string REST_URL = "http://www.flickr.com/services/rest/?method=flickr.photos.getRecent&format=rest"; 
//total string is something like http://www.flickr.com/services/rest/?method=flickr.photos.search&format=rest&text=SEARCH KEYWORD&api_key=GET YOUR OWN API KEY&page=1
static const std::string REST_URL = "http://www.flickr.com/services/rest/?method=flickr.photos.search&format=rest"; 
const std::string SEARCH_KEYWORD = "openFrameworks"; //spaces are allowed!

#define SLIDESHOW_SCREEN_WIDTH 1024
#define SLIDESHOW_SCREEN_HEIGHT 768

class RemoteImageWithStatus
{
public:
	//ofTexture theTexture;
	ofImage theImage;
	bool isLoaded;
	string theUrl;
};

class testApp : public ofSimpleApp{

	public:

	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	
	std::string makeFlickrUrl(Node* p_farm, Node* p_server, Node* p_id, Node* p_secret); //make a flickr URL
	std::vector<std::string> getFlickrImageUrls(const std::string& rest_url, const std::string& api_key, std::string resultsPage, const std::string& search_keyword); 
	//using the supplied variables as keywords
	std::string resultsPageAsString();
	void addNewURLs(); //scape the URLs of images from flickr.com via REST and put them into a vector

	void onThreadedStreamReceived(const void* pSender, StreamEventArgs & args_); 	
	
	void startGettingImage(int vecIndex); //start getting the data from the url described at the index of the webImages vector
	
	int getNextImageIndex();
	int getCurrentImageIndex();
	ofImage * getNextImage();
	
private:
	
	ofImage nLogoImage;
	
	int flickrResultsPage;
	
	std::vector<RemoteImageWithStatus> webImages;
	
	SimpleWebScraper sws;

	ofMemoryImage	threadedMemImage;
	StreamEventArgs * args;
	unsigned char * pixArr; //temporary memory store for newly loaded web image data
	
	bool received; //have we recieved the latest loading image yet?
	
	ofImage *						currentImage; 
	bool							displayNextImage;
	int								currentImageIndex;
	
	int								currentLoadingImageIndex; //the image we are currently loading
};

#endif