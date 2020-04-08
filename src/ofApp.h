#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

class paramSmoother
{
    //from Will Pirkle
    public:
        paramSmoother(float smoothingTimeMS, int sampleRate)
        {
            a = exp(-TWO_PI / (smoothingTimeMS * 0.001 * sampleRate));
            b = 1.0 - a;
            z = 0.0;
        }
    
    
        inline float process(float input)
        {
            z = (input * b) + (z * a);
            return z;
        }
    
    private:
        float a;
        float b;
        float z;
};

struct smoothValue
{
    smoothValue() = default;
    smoothValue(float current, float target, float timeMS)
    {
        currentValue = current;
        targetValue = target;
        smoother = new paramSmoother(timeMS, 44100);
    }
    float targetValue;
    float currentValue;
    paramSmoother* smoother;
    
    void process()
    {
        currentValue = smoother->process(targetValue);
    }
};

class ofApp : public ofBaseApp
{
    
    public:
    
        void setup();
        void update();
        void draw();
    
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    
        void audioIn(ofSoundBuffer & input);
    
        vector <float> mic0;
        vector <float> mic1;
    
    
        int     bufferCounter;
        int     drawCounter;
    
        float smoothedVol;
        float scaledVol;
        float smoothedPeak;
        float scaledPeak;

        ofSoundStream inputStream, outputStream;
        ofSoundStreamSettings settings;
    
    
        int sampleRate;
        int bufferSize;
        int numChannels;
    
        void audioOut(ofSoundBuffer & buffer);
        vector<float> inputBuffer;
    
        ofxDatGui* gui;
        ofxDatGuiSlider* s1;
        ofxDatGuiSlider* s2;
        ofxDatGuiMatrix* m1;

    
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
        void onGuiSliderEvent(ofxDatGuiSliderEvent e);
        void onMatrixEvent(ofxDatGuiMatrixEvent e);
 

        void exit();
        int audioInDeviceID;
        int audioOutDeviceID;

        smoothValue peak;
        smoothValue decibel;
};
