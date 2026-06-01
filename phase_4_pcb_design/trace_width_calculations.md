# Trace Width Calculations

## Assumptions

Trace widths are preliminary using conservative IPC-2221-style planning values. Final widths should be recalculated in the PCB CAD tool using the actual copper weight, layer, allowable temperature rise, board thickness, copper thickness tolerance, ambient temperature, and whether copper pours or thermal reliefs are used.

Baseline assumptions:

- External copper: 1 oz, 35 um.
- Internal copper: 1 oz, 35 um.
- Preferred temperature rise for power traces: 10 C to 20 C.
- High-current heater routing should use copper pours, short paths, and/or off-board wiring where possible.

## Current Estimates

| Load | Voltage | Power | Current |
| --- | --- | --- | --- |
| 300 W heater at 120 VAC | 120 VAC | 300 W | 2.5 A |
| 500 W heater at 120 VAC | 120 VAC | 500 W | 4.17 A |
| 800 W heater at 120 VAC | 120 VAC | 800 W | 6.67 A |
| 300 W heater at 230 VAC | 230 VAC | 300 W | 1.30 A |
| 500 W heater at 230 VAC | 230 VAC | 500 W | 2.17 A |
| 800 W heater at 230 VAC | 230 VAC | 800 W | 3.48 A |
| 24 V fan, 5 W | 24 VDC | 5 W | 0.21 A |
| 24 V fan, 20 W | 24 VDC | 20 W | 0.83 A |
| 12 V fan, 20 W | 12 VDC | 20 W | 1.67 A |
| 3.3 V logic peak | 3.3 VDC | 2 W planning | 0.61 A |

## Recommended Minimum External Trace Widths

These are layout planning targets for 1 oz external copper.

| Net Class | Current | Recommended Minimum Width | Preferred Implementation |
| --- | --- | --- | --- |
| Low-speed signal | < 50 mA | 0.15 mm to 0.20 mm | Standard signal traces. |
| I2C / GPIO to connectors | < 50 mA | 0.20 mm to 0.25 mm | Add ESD/series resistance for harnesses. |
| 3.3 V logic distribution | 0.5 A to 1 A | 0.50 mm to 1.00 mm | Use pours where practical. |
| 12 V auxiliary distribution | 1 A to 2 A | 1.00 mm to 2.00 mm | Use pours and short routes. |
| Fan power | 1 A to 3 A startup | 1.50 mm to 3.00 mm | Use pours, connector near supply. |
| Heater AC 2 A to 4 A | 2 A to 4 A | 2.00 mm to 4.00 mm | Use pours, short routes, thermal review. |
| Heater AC 4 A to 7 A | 4 A to 7 A | 4.00 mm to 8.00 mm or more | Prefer off-board wiring or 2 oz copper. |

## Heater Routing Recommendation

For a consumer appliance heater, do not rely on long narrow PCB traces for the full heater current. Preferred architecture:

- Bring protected AC onto PCB.
- Switch through triac/relay/SSR region with short, wide copper.
- Route heater power through robust connector or wire harness.
- Keep high-current PCB path short.
- Use 2 oz copper or reinforced copper if 120 V / 800 W current is required on PCB.

## Example Calculations

### 500 W Heater At 120 VAC

Current:

```text
I = P / V
I = 500 W / 120 V
I = 4.17 A
```

Recommendation:

- Use at least 4 mm external copper width for short traces on 1 oz copper.
- Prefer copper pour instead of simple trace.
- Validate temperature rise in thermal test.
- Keep triac thermal dissipation separate from trace heating.

### 800 W Heater At 120 VAC

Current:

```text
I = 800 W / 120 V
I = 6.67 A
```

Recommendation:

- Avoid long PCB routing.
- Use short copper pours, 2 oz copper, or off-board wiring.
- Select connector and fuse with proper current and temperature rating.
- Perform abnormal-operation thermal validation.

### 20 W Fan At 12 V

Current:

```text
I = 20 W / 12 V
I = 1.67 A
```

Recommendation:

- Use 2 mm external trace or polygon pour.
- Size for startup current, not only running current.
- Add bulk capacitance near connector.

## Via Current Planning

Approximate conservative planning:

- 0.3 mm finished via: use several vias in parallel for more than 0.5 A.
- 0.5 mm finished via: still use multiple vias for power transitions.
- Avoid via transitions for heater current when possible.
- Use via arrays for fan power and regulator power pours.

## Clearance Planning For High Voltage Nets

Preliminary board-level spacing targets:

| Net Relationship | Minimum Planning Target |
| --- | --- |
| AC line to AC neutral, fused side | 2.5 mm or more, final by standard. |
| Primary AC to SELV low voltage | 6.4 mm or more for universal mains planning. |
| Across isolation barrier | Follow AC-DC module/opto certification and PCB creepage rules. |
| Mains pads to low-voltage test pads | Keep physically separated and guarded. |

These values are planning numbers. Final requirements depend on standard, working voltage, pollution degree, material group, coating, altitude, insulation class, and certification lab interpretation.

