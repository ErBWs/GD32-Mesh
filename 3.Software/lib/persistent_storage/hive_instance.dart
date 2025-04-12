import 'dart:io';
import 'package:hive_flutter/hive_flutter.dart';
import 'package:meshw/persistent_storage/user_credential.dart';
import 'package:meshw/persistent_storage/user_token.dart';
import 'package:path_provider/path_provider.dart';

class HiveInstance {
  late Directory directory;
  late Box<UserCredential> userCredential;
  late Box<UserToken> userToken;

  Future init() async {
    directory = await getApplicationSupportDirectory();
    await Hive.initFlutter('${directory.path}/hive');
    Hive.registerAdapter(UserCredentialAdapter());
    Hive.registerAdapter(UserTokenAdapter());
    userCredential = await Hive.openBox('credential');
    userToken = await Hive.openBox('token');
  }
}

HiveInstance hiveInstance = HiveInstance();
