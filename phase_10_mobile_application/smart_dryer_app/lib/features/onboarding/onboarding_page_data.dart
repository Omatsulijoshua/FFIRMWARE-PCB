import 'package:flutter/material.dart';

class OnboardingPageData {
  const OnboardingPageData({
    required this.icon,
    required this.title,
    required this.body,
  });

  final IconData icon;
  final String title;
  final String body;
}

const onboardingPages = [
  OnboardingPageData(
    icon: Icons.air,
    title: 'Dry gear with precision',
    body:
        'Choose profiles for shoes, helmets, gloves, sports gear, and technical fabrics.',
  ),
  OnboardingPageData(
    icon: Icons.shield_outlined,
    title: 'Built around safety',
    body:
        'DryGuard monitors temperature, humidity, fan feedback, and cooldown before completion.',
  ),
  OnboardingPageData(
    icon: Icons.bluetooth_connected,
    title: 'Add your dryer',
    body:
        'Pair over Bluetooth, provision WiFi, and keep local control even without cloud access.',
  ),
];
