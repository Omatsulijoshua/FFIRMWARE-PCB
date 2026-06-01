# Manufacturing Outputs Plan

## CAD Outputs Required For PCB Fabrication

When schematic and layout are captured in CAD, release package shall include:

- Gerber or ODB++ fabrication files.
- NC drill files.
- IPC-356 netlist if supported.
- Fabrication drawing.
- Assembly drawing top and bottom.
- Pick-and-place files.
- Full BOM with manufacturer part numbers and approved alternates.
- Schematic PDF.
- PCB layout PDF.
- 3D STEP file.
- Test point map.
- Programming fixture requirements.
- Controlled impedance notes if USB is included.

## Fabrication Drawing Requirements

Fabrication drawing shall specify:

- Board dimensions and tolerances.
- Layer stackup and copper weights.
- Material and Tg.
- Surface finish, recommended ENIG or lead-free HASL depending on cost and assembly needs.
- Solder mask color.
- Silkscreen color.
- Minimum trace/space.
- Minimum drill.
- Slots and cutouts.
- Creepage/clearance critical notes.
- UL marking requirement if applicable.
- Panelization requirements.

## Assembly Drawing Requirements

Assembly drawing shall specify:

- Component outlines and reference designators.
- Polarity marks.
- Connector orientation.
- No-pop/DNP components.
- Safety-critical components.
- Torque or mechanical notes for heatsinks if triac uses external heatsink.
- Keepout zones.
- Thermal cutoff harness or placement notes.

## Preliminary DNP Options

| DNP Part | Purpose |
| --- | --- |
| Optional EMI filter components | Populate after EMC testing if needed. |
| Optional sensor LDO | Populate if buck noise affects sensor stability. |
| Optional USB-C service port | Product decision dependent. |
| Optional zero-cross detector | Future diagnostics/control. |
| Optional fan current sense | Populate if tach feedback unavailable or extra diagnostics needed. |
| Optional snubber values | Tune after load/EMI validation. |

## Factory Test Requirements

The PCB must support:

- Power rail verification.
- ESP32 programming.
- BLE advertising check.
- WiFi radio check.
- Sensor bus check.
- Fan PWM/tach check.
- Heater enable signal check without unsafe heater energization.
- Status LED/buzzer check.
- NVS serial number programming.
- Fault injection checks where practical.

## Safety-Critical Manufacturing Controls

- Safety components must be locked in BOM and not substituted by assembler without written approval.
- AC connector, fuse, MOV, optotriac, triac, thermal cutoff, and AC-DC module require approved vendor control.
- Assembly inspection must verify creepage slots are clean and not bridged by solder, flux residue, wire strands, labels, or conformal coating defects.
- High-pot and ground continuity tests must be defined at appliance assembly level.
- Firmware loaded at factory must match hardware revision and region-specific power configuration.

## EVT PCB Bring-Up Plan

1. Visual inspection before power.
2. Low-voltage isolated supply test without heater connected.
3. 3.3 V rail test.
4. ESP32 programming and boot log.
5. Sensor I2C scan.
6. Fan output test with current-limited supply.
7. Heater driver signal test with isolated low-energy fixture.
8. Full AC heater test only in protected appliance test setup.
9. Thermal camera review of regulator, triac, connectors, and power traces.
10. BLE/WiFi range spot check.
11. Fault injection: sensor disconnect, fan stall, simulated overtemperature.

## Release Naming

Recommended board revision format:

```text
SDHS-MAIN-PCB-REV-A
SDHS-MAIN-PCB-REV-B
```

Firmware should read or be configured with matching hardware revision:

```text
HW_MAIN_REV_A
```

