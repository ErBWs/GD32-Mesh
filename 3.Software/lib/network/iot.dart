import 'package:dio/dio.dart';
import 'package:flutter/foundation.dart';
import 'package:meshw/persistent_storage/user_token.dart';

class Iot {
  final Dio dio = Dio(BaseOptions(
    connectTimeout: const Duration(milliseconds: 12000),
    receiveTimeout: const Duration(milliseconds: 12000),
  ));

  Future<UserToken> getAuthToken({
    required String userName,
    required String password,
  }) async {
    final response = await dio.post(
      "https://iam.cn-east-3.myhuaweicloud.com/v3/auth/tokens",
      data: {
        "auth": {
          "identity": {
            "methods": ["password"],
            "password": {
              "user": {
                "name": userName,
                "password": password,
                "domain": {"name": "erbw_s"}
              }
            }
          },
          "scope": {
            "project": {"id": "be623ce1bf6c4ce9b9609768b1d4e68e"}
          }
        }
      },
    );

    return UserToken(
      token: response.headers.value("X-Subject-Token") ?? "",
      time: DateTime.now().millisecondsSinceEpoch,
    );
  }

  Future<Response> getShadowData({
    required String userName,
    required String password,
    required String token,
  }) async {
    String shadowUrl =
        "https://0c5e592236.st1.iotda-app.cn-east-3.myhuaweicloud.com/v5/iot/{project_id}/devices/{device_id}/shadow";
    shadowUrl = shadowUrl.replaceFirst(
        "{project_id}", "be623ce1bf6c4ce9b9609768b1d4e68e");
    shadowUrl = shadowUrl.replaceFirst(
        "{device_id}", "67ea52cbc957870e570e769f_LoRaGateway");

    Options options = Options(
      responseType: ResponseType.json,
      headers: {
        'X-Auth-Token': token,
      },
    );

    try {
      final response = await dio.get(shadowUrl, options: options);
      return response;
    } catch (e) {
      debugPrint('请求失败: $e');
      rethrow;
    }
  }
}

Iot iot = Iot();
