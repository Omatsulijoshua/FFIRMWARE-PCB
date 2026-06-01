import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';

import '../../shared/widgets/brand_logo.dart';
import '../../shared/widgets/device_preview_card.dart';
import '../../shared/widgets/app_scaffold.dart';
import 'onboarding_page_data.dart';

class OnboardingScreen extends StatefulWidget {
  const OnboardingScreen({super.key});

  @override
  State<OnboardingScreen> createState() => _OnboardingScreenState();
}

class _OnboardingScreenState extends State<OnboardingScreen> {
  final PageController _controller = PageController();
  int _index = 0;

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  void _next() {
    if (_index == onboardingPages.length - 1) {
      context.go('/home');
      return;
    }
    _controller.nextPage(
      duration: const Duration(milliseconds: 280),
      curve: Curves.easeOutCubic,
    );
  }

  @override
  Widget build(BuildContext context) {
    return AppScaffold(
      title: 'DryGuard',
      actions: [
        TextButton(
          onPressed: () => context.go('/home'),
          child: const Text('Skip'),
        ),
      ],
      child: ListView(
        children: [
          const Row(
            children: [
              BrandLogo(size: 44, showShadow: false),
              SizedBox(width: 12),
              Expanded(
                child: Text(
                  'Premium care for gear that works hard.',
                  style: TextStyle(fontSize: 18, fontWeight: FontWeight.w700),
                ),
              ),
            ],
          ),
          const SizedBox(height: 20),
          const Center(
            child: SizedBox(
              width: 300,
              child: DevicePreviewCard(),
            ),
          ),
          const SizedBox(height: 18),
          SizedBox(
            height: 218,
            child: PageView.builder(
              controller: _controller,
              onPageChanged: (value) => setState(() => _index = value),
              itemCount: onboardingPages.length,
              itemBuilder: (context, index) {
                final item = onboardingPages[index];
                return _OnboardingCopy(page: item);
              },
            ),
          ),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              for (var i = 0; i < onboardingPages.length; i++)
                AnimatedContainer(
                  duration: const Duration(milliseconds: 180),
                  width: i == _index ? 26 : 8,
                  height: 8,
                  margin: const EdgeInsets.symmetric(horizontal: 4),
                  decoration: BoxDecoration(
                    color: i == _index
                        ? Theme.of(context).colorScheme.primary
                        : Theme.of(context).colorScheme.outlineVariant,
                    borderRadius: BorderRadius.circular(99),
                  ),
                ),
            ],
          ),
          const SizedBox(height: 18),
          FilledButton.icon(
            onPressed: _next,
            icon: Icon(
              _index == onboardingPages.length - 1
                  ? Icons.add_link
                  : Icons.arrow_forward,
            ),
            label: Text(
              _index == onboardingPages.length - 1 ? 'Add Device' : 'Continue',
            ),
          ),
        ],
      ),
    );
  }
}

class _OnboardingCopy extends StatelessWidget {
  const _OnboardingCopy({required this.page});

  final OnboardingPageData page;

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Icon(
          page.icon,
          size: 34,
          color: Theme.of(context).colorScheme.primary,
        ),
        const SizedBox(height: 12),
        Text(
          page.title,
          textAlign: TextAlign.center,
          style: Theme.of(context).textTheme.headlineSmall?.copyWith(
                fontWeight: FontWeight.w800,
              ),
        ),
        const SizedBox(height: 10),
        Text(
          page.body,
          textAlign: TextAlign.center,
          style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                color: Theme.of(context).colorScheme.onSurfaceVariant,
              ),
        ),
      ],
    );
  }
}
