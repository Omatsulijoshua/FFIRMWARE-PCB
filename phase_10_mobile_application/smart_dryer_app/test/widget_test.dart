import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:smart_dryer_app/app/app.dart';

void main() {
  testWidgets('DryGuard launch screen renders', (tester) async {
    await tester.pumpWidget(const ProviderScope(child: SmartDryerApp()));

    expect(find.text('DryGuard'), findsOneWidget);
    expect(find.text('Smart drying. Safer gear.'), findsOneWidget);

    await tester.pump(const Duration(milliseconds: 2300));
    await tester.pumpAndSettle();

    expect(find.text('Dry gear with precision'), findsOneWidget);
  });
}
