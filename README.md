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
or [Existential Audio: BlackHole](https://github.com/ExistentialAudio/BlackHole).

Download [OBSAudioRouter](https://github.com/lysdexic-audio/OBSAudioRouter/releases)

