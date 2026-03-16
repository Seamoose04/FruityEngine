import math


def round_vec(v: list, decimals: int = 3) -> list:
    return [round(x, decimals) for x in v]


def build_transform_json(obj, use_local: bool = False, precision: int = 3) -> dict:
    """
    Extract position/rotation/scale from a Blender object and convert
    from Blender's Z-up coordinate system to the engine's Y-up system.

    use_local=True  → matrix_local  (relative to parent, for children)
    use_local=False → matrix_world  (absolute, for root objects)
    """
    mat = obj.matrix_local if use_local else obj.matrix_world
    loc, quat, scale = mat.decompose()
    rot = quat.to_euler('XYZ')

    return {
        "position": round_vec([loc.x,              loc.z,               -loc.y             ], precision),
        "rotation": round_vec([math.degrees(rot.x), math.degrees(rot.z), -math.degrees(rot.y)], precision),
        "scale":    round_vec([scale.x,             scale.z,              scale.y            ], precision),
    }
