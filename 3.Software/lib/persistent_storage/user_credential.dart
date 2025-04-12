import 'package:hive/hive.dart';

part 'user_credential.g.dart';

@HiveType(typeId: 0)
class UserCredential {
  @HiveField(0)
  String userName;
  @HiveField(1)
  String password;

  UserCredential({
    required this.userName,
    required this.password,
  });
}
