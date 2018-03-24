# qtws
A qt webengine program to easily create very basic and lightweight desktop webapps.

It will be available in the AUR soon.
## Requirements
- qt5
- qtwebegine (with proprietary codecs)

Arch users do not need to compile qtwebengine with proprietary codecs, the offical package has them enabled.

Ubuntu 17.04/17.10 - qtwebengine comes with proprietary codecs

       Grab the deb package from releases
       chmod +x widevine.sh
       sudo ./widevine.sh

Most others must compile qtwebengine with proprietary codecs.

Read this <html>http://blog.qt.io/blog/2016/06/03/netflix-qt-webengine-5-7/</html>

## Installation:
- Install qt5
- Compile qtwebengine according to the link above
- Dowload the source.
- cd into the folder
- Type make clean
- Type qmake -config release
- Type make
- Binary will be labeled netflix

## Features
The installed qtws allows you to easily create an embedded version of an online webapp. The basic version of qtws features a menu that can be activated with a right click anywhere. This menu allows to go back (Alt+left arrow), to go to the home (Ctrl+H) of the webapp and to reload the page (Ctrl+R or F5). It is also possible to switch to the fullscreen mode pushing F11.

To run qtws it is necessary to specify a configuration file which gives instructions about the webapp that needs to be run. This is an example for YouTube:

```json
{
        "name": "YouTube",
        "home": "http://youtube.com",
        "icon": "youtube-desktop.svg",
        "saveSession": false,
        
        "plugins": [],
        "scope": ["youtube.com"],
        
        "menu": [
            {
                "title": "Trending",
                "action": "http://youtube.com/feed/trending"
            },
            {
                "title": "Subscriptions",
                "action": "http://youtube.com/feed/subscriptions"
            },
            {
                "title": "History",
                "action": "http://youtube.com/feed/history"
            },
            {
                "title": "Playlist",
                "action": "http://youtube.com/playlist"
            },
            {
                "title": "Trending",
                "action": "http://youtube.com/feed/trending"
            },
            {
                "title": "Settings",
                "action": "http://youtube.com/account"
            }
        ]
}
```

The fields of the json are the following (required in italics):
- *name*: name of the webapp (string);
- *home*: URL of the homepage of the webapp (string);
- *icon*: local path of the icon that the application should show (string);
- *saveSession*: if the session has to be saved when the window is closed (e.g., the last page visited) (string);
- *plugins*: list of the plugins needed (ignored at the moment) (array of strings);
- *scope*: domains that are allowed in the webapps. URLs belonging to other domains will be openend with the browser (array of strings);
- menu: additional entries in the contextual menu (array of objects with a title, indicating the name of the menu entry, and an action, indicating the URL that will be set if the entry is selected).

## Troubleshooting 
If you receiving an error loading Netflix or similar webapps, try

```sh
qtws configfile.json --register-pepper-plugins="/usr/lib/qt/plugins/ppapi/libwidevinecdmadapter.so; application/x-ppapi-widevine-cdm"
```
