# OBSAudioRouter
A simple microphone + external audiodevices passthrough/rerouter for MacOS 

OBSAudioRouter passes through your analogue Mic channel on your external soundcard to OBS (24.0.6)

This is a workaround device for MacOS users of [OBS](https://github.com/obsproject/obs-studio) attempting to use external audiodevices (particularly firewire) where no audio will be retrieved by OBS.

> `23:19:17.098: coreaudio: device '2882 [4945]' initialized` <br>
> `23:19:17.516: [input_callback]:[device '2882 [4945]'] audio retrieval failed: -50` <br>
> `23:19:17.527: [input_callback]:[device '2882 [4945]'] audio retrieval failed: -50` <br>
> `23:19:17.539: [input_callback]:[device '2882 [4945]'] audio retrieval failed: -50` <br>
> `23:19:17.550: [input_callback]:[device '2882 [4945]'] audio retrieval failed: -50` <br>

## Instructions

First install either [Soundflower](https://github.com/mattingalls/Soundflower/releases/tag/2.0b2)
or [BlackHole](https://github.com/ExistentialAudio/BlackHole).

Download [OBSAudioRouter](https://github.com/lysdexic-audio/OBSAudioRouter/releases)

<img src="https://github.com/lysdexic-audio/OBSAudioRouter/blob/master/OBSAudioRouter.png" width="400">

 `1. Select your Input Soundcard` <br>
 `2. Select your Mic channel` <br>
 `3. Select your Sample rate` <br>
 `4. Select your Output Soundcard (Soundflower 2ch or Blackhole)` <br>
 `5. Select the same Output card as the Microphone/Auxillary input inside OBS settings` <br>
 
If you've made an aggregate device to get desktop/system audio bundled with your mic input - you can get all three channels by picking the highest channel in the channel selector. In my case, I route my system audio to channels 2 + 3, with my mic on channel 1 and select "3" in the channel selector on OBSAudioRouter.

<img src="https://github.com/lysdexic-audio/OBSAudioRouter/blob/master/OBSSettings.png" width="450">

Built with [openFrameworks 10.0](http://www.openframeworks.cc) and [ofxDatGui](https://github.com/braitsch/ofxDatGui)

