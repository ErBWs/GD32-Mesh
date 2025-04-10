import 'package:hive/hive.dart';

part 'user_credential.g.dart';

@HiveType(typeId: 0)
class UserCredential {
  @HiveField(0)
  String projectID;
  @HiveField(1)
  String deviceID;
  @HiveField(2)
  String userName;
  @HiveField(3)
  String password;

  UserCredential({
    required this.projectID,
    required this.deviceID,
    required this.userName,
    required this.password,
  });
}
