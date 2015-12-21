# -*- coding: utf-8 -*-

import os
import json

def main():
    json_map = {}
    filelist = os.walk(os.curdir)
    begin = len(os.curdir + os.path.sep)
    for dirpath, dirnames, filenames in filelist:
        if dirpath != os.curdir:
            filearray = []
            for filename in filenames:
                if os.path.splitext(filename)[1] == ".plist":
                    key = dirpath[begin:].replace(os.path.sep,'/')
                    json_map[key] = filearray
                    filearray.append("".join((key, '/', filename)))
    if len(json_map) > 0:
        file_handle = open("TexturePacker.json", "w")
        file_handle.write(json.dumps(json_map))

if __name__ == "__main__":
    main()