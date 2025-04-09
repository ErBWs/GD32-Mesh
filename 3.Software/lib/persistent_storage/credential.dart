import 'package:isar/isar.dart';

part 'credential.g.dart';

@collection
class Credential {
  Id id = Isar.autoIncrement;

  String projectID;
  String deviceID;
  String userName;
  String password;

  Credential({
    required this.projectID,
    required this.deviceID,
    required this.userName,
    required this.password,
  });
}
