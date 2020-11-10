#coding:utf-8


import bpy


obj = bpy.context.selected_objects
for i,o in enumerate(obj):
    #o.name = "map_commit_%0d"%i
    #o.MaterialProps.texture_alb = "\\BGHeight.png"
    #o.MaterialProps.texture_nor = "\\mapNormal.png"