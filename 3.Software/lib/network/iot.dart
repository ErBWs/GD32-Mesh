import 'package:dio/dio.dart';
import 'package:flutter/foundation.dart';

class Iot {
  final Dio dio = Dio(BaseOptions(
    connectTimeout: const Duration(milliseconds: 12000),
    receiveTimeout: const Duration(milliseconds: 12000),
  ));

  Future<String> getAuthToken({
    required String projectID,
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
            "project": {"id": projectID}
          }
        }
      },
    );

    return response.headers.value("X-Subject-Token") ?? "";
  }

  Future<Response> getShadowData({
    required String projectID,
    required String deviceID,
    required String userName,
    required String password,
    required String token,
  }) async {
    String shadowUrl =
        "https://0c5e592236.st1.iotda-app.cn-east-3.myhuaweicloud.com/v5/iot/{project_id}/devices/{device_id}/shadow";
    shadowUrl = shadowUrl.replaceFirst("{project_id}", projectID);
    shadowUrl = shadowUrl.replaceFirst("{device_id}", deviceID);

    Options options = Options(
      responseType: ResponseType.json,
      headers: {
        'X-Auth-Token': token,
      },
    );

    try {
      final response = await dio.get(shadowUrl, options: options);
      debugPrint(response.data.toString());
      return response;
    } catch (e) {
      debugPrint('请求失败: $e');
      rethrow;
    }
  }
}

Iot iot = Iot();
