import 'dart:io';
import 'package:flutter/material.dart';
import 'package:hive/hive.dart';
import 'package:meshw/persistent_storage/user_credential.dart';
import 'package:path_provider/path_provider.dart';

class HiveInstance {
  late Directory directory;

  Future init() async {
    directory = await getTemporaryDirectory();
    debugPrint(directory.path);

    debugPrint(DateTime.now().difference(DateTime.fromMillisecondsSinceEpoch(1742665644 * 1000)).inDays.toString());
  }
}

HiveInstance hiveInstance = HiveInstance();
