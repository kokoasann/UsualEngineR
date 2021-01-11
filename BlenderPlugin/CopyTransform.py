#coding:utf-8

import bpy
from bpy.props import StringProperty
from bpy.props import BoolProperty
from bpy.props import IntProperty
from bpy.props import CollectionProperty
from bpy.types import PropertyGroup
from bpy.props import PointerProperty
import bmesh

import mathutils as math
import copy
import struct
import os

#アドオンの詳細
bl_info = {
    "name": "CopyTransform",
    "description": "",
    "author": "kokoasann",
    "version": (1, 0, 0, 0),
    "blender": (2, 80, 2),
    "category": "Properties",
    "location": "UI",
    "warning": "",
    "wiki_url": "",
    "tracker_url": ""
}

#copy transform panel instance
copytransform_panel = None

class SelectAttr:
    select_mode = "OBJECT"
    select_type = "MESH"
    select_element = ""

    def Update():
        actObj = bpy.context.active_object
        if actObj == None:
            SelectAttr.select_mode = ""
            SelectAttr.select_type = ""
            SelectAttr.select_element = ""
            return
        #print("UPDATE")
        isChange = False
        if SelectAttr.select_mode != actObj.mode or SelectAttr.select_type != actObj.type:
            #print("change!!")
            isChange = True
        SelectAttr.select_mode = actObj.mode
        SelectAttr.select_type = actObj.type
        
        if SelectAttr.select_mode == "EDIT" and SelectAttr.select_type == "ARMATURE":
            for bone in actObj.data.edit_bones:
                if bone.select:
                    SelectAttr.select_element = "BONE"
                    break
                elif bone.select_head:
                    SelectAttr.select_element = "HEAD"
                elif bone.select_tail:
                    SelectAttr.select_element = "TAIL"
        #if isChange and copytransform_panel != None:
            #copytransform_panel.draw(bpy.context)
        

#bpy.app.handlers.frame_change_post.append(SelectAttr.Update)

elementenum = ["BONE",
                "HEAD",
                "TAIL"]

typeenum = ["MESH",
            "CURVE",
            "SURFACE",
            "META",
            "FONT",
            "HAIR",
            "POINTCLOUD",
            "VOLUME",
            "GPENCIL",
            "ARMATURE",
            "LATTICE",
            "EMPTY",
            "LIGHT",
            "LIGHT_PROBE",
            "CAMERA",
            "SPEAKER"]
            
modeenum = ["OBJECT", 
            "EDIT", 
            "POSE", 
            "SCULPT", 
            "VERTEX_PAINT", 
            "WEIGHT_PAINT", 
            "TEXTURE_PAINT", 
            "PARTICLE_EDIT", 
            "EDIT_GPENCIL", 
            "SCULPT_GPENCIL", 
            "PAINT_GPENCIL", 
            "VERTEX_GPENCIL", 
            "WEIGHT_GPENCIL"]

class CopyTransform_PT_Panel(bpy.types.Panel):
    bl_label = "CopyTransform"
    bl_category = "Item"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    
    def draw(self,context):
        global copytransform_panel
        copytransform_panel = self
        
        SelectAttr.Update()
        
        #print("DRAW")
        if SelectAttr.select_mode == "OBJECT":
            self.layout.operator("copytransform.copyposition")
            self.layout.operator("copytransform.pasteposition")
        elif SelectAttr.select_mode == "EDIT":
            if SelectAttr.select_type == "ARMATURE":
                if SelectAttr.select_element == "BONE":
                    self.layout.operator("copytransform.copyposition_head")
                    self.layout.operator("copytransform.copyposition_tail")
                    self.layout.operator("copytransform.pasteposition_head")
                    self.layout.operator("copytransform.pasteposition_tail")
                elif SelectAttr.select_element == "HEAD":
                    self.layout.operator("copytransform.copyposition_head")
                    self.layout.operator("copytransform.pasteposition_head")
                elif SelectAttr.select_element == "TAIL":
                    self.layout.operator("copytransform.copyposition_tail")
                    self.layout.operator("copytransform.pasteposition_tail")
                #print("EDIT DRAW")
            else:
                self.layout.operator("copytransform.copyposition")
                self.layout.operator("copytransform.pasteposition")
            
        elif SelectAttr.select_mode == "POSE":
            self.layout.operator("copytransform.copyposition")
            self.layout.operator("copytransform.pasteposition")
        else:
            self.layout.label(text="None select")
        
        
class TFClipbord:
    posList = []
    rotList = []
    scaList = []
    
    MAX_HISTRY = 3
    
    def AddPos(pos):
        TFClipbord.posList.append(pos)
        if len(TFClipbord.posList) > TFClipbord.MAX_HISTRY:
            TFClipbord.posList = TFClipbord.posList[1:]
    def AddRot(rot):
        TFClipbord.rotList.append(rot)
        if len(TFClipbord.rotList) > TFClipbord.MAX_HISTRY:
            TFClipbord.rotList = TFClipbord.rotList[1:]
    def AddSca(sca):
        TFClipbord.scaList.append(sca)
        if len(TFClipbord.scaList) > TFClipbord.MAX_HISTRY:
            TFClipbord.scaList = TFClipbord.scaList[1:]
            
    def GetPos():
        return TFClipbord.posList[-1]
    def GetRot():
        return TFClipbord.rotList[-1]
    def GetSca():
        return TFClipbord.scaList[-1]
    
        
def CalcEditMeshVertexMedianPosition(mesh,worldMat):
    bpy.ops.object.mode_set(mode="OBJECT")
    v_sel = [v.co for v in mesh.vertices if v.select]
    pos = sum(v_sel,math.Vector()) / len(v_sel)
    #print(worldMat @ pos)
    bpy.ops.object.mode_set(mode="EDIT")
    return worldMat @ pos

def SetEditMeshVertexPosition(pos,obj,worldMat):
    bpy.ops.object.mode_set(mode="OBJECT")
    mesh = obj.data
    bm = bmesh.new()
    bm.from_mesh(mesh)
    #bm = bmesh.from_edit_mesh(mesh)
    
    iworldMat = worldMat.copy()
    iworldMat.invert()
    lpos = iworldMat @ pos
    print(lpos)
    
    v_sel = [v.co for v in mesh.vertices if v.select]
    avpos = sum(v_sel,math.Vector()) / len(v_sel)
    for v in bm.verts:
        if not v.select:
            continue
        baseVec = v.co - avpos
        newpos = baseVec + lpos
        v.co = newpos
    bm.to_mesh(mesh)
    bm.free()
    #bmesh.update_edit_mesh(mesh)
    bpy.ops.object.mode_set(mode="EDIT")
    
##
## COPY
##
        
#copy position
class OT_CopyPosition(bpy.types.Operator):
    bl_idname = "copytransform.copyposition"
    bl_label = "Copy Position"
    
    def invoke(self,context,event):
        if SelectAttr.select_mode == "OBJECT":
            pos = bpy.context.active_object.location
            TFClipbord.AddPos(pos)
            print("copy")
            print(pos)
        elif SelectAttr.select_mode == "EDIT":
            wmat = bpy.context.active_object.matrix_world
            mesh = bpy.context.active_object.data
            pos = CalcEditMeshVertexMedianPosition(mesh,wmat)
            TFClipbord.AddPos(pos)
        return {'FINISHED'}

    def execute(self,context):
        return {"FINISHED"}
    
#copy bone head position    
class OT_CopyPosition_Head(bpy.types.Operator):
    bl_idname = "copytransform.copyposition_head"
    bl_label = "Copy Position Head"
    
    def invoke(self,context,event):
        wmat = context.active_object.matrix_world
        for bone in context.active_object.data.edit_bones:
            if bone.select_head:
                TFClipbord.AddPos(wmat @ bone.head)
                break
        return {'FINISHED'}

    def execute(self,context):
        return {"FINISHED"}
    
# copy bone tail position
class OT_CopyPosition_Tail(bpy.types.Operator):
    bl_idname = "copytransform.copyposition_tail"
    bl_label = "Copy Position Tail"
    
    def invoke(self,context,event):
        wmat = context.active_object.matrix_world
        for bone in context.active_object.data.edit_bones:
            if bone.select_tail:
                TFClipbord.AddPos(wmat @ bone.tail)
        return {'FINISHED'}

    def execute(self,context):
        return {"FINISHED"}
    
##
## PASTE
##

#paste position    
class OT_PastePosition(bpy.types.Operator):
    bl_idname = "copytransform.pasteposition"
    bl_label = "Paste Position"
    
    def invoke(self,context,event):
        if SelectAttr.select_mode == "OBJECT":
            pos = TFClipbord.GetPos()
            bpy.context.active_object.location = pos
            print("paste")
            print(pos)
        elif SelectAttr.select_mode == "EDIT":
            pos = TFClipbord.GetPos()
            #mesh = bpy.context.active_object.to_mesh()
            #mesh.update()
            wmat = bpy.context.active_object.matrix_world
            SetEditMeshVertexPosition(pos,bpy.context.active_object,wmat)
            #mesh.update()
            print("paste edit")
        return {'FINISHED'}

    def execute(self,context):
        
        return {"FINISHED"}

#paste bone head position    
class OT_PastePosition_Head(bpy.types.Operator):
    bl_idname = "copytransform.pasteposition_head"
    bl_label = "Paste Position Head"
    
    def invoke(self,context,event):
        wmat = context.active_object.matrix_world.copy()
        wmat.invert()
        for bone in context.active_object.data.edit_bones:
            if bone.select_head:
                #print(bone)
                bone.head = wmat @ TFClipbord.GetPos()
                break
        return {'FINISHED'}

    def execute(self,context):
        return {"FINISHED"}
    
# paste bone tail position
class OT_PastePosition_Tail(bpy.types.Operator):
    bl_idname = "copytransform.pasteposition_tail"
    bl_label = "Paste Position Tail"
    
    def invoke(self,context,event):
        wmat = context.active_object.matrix_world.copy()
        wmat.invert()
        for bone in context.active_object.data.edit_bones:
            if bone.select_tail:
                #print(bone)
                bone.tail = wmat @ TFClipbord.GetPos()
                break
        return {'FINISHED'}

    def execute(self,context):
        return {"FINISHED"}

##
## INITIALIZE
##

classs = (
    CopyTransform_PT_Panel,
    OT_CopyPosition,
    OT_CopyPosition_Head,
    OT_CopyPosition_Tail,
    OT_PastePosition,
    OT_PastePosition_Head,
    OT_PastePosition_Tail,
)

def register():
    for c in classs:
        bpy.utils.register_class(c)
    

def unregister():
    for c in classs:
        bpy.utils.unregister_class(c)
    
if __name__ == "__main__":
  register()