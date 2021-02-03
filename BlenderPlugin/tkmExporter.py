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

class Bone:
    def __init__(self):
        self.name = ""
        self.index = 0
        
class Mate:
    def __init__(self):
        self.albedo = ""
        self.normal = ""
        self.specular = ""
        
class Vert:
    def __init__(self):
        self.index=-1
        self.real_index = -1
        self.index_tr=-1
        self.poly_index = -1
        
        self.pos = math.Vector((0,0,0))
        self.nor = math.Vector((0,0,0))
        self.uv = math.Vector((0,0))
        self.weight = [0,0,0,0]
        self.indices = [-1,-1,-1,-1]
        self.boneNames = []

class Mesh:
    def __init__(self):
        self.numMaterial = 1
        self.numVert = 0
        self.numPolygon = 0
        self.indexSize = 0
        
        self.materialList = [Mate()]
        
        
        self.vertList = []
        self.indeces = []
        
        self.vertGroups = {}
        
    def print(self):
        print("num mate: ",self.numMaterial)
        print("num vert: ",self.numVert)
        print("num poly: ",self.numPolygon)
        print("ind size: ",self.indexSize)
        print("mates")
        for mate in self.materialList:
            print("albedo: ",mate.albedo)
            print("normal: ",mate.normal)
            print("specular: ",mate.specular)
            
        for vert in self.vertList:
            print("ind: ",vert.index)
            print("real ind: ",vert.real_index)
            print("ind tr: ",vert.index_tr)
            print("pos: ",vert.pos)
            print("nor: ",vert.nor)
            print("uv: ",vert.uv)
            print("weight: ",vert.weight)
            print("w ind: ",vert.indices)
            
        print("indeces: ",self.indeces)

def transToYUp(matrix):
    #ZUpをYUpに変えるための回転行列
    rotMat = math.Matrix([ [1, 0,0,0],\
                                [0, 0,1,0],\
                                [0,-1,0,0],\
                                [0, 0,0,1]])
    matrix = rotMat @ matrix
    return matrix
        
        
def build_mesh(o):
    meshParts = Mesh()
    
    #os.path.basename(o.MaterialProps.texture_alb).split('.', 1)[0]
    meshParts.materialList[0].albedo = "/"+os.path.basename(o.MaterialProps.texture_alb)
    meshParts.materialList[0].normal = "/"+os.path.basename(o.MaterialProps.texture_nor)
    meshParts.materialList[0].specular = "/"+os.path.basename(o.MaterialProps.texture_spe)
        
    objmat = transToYUp(o.matrix_world.copy())
    rotmat = objmat.copy()
    rotmat = rotmat.Translation((0,0,0))
    
    msh = o.to_mesh()
    
    # num polygon
    #numPly = len(msh.vertices)
    
    #vertex data build
    vertList = []
    for vt in msh.vertices:
        vert = Vert()
        vert.index = vt.index
        vert.index_tr = vt.index
        vert.pos = objmat @ vt.co
        vert.nor = rotmat @ vt.normal
        
        for i,w in enumerate(vt.groups):
            if i == 4:
                print("out of w ind",o)
                break
            vert.indices[i] = w.group
            vert.weight[i] = w.weight
            #vert.boneNames.append(w.name)
            
        vertList.append(vert)
        
    for group in o.vertex_groups:
        meshParts.vertGroups[group.index] = group.name
    #print(meshParts.vertGroups)
        
    #print("mesh split to a polygon for index")
    vlist = []
    pindlist = []
    count = 0
    for pl in msh.polygons:
        for vi in pl.vertices:
            for vert in vertList:
                if vert.index == vi:
                    v = copy.deepcopy(vert)
                    v.real_index = count
                    v.poly_index = pl.index
                    vlist.append(v)
                    count += 1
                    break
                
    
    #print("set uv")
    for ly in msh.uv_layers:
        
        for idx, dat in enumerate(msh.uv_layers[ly.name].data):
            vlist[idx].uv.x = dat.uv.x
            vlist[idx].uv.y = 1.-dat.uv.y
                
            
        
    msh.calc_loop_triangles()
    
    # num polygon
    meshParts.numPolygon = len(msh.loop_triangles)
    indList = []
    #print("build index data")
    for tr in msh.loop_triangles:
        #print("poly: %d"%tr.polygon_index)
        for v in tr.vertices:

            for ve in vlist:
                if ve.index == v and tr.polygon_index == ve.poly_index:
                    indList.append(ve.real_index)
                    #print(" %d"%ve.real_index,end="")
                    break
        #print("")

    #print(len(indList))
    meshParts.vertList += vlist
    meshParts.indeces = indList
    
    return meshParts

            
    rVertList = []
    indList = []
    for tr in msh.loop_triangles:
        for i,v in enumerate(tr.loops):
            ve = None
            for vert in vertList:
                if vert.index == tr.vertices[i]:
                    ve = vert
            if v in indList:
                new_index = 0
                while True:
                    if new_index in indList:
                        new_index += 1
                    else:
                        break
                
                new_vert = copy.deepcopy(ve)
                new_vert.index_tr = new_index
                rVertList.append(new_vert)
                indList.append(new_index)
            else:
                ve.index_tr = v
                indList.append(v)
                rVertList.append(ve)
                
    
    meshParts.vertList.append(rVertList)
    return meshParts
        

def build_data(obj,f):
    
    meshList = []
    if obj.type == "ARMATURE":
        for o in obj.children:
            meshList.append(build_mesh(o))
            
        boneIDs = {}
        bpy.ops.object.mode_set(mode='EDIT')
        edit_bones = list(bpy.context.object.data.edit_bones)
        for i,bone in enumerate(edit_bones):
            boneIDs[bone.name] = i
        #print(boneIDs)
        for msh in meshList:
            for vert in msh.vertList:
                for j,k in enumerate(vert.indices):
                    if k == -1:
                        break
                    #print(k)
                    vert.indices[j] = boneIDs[msh.vertGroups[k]]
    else:
        meshList.append(build_mesh(obj))

    
    # num's meshes
    numMesh = len(meshList)
    #print("num mesh: %d"%numMesh)
    
    # tkm version
    f.write(struct.pack("B",100))
    # is flat shading
    f.write(struct.pack("<B",0))
    # num mesh
    f.write(struct.pack("<H",numMesh))
    
    for msh in meshList:
        #msh.print()
        indexSize = 2
        if len(msh.indeces) > 65535:
            indexSize = 4
            
        f.write(struct.pack("<I",len(msh.materialList)))
        f.write(struct.pack("<I",len(msh.vertList)))
        f.write(struct.pack("<Bbbb",indexSize,0,0,0))
        
        
        for mate in msh.materialList:
            
            if len(mate.albedo) > 1:
                f.write(struct.pack("<I",len(mate.albedo)))
                f.write(mate.albedo.encode(encoding="sjis")+b"\0")
                #for i in range(len(mate.albedo)%4):
                    #f.write(struct.pack("B",0))
            else:
                f.write(struct.pack("<I",0))
            
            if len(mate.normal) > 1:
                f.write(struct.pack("<I",len(mate.normal)))
                f.write(mate.normal.encode(encoding="sjis")+b"\0")
                #for i in range(len(mate.normal)%4):
                    #f.write(struct.pack("B",0))
            else:
                f.write(struct.pack("<I",0))
            
            if len(mate.specular) > 1:
                f.write(struct.pack("<I",len(mate.specular)))
                f.write(mate.specular.encode(encoding="sjis")+b"\0")
                #for i in range(len(mate.specular)%4):
                    #f.write(struct.pack("B",0))
            else:
                f.write(struct.pack("<I",0))
            
        for vert in msh.vertList:
            for v in vert.pos:
                f.write(struct.pack("<f",v))
                
            for v in vert.nor:
                f.write(struct.pack("<f",v))
            for v in vert.uv:
                f.write(struct.pack("<f",v))
            for v in vert.weight:
                f.write(struct.pack("<f",v))
            for v in vert.indices:
                #print(v)
                f.write(struct.pack("<h",v))
                
        f.write(struct.pack("<I",msh.numPolygon))
        format = "<H"
        if indexSize == 4:
            format = "<I"
        for v in msh.indeces:
            #print(" %d"%v)
            f.write(struct.pack(format,v+1))
            
        if indexSize == 2:
            if (msh.numPolygon*3) % 2 != 0:
                f.write(struct.pack("H",0))
        
class Cell:
    def __init__(self,ind):
        # this index
        self.index = ind
        #indeces
        self.verts = []
        #position
        self.center = None
        #triangle polygon index
        self.adjacentCell = []
        self.vertPos = []
        
    def addCell(self,cellind):
        self.adjacentCell.append(cellind)
        
    def isAdjacentCell(self,cell):
        cnt = 0
        for v in cell.verts:
            if v in self.verts:
                cnt += 1
        if cnt >= 2:
            return True
        return False
        
def wrightCell(o,f):
    print("start cell export")
    mesh = o.to_mesh()
    mesh.calc_loop_triangles()
    cellList = []
    
    for tri in mesh.loop_triangles:
        cell = Cell(tri.index)
        cell.verts = tri.vertices
        center = tri.center.copy()
        print(center)
        y = center.y
        center.y = center.z
        center.z = y * -1
        cell.center = center
        
        for i in tri.vertices:
            v = mesh.vertices[i].co.copy()
            y = v.y
            v.y = v.z
            v.z = y * -1
            cell.vertPos.append(v)
        
        cellList.append(cell)
        
        
        
    for cell in cellList:
        #print("build adjacent")
        for aCell in cellList:
            if cell.index == aCell.index:
                continue
            if aCell.isAdjacentCell(cell):
                cell.addCell(aCell.index)
                
    f.write(struct.pack("<I",len(cellList)))
    for cell in cellList:
        #print("C")
        #print("wright cell")
        #f.write(struct.pack("<I",cell.index))
        for vp in cell.vertPos:
            for v in vp:
                f.write(struct.pack("<f",v))
        for v in cell.adjacentCell:
            #print(v)
            f.write(struct.pack("<I",v))
            
        if 3-len(cell.adjacentCell) != 0:
            for i in range(3-len(cell.adjacentCell)):
                #print(-1)
                f.write(struct.pack("<i",-1))
                
        for v in cell.center:
            f.write(struct.pack("<f",v))
        
        
    
        
#アドオンの詳細
bl_info = {
    "name": "tkmExporter",
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
        
class MaterialProps(PropertyGroup):
    texture_alb : StringProperty(
        name = "Texture",
        description = "texture",
        default = "",
        maxlen = 256,
    )
    texture_nor : StringProperty(
        name = "Normal",
        description = "texture",
        default = "",
        maxlen = 256,
    )
    texture_spe : StringProperty(
        name = "Specular",
        description = "texture",
        default = "",
        maxlen = 256,
    )

        
class PT_Panel(bpy.types.Panel):
    bl_label = "tkmExporter"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"
    
    def draw(self, context):
        #print(type(context))
        self.layout.operator("tkmexporter.export")
        self.layout.operator("tkmexporter.exportselect")
        
        row = self.layout.row()
        row.prop(context.object.MaterialProps,"texture_alb",text="albedo")
        row.operator("tkmexporter.open_texture",icon="FILE_FOLDER")
        
        row = self.layout.row()
        row.prop(context.object.MaterialProps,"texture_nor",text="normal")
        row.operator("tkmexporter.open_normal",icon="FILE_FOLDER")
        
        row = self.layout.row()
        row.prop(context.object.MaterialProps,"texture_spe",text="specular")
        row.operator("tkmexporter.open_specular",icon="FILE_FOLDER")
        
        self.layout.operator("tkmexporter.cell_export")


class OT_Export(bpy.types.Operator):
    bl_idname = "tkmexporter.export"
    bl_label = "Export Material"
    
    filename_ext = ".tkm"
    filepath : StringProperty(subtype = "FILE_PATH")
    
    def invoke(self,context,event):
        filepath = "untitle"+self.filename_ext
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    def execute(self,context):
        with open(self.filepath,"wb") as f:
            obj = bpy.context.active_object
            build_data(obj,f)
        return {"FINISHED"}


class OT_ExportSelect(bpy.types.Operator):
    bl_idname = "tkmexporter.exportselect"
    bl_label = "Export Select Material"
    
    filename_ext = ".tkm"
    filepath : StringProperty(subtype = "FILE_PATH")
    
    def invoke(self,context,event):
        filepath = "untitle"+self.filename_ext
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    def execute(self,context):
        objs = bpy.context.selected_objects
        for i,obj in enumerate(objs):
            path = self.filepath[:self.filepath.rfind(".")]
            path += "_"+str(i)+self.filename_ext
            with open(path,"wb") as f:
                build_data(obj,f)
        return {"FINISHED"}

class OT_OpenTexture(bpy.types.Operator):
    bl_idname = "tkmexporter.open_texture"
    bl_label = "Open"
    
    filename_ext = ""
    filepath : StringProperty(subtype = "FILE_PATH")
    def invoke(self,context,event):
        self.filepath = "untitle"+self.filename_ext
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    def execute(self,context):
        bpy.context.active_object.MaterialProps.texture_alb = self.filepath
        return {"FINISHED"}
    
class OT_OpenNormal(bpy.types.Operator):
    bl_idname = "tkmexporter.open_normal"
    bl_label = "Open"
    
    filename_ext = ""
    filepath : StringProperty(subtype = "FILE_PATH")
    def invoke(self,context,event):
        self.filepath = "untitle"+self.filename_ext
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    def execute(self,context):
        bpy.context.active_object.MaterialProps.texture_nor = self.filepath
        return {"FINISHED"}
    
class OT_OpenSpecular(bpy.types.Operator):
    bl_idname = "tkmexporter.open_specular"
    bl_label = "Open"
    
    filename_ext = ""
    filepath : StringProperty(subtype = "FILE_PATH")
    def invoke(self,context,event):
        self.filepath = "untitle"+self.filename_ext
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    def execute(self,context):
        bpy.context.active_object.MaterialProps.texture_spe = self.filepath
        return {"FINISHED"}
    
class OT_CellExport(bpy.types.Operator):
    bl_idname = "tkmexporter.cell_export"
    bl_label = "Export Cell"
    
    filename_ext = ".clb"
    
    filepath : StringProperty(
        name="clb_FilePath",
        description="Filepath used for exporting the file",
        default = "untitled.clb",
        maxlen=1024,
        subtype='FILE_PATH',
    )
    
    def invoke(self,context,event):
        filepath = "untitle"+self.filename_ext
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    def execute(self,context):
        with open(self.filepath,"wb") as f:
            obj = bpy.context.active_object
            wrightCell(obj,f)
        return {"FINISHED"}

classs = (
    PT_Panel,
    OT_Export,
    OT_ExportSelect,
    OT_OpenTexture,
    OT_OpenNormal,
    OT_OpenSpecular,
    OT_CellExport,
    MaterialProps,
    
)
def register():
    for c in classs:
        bpy.utils.register_class(c)
    bpy.types.Object.MaterialProps = PointerProperty(type=MaterialProps)
    #bpy.types.Scene.armatures = CollectionProperty(type=ItemSet)
    

def unregister():
    for c in classs:
        bpy.utils.unregister_class(c)
    del bpy.types.Object.MaterialProps
    #del bpy.types.Scene.armatures


if __name__ == "__main__":
    register()


def test():
    obj = bpy.context.active_object
    msh = obj.to_mesh()

    print("num of vertices:", len(msh.vertices))
    for vt in msh.vertices:
        print("vertex index:{0:2} co:{1} normal:{2}".format(vt.index, vt.co, vt.normal))
        for w in vt.groups:
            print("group: %d weight: %f"%(w.group,w.weight))
        
    print("Weight: ")
    for gr in obj.vertex_groups:
        #print(type(gr))
        print(gr.name)#+" Weight:{0:2}".format(gr.weight(1)))

    if True:
        print("num of polygons:", len(msh.polygons))
        for pl in msh.polygons:
            print("polygon index:{0:2} ".format(pl.index), end="")
            print("vertices:", end="")
            for vi in pl.vertices:
                print("{0:2}, ".format(vi), end="")
            print("")
        for pl in msh.polygons:
            print("polygon index:{0:2} ".format(pl.index))
            print(" > loops:", end="")
            print(" total:", pl.loop_total, end="")
            print(" start:", pl.loop_start, end="")
            print(" indices:", end="")
            for lp in pl.loop_indices:
                print("{0:2}, ".format(lp), end="")
            print("")
        

    print("Triangle!!(it maybe use...)")
    msh.calc_loop_triangles()
    for tr in msh.loop_triangles:
        print("index:{0} ".format(tr.index))
        
        print("vertex:",end="")
        for v in tr.vertices:
            print("{0:2}".format(v),end="")
            
        print("\nloops:  ",end="")
        for v in tr.loops:
            print("%d "%v,end="")
            
        print("")
        
        
    print("uvs: ",len(msh.uv_layers))
    for ly in msh.uv_layers:
        print(ly.name+":")
        for idx, dat in enumerate(msh.uv_layers[ly.name].data):
            print("  {0}:{1}".format(idx, dat.uv))
        print("")
        
        
def DataTest():
    o = bpy.context.active_object
    
    m = build_mesh(o)
            
    print("num vertex: %d"%len(m.vertList)) ## 41 ????
    print("num polygon: %d"%m.numPolygon)

    for v in m.vertList:
        print("index: {}\npos {}".format(v.real_index,v.pos))
        print("uv: {}\n".format(v.uv))
        
    print("!!!")
        
def out_test():
    o = bpy.context.active_object
    path = "G:\\Programing\\python\\TkmExporter\\test.tkm"
    with open(path,"wb") as f:
        build_data(o,f)
        
#out_test()
#DataTest()
#test()

