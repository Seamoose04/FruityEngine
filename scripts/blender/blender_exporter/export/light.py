from .transform import build_transform_json, round_vec


def build_light_json(obj, precision: int = 3) -> dict:
    light = obj.data

    if light.type == "SUN":
        return {
            "type": "GameObject",
            "properties": {
                "AmbientLightProperty": {
                    "color":     round_vec(list(light.color[:3]), precision),
                    "intensity": round(light.energy, precision),
                },
            },
        }

    elif light.type == "POINT":
        transform = build_transform_json(obj, use_local=False, precision=precision)
        return {
            "type": "GameObject",
            "properties": {
                "Transform": transform,
                "PointLightProperty": {
                    "color":     round_vec(list(light.color[:3]), precision),
                    "intensity": round(light.energy, precision),
                },
            },
        }

    else:
        raise ValueError(f"[FruityEngine] Unsupported light type '{light.type}' on '{obj.name}' — only POINT and SUN are supported")
