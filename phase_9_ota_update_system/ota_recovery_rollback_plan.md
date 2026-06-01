# OTA Recovery And Rollback Plan

## Rollback Strategy

The partition table contains factory, `ota_0`, and `ota_1` app slots. Rollback support is enabled through `CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE`.

After booting a newly installed OTA image:

1. Firmware checks the running partition state.
2. If state is pending verification, firmware performs safety self-test.
3. If self-test passes, firmware marks image valid.
4. If self-test fails, firmware marks image invalid and reboots to previous valid image.

## Recovery Behaviors

| Failure | Recovery |
| --- | --- |
| Manifest download failure | Remain on current firmware and report failure. |
| Manifest invalid | Reject update before image download. |
| Image download failure | Remain on current firmware and report failure. |
| Power loss during download | Boot current valid image. |
| Power loss after staging | Bootloader selects valid/pending image according to ESP-IDF OTA state. |
| New image self-test failure | Mark invalid and rollback. |
| WiFi unavailable after update | BLE remains available for recovery/provisioning. |

## Recovery Mode

Recovery mode is a BLE-visible state used when:

- WiFi credentials are invalid after repeated failures.
- OTA failed repeatedly.
- New firmware rolled back.
- App requests firmware recovery information.

Recovery mode shall allow:

- Read firmware version and boot partition state.
- Clear WiFi credentials.
- Provision WiFi again.
- Retry OTA from known-good manifest.
- Factory reset after user confirmation.

## User Experience

The app should show:

- Update available.
- Downloading.
- Installing.
- Rebooting.
- Validating.
- Success.
- Failed with reason.
- Rolled back to previous version.

The app should never ask the user to unplug the device during installation.

