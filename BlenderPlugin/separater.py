#coding:utf-8

import bpy

obj = bpy.context.active_object
for i in range(len(obj.data.polygons)):
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_mode(type="FACE")
    bpy.ops.mesh.select_all(action="DESELECT")
    
    bpy.ops.object.mode_set(mode="OBJECT")
    obj.data.polygons[0].select = True
    bpy.ops.object.mode_set(mode="EDIT")
    
    bpy.ops.mesh.separate()