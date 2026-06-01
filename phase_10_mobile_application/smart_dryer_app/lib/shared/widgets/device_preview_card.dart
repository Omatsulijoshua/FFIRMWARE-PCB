import 'package:flutter/material.dart';

class DevicePreviewCard extends StatelessWidget {
  const DevicePreviewCard({super.key});

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    return AspectRatio(
      aspectRatio: 1.16,
      child: DecoratedBox(
        decoration: BoxDecoration(
          color: colorScheme.surface,
          borderRadius: BorderRadius.circular(8),
          border: Border.all(color: colorScheme.outlineVariant),
        ),
        child: Stack(
          children: [
            Positioned.fill(
              child: CustomPaint(
                painter: _AirflowPainter(colorScheme),
              ),
            ),
            Align(
              alignment: Alignment.center,
              child: Container(
                width: 172,
                height: 206,
                decoration: BoxDecoration(
                  color: const Color(0xfff7fbfb),
                  borderRadius: BorderRadius.circular(28),
                  border: Border.all(color: const Color(0xffd4e6e7)),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black.withValues(alpha: 0.08),
                      blurRadius: 24,
                      offset: const Offset(0, 14),
                    ),
                  ],
                ),
                child: Column(
                  children: [
                    const SizedBox(height: 18),
                    Container(
                      width: 76,
                      height: 8,
                      decoration: BoxDecoration(
                        color: const Color(0xff174b56),
                        borderRadius: BorderRadius.circular(99),
                      ),
                    ),
                    const Spacer(),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        _Outlet(color: colorScheme.primary),
                        const SizedBox(width: 12),
                        const _Outlet(color: Color(0xfff26f4f)),
                      ],
                    ),
                    const Spacer(),
                    Container(
                      width: 110,
                      height: 34,
                      decoration: BoxDecoration(
                        color: const Color(0xff06313c),
                        borderRadius: BorderRadius.circular(18),
                      ),
                      child: const Center(
                        child: Icon(
                          Icons.air,
                          color: Colors.white,
                          size: 18,
                        ),
                      ),
                    ),
                    const SizedBox(height: 18),
                  ],
                ),
              ),
            ),
            const Positioned(
              left: 22,
              bottom: 18,
              child: _MetricPill(
                icon: Icons.thermostat,
                label: '48 C',
                color: Color(0xfff26f4f),
              ),
            ),
            Positioned(
              right: 22,
              top: 18,
              child: _MetricPill(
                icon: Icons.water_drop,
                label: '42%',
                color: colorScheme.primary,
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class _Outlet extends StatelessWidget {
  const _Outlet({required this.color});

  final Color color;

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 54,
      height: 84,
      decoration: BoxDecoration(
        color: color.withValues(alpha: 0.10),
        borderRadius: BorderRadius.circular(26),
        border: Border.all(color: color.withValues(alpha: 0.28)),
      ),
      child: Icon(Icons.keyboard_arrow_up, color: color, size: 32),
    );
  }
}

class _MetricPill extends StatelessWidget {
  const _MetricPill({
    required this.icon,
    required this.label,
    required this.color,
  });

  final IconData icon;
  final String label;
  final Color color;

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 8),
      decoration: BoxDecoration(
        color: Colors.white,
        borderRadius: BorderRadius.circular(99),
        boxShadow: [
          BoxShadow(
            color: Colors.black.withValues(alpha: 0.08),
            blurRadius: 18,
            offset: const Offset(0, 8),
          ),
        ],
      ),
      child: Row(
        mainAxisSize: MainAxisSize.min,
        children: [
          Icon(icon, color: color, size: 16),
          const SizedBox(width: 6),
          Text(
            label,
            style: const TextStyle(fontWeight: FontWeight.w700),
          ),
        ],
      ),
    );
  }
}

class _AirflowPainter extends CustomPainter {
  const _AirflowPainter(this.colorScheme);

  final ColorScheme colorScheme;

  @override
  void paint(Canvas canvas, Size size) {
    final coolPaint = Paint()
      ..color = colorScheme.primary.withValues(alpha: 0.16)
      ..style = PaintingStyle.stroke
      ..strokeWidth = 16
      ..strokeCap = StrokeCap.round;
    final warmPaint = Paint()
      ..color = const Color(0xfff26f4f).withValues(alpha: 0.18)
      ..style = PaintingStyle.stroke
      ..strokeWidth = 12
      ..strokeCap = StrokeCap.round;

    final coolPath = Path()
      ..moveTo(size.width * 0.16, size.height * 0.72)
      ..cubicTo(
        size.width * 0.36,
        size.height * 0.48,
        size.width * 0.60,
        size.height * 0.58,
        size.width * 0.84,
        size.height * 0.28,
      );
    final warmPath = Path()
      ..moveTo(size.width * 0.14, size.height * 0.32)
      ..cubicTo(
        size.width * 0.36,
        size.height * 0.18,
        size.width * 0.66,
        size.height * 0.26,
        size.width * 0.88,
        size.height * 0.58,
      );

    canvas.drawPath(coolPath, coolPaint);
    canvas.drawPath(warmPath, warmPaint);
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) => false;
}
