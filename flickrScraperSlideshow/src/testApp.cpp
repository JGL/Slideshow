#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	HTTPStreamFactory::registerFactory(); 
	
	cout << "Made with openFrameworks version :" << ofGetVersionInfo() << endl; 
	
	ofSetVerticalSync(true);
	
	sws.threadedStreamReady	+= Delegate<testApp, StreamEventArgs>(this, &testApp::onThreadedStreamReceived);
	
	threadedMemImage.setUseTexture(false); // we can't use a texture with this one
	received = false;
	
	flickrResultsPage = 1; //the first results page to start with
	
	addNewURLs(); //scape the URLs of images from flickr.com via REST and put them into a vector
	
	startGettingImage(0); //try to get the first image straight away
	
	currentImage = NULL; 
	displayNextImage = true;
	currentImageIndex = -1; //this will be incremented to 0 in the first update call as displayNextImage is true
	
	currentLoadingImageIndex = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	
	if (received) { //we have just recieved a new image from the web
		received = false;
		
		int pixSize = threadedMemImage.width*threadedMemImage.height*3;
		
		pixArr = new unsigned char[pixSize];
		memcpy(pixArr, threadedMemImage.getPixels(), pixSize);
		webImages[currentLoadingImageIndex].theImage.allocate(threadedMemImage.width, threadedMemImage.height, OF_IMAGE_COLOR); //setting type too
		webImages[currentLoadingImageIndex].theImage.setFromPixels(pixArr, threadedMemImage.width, threadedMemImage.height, OF_IMAGE_COLOR); //remembering to set correct type
		webImages[currentLoadingImageIndex].isLoaded = true; //it has now been loaded	
		webImages[currentLoadingImageIndex].theImage.resize(ofGetWidth(), ofGetHeight()); //full screen please
		
		//now start loading the next image
		
		int nextImageToLoadIndex = currentLoadingImageIndex + 1; // TODO fix this for when the vector is expanded to display more images
		if((nextImageToLoadIndex < webImages.size()) &&  (webImages[nextImageToLoadIndex].isLoaded == false))
		{
			startGettingImage(nextImageToLoadIndex);
			currentLoadingImageIndex = nextImageToLoadIndex;
			received = false; //just to make sure (-;
		}
		
	}
	
	if (ofGetFrameNum()%200 == 0) //TODO: fix this to be varible with Poco timers (i.e.- non frame based) change this command to change the rate of the slideshow
		displayNextImage=true; 
	
	if(displayNextImage)
	{ 
        ofImage * nextImage = getNextImage(); 
        
		if(nextImage!=NULL)
		{ 
			currentImage=nextImage; 
			displayNextImage=false;
			//cout << "Current Displayed image" <<  getCurrentImageIndex() << endl; 
			//cout << "Currently loading image" << currentLoadingImageIndex  << endl; 
        } 
	} 	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	// draw the image from flickr here
	if(webImages[getCurrentImageIndex()].isLoaded == true  && currentImage) //testing for the loaded flag on the image in question and for the existence of a pointer to it
		currentImage->draw(0,0);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	switch (key){
		case ' ':
			cout << "Spacebar pressed" << endl; 
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}	

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}

/** url looks like 
 * http://farm{farm-id}.static.flickr.com/{server-id}/{id}_{secret}.jpg
 * or 
 * http://farm{farm-id}.static.flickr.com/{server-id}/{id}_{secret}_[mstb].jpg 
 * or 
 * http://farm{farm-id}.static.flickr.com/{server-id}/{id}_{o-secret}_o.(jpg|gif|png) 
 * 
 * see http://www.flickr.com/services/api/misc.urls.html 
 */ 
std::string testApp::makeFlickrUrl( 
						  Node* p_farm, Node* p_server, Node* p_id, Node* p_secret) 
{ 
    if (p_farm && p_id && p_server && p_secret) { 
        return "http://farm" + p_farm->getNodeValue() + 
		".static.flickr.com/" + p_server->getNodeValue() + 
		"/" + p_id->getNodeValue() + 
		"_" + p_secret->getNodeValue() + ".jpg"; 
    } else { 
        return ""; 
    } 
} 


/* 
 * Take in a Flickr rest style api url that doesn't require 
 * authenticaiton and an api_key 
 */ 
std::vector<std::string> testApp::getFlickrImageUrls( 
													 const std::string& rest_url, const std::string& api_key, std::string resultsPage, const std::string& search_keyword) 
{ 
    // Get REST style xml as string from flickr 
    std::auto_ptr<std::istream> 
    pStr(URIStreamOpener::defaultOpener().open( rest_url +  "&api_key=" + api_key + "&page=" + resultsPage + "&text=" + search_keyword)); 
    std::string str; 
    StreamCopier::copyToString(*pStr.get(), str); 
	
    // Set up XML Parser 
    DOMParser parser; 
    Document* pDoc = parser.parseString(str); 
    NodeIterator it(pDoc, NodeFilter::SHOW_ELEMENT); 
    Node* pNode = it.nextNode(); 
    NamedNodeMap *nnm; 
    std::vector<std::string> urls; 
    std::string current_url; 
    while (pNode) { 
        nnm = pNode->attributes(); 
        current_url = makeFlickrUrl( 
									nnm->getNamedItem("farm"), 
									nnm->getNamedItem("server"), 
									nnm->getNamedItem("id"), 
									nnm->getNamedItem("secret")); 
        if (current_url.length() > 0) 
            urls.push_back(current_url); 
        pNode = it.nextNode(); 
    } 
    nnm->release(); 
    return urls; 
} 

std::string testApp::resultsPageAsString()
{
	std::ostringstream oss;
	oss << flickrResultsPage;
	
	return oss.str();	
}

void testApp::addNewURLs()
{
	std::vector<std::string> img_urls = getFlickrImageUrls(REST_URL, API_KEY, resultsPageAsString(), SEARCH_KEYWORD); //get the next 100 in TODO: change to a threaded get of xml and expand
																										//the looping logic 100 images for now...
	
	int startFrom = webImages.size();
	
	webImages.resize(webImages.size() + img_urls.size());
	
	for(int i = 0; i < webImages.size(); i++)
	{
		webImages[startFrom + i].theUrl = img_urls[i];
		webImages[startFrom + i].theImage.allocate(100,100, OF_IMAGE_COLOR);
		//webImages[startFrom + i].theTexture = NULL; // will be initialised and allocated when data available
		webImages[startFrom + i].isLoaded = false;
	}
	
}

void testApp::onThreadedStreamReceived(const void* pSender, StreamEventArgs & args_)
{
	
	cout << "threaded stream received. " << endl;
	args = &args_;
	threadedMemImage.loadFromData(args_.buff,args_.bytesToRead);
	received = true;
}

void testApp::startGettingImage(int vecIndex)
{
	cout << "The URL I have started to load is " << webImages[vecIndex].theUrl << endl;
	sws.getStreamThreaded(webImages[vecIndex].theUrl); //TODO check for loaded status?
}

int testApp::getNextImageIndex()
{
	int nextImageIndex = (currentImageIndex+1)%webImages.size();
	
	return nextImageIndex;
}

int testApp::getCurrentImageIndex()
{
	return currentImageIndex;
}

ofImage* testApp::getNextImage()
{
	int nextImageIndex = getNextImageIndex();
	
	if(webImages[nextImageIndex].isLoaded)
	{
		ofImage * nextImage = &webImages[nextImageIndex].theImage;
		currentImageIndex = nextImageIndex;
		return nextImage;
	}
	
	return NULL; //defaults to null
}