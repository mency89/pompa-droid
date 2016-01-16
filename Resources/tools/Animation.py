# -*- coding: utf-8 -*-
import re
import os
import sys
import json

def Sorting(path):
    dictionary = {}
    for filename in os.listdir(path):
        result = re.match("(\w+)_(\d{2}).png", filename)
        if result:
            dictionary.setdefault(result.group(1),[])
            dictionary[result.group(1)].append(result.group(2))
    for key in dictionary:
        dictionary[key].sort()
    for key in dictionary:
        for index, number in enumerate(dictionary[key]):
            dictionary[key][index] = "".join((key, "_", number, ".png"))
    return dictionary

if __name__ == '__main__':
    dictionary = Sorting(os.curdir)
    json_string = json.dumps(dictionary)
    with open("Animation.json", "w") as handle:
        handle.write(json_string)
        handle.close()