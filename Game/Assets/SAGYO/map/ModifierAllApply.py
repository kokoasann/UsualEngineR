#coding:utf-8

import bpy

obj = bpy.context.selected_objects


for o in obj:
    # set the object to active
    bpy.context.view_layer.objects.active = o

    # this operator will 'work' or 'operate' on the active object we just set
    #bpy.ops.object.modifier_apply(modifier="Subdivision")
    #bpy.ops.object.modifier_apply(modifier="Displace")
    bpy.ops.object.modifier_apply(modifier="Decimate")