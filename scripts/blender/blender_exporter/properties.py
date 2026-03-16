import bpy


FRUITY_MATERIAL_TYPES = [
    ("NONE",     "Not Set",          "Material type has not been configured"),
    ("FLAT",     "FlatMaterial",     "Unlit flat color material"),
    ("EMISSIVE", "EmissiveMaterial", "Emissive/glowing material"),
]


def register():
    bpy.types.Material.fruity_type = bpy.props.EnumProperty(
        name="Type",
        description="FruityEngine material type",
        items=FRUITY_MATERIAL_TYPES,
        default="NONE",
    )
    bpy.types.Material.fruity_intensity = bpy.props.FloatProperty(
        name="Intensity",
        description="Emissive intensity (HDR, values above 1.0 drive bloom)",
        default=1.0,
        min=0.0,
        soft_max=10.0,
    )


def unregister():
    del bpy.types.Material.fruity_type
    del bpy.types.Material.fruity_intensity


# ---------------------------------------------------------------------------
# To add new properties in future (e.g. object types for spawn points):
#
#   FRUITY_OBJECT_TYPES = [("NONE", ...), ("SPAWN_POINT", ...), ...]
#
#   In register():
#       bpy.types.Object.fruity_type = bpy.props.EnumProperty(...)
#
#   In unregister():
#       del bpy.types.Object.fruity_type
# ---------------------------------------------------------------------------
