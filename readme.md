# List Applications
List Applications (.desktop) to pick up for example with dmenu


## Together with DMenu
[![Watch the video](https://i.ytimg.com/vi/crax1fvLPxs/hqdefault.jpg)](https://www.youtube.com/watch?v=crax1fvLPxs)


## How to use
- Simple call with out params returns all defined apps in    
   **/usr/share/applications**    
 **/usr/local/share/applications**  
- You can extend it with other directories of course  
- A call with a app name returns the exec part of **.desktop**

## Execute Script **DAppsLaunch** (lapps+dmenu)

```bash
#!/bin/bash
title="Apps:"
app=$(lapps | dmenu -i -c -p "$title"  -l 15)
[[ -z "$app" ]] && exit
echo $app
exec $(lapps "$app")
```

## For what i use it 
- Together with **dmenu** as app launcher for all the apps defined  
in .desktop-files


