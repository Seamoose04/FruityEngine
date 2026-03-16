from .transform import build_transform_json, round_vec


def get_base_color(mat):
    if mat.use_nodes:
        principled = next((n for n in mat.node_tree.nodes if n.type == 'BSDF_PRINCIPLED'), None)
        if principled:
            return list(principled.inputs['Base Color'].default_value[:3])
    return list(mat.diffuse_color[:3])

def build_material_json(mat, precision: int = 3) -> dict:
    """Convert a Blender material (with FruityEngine props) to engine material JSON."""
    color = round_vec(get_base_color(mat), precision)  # drop alpha
    if mat.fruity_type == "FLAT":
        return {"type": "FlatMaterial", "color": color}
    elif mat.fruity_type == "EMISSIVE":
        return {"type": "EmissiveMaterial", "color": color, "intensity": round(mat.fruity_intensity, precision)}
    else:
        raise ValueError(f"[FruityEngine] Unhandled material type: {mat.fruity_type}")


def build_object_json(obj, is_chunk: dict, use_local: bool = False, precision: int = 3) -> dict:
    """
    Recursively build the JSON representation of a mesh GameObject.

    is_chunk: dict mapping mesh data block name → bool (shared = True)
    use_local: True for children (local-space transform), False for roots (world-space)
    """
    mesh_name = obj.data.name
    mat = obj.material_slots[0].material
    transform = build_transform_json(obj, use_local)

    # Recurse into Blender children that are meshes
    children = [
        build_object_json(child, is_chunk, use_local=True)
        for child in obj.children
        if child.type == "MESH"
    ]

    if is_chunk[mesh_name]:
        # Instance: $import the chunk template, override the transform
        result = {
            "$import": f"{mesh_name}.json",
            "$override": {
                "properties": {
                    "Transform": {
                        "!position": transform["position"],
                        "!rotation": transform["rotation"],
                        "!scale":    transform["scale"],
                    }
                },
            },
        }
        if children:
            # children doesn't exist in the chunk template, so no ! needed
            result["$override"]["children"] = children
    else:
        # Unique object: inline everything
        result = {
            "type": "GameObject",
            "properties": {
                "Transform": transform,
                "MeshRenderer": {
                    "mesh": f"{mesh_name}.obj",
                    "material": build_material_json(mat, precision),
                },
            },
        }
        if children:
            result["children"] = children

    return result


def build_chunk_json(obj) -> dict:
    """
    Build the chunk template JSON for a shared mesh data block.
    Uses identity transform — instances override this at export time.
    """
    mesh_name = obj.data.name
    mat = obj.material_slots[0].material
    return {
        "type": "GameObject",
        "properties": {
            "Transform": {
                "position": [0, 0, 0],
                "rotation": [0, 0, 0],
                "scale":    [1, 1, 1],
            },
            "MeshRenderer": {
                "mesh": f"{mesh_name}.obj",
                "material": build_material_json(mat, precision),
            },
        },
    }
