#coding:utf-8

import bpy
from bpy.props import StringProperty
from bpy.props import BoolProperty
from bpy.props import IntProperty
from bpy.props import CollectionProperty
from bpy.types import PropertyGroup
from bpy.props import PointerProperty

import mathutils as math
import copy
import struct
import os

#アドオンの詳細
bl_info = {
    "name": "eventMovieExp",
    "description": "",
    "author": "kokoasann",
    "version": (1, 0, 0, 0),
    "blender": (2, 80, 2),
    "category": "Properties",
    "location": "Window",
    "warning": "",
    "wiki_url": "",
    "tracker_url": ""
}

def transToYUp(matrix):
    #ZUpをYUpに変えるための回転行列
    rotMat = math.Matrix([ [1, 0,0,0],\
                                [0, 0,1,0],\
                                [0,-1,0,0],\
                                [0, 0,0,1]])
    matrix = rotMat @ matrix
    return matrix


class PT_Panel(bpy.types.Panel):
    bl_label = "eventMovieExp"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"
    
    def draw(self, context):
        #print(type(context))
        self.layout.operator("event_movie_exp.export")


class OT_Export(bpy.types.Operator):
    bl_idname = "event_movie_exp.export"
    bl_label = "Export Material"
    
    filename_ext = ".evm"
    filepath : StringProperty(
        name="EVM_filepath",
        description = "export file path",
        maxlen = 1024,
        subtype = "FILE_PATH",
        default="untitled.evm"
        )
    
    def invoke(self,context,event):
        self.filepath = "untitled"+self.filename_ext
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    def execute(self,context):
        with open(self.filepath,"wb") as f:
            time = 0.#経過時間を入れとく変数
            spf = 1. / bpy.context.scene.render.fps#フレームあたりの秒数
            
            f.write( struct.pack("<i", len(bpy.context.scene.timeline_markers)))
            for mark in bpy.context.scene.timeline_markers:
                fr = spf * mark.frame
                #ｲｳﾞｪﾝﾄﾉｵｺﾙｼﾞｶﾝ(ﾋﾞｮｳ)
                f.write( struct.pack("<f", fr) )
                #イヴェント名の出力
                f.write( struct.pack("<i", len(mark.name)) )
                f.write(mark.name.encode()+b"\0")
                
            
            f.write(struct.pack("<i", len(bpy.context.selected_objects)))
            id = 0
            for obj in bpy.context.selected_objects:
                f.write(struct.pack("<i", id))
                f.write(struct.pack("<i", len(obj.name)))
                f.write(obj.name.encode()+b"\0")
                id+=1
                
            frame_end = bpy.context.scene.frame_end
            frame_start = bpy.context.scene.frame_start
            f.write(struct.pack("<i", frame_end+1 - frame_start))
            for frame in range(frame_start, frame_end+1):
                bpy.context.scene.frame_set(frame)
                f.write(struct.pack("<f", time))
                
                cam = bpy.context.scene.objects["Camera"]
                cammat = cam.matrix_world.copy()
                #cammat = transToYUp(cammat)
                cammat.transpose()
                for vec3 in cammat:
                    for i , m in enumerate(vec3):
                        #if i != 3:
                        f.write(struct.pack("<f", m))
                            
                for obj in bpy.context.selected_objects:
                    mat = obj.matrix_world.copy()
                    #mat = transToYUp(mat)
                    mat.transpose()
                    #print(mat)
                    for vec3 in mat:
                        for i , m in enumerate(vec3):
                            #if i != 3:
                            f.write(struct.pack("<f", m))
                                
                time += spf
                
        return {"FINISHED"}
    
    
classs = (
    PT_Panel,
    OT_Export,
)

def register():
    for c in classs:
        bpy.utils.register_class(c)
    #bpy.types.Scene.armatures = CollectionProperty(type=ItemSet)
    

def unregister():
    for c in classs:
        bpy.utils.unregister_class(c)
    #del bpy.types.Scene.armatures


if __name__ == "__main__":
    register()