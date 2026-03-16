import bpy


class FRUITY_PT_material(bpy.types.Panel):
    """FruityEngine material settings — lives in the material properties tab."""
    bl_label       = "FruityEngine"
    bl_idname      = "FRUITY_PT_material"
    bl_space_type  = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context     = "material"

    def draw(self, context):
        layout = self.layout
        mat = context.material

        if mat is None:
            layout.label(text="No material selected")
            return

        layout.prop(mat, "fruity_type")

        if mat.fruity_type == "NONE":
            layout.label(text="⚠ Material type not set", icon="ERROR")
        elif mat.fruity_type == "EMISSIVE":
            layout.prop(mat, "fruity_intensity")


class FRUITY_PT_viewport(bpy.types.Panel):
    """FruityEngine export button — lives in the N panel (View3D sidebar)."""
    bl_label       = "FruityEngine"
    bl_idname      = "FRUITY_PT_viewport"
    bl_space_type  = "VIEW_3D"
    bl_region_type = "UI"
    bl_category    = "FruityEngine"

    def draw(self, context):
        layout = self.layout
        layout.label(text="FruityEngine Exporter")
        layout.operator("fruity.export", text="Export Scene")


# ---------------------------------------------------------------------------
# To add a new panel in future (e.g. object type settings):
#
#   class FRUITY_PT_object(bpy.types.Panel):
#       bl_label       = "FruityEngine"
#       bl_idname      = "FRUITY_PT_object"
#       bl_space_type  = "PROPERTIES"
#       bl_region_type = "WINDOW"
#       bl_context     = "object"
#
#       def draw(self, context):
#           ...
#
# Then add it to CLASSES in __init__.py.
# ---------------------------------------------------------------------------


CLASSES = [
    FRUITY_PT_material,
    FRUITY_PT_viewport,
]
