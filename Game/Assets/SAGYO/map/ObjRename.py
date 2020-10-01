#coding:utf-8


import bpy


obj = bpy.context.selected_objects
for i,o in enumerate(obj):
    o.name = "map_test_%0d"%i
    