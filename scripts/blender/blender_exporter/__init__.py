bl_info = {
    "name":        "FruityEngine Exporter",
    "author":      "Seamus",
    "version":     (0, 2),
    "blender":     (3, 0, 0),
    "category":    "Import-Export",
    "description": "Exports scenes for FruityEngine",
}

from . import properties
from .operators import CLASSES as OPERATOR_CLASSES
from .panels    import CLASSES as PANEL_CLASSES

ALL_CLASSES = OPERATOR_CLASSES + PANEL_CLASSES


def register():
    import bpy
    properties.register()
    for cls in ALL_CLASSES:
        bpy.utils.register_class(cls)


def unregister():
    import bpy
    for cls in reversed(ALL_CLASSES):
        bpy.utils.unregister_class(cls)
    properties.unregister()
