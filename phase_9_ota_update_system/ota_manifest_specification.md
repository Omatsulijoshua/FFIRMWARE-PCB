# OTA Manifest Specification

## Format

The OTA manifest is JSON.

```json
{
  "product": "sdhs",
  "version": "1.2.0",
  "build": "2026.05.30.1",
  "hardwareMin": "HW_MAIN_REV_A",
  "hardwareMax": "HW_MAIN_REV_Z",
  "imageUrl": "https://updates.example.com/sdhs/1.2.0/firmware.bin",
  "imageSize": 1572864,
  "sha256": "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef",
  "releaseNotes": "Improves drying completion prediction.",
  "required": false
}
```

## Fields

| Field | Required | Meaning |
| --- | --- | --- |
| `product` | Yes | Must equal `sdhs`. |
| `version` | Yes | Semantic firmware version. |
| `build` | Yes | Build identifier. |
| `hardwareMin` | Yes | Earliest compatible hardware revision. |
| `hardwareMax` | Yes | Latest compatible hardware revision. |
| `imageUrl` | Yes | HTTPS URL to firmware binary. |
| `imageSize` | Yes | Expected image size in bytes. |
| `sha256` | Yes | Expected image SHA-256 as lowercase hex. |
| `releaseNotes` | No | User-facing summary. |
| `required` | No | Whether app should strongly prompt update. |

## Validation Rules

- Product must match.
- Version must be newer than current production firmware unless factory override is enabled.
- Hardware revision must fall within compatibility range.
- URL must use HTTPS in production.
- SHA-256 must be exactly 64 hex characters.
- Image size must be nonzero and fit available OTA partition.

## Compatibility Policy

Hardware compatibility is intentionally conservative. A firmware image must explicitly support the current hardware revision before installation.

## Version Policy

Use semantic versions:

```text
MAJOR.MINOR.PATCH
```

- MAJOR: incompatible behavior or hardware contract changes.
- MINOR: new features or compatible protocol additions.
- PATCH: fixes and minor improvements.

