import 'dart:io';
import 'package:isar/isar.dart';
import 'package:meshw/persistent_storage/credential.dart';
import 'package:path_provider/path_provider.dart';

class IsarInstance {
  late Isar isar;
  late Directory directory;

  late IsarCollection<Credential> credential;

  Future init() async {
    directory = await getApplicationDocumentsDirectory();
    isar = await Isar.open(
      [CredentialSchema],
      directory: directory.path,
    );
    credential = isar.credentials;
  }
}

IsarInstance isarInstance = IsarInstance();
