# -*- coding: utf-8 -*-
import re
import os
import sys
import json

def animation_grouping(): # 动画分组
    dictionary = {}
    for filename in os.listdir(os.curdir):
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

def main():
    dictionary = animation_grouping()
    json_string = json.dumps(dictionary)
    with open("animation_group.json", "w") as handle:
        handle.write(json_string)
        handle.close()

if __name__ == '__main__':
    main()