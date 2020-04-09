#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowPosition(0, 0);
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofBackground(54, 54, 54);
    
    inputStream.printDeviceList();
    
    bufferSize = 256;
    sampleRate = 44100;
    inputBuffer.resize(bufferSize);
    numChannels = 1;

    
    mic0.assign(bufferSize, 0.0);
    mic1.assign(bufferSize, 0.0);
    
    bufferCounter    = 0;

    decibel.smoother = new paramSmoother(200, ofGetFrameRate());
    peak.smoother = new paramSmoother(100, ofGetFrameRate());
    
    auto devices = inputStream.getDeviceList();
    

    
    vector<string> scOptions;
    
    if(devices.size() > 0) {
        for (int i=0; i<devices.size(); i++)
        {
            //string device1 = ofSplitString(ofToString(devices[i]), ":")[2];
            //string device2 = ofSplitString(ofToString(devices[i]), ":")[3];
            //string device3 = ofSplitString(ofToString(devices[i]), ":")[4];
            string device = ofSplitString(ofToString(devices[i]), "]")[1];
            //string device = device1 + device2 + device3;
            scOptions.push_back(ofToString(device) + "]");
        }
    } else {
        
    }
    
    vector<string> srOptions { "44100", "48000", "88200", "96000" };
    
    // GUI
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    gui->addDropdown("SELECT INPUT SOUNDCARD", scOptions);
    m1 = gui->addMatrix("Channel", 10, true);
    gui->addDropdown("Sample Rate", srOptions);
    s1 = gui->addSlider("input audio", 0.0f, 1.0f);
    gui->addBreak()->setHeight(50);
    gui->addDropdown("SELECT OUTPUT SOUNDCARD", scOptions);
    s2 = gui->addSlider("output audio", 0.0f, 1.0f);
    
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    gui->onSliderEvent(this, &ofApp::onGuiSliderEvent);
    gui->onMatrixEvent(this, &ofApp::onMatrixEvent);
    
    gui->addHeader(":: SOUNDCARD REROUTER FOR OBS [LYS.//] ::");
    gui->addFooter();
    
    s1->setEnabled(false);
    s2->setEnabled(false);
    s1->setValue(0.0f);
    s2->setValue(0.0f);
    m1->setRadioMode(true);
    
    ofColor green(0, 200, 0);
    s1->setStripeColor(green);
    s2->setStripeColor(green);
    
    s1->mSliderFill = green;
    s2->mSliderFill = green;
    
    gui->setWidth(745);
}

//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    auto devices = inputStream.getDeviceList();
    switch(e.parent)
    {
        case 1:
            audioInDeviceID = e.child;
            inputStream.stop();
            //inputStream.close();
            ofSoundStreamClose();
            cout << "Audio Input Setting: " << e.target->getLabel() << " Selected" << endl;
            cout << "Audio Input Setting: " << e.child << " Selected" << endl;
            settings.setInDevice(devices[audioInDeviceID]);
            settings.setInListener(this);
            settings.sampleRate = sampleRate;
            settings.numInputChannels = numChannels;
            settings.numOutputChannels = 2;
            settings.bufferSize = bufferSize;
            inputStream.setup(settings);
            //inputStream.start();
            ofSoundStreamStart();
            break;
            
        case 3:
            inputStream.stop();
            //inputStream.close();
            ofSoundStreamClose();
            sampleRate = ofToFloat(e.target->getLabel());
            settings.sampleRate = sampleRate;
            inputStream.setup(settings);
            //inputStream.start();
            ofSoundStreamStart();
            cout << "Samplerate Setting: " << sampleRate << " Selected" << endl;
            break;
            
        case 6:
            audioOutDeviceID = e.child;
            cout << "Audio Output Setting: " << e.target->getLabel() << " Selected" << endl;
            cout << "Audio Output Setting: " << e.child << " Selected" << endl;
            inputStream.stop();
            //inputStream.close();
            ofSoundStreamClose();
            settings.setOutDevice(devices[audioOutDeviceID]);
            settings.setOutListener(this);
            settings.sampleRate = sampleRate;
            settings.numInputChannels = numChannels;
            settings.numOutputChannels = 2;
            settings.bufferSize = bufferSize;
            inputStream.setup(settings);
            //inputStream.start();
            ofSoundStreamStart();
            break;
            
        default:
            break;
    }
}


//--------------------------------------------------------------
void ofApp::onGuiSliderEvent(ofxDatGuiSliderEvent e)
{

}

//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowShape(745, gui->getHeight());
    decibel.currentValue = decibel.smoother->process(decibel.targetValue);

    peak.currentValue = peak.smoother->process(peak.targetValue);
    if ( peak.currentValue >= 0.8 ) {
        s1->mSliderFill = ofColor::red;
        s2->mSliderFill = ofColor::red;
    } else {
        s1->mSliderFill = ofColor::green;
        s2->mSliderFill = ofColor::green;
    }
    
}

//--------------------------------------------------------------
void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    numChannels = e.child + 1;
    inputStream.stop();
    ofSoundStreamClose();
    settings.numInputChannels = numChannels;
    inputStream.setup(settings);
    ofSoundStreamStart();
    cout << "Channel Selected: " << numChannels << endl;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    //s1->setValue(20 * log10(decibel.currentValue));
    //s2->setValue(20 * log10(decibel.currentValue));
    
    s1->setValue(peak.currentValue);
    s2->setValue(peak.currentValue);
    
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input)
{
    float curVol = 0.0;
    // samples are "interleaved"
    int numCounted = 0;
    for (size_t i = 0; i < input.getNumFrames(); i++)
    {
        mic0[i] = input[i*numChannels];
        curVol += mic0[i] * mic0[i];
        //curVol += right[i] * right[i];
        numCounted+=1;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    decibel.targetValue = curVol;
    
    bufferCounter++;
    float sum = 0;
    for (int i = 0; i < bufferSize; i++)
    {
        inputBuffer[i] = input[i*numChannels];
        //inputBuffer[i*nChannels + 1] = input[i];
        if ( fabs(input[i]) >= peak.targetValue ) peak.targetValue = input[i];
    }

    if (bufferCounter % 10 == 0) peak.targetValue = 0;
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer)
{

    for (int i =0; i < bufferSize; i++)
    {
        float inputSample;
        float currentSample;
        
        inputSample = inputBuffer[i];
        currentSample = inputSample;
        
        buffer[i*buffer.getNumChannels() + 0] = currentSample;
        buffer[i*buffer.getNumChannels() + 1] = currentSample;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
void ofApp::exit()
{
    ofSoundStreamClose();
}
