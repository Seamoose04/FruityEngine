import math
from .transform import build_transform_json, round_vec


def build_camera_json(obj, precision: int = 3) -> dict:
    """
    Build the JSON representation of a camera GameObject.
    Applies a -90 degree X rotation offset to compensate for Blender
    cameras pointing down -Y vs the engine's -Z forward convention.
    """
    cam = obj.data
    transform = build_transform_json(obj, use_local=False, precision=precision)

    # Cameras in Blender point down -Y; engine expects -Z forward.
    # Compensate with a -90 degree offset on the X axis.
    transform["rotation"][0] -= 90.0

    return {
        "type": "GameObject",
        "properties": {
            "Transform": transform,
            "Camera": {
                "fov":  round_vec([math.degrees(cam.angle)], precision)[0],
                "near": round(cam.clip_start, precision),
                "far":  round(cam.clip_end, precision),
            },
        },
    }
