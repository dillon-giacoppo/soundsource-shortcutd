# SoundSource Shortcut Trigger

SoundSource does not allow different EQ settings to be set for different 
outputs. It is possible to configure the current EQ settings with a macOS 
shortcut as described in [the manual](https://rogueamoeba.com/support/manuals/soundsource/?page=shortcuts).

This background daemon will trigger the shortcut automatically whenever the 
audio output is changed allowing the preferred EQ settings to immediately take 
effect.

## Install macOS

Generate the desired configuration with cmake, compile and install:

```sh
cmake -S . -B cmake-build-release 
cmake --build cmake-build-release --config Release
sudo cmake --install cmake-build-release --config Release
```

This installs `soundsource-shortcutd` to `/usr/local/bin` and
`foo.dill.soundsource-shortcutd.plist` to `~/Library/LaunchAgents`

## Configure

**Default Configuration**

Create a shortcut named `Headphone EQ`. For example:

![example shortcut](https://rogueamoeba.com/support/manuals/soundsource/images/shortcuts-example-headphoneeq@2x.png)

**Alternative Shortcut Name**

An alternative shortcut name can be set in `~/Library/LaunchAgents/foo.dill.soundsource-shortcutd.plist`.

Restart the service after modifying the plist with:

```shell
launchctl unload ~/Library/LaunchAgents/foo.dill.soundsource-shortcutd.plist
launchctl load ~/Library/LaunchAgents/foo.dill.soundsource-shortcutd.plist
```
