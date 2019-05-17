# qtws
[![Build Status](https://travis-ci.org/intersimone999/qtws.svg?branch=master)](https://travis-ci.org/intersimone999/qtws)

A qt webengine program to easily create very basic and lightweight desktop webapps.

Available in the [AUR](https://aur.archlinux.org/packages/qtws-base/). The official store is available at [mattmezza/qtws-store](https://github.com/mattmezza/qtws-store/).

## Requirements
- qt5
- qtwebegine (with proprietary codecs, needed for some webapps like Netflix, but not for all of them)

Arch users do not need to compile qtwebengine with proprietary codecs, the offical package has them enabled.

Ubuntu 17.04/17.10 - qtwebengine comes with proprietary codecs

       Grab the deb package from releases
       chmod +x widevine.sh
       sudo ./widevine.sh

Most others must compile qtwebengine with proprietary codecs.

Read this <html>http://blog.qt.io/blog/2016/06/03/netflix-qt-webengine-5-7/</html>

## Installation
- Install qt5
- Compile qtwebengine according to the link above
- `git clone https://github.com/intersimone999/qtws.git`
- `cd qtws/build`
- Type `qmake -config release ..`
- Type `make`
- Binary will be labeled `qtws`

## Features
The installed qtws allows you to easily create an embedded version of an online webapp. The basic version of qtws features a menu that can be activated with a right click anywhere. This menu allows to go back (Alt+left arrow), to go to the home (Ctrl+H) of the webapp and to reload the page (Ctrl+R or F5). It is also possible to switch to the fullscreen mode pushing F11.

Each app is completely isolated from the others: it will have its own cache, cookies and storage.

To run qtws it is necessary to specify a configuration file which gives instructions about the webapp that needs to be run. This is an example for YouTube:

```json
{
        "name": "YouTube",
        "home": "http://youtube.com",
        "icon": "youtube-desktop.svg",
        "saveSession": false,
        
        "plugins": [],
        "scope": ["https?:\/\/.*\.youtube\.com(?:\/.*|)"],
        
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
- *saveSession*: if the session has to be saved when the window is closed (e.g., the last page visited) (boolean);
- *plugins*: list of the plugins needed (ignored at the moment) (array of strings);
- *scope*: regular expressions of the domains that are allowed in the webapp. URLs belonging to domains not matching with any of the scopes will be openend with the browser (array of strings);
- menu: additional entries in the contextual menu (array of objects with a title, indicating the name of the menu entry, and an action, indicating the URL that will be set if the entry is selected).
- alwaysOnTop: sets the app always on top.
- cacheMB: maximum size of the cache (in MB). Default is 50.


## Troubleshooting 
If you receiving an error loading Netflix or similar webapps, try

```sh
qtws configfile.json --register-pepper-plugins="/usr/lib/qt/plugins/ppapi/libwidevinecdmadapter.so; application/x-ppapi-widevine-cdm"
```
