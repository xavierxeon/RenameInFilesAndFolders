#!/usr/bin/env python3

import os

from xxpystuff.pyside2 import Resource, UiFile

def main():

    resource = Resource()
    for entry in os.scandir():
        if not entry.is_file():
            continue

        if entry.name.endswith('.svg'):
            print('RC: ' + entry.name)
            resource.append(entry.name)

    rcFileName = 'Icons.qrc'
    resource.createRCFile(rcFileName)     

if __name__ == '__main__':
    main()
