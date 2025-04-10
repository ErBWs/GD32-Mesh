import 'package:hive/hive.dart';

part 'user_token.g.dart';

@HiveType(typeId: 1)
class UserToken {
  @HiveField(0)
  String token;
  @HiveField(1)
  int time;

  UserToken({
    required this.token,
    required this.time,
  });
}
