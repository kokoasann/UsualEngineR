#coding:utf-8


import bpy


obj = bpy.context.selected_objects


for o in obj:
    # set the object to active
    bpy.context.view_layer.objects.active = o

    # this operator will 'work' or 'operate' on the active object we just set
    bpy.ops.object.modifier_add(type="DECIMATE")
    
    mo = o.modifiers["Decimate"]
    mo.decimate_type = "COLLAPSE"
    mo.ratio = 0.08