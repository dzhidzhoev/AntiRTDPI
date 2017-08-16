# AntiRTDPI
macOS kernel extension helping to neutralise Rostelecom's passive DPI. 
Simple and dirty.
Tested on macOS Sierra 10.12.6.

It is an IP Filtering Network Kernel Extension (NKE). It blocks packets being sent by Rostelecom' when you're trying to access website blocked in Russia. It uses simple and stupid search criteria based on beginning of such packets.

## Build
Just open AntiRTDPI.xcodeproj in Xcode and build it!

## Install
To use it you need to disable System Integrity Protection feature (do it at your own risk!):
1. Reboot your Mac
2. When your computer turns off hold down command+R to boot into Recovery System.
3. Now, you need to click on the Utilities menu and choose Terminal.
4. Up next, you will have to type ``` csrutil disable ``` and press return.
5. Close the Terminal app.
6. Restart your Mac.

Now, you should move AntiRTDPI.kext file to /Library/Extensions folder ***as a root*** - it is very important.
Just open Terminal and execute this command:
```
sudo cp -R <path to AntiRTDPI.kext> /Library/Extensions
```

## Usage
To load kernel extension execute this command:
```
sudo kextload /Library/Extensions/AntiRTDPI.kext
```

Note: if you see '/Library/Extensions/AntiRTDPI.kext failed to load - (libkern/kext) dependency resolution failure; check the system/kernel logs for errors or try kextutil(8)' error message you should update kext's dependencies and rebuild it. To update kext's dependencies execute 'kextlibs -xml /Library/Extensions/AntiRTDPI.kext', copy output, open project's Info.plist file and replace '<key>OSBundleLibraries</key><dict>...</dict>' section with copied output. Then rebuild project and use new kext file.

To unload execute this command:
```
sudo kextunload /Library/Extensions/AntiRTDPI.kext
```

Enjoy it! If you experience any issues or have suggestions - I'll be glad to hear you!
