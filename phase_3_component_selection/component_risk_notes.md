# Component Risk And Sourcing Notes

## Key Risks

| Area | Risk | Mitigation |
| --- | --- | --- |
| Heater | Off-the-shelf PTC heater geometry may not match enclosure airflow. | Treat DBK HRP as a reference family; engage heater supplier for custom wattage, dimensions, and approvals. |
| Fan | Airflow targets depend heavily on duct losses, outlet count, and noise limits. | Build airflow test rig before final fan lock. Prefer tach feedback. |
| AC switching | Triac switching needs thermal, EMI, snubber, isolation, and certification review. | Use zero-cross slow-cycle control first. Validate with compliance pre-scan. |
| Thermal cutoff | Functional temperature cannot be selected from paper architecture alone. | Choose after thermal mapping under blocked-airflow, fan-failure, and high-ambient tests. |
| Sensor contamination | Humidity sensor may drift from lint, dust, oils, and moisture exposure. | Place behind protective airflow membrane/filter geometry; design service strategy. |
| ESP32 RF | Poor module placement can hurt WiFi/BLE performance. | Follow Espressif antenna keepout and enclosure material guidance. |
| Power budget | IRM-10-12 may be too small if fan power increases. | Keep IRM-20/IRM-30 footprint option or separate fan supply decision before PCB layout. |
| BOM lifecycle | Exact stock and pricing are unstable. | Use authorized distributors and define approved alternates before pilot build. |

## Recommended Approved Alternates

| Function | Primary | Alternate | Notes |
| --- | --- | --- | --- |
| MCU module | ESP32-S3-WROOM-1-N16R8 | ESP32-S3-WROOM-1-N8R8 | Lower flash/PSRAM option if firmware footprint permits. |
| RH/temp sensor | SHT41-AD1B-R2 | SHT31/SHT35 family | Alternate Sensirion family if package or supply availability requires. |
| Heater-zone temp | TMP117AIDRVR | NTC thermistor plus ADC | NTC may be better for hotter heater-zone placement. |
| AC-DC supply | IRM-10-12 | IRM-20-12 / IRM-30-12 | Use larger model if fan or indicators need more power. |
| Buck regulator | TPS54302DDCR | TPS54202/TPS562201 class | Keep layout/package impact in mind. |
| Optotriac | MOC3063M | MOC3062M or equivalent approved optotriac | Verify trigger current and safety approvals. |
| Triac | BTA16-600BW | BTA16-800BW / T1635 series | Choose voltage/current rating after AC region decision. |
| MOV | V14E275P | Region-specific Littelfuse UltraMOV alternate | Rating depends on 120 V, 230 V, or universal input strategy. |
| Thermal cutoff | Cantherm thermal cutoff | SEFUSE / Thermik equivalent | Must be agency-approved and thermally validated. |

## Sourcing Rules For Production

- Buy safety-related parts only through authorized distributors or direct manufacturer channels.
- Do not substitute thermal cutoffs, fuses, MOVs, optocouplers, or AC switching devices without engineering approval.
- Require manufacturer datasheets, certificates, and material compliance documents for certification package.
- Lock approved vendor list before pilot production.
- Track component lifecycle status before tooling.
- Keep at least one approved alternate for commodity passives, connectors, sensors, regulators, and protection parts.

## Compliance Notes

- The ESP32-S3 module reduces RF design risk but does not automatically certify the finished appliance. Final enclosure, antenna keepout, power supply, cabling, and firmware behavior still affect FCC/CE testing.
- AC heater switching and power entry design must be reviewed against the target appliance safety standard.
- Creepage and clearance are PCB design requirements, not component-selection details alone.
- Heater and enclosure plastics must be selected with thermal and flammability ratings aligned to certification strategy.
- RoHS and REACH documentation should be collected during component approval, not at the end of the project.

## Decisions Needed Before Phase 4

1. Confirm first-launch AC region: 120 V, 230 V, or universal.
2. Confirm target heater power class.
3. Confirm whether the first PCB must support both 12 V and 24 V fans.
4. Confirm whether USB-C service/debug is included.
5. Confirm whether humidity sensor is main-board mounted or on a small airflow daughterboard.
6. Confirm whether the heater should be an off-the-shelf PTC assembly or supplier-custom assembly.

