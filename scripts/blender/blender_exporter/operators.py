import bpy
import os
import json

from .export import build_object_json, build_chunk_json, build_camera_json, build_light_json


def write_json(path: str, data) -> None:
    with open(path, "w") as f:
        json.dump(data, f, indent=4)


class FRUITY_OT_export(bpy.types.Operator):
    bl_idname     = "fruity.export"
    bl_label      = "Export FruityEngine Scene"
    bl_description = "Export the current scene for FruityEngine"

    directory: bpy.props.StringProperty(
        name="Assets Folder",
        description="Path to your Game/assets/ folder",
        subtype="DIR_PATH",
    )
    scene_name: bpy.props.StringProperty(
        name="Scene Name",
        description="Name of the scene to export (e.g. test_scene)",
        default="my_scene",
    )
    precision: bpy.props.IntProperty(
        name="Precision",
        description="Decimal places to round floats in exported JSON",
        default = 3,
        min=0,
        max=6
    )

    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {"RUNNING_MODAL"}

    def execute(self, context):
        assets_dir = bpy.path.abspath(self.directory)

        # ------------------------------------------------------------------
        # Directory structure
        # ------------------------------------------------------------------
        dirs = {
            "meshes":  os.path.join(assets_dir, "meshes"),
            "chunks":  os.path.join(assets_dir, "chunks"),
            "objects": os.path.join(assets_dir, "scenes", self.scene_name, "objects"),
            "scene":   os.path.join(assets_dir, "scenes", self.scene_name),
        }
        for path in dirs.values():
            os.makedirs(path, exist_ok=True)

        # ------------------------------------------------------------------
        # Collect objects
        # ------------------------------------------------------------------
        mesh_objects   = [o for o in context.scene.objects if o.type == "MESH"]
        camera_objects = [o for o in context.scene.objects if o.type == "CAMERA"]
        light_objects = [o for o in context.scene.objects if o.type == "LIGHT"]

        # ------------------------------------------------------------------
        # Validate materials (all mesh objects, regardless of hierarchy)
        # ------------------------------------------------------------------
        errors = []
        for obj in mesh_objects:
            if not obj.material_slots:
                errors.append(f"  '{obj.name}' has no material assigned")
                continue
            mat = obj.material_slots[0].material
            if mat is None or mat.fruity_type == "NONE":
                errors.append(f"  '{obj.name}' material type is not set")
        if errors:
            msg = "Export aborted — fix materials:\n" + "\n".join(errors)
            self.report({"ERROR"}, msg)
            print(f"[FruityEngine] {msg}")
            return {"CANCELLED"}

        # ------------------------------------------------------------------
        # Determine which mesh data blocks are shared (linked duplicates)
        # ------------------------------------------------------------------
        mesh_data_users: dict[str, list] = {}
        for obj in mesh_objects:
            mesh_data_users.setdefault(obj.data.name, []).append(obj)
        is_chunk = {name: len(users) > 1 for name, users in mesh_data_users.items()}

        # ------------------------------------------------------------------
        # Export each unique mesh as .obj
        # ------------------------------------------------------------------
        exported_meshes: set[str] = set()
        for obj in mesh_objects:
            mesh_name = obj.data.name
            if mesh_name in exported_meshes:
                continue
            exported_meshes.add(mesh_name)

            obj_path = os.path.join(dirs["meshes"], f"{mesh_name}.obj")
            bpy.ops.object.select_all(action="DESELECT")
            obj.select_set(True)
            context.view_layer.objects.active = obj

            bpy.ops.wm.obj_export(
                filepath=obj_path,
                export_selected_objects=True,
                forward_axis="NEGATIVE_Z",
                up_axis="Y",
                export_materials=False,
                export_triangulated_mesh=True,
                apply_modifiers=False,
                apply_transform=False,
                export_object_groups=False,
            )
            print(f"[FruityEngine] Exported mesh: {obj_path}")

        # ------------------------------------------------------------------
        # Write chunk JSON for shared mesh data blocks
        # ------------------------------------------------------------------
        for mesh_name, users in mesh_data_users.items():
            if not is_chunk[mesh_name]:
                continue
            chunk_path = os.path.join(dirs["chunks"], f"{mesh_name}.json")
            write_json(chunk_path, build_chunk_json(users[0], precision=self.precision))
            print(f"[FruityEngine] Exported chunk: {chunk_path}")

        # ------------------------------------------------------------------
        # Write per-object JSON (root objects only — children are inlined)
        # ------------------------------------------------------------------
        scene_imports = []
        root_mesh_objects = [o for o in mesh_objects if o.parent is None]

        for obj in root_mesh_objects:
            obj_filename = f"{obj.name}.json"
            obj_path = os.path.join(dirs["objects"], obj_filename)
            write_json(obj_path, build_object_json(obj, is_chunk, precision=self.precision))
            scene_imports.append(f"objects/{obj_filename}")
            print(f"[FruityEngine] Exported object: {obj_path}")

        # ------------------------------------------------------------------
        # Camera
        # ------------------------------------------------------------------
        if len(camera_objects) == 0:
            self.report({"WARNING"}, "No camera found — skipping")
            print("[FruityEngine] WARNING: No camera in scene")
        else:
            if len(camera_objects) > 1:
                self.report({"WARNING"}, f"Multiple cameras found — using '{camera_objects[0].name}'")
                print(f"[FruityEngine] WARNING: Multiple cameras, using '{camera_objects[0].name}'")

            cam_filename = f"{camera_objects[0].name}.json"
            cam_path = os.path.join(dirs["objects"], cam_filename)
            write_json(cam_path, build_camera_json(camera_objects[0], precision=self.precision))
            scene_imports.append(f"objects/{cam_filename}")
            print(f"[FruityEngine] Exported camera: {cam_path}")

        # ------------------------------------------------------------------
        # Lights
        # ------------------------------------------------------------------
        for obj in light_objects:
            if obj.data.type not in ("POINT", "SUN"):
                print(f"[FruityEngine] Skipping unsupported light '{obj.name}' (type: {obj.data.type})")
                continue
            light_filename = f"{obj.name}.json"
            light_path = os.path.join(dirs["objects"], light_filename)
            write_json(light_path, build_light_json(obj, precision=self.precision))
            scene_imports.append(f"objects/{light_filename}")
            print(f"[FruityEngine] Exported light: {light_path}")

        # ------------------------------------------------------------------
        # Default post process stack
        # ------------------------------------------------------------------
        post_process_json = {
            "type": "GameObject",
            "properties": {
                "PostProcessVolume": {
                    "effects": [
                        {"id": "scene",  "type": "Scene"},
                        {"id": "output", "type": "RenderToScreen", "input": "scene"},
                    ]
                }
            }
        }
        post_path = os.path.join(dirs["objects"], "postprocess.json")
        write_json(post_path, post_process_json)
        scene_imports.append("objects/postprocess.json")
        print(f"[FruityEngine] Exported post process stack: {post_path}")

        # ------------------------------------------------------------------
        # scene.json
        # ------------------------------------------------------------------
        scene_path = os.path.join(dirs["scene"], "scene.json")
        write_json(scene_path, {"objects": scene_imports})
        print(f"[FruityEngine] Exported scene: {scene_path}")

        self.report({"INFO"}, f"Export complete: {len(root_mesh_objects)} root object(s) to '{self.scene_name}'")
        return {"FINISHED"}

    def draw(self, context):
        self.layout.prop(self, "scene_name")
        self.layout.prop(self, "precision")


CLASSES = [
    FRUITY_OT_export,
]
